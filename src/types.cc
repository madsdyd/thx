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
#include "types.hh"
/* Probably only Linux */
#include <sys/time.h>
#include <unistd.h>
/* Various functions. */

/* ************************************************************
   Get the system time, convert it to a double, return it */
system_time_t system_gettime() {
  /* The return code is ignored */
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tmp;
  gettimeofday(&tmp, NULL);
  return tmp.tv_sec + (tmp.tv_usec) / 1000000.0;
#else
  /* If we do not have gettimeofday, what then? We need some way
     of getting at least milliseconds resolution, and encode the current
     time in a double */
#error "gettimeofday is currently required - look in comments for info"
  return 0;
#endif
}
