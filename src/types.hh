/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arrøe <skjalm@arroe.dk>
    Copyright (C) 2001 contributers of the THX project
    Please see the file "AUTHORS" for a list of contributers

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
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
