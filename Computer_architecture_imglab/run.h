/*
 * Skeleton code for use with Computer Architecture 2018 assignment 3,
 * LIACS, Leiden University.
 */

#ifndef __RUN_H__
#define __RUN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "image.h"

/* Number of bins for the histogram. */
#define N_BINS 128

/* The prototypes of the run functions for the different image operations
 * that are implemented. "ops-baseline.c" contains the baseline
 * implementation of the actual operations. You will need to create copies
 * of "ops-baseline.c" to implement optimized variants of the image
 * operations.
 */

image_t  *run_op_transpose  (const image_t *source, const int n_repeat);
void      run_op_histogram  (const image_t *source, const int n_repeat);
image_t  *run_op_transgram  (const image_t *source, const int n_repeat);
void      run_op_selgray    (image_t *source, const int n_repeat);
image_t  *run_op_light2d    (const image_t *source, const int n_repeat);

#ifdef __cplusplus
}
#endif

#endif /* ! __RUN_H__ */
