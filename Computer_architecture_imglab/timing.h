/*
 * Skeleton code for use with Computer Architecture 2018 assignment 3,
 * LIACS, Leiden University.
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include <time.h>

int    timespec_subtract  (struct timespec *result,
                           struct timespec *x,
                           struct timespec *y);
void   print_elapsed_time (const char      *description,
                           struct timespec *end_time,
                           struct timespec *start_time);


/* Important: for get_time() it is important to select the correct
 * clock:
 *  - CLOCK_PROCESS_CPUTIME_ID returns "CPU time" which includes
 *    the CPU time consumed by all threads in the process. So for
 *    a multi-threaded code this might not give the timing information
 *    you want.
 *  - CLOCK_REALTIME gives wall-clock time, but may be less accurate.
 *  - CLOCK_THREAD_CPUTIME_ID returns thread-specific CPU time, which
 *    should be fine when run from the master (main) thread since all
 *    OpenMP threads should join here before reading the end time.
 */

static inline void
get_time(struct timespec *t)
{
#ifdef ENABLE_TIMING
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, t);
#endif
}

#endif /* ! __TIMING_H__ */
