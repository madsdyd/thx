#ifndef __TYPES_H__
#define __TYPES_H__
/* Various _very_ common stuff
   Please, do not put stuff in here that is not used by nearly all files. */
#include <math.h>
#ifndef M_PI
#define M_PI 3.1415926535897931160
#endif

/* Min and max */
#define mymin(a,b) (((a)<(b))?(a):(b))
#define mymax(a,b) (((a)>(b))?(a):(b))
#define mymax3(a,b,c) (mymax((a),mymax((b),(c))))

#ifndef NULL
#define NULL 0
#endif

/* Time measurement. Under Linux/BSD this will be gettimeofday
   On other systems it should be redefined */
#if 1
#define system_time_t double
#define TIME_NULL 0.0
#else
#error "Non Linux system and system_time_t is not defined"
#endif
system_time_t system_gettime();
#endif
