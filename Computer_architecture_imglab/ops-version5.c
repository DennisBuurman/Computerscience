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

//=====SIMD includes=====
#include <immintrin.h>
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
//=======================

/* Computes the transpose of @src and stores this in @dst. @dst is
 * expected to have been created already with the correct dimensions.
 */
void 
op_transpose(image_t *dst, const image_t *src, int const B, 
									int const xx, int const yy)
{
  assert(dst->width == src->height && src->height == dst->width);
	
	for (int y = yy; y < yy + B; y+=B){
		for (int x = xx; x < xx + B; x++){
			for (int i = y; i < y+B; i++){
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
	int x, y, i; //indexes of for loops
	unsigned int idx;
	uint32_t color; //RGBA value of 1 pixel
	
	float mulx = 0.2126f, muly = 0.7152f, mulz = 0.0722; //multipliers of compute_intensity
  float n = (float)N_BINS;
	
	__m128 vx, vy, vz, vw, result; //vectorised RGBA-values of 4 pixels
  
  //store value used for MIN in vector vmin
  float min = (float)N_BINS-1;
  __m128 vmin = _mm_broadcast_ss (&min);
  
  //store factors of compute_intensity in vectors
  __m128 factorx = _mm_broadcast_ss (&mulx); 
  __m128 factory = _mm_broadcast_ss (&muly);
  __m128 factorz = _mm_broadcast_ss (&mulz);
  //store N_BINS in vector
  __m128 factorn = _mm_broadcast_ss (&n);
 
	for (y = 0; y < image->height; y++){
  	for (x = 0; x < image->width; x+=4){
  		//unpack 4 pixels and vectorize them into vx,vy,vz,vw
  		for (i = 0; i < 4; i++){
  			color = *image_get_pixel(image, x+i, y);
  			vw[i] = (color & 0xff) / 255.f;
  			color >>= 8; 
  			vz[i] = (color & 0xff) / 255.f;
  			color >>= 8;
  			vy[i] = (color & 0xff) / 255.f;
  			color >>= 8;
  			vx[i] = (color & 0xff) / 255.f;
  		}
			//compute_intensity
  		//multiply with factors
      vx = _mm_mul_ps(vx, factorx);
			vy = _mm_mul_ps(vy, factory);
      vz = _mm_mul_ps(vz, factorz);
      
      //store endresult of compute_intensity into vector result
      result = _mm_add_ps (vx, vy);
      result = _mm_add_ps (result, vz);
      result = _mm_mul_ps (result, vw);
      
      //multiply with N_BINS
      result = _mm_mul_ps (result, factorn);			
			
			//get min of result and vmin
			result = _mm_min_ps (result, vmin);
			
			for (i = 0; i < 4; i++){
      	idx = (unsigned int)result[i];
    		bins[idx] += 1;
			}
    }
  }
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
  
  float fhue = (float)hue; //cast parameter to float
  float fspread = (float)spread; //cast parameter to float
  
  uint32_t color; //RGBA value of 1 pixel
  rgba_t endcolor; //RGBA value of endresult
  
  float waarde_ = 1.f; //used for weight, gray, weight
  float factor2 = 2.f, factor3 = 4.f; //used for compute_hue
  float factor60 = 60.f, addition360 = 360.f; //used for compute_hue
  
  int x, y, i; //indexes of for loops
  
  __m128 vfactor2 = _mm_broadcast_ss(&factor2);
  __m128 vfactor3 = _mm_broadcast_ss(&factor3);
  
  __m128 vfactor60 = _mm_broadcast_ss(&factor60);
  __m128 vadd360 = _mm_broadcast_ss(&addition360);
  __m128 vzero = {0.0,0.0,0.0,0.0}; //zero vector used for compute_hue
  __m128 waarde = {1.f,1.f,1.f,1.f}; //vector 
  __m128 negative = {-1,-1,-1,-1};
	__m128 vhue = _mm_broadcast_ss(&fhue); //vector with parameter hue
	__m128 vspread = _mm_broadcast_ss(&fspread); //vector with parameter spread
	__m128 vweight, vweight2, vdiff; //vectors to store weight values and diff
	__m128 vintensity;
	
	//store factors of compute_intensity in vectors
	__m128 mulx = {0.2126f, 0.2126f, 0.2126f, 0.2126f}, 
				 muly = {0.7152f, 0.7152f, 0.7152f, 0.7152f}, 
				 mulz = {0.0722, 0.0722, 0.0722, 0.0722};
	
  __m128 vx, vy, vz, vw; //vectorised RGBA-values of 4 pixels
  __m128 vxgray, vygray, vzgray, vwgray; //vectorised RGBA-values of 4 gray-pixels
  __m128 max, min, cmm; //vectors to store max, min and max-min values for compute_hue
  __m128 result1, result2, result3, endresult; //vectors to store different results
  
  for (y = 0; y < image->height; y++){
  	for (x = 0; x < image->width; x+=4){
      //unpack 4 pixels and vectorize them into vx,vy,vz,vw
      for (i = 0; i < 4; i++){
      	color = *image_get_pixel(image, x+i, y);
  			vw[i] = (color & 0xff) / 255.f;
  			color >>= 8; 
  			vz[i] = (color & 0xff) / 255.f;
  			color >>= 8;
  			vy[i] = (color & 0xff) / 255.f;
  			color >>= 8;
  			vx[i] = (color & 0xff) / 255.f;
      }

      //BEGIN OF COMPUTE HUE ================================
      //get min and max of the RGB values of color
      max = _mm_max_ps (vx, _mm_max_ps(vy, vz));
      min = _mm_min_ps (vx, _mm_min_ps(vy, vz));
      
      //compute max-min              
      cmm = _mm_sub_ps (max, min);

			//the division in the if statements
			result1 = _mm_div_ps (_mm_sub_ps(vy, vz), cmm);
			result2 = _mm_div_ps (_mm_sub_ps(vz, vx), cmm);
			result3 = _mm_div_ps (_mm_sub_ps(vx, vy), cmm);
			
			//additions in the if statements
			result2 = _mm_add_ps (result2, vfactor2); //2.f
			result3 = _mm_add_ps (result3, vfactor3); //4.f
			
			//decide which condition is true of the if-else if-else statements and 
			//perform the and-operation with the computed results
			result1 = _mm_and_ps (result1, _mm_cmpeq_ps (vx, max));
			result2 = _mm_and_ps (result2, _mm_cmpeq_ps (vy, max));
			result3 = _mm_and_ps (result3, _mm_cmpeq_ps (vz, max));
			
			//add all the results and store into endresult
			endresult = _mm_add_ps (result1, _mm_add_ps(result2, result3));
			
			//multiply with 60.f
			endresult = _mm_mul_ps (endresult, vfactor60);
			
			//check if hue < 0 is true, then add 360.f to endresult
			endresult = _mm_add_ps (endresult, _mm_and_ps(vadd360, _mm_cmplt_ps (endresult, vzero)));
			//compute_hue returns int, so floor all the floats
			endresult = _mm_floor_ps(endresult);
			//END OF COMPUTE_HUE ==================================
			
			//COMPUTE ABS =========================================
			//compute possible result where hue - compute_hue < 0
			result1 = _mm_and_ps (_mm_sub_ps(vhue, endresult), _mm_cmpgt_ps(_mm_sub_ps(vhue, endresult), vzero));
			//compute possible result where hue - compute_hue > 0
			result2 = _mm_and_ps (_mm_sub_ps(vhue, endresult), _mm_mul_ps(negative, (_mm_cmplt_ps(_mm_sub_ps(vhue, endresult), vzero))));
			
			//add the two possible results to vdiff
			vdiff = _mm_add_ps (result1, result2);
			//this is also returns an int, so floor all floats
			vdiff = _mm_floor_ps(vdiff);
			//END COMPUTE ABS =====================================
			
			//COMPUTE WEIGHT ======================================
			//compute possible result when diff <= spread
			result1 = _mm_and_ps (_mm_cmple_ps(vdiff, vspread), _mm_div_ps(vdiff,vspread));
			//compute possible result when diff > spread
			result2 = _mm_and_ps (_mm_cmpgt_ps(vdiff, vspread), waarde);
			//add the two possible results to vweight
			vweight = _mm_add_ps (result1, result2);
			//END OF COMPUTE_WEIGHT ===============================
			
			//COMPUTE INTENSITY ===================================
			//perform the operations with the multipliers stored int mulx, muly, mulz
			vintensity = _mm_mul_ps(vw, _mm_add_ps (_mm_add_ps(_mm_mul_ps(vx, mulx), _mm_mul_ps(vy, muly)), _mm_mul_ps(vz, mulz)));
			//END OF COMPUTE INTENSITY ============================
			
			//store vintensity and 1.f into gray
			for (int i = 0; i < 4; i++){
				vxgray[i] = vintensity[i];
				vygray[i] = vintensity[i];
				vzgray[i] = vintensity[i]; 
			}
			vwgray = _mm_broadcast_ss(&waarde_);
			
			
			//multiply gray values with vweight
			vxgray = _mm_mul_ps(vxgray, vweight);
			vygray = _mm_mul_ps(vygray, vweight);
			vzgray = _mm_mul_ps(vzgray, vweight);
			vwgray = _mm_mul_ps(vwgray, vweight);
			
			//compute 1.f - vweight and store into vweight2
			vweight2 = _mm_sub_ps(waarde, vweight);
			
			//multiply original pixel with vweigth2
			vx = _mm_mul_ps(vx, vweight2);
			vy = _mm_mul_ps(vy, vweight2);
			vz = _mm_mul_ps(vz, vweight2);
			vw = _mm_mul_ps(vw, vweight2);
			
			//add gray with original pixel
			vx = _mm_add_ps(vx, vxgray);
			vy = _mm_add_ps(vy, vygray);
			vz = _mm_add_ps(vz, vzgray);
			vw = _mm_add_ps(vw, vwgray);
			
			//write result to endcolor and store into image	
			for (int i = 0; i < 4; i++){
				endcolor.x = vx[i];
				endcolor.y = vy[i];
				endcolor.z = vz[i];
				endcolor.w = vw[i];
				
				RGBA_pack(*image_get_pixel(image, x+i, y), endcolor);
			}
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

image_t * run_op_transpose(const image_t *source, const int n_repeat){
  image_t *result = image_new_from_image_T(source);
	
	int const B = 128;
	
  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++){
    //loop blocking with blocksize B
    for (int yy = 0; yy < source->height; yy+=B){
    	for (int xx = 0; xx < source->width; xx+=B){
    		op_transpose(result, source, B, xx, yy);
    	}
    }
    
  }

  get_time(&end_time);

  print_elapsed_time("transpose", &end_time, &start_time);

  return result;
}

void run_op_histogram(const image_t *source, const int n_repeat) {
  unsigned int bins[N_BINS];
  
	//int const B = 64;
	
  struct timespec start_time, end_time;
  get_time(&start_time);

  for (int Z = 0; Z < n_repeat; Z++){
  	for (int i = 0; i < N_BINS; i++)
    	bins[i] = 0;
  
    op_histogram(source, bins);
  }

  get_time(&end_time);

  print_elapsed_time("histogram", &end_time, &start_time);

  /* Print result to stdout. */
  for (int i = 0; i < N_BINS; i++)
    printf("%d ", bins[i]);
  printf("\n");
}

image_t * run_op_transgram(const image_t *source, const int n_repeat){
	image_t *result = image_new_from_image_T(source);
	unsigned int bins[N_BINS];
	
	int const B = 128;

	struct timespec start_time, end_time;
	get_time(&start_time);


	for (int Z = 0; Z < n_repeat; Z++){
		for (int i = 0; i < N_BINS; i++)
	   	bins[i] = 0;
	   	
		for (int yy = 0; yy < source->height; yy+=B){
    	for (int xx = 0; xx < source->width; xx+=B){
    	
				for (int y = yy; y < yy + B; y++){
					for (int x = xx; x < xx + B; x++){
				 		//TRANSPOSE
				 		*image_get_pixel(result, y, x) = *image_get_pixel(source, x, y);
				 		
				 		//HISTOGRAM
				 		rgba_t color;
				  	RGBA_unpack(color, *image_get_pixel(source, x, y));

				  	unsigned int idx = (unsigned int)(compute_intensity(color) * N_BINS);
				  	idx = MIN(N_BINS - 1, idx);
				  	bins[idx] += 1;
					}
				}
			}
		}
	}
	
	get_time(&end_time);
	print_elapsed_time("transgram", &end_time, &start_time);

	//Print result to stdout
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

  for (int Z = 0; Z < n_repeat; Z++){
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
