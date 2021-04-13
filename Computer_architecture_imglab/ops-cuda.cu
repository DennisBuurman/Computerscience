/*
 * Skeleton code for use with Computer Architecture 2018 assignment 3,
 * LIACS, Leiden University.
 * 
 * From Dennis Buurman (s2027100) and Jerry Schonenberg (s2041022)
 */

#include "run.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Some simple assert macro and inline function to handle CUDA-errors
 * more easily.
 */
#define CUDA_ASSERT(ans) { cudaAssert((ans), __FILE__, __LINE__); }

inline void
cudaAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
  if (code != cudaSuccess)
    {
      fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code),
              file, line);

      if (abort)
        exit(code);
    }
}

/*
 * Forward defines for the 2D lighting kernel.
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

/* TODO: enable once you need these */
#if 0
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
#endif


/*
 * CUDA kernels
 */

/* Example kernel for an image copy operation. */
__global__ void
op_cuda_copy(uint32_t *dst, const uint32_t *src, const int rowstride,
             const int x, const int y,
             const int width, const int height)
{
  const int xx = blockIdx.x * blockDim.x + threadIdx.x;
  const int yy = blockIdx.y * blockDim.y + threadIdx.y;

  if (xx < x || xx >= width || yy < y || y >= height)
    return;

  /* Get the pixel in src and store in dst. */
  uint32_t pixel = *image_get_pixel_data(src, rowstride, xx, yy);
  *image_get_pixel_data(dst, rowstride, xx, yy) = pixel;
}

__global__ void
op_cuda_transpose(uint32_t *dst, const uint32_t *src, const int rowstride,
             			const int x, const int y,
             			const int width, const int height)
{	
	const int xx = blockIdx.x * blockDim.x + threadIdx.x;
  const int yy = blockIdx.y * blockDim.y + threadIdx.y;
  
  if (xx < x || xx >= width || yy < y || y >= height)
    return;
  
  /* Get the pixel in src and transpose into dst. */
  uint32_t pixel = *image_get_pixel_data(src, rowstride, xx, yy);
  *image_get_pixel_data(dst, rowstride, yy, xx) = pixel;
}

/* Computes the intensity of @color. To do so, we use CIE 1931 weights
 * multiplied by alpha: Y = A( 0.2126R + 0.7152G + 0.0722B ).
 */
__device__ static float
compute_intensity(rgba_t color)
{
	return color.w * (0.2126f * color.x + 0.7152f * color.y + 0.0722 * color.z);
}

/* Computes the image histogram of @image, based on grayscale intensity.
 * The histogram is saved in the @bins argument.
 */
__global__ void //TODO vragen: - 3 pixels verschillen met origineel
op_cuda_histogram(unsigned int bins_D[], const uint32_t *src, const int rowstride,
             			const int x, const int y,
             			const int width, const int height)
{
	const int xx = blockIdx.x * blockDim.x + threadIdx.x;
  const int yy = blockIdx.y * blockDim.y + threadIdx.y;
  
  if (xx < x || xx >= width || yy < y || y >= height)
    return;
  
  rgba_t color;
  RGBA_unpack(color, *image_get_pixel_data(src, rowstride, xx, yy));
  
  unsigned int idx = (unsigned int)(compute_intensity(color) * N_BINS);
  idx = MIN(N_BINS-1, idx);
  atomicAdd(&bins_D[idx], 1);
}

__device__ static int
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

__global__ void
op_cuda_selgray(uint32_t *src, const int rowstride, const int x, const int y,
								const int width, const int height, const int hue, 
								const int spread)
{
	const int xx = blockIdx.x * blockDim.x + threadIdx.x;
  const int yy = blockIdx.y * blockDim.y + threadIdx.y;
  
  if (xx < x || xx >= width || yy < y || y >= height)
    return;
  
  rgba_t color;
  RGBA_unpack(color, *image_get_pixel_data(src, rowstride, xx, yy));

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
            
  RGBA_pack(*image_get_pixel_data(src, rowstride, xx, yy), color);
}

extern "C"
{

image_t *
run_op_transpose(const image_t *source, const int n_repeat)
{
	/* Allocate buffers to contain initial image and result */
	uint32_t *result_D; uint32_t *image_D;
	int n_pixels = source->height * source->width;
	int n_bytes = n_pixels * sizeof(uint32_t);
	
	CUDA_ASSERT(cudaMalloc(&result_D, n_bytes));
	CUDA_ASSERT(cudaMalloc(&image_D, n_bytes));
	
  /* Calculate the block size and the number of thread blocks */
  const dim3 blocksize(16, 16);
  const dim3 numblocks((source->width % blocksize.x) ?
                          source->width / blocksize.x + 1 :
                          source->width / blocksize.x,
                       (source->height % blocksize.y) ?
                          source->height / blocksize.y + 1 :
                          source->height / blocksize.y);

	/* Copy the input image to the initial buffer */
	CUDA_ASSERT(cudaMemcpy(image_D, source->data, n_bytes, cudaMemcpyHostToDevice));

  cudaEvent_t start, stop;
  CUDA_ASSERT(cudaEventCreate(&start));
  CUDA_ASSERT(cudaEventCreate(&stop));

  /* Start the timer */
  CUDA_ASSERT(cudaEventRecord(start));

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_cuda_transpose<<<numblocks, blocksize>>>(result_D, image_D,
                                                  source->rowstride,
                                                  0, 0,
                                                  source->width,
                                                  source->height);
      CUDA_ASSERT(cudaGetLastError());
    }

  /* Stop timer */
  CUDA_ASSERT(cudaEventRecord(stop));
  CUDA_ASSERT(cudaEventSynchronize(stop));

  image_t *result = image_new_from_image_T(source);
  
	CUDA_ASSERT(cudaMemcpy(result->data, result_D, n_bytes, cudaMemcpyDeviceToHost));

  float msec = 0;
  
  CUDA_ASSERT(cudaEventElapsedTime(&msec, start, stop));
  
  CUDA_ASSERT(cudaFree(result_D));
  CUDA_ASSERT(cudaFree(image_D));
  
  fprintf(stderr, "elapsed time GPU transpose: %f s\n", msec / 1000.);

  return result;
}


void
run_op_histogram(const image_t *source, const int n_repeat)
{
  /* Allocate buffers to contain initial image and result */
  uint32_t *image_D;
  unsigned int *bins_D;
  unsigned int bins[N_BINS];
	int n_pixels = source->height * source->width;
	int n_bytes = n_pixels * sizeof(uint32_t);
	
	for (int i = 0; i < N_BINS; i++){
		bins[i] = 0;
	}
	
	CUDA_ASSERT(cudaMalloc(&image_D, n_bytes));
	CUDA_ASSERT(cudaMalloc(&bins_D, N_BINS*sizeof(unsigned int)));
	
  /* Calculate the block size and the number of thread blocks */
	const dim3 blocksize(16, 16);
  const dim3 numblocks((source->width % blocksize.x) ?
                          source->width / blocksize.x + 1 :
                          source->width / blocksize.x,
                       (source->height % blocksize.y) ?
                          source->height / blocksize.y + 1 :
                          source->height / blocksize.y);
                       
	/* Copy the input image to the initial buffer */
	CUDA_ASSERT(cudaMemcpy(image_D, source->data, n_bytes, cudaMemcpyHostToDevice));

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  /* Start the timer */
  CUDA_ASSERT(cudaEventRecord(start));

  for (int Z = 0; Z < n_repeat; Z++)
    {
      CUDA_ASSERT(cudaMemcpy(bins_D, bins, N_BINS*sizeof(unsigned int), 
      						cudaMemcpyHostToDevice));
      op_cuda_histogram<<<numblocks, blocksize>>>(bins_D, image_D,
                                                  source->rowstride,
                                                  0, 0,
                                                  source->width,
                                                  source->height);
      CUDA_ASSERT(cudaGetLastError());
    }

  /* Stop timer */
  CUDA_ASSERT(cudaEventRecord(stop));
  CUDA_ASSERT(cudaEventSynchronize(stop));

	CUDA_ASSERT(cudaMemcpy(bins, bins_D, N_BINS*sizeof(unsigned int), cudaMemcpyDeviceToHost));

  float msec = 0;
  
  CUDA_ASSERT(cudaEventElapsedTime(&msec, start, stop));
  fprintf(stderr, "elapsed time GPU histogram: %f s\n", msec / 1000.);

  CUDA_ASSERT(cudaFree(image_D));
  CUDA_ASSERT(cudaFree(bins_D));
  
  /* Print result to stdout. */
  for (int i = 0; i < N_BINS; i++)
    printf("%d ", bins[i]);
  printf("\n");
}

image_t *
run_op_transgram(const image_t *source, const int n_repeat)
{
  /* Allocate buffers to contain initial image and result */
	uint32_t *image_D; uint32_t *result_D;
  unsigned int *bins_D;
  unsigned int bins[N_BINS];
	int n_pixels = source->height * source->width;
	int n_bytes = n_pixels * sizeof(uint32_t);
	
	for (int i = 0; i < N_BINS; i++){
		bins[i] = 0;
	}
	
  /*Copy the input image to the initial buffer */
	CUDA_ASSERT(cudaMalloc(&image_D, n_bytes));
	CUDA_ASSERT(cudaMalloc(&bins_D, N_BINS*sizeof(unsigned int)));
	CUDA_ASSERT(cudaMalloc(&result_D, n_bytes));
  
  /*Calculate the block size and the number of thread blocks */
	const dim3 blocksize(16, 16);
  const dim3 numblocks((source->width % blocksize.x) ?
                          source->width / blocksize.x + 1 :
                          source->width / blocksize.x,
                       (source->height % blocksize.y) ?
                          source->height / blocksize.y + 1 :
                          source->height / blocksize.y);

	/* Copy the input image to the initial buffer */
	CUDA_ASSERT(cudaMemcpy(image_D, source->data, n_bytes, cudaMemcpyHostToDevice));

  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  /* Start the timer */
  CUDA_ASSERT(cudaEventRecord(start));

  for (int Z = 0; Z < n_repeat; Z++)
    {
      CUDA_ASSERT(cudaMemcpy(bins_D, bins, N_BINS*sizeof(unsigned int), 
      						cudaMemcpyHostToDevice));
      
      op_cuda_transpose<<<numblocks, blocksize>>>(result_D, image_D,
                                                  source->rowstride,
                                                  0, 0,
                                                  source->width,
                                                  source->height);
      
      op_cuda_histogram<<<numblocks, blocksize>>>(bins_D, result_D,
                                                  source->rowstride,
                                                  0, 0,
                                                  source->width,
                                                  source->height);
      
      CUDA_ASSERT(cudaGetLastError());
    }

  /* Stop timer */
  CUDA_ASSERT(cudaEventRecord(stop));
  CUDA_ASSERT(cudaEventSynchronize(stop));

  image_t *result = image_new_from_image_T(source);
  
  /*copy results back */
	CUDA_ASSERT(cudaMemcpy(bins, bins_D, N_BINS*sizeof(unsigned int), cudaMemcpyDeviceToHost));
	CUDA_ASSERT(cudaMemcpy(result->data, result_D, n_bytes, cudaMemcpyDeviceToHost));

  float msec = 0;
  CUDA_ASSERT(cudaEventElapsedTime(&msec, start, stop));
  fprintf(stderr, "elapsed time GPU transgram: %f s\n", msec / 1000.);

	CUDA_ASSERT(cudaFree(image_D));
  CUDA_ASSERT(cudaFree(bins_D));
  CUDA_ASSERT(cudaFree(result_D));

  /* Print result to stdout. */
  for (int i = 0; i < N_BINS; i++)
    printf("%d ", bins[i]);
  printf("\n");

  return result;
}

void
run_op_selgray(image_t *source, const int n_repeat)
{
  /*Allocate buffers to contain image */
	uint32_t *image_D;
	int n_pixels = source->height * source->width;
	int n_bytes = n_pixels * sizeof(uint32_t);
	
	CUDA_ASSERT(cudaMalloc(&image_D, n_bytes));
  
  /*Copy the input image to the buffer */
	CUDA_ASSERT(cudaMemcpy(image_D, source->data, n_bytes, cudaMemcpyHostToDevice));
	
  /*Calculate the block size and the number of thread blocks */
	const dim3 blocksize(16, 16);
  const dim3 numblocks((source->width % blocksize.x) ?
                          source->width / blocksize.x + 1 :
                          source->width / blocksize.x,
                       (source->height % blocksize.y) ?
                          source->height / blocksize.y + 1 :
                          source->height / blocksize.y);
  
  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  /* Start the timer */
  CUDA_ASSERT(cudaEventRecord(start));

  for (int Z = 0; Z < n_repeat; Z++)
    {
      op_cuda_selgray<<<numblocks, blocksize>>>(image_D, 
      																					source->rowstride, 
      																					0, 0,
      																					source->width,
      																					source->height,
      																					40, 30);
      
      CUDA_ASSERT(cudaGetLastError());
    }

  /* Stop timer */
  CUDA_ASSERT(cudaEventRecord(stop));
  CUDA_ASSERT(cudaEventSynchronize(stop));
	
	CUDA_ASSERT(cudaMemcpy(source->data, image_D, n_bytes, cudaMemcpyDeviceToHost));
	
	CUDA_ASSERT(cudaFree(image_D));
	
  float msec = 0;
  CUDA_ASSERT(cudaEventElapsedTime(&msec, start, stop));
  fprintf(stderr, "elapsed time GPU selgray: %f s\n", msec / 1000.);
}


image_t *
run_op_light2d(const image_t *source, const int n_repeat)
{
#if 0
  /* TODO: enable once you need this. */
  /* Define two example light sources */
  static const light_t lights[] =
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
#endif

  /* TODO: Allocate buffers to contain initial image and result */

  /* TODO: Copy the input image to the initial buffer */

  /* TODO: Calculate the block size and the number of thread blocks */


  cudaEvent_t start, stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  /* Start the timer */
  CUDA_ASSERT(cudaEventRecord(start));

  for (int Z = 0; Z < n_repeat; Z++)
    {
      /* TODO: call CUDA kernel */
    }

  /* Stop timer */
  CUDA_ASSERT(cudaEventRecord(stop));
  CUDA_ASSERT(cudaEventSynchronize(stop));

  image_t *result = image_new_from_image(source);
  /* TODO: copy back result */

  float msec = 0;
  CUDA_ASSERT(cudaEventElapsedTime(&msec, start, stop));
  fprintf(stderr, "elapsed time GPU light2d: %f s\n", msec / 1000.);

  return result;
}


} /* end extern "C" */
