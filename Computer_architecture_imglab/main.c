/*
 * Skeleton code for use with Computer Architecture 2018 assignment 3,
 * LIACS, Leiden University.
 */

#include "run.h"
#include "timing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define N_REPEAT 5


typedef enum
{
  OP_UNKNOWN = 0,
  OP_TRANSPOSE,
  OP_TRANSGRAM, /* Transpose immediately followed by histogram */
  OP_HISTOGRAM,
  OP_SELGRAY,
  OP_LIGHT2D
}
op_t;

op_t
translate_op(const char *str)
{
  /* Keeping this simple for now ... */
  if (!strncmp(str, "transpose", 9))
    return OP_TRANSPOSE;
  else if (!strncmp(str, "histogram", 9))
    return OP_HISTOGRAM;
  else if (!strncmp(str, "transgram", 9))
    return OP_TRANSGRAM;
  else if (!strncmp(str, "selgray", 7))
    return OP_SELGRAY;
  else if (!strncmp(str, "light2d", 7))
    return OP_LIGHT2D;

  return OP_UNKNOWN;
}


static image_t *
run_op(image_t *source, const op_t op, const int n_repeat)
{
  switch (op)
    {
      case OP_TRANSPOSE:
        return run_op_transpose(source, n_repeat);

      case OP_HISTOGRAM:
        run_op_histogram(source, n_repeat);
        return NULL;

      case OP_TRANSGRAM:
        return run_op_transgram(source, n_repeat);

      case OP_SELGRAY:
        run_op_selgray(source, n_repeat);
        return source; /* in-place image operation */

      case OP_LIGHT2D:
        return run_op_light2d(source, n_repeat);

      default:
        /* Fall through */
        break;
    }

  /* Cannot be reached */
  assert("Should not be reached" && 0);
  return NULL;
}

/*
 * main
 */

int
main(int argc, char **argv)
{
  if (argc < 2)
    {
      fprintf(stderr, "usage: %s <op> <infile> [outfile]\n", argv[0]);
      fprintf(stderr, "\n  where <infile> and [outfile] are PNG files.");
      fprintf(stderr, "\n  and <op> is one of transpose, histogram, transgram, selgray, light2d\n");
      fprintf(stderr, "\n  [outfile] is an optional parameter.\n");
      return -1;
    }

  const char *op_str = argv[1];
  const char *infile = argv[2];
  const char *outfile = argv[3];

  op_t op = translate_op(op_str);
  if (op == OP_UNKNOWN)
    {
      fprintf(stderr, "'%s' is not a valid operation.\n", op_str);
      return -1;
    }

  /* Load PNG image */
  struct timespec start_time, end_time;
  get_time(&start_time);

  image_t *image = image_new_from_pngfile(infile);
  if (!image)
    return -1;

  get_time(&end_time);
  print_elapsed_time("file load", &end_time, &start_time);

  /* Execute */
  image_t *result = NULL;
  result = run_op(image, op, N_REPEAT);

  /* Save result if desired and if applicable */
  int retval = 0;
  if (outfile && result && !image_save_as_pngfile(result, outfile))
    retval = -1;

  image_free(image);

  /* Guard for in-place operations */
  if (result != image)
    image_free(result);

  return retval;
}
