#include "types.hh"
/* Probably only Linux */
#include <sys/time.h>
#include <unistd.h>
/* Various functions. */

/* ************************************************************
   Get the system time, convert it to a double, return it */
system_time_t system_gettime() {
  /* The return code is ignored */
  struct timeval tmp;
  gettimeofday(&tmp, NULL);
  return tmp.tv_sec + (tmp.tv_usec) / 1000000.0;
}
