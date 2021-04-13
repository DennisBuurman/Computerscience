/*
 * Skeleton code for use with Computer Architecture 2018 assignment 3,
 * LIACS, Leiden University.
 * 
 * From Dennis Buurman (s2027100) and Jerry Schonenberg (s2041022)
 */

#include "run.h"
#include "timing.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* Computes the transpose of @src and stores this in @dst. @dst is
 * expected to have been created already with the correct dimensions.
 */
void 
op_transpose(image_t *dst, const image_t *src)
{
  assert(dst->width == src->height && src->height == dst->width);
	
	int const N = 64; //number of values which are processed in one loop iteration
	
	for (int y = 0; y < dst->height; y+=N){
		for (int x = 0; x < dst->width; x++){
			for (int i = y; i < y+N; i++){ 
				*image_get_pixel(dst, i, x) = *image_get_pixel(src, x, i);
			}
		}
	}
}

/* Computes the intensity of @color. To do so, we use CIE 1931 weights
 * multiplied by alpha: Y = A( 0.2126R + 0.7152G + 0.0722B ).
 */
static float
compute_intensity(rgba_t color)
{
  return color.w * ( 0.2126f * color.x + 0.7152f * color.y + 0.0722 * color.z);
}

/* Computes the image histogram of @image, based on grayscale intensity.
 * The histogram is saved in the @bins argument.
 */
void
op_histogram(const image_t *image, unsigned int bins[N_BINS])
{
  /* Using our own loop here instead of memset() on purpose, so we can
   * play with optimizations.
   */
  for (int i = 0; i < N_BINS; i++)
    bins[i] = 0;

  for (int x = 0; x < image->width; x++)
    {
      for (int y = 0; y < image->height; y++)
        {
          rgba_t color;
          RGBA_unpack(color, *image_get_pixel(image, x, y));

          unsigned int idx = (unsigned int)(compute_intensity(color) * N_BINS);
          idx = MIN(N_BINS - 1, idx);
          bins[idx] += 1;
        }
    }
}


/* Computes the hue (tint) of an RGB(A) quadruple.
 *
 * See http://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/
 */
static int
compute_hue (rgba_t color)
{
  float hue = 0.f;

  /* Find the 'largest' component of either of r, g or b */
  float c_max = fmax(fmax(color.x, color.y), color.z);
  float c_min = fmin(fmin(color.x, color.y), color.z);

  /* The hue depends on which component in the largest */
  if (c_max == color.x)
    hue = (color.y - color.z) / (c_max - c_min);
  else if (c_max == color.y)
    hue = 2.f + (color.z - color.x) / (c_max - c_min);
  else
    hue = 4.f + (color.x - color.y) / (c_max - c_min);

  /* Ensure hue is in the range of [0-360]. */
  hue *= 60.f;
  if (hue < 0)
    hue += 360.f;

  return (int)hue;
}

/* Performs the selective grayscale operation *in-place* on @image
 * using the @hue and @spread parameters. When a pixel's hue differs
 * more from the given @hue than @spread, the gray color value is used,
 * and the original color value is retained otherwise.
 */
void
op_selective_grayscale(image_t *image, const int hue, const int spread)
{
  /* We compute the 'selective grayscale' here, this is the general idea:
   * - Iterate over every pixel in the source image.
   * - We compute each pixel's 'intensity' from its RGB triplet.
   * - We compute the hue of each pixel.
   * - When the hue differs no more than 'spread' from the given hue,
   * we retain the original color, otherwise we use the computed gray value.
   * - To reduce artifacts, we 'weigh' the color and gray components,
   * based on the difference between the pixel's hue and the given hue.
   */
  for (int x = 0; x < image->width; x++)
    {
      for (int y = 0; y < image->height; y++)
        {
          rgba_t color;
          RGBA_unpack(color, *image_get_pixel(image, x, y));

          /* Compute the 'hue' (tint) and the difference with the
           * given hue...
           */
          int diff = (int)abs(hue - compute_hue(color));

          /* ...this difference determines whether we pick the gray or the
           * original color. We use a linear weight to reduce artifacts in
           * the final image.
           */
          float weight = (diff <= spread) ? (float)diff / (float)spread : 1.f;

          /* Apply the weights to the 'color' and 'gray' components. */
          rgba_t gray;
          float intensity = compute_intensity(color);
          RGBA(gray, intensity, intensity, intensity, 1.f);

          RGBA_mults(gray, gray, weight);
          RGBA_mults(color, color, 1.f-weight);

          /* Finally, add both components to produce the resulting pixel. */
          RGBA_add(color, color, gray);
          RGBA_pack(*image_get_pixel(image, x, y), color);
        }
    }
}

/*
 * 2D Lighting engine.
 */

typedef struct
{
  // Location in relative coordinates (0,0) = top left, (1,1) = bottom right
  float x, y;
  // Height of the light above the image plane, for faux 3D effect
  float height;
  // Intensity and color of the light, alpha value is unused
  rgba_t intensity;
} light_t;

/* We add a certain constant value to z to smooth out shading, 1.1
 * was empirically determined to work well :)
 */
static const float SMOOTH_OUT_FACTOR = 1.1f;

/* This constant is used to scale the heightmap values (0.0-1.0) to the
 * image domain. Note that increasing this values reduces round-off
 * errors in exchange for worse performance.
 */
static const float HEIGHT_SCALE = 128.0f;

/* The color of the ambient light. */
static const rgba_t AMBIENT = { 0.1, 0.1, 0.2, 1.0 };


/* Given a @src image and @numLights @lights, an image that contains the
 * lights and shadows due to these lights is computed and stored in @dst.
 * @dst must have the same dimensions as @src.
 */
void
op_light2d(image_t *dst, const image_t *src,
           const light_t lights[], const int numLights)
{
  assert (dst->width == src->width && dst->height == src->height);

  for (int x = 0; x < src->width; x++)
    {
      for (int y = 0; y < src->height; y++)
        {
          rgba_t pixel;
          RGBA_unpack(pixel, *image_get_pixel(src, x, y));

          /* Compute the inverted heightmap value for this pixel */
          float in = compute_intensity(pixel);
          int z = (1.0-in) * HEIGHT_SCALE;

          rgba_t light = AMBIENT;

          /* Cast rays from every light source to this pixel */
          for (int l = 0; l < numLights; l++)
            {
              /* Position of the light in image space */
              int lx = lights[l].x * (float)(src->width-1);
              int ly = lights[l].y * (float)(src->height-1);
              int lz = lights[l].height * HEIGHT_SCALE; // Scale the faux-Z axis

              /* The DDA algorithm is used to draw lines and not Bresenham. */
              float dx = x - lx;
              float dy = y - ly;
              float dz = z - lz;
              /* The number of steps is determined by the longest axis. */
              int steps = MAX( MAX( fabsf(dx), fabsf(dy) ), fabsf(dz) );

              /* We run the algorithm in the float-domain for simplicity,
               * however we risk sampling the same pixels multiple times.
               */
              dx /= steps; dy /= steps; dz /= steps;
              float nx = lx, ny = ly, nz = lz;

              bool occluded = false;
              for (int i = 0; i < steps-1; i++)
                {
                  nx += dx; ny += dy; nz += dz;

                  /* No obstacles below the imaginary Z-plane, so we can stop */
                  if (nz <= 0)
                    break;

                  rgba_t value;
                  RGBA_unpack(value, *image_get_pixel(src, (int)nx, (int)ny));

                  /* We interpret the input image as an inverted heightmap */
                  float height = (1.0-compute_intensity(value)) * HEIGHT_SCALE;

                  /* Obstacle is tall enough for the ray to hit it = shadow */
                  if (height > nz + SMOOTH_OUT_FACTOR)
                    {
                      occluded = true;
                      break;
                    }
                }

              /* The ray didn't hit any obstacle */
              if (!occluded)
                {
                  /* Add some simple linear attenuation */
                  float dist = sqrt(pow(x - lx, 2) + pow(y - ly, 2));
                  float attenuate = dist / (float)src->width;
                  attenuate = CLAMP(attenuate, 0.0, 1.0 );

                  rgba_t tmp;
                  RGBA_mults(tmp, lights[l].intensity, 1.0-attenuate);
                  RGBA_add(light, light, tmp);
                }
            }

            /* Make sure the light value is at [0.0,1.0] */
            RGBA_clamp(light, light, 0.0, 1.0);

            /* Multiply the computed light with the existing image pixel and
             * write it to the destination buffer.
             */
            RGBA_mult(pixel, pixel, light);
            RGBA_pack(*image_get_pixel(dst, x, y), pixel);
        }
    }
}


/*
 * Implementation of the run functions that will invoke the different
 * image operations implemented above.
 */

image_t *
run_op_transpose(const image_t *source, const int n_repeat)
{
  image_t *result = image_new_from_image_T(source);

  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_transpose(result, source);
    }

  get_time(&end_time);

  print_elapsed_time("transpose", &end_time, &start_time);

  return result;
}

void
run_op_histogram(const image_t *source, const int n_repeat)
{
  unsigned int bins[N_BINS];

  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_histogram(source, bins);
    }

  get_time(&end_time);

  print_elapsed_time("histogram", &end_time, &start_time);

  /* Print result to stdout. */
  for (int i = 0; i < N_BINS; i++)
    printf("%d ", bins[i]);
  printf("\n");
}

image_t *
run_op_transgram(const image_t *source, const int n_repeat)
{
  image_t *result = image_new_from_image_T(source);
  unsigned int bins[N_BINS];

  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++){
    op_transpose(result, source);
    op_histogram(result, bins);
  }
	
  get_time(&end_time);

  print_elapsed_time("transgram", &end_time, &start_time);

  /* Print result to stdout. */
  for (int i = 0; i < N_BINS; i++)
    printf("%d ", bins[i]);
  printf("\n");

  return result;
}

void
run_op_selgray(image_t *source, const int n_repeat)
{
  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_selective_grayscale(source, 40, 30);
    }

  get_time(&end_time);

  print_elapsed_time("selgray", &end_time, &start_time);
}

image_t *
run_op_light2d(const image_t *source, const int n_repeat)
{
  /* Define two example light sources */
  static const light_t const lights[] =
    {
      {
        0.75,      // X
        0.75,      // Y
        2.0,       // Height above image, to produce faux Z-axis
        { 0.7, 0.5, 0.5, 1.0 } // Intensity RGBA
      },
      {
        0.25,      // X
        0.25,      // Y
        2.0,       // Height
        { 0.5, 0.7, 0.7, 1.0 } // Intensity RGBA
      }
    };

  const int n_lights = sizeof(lights) / sizeof(lights[0]);
  image_t *result = image_new_from_image(source);


  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_light2d(result, source, lights, n_lights);
    }

  get_time(&end_time);

  print_elapsed_time("light2d", &end_time, &start_time);

  return result;
}
