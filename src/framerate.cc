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
#include "framerate.hh"

/* Variables and types local to this module */
#define MAXFRAMETIMES 5

typedef struct {
  int framenum;
  system_time_t time;
} frameinfo_t;

frameinfo_t frameinfos[MAXFRAMETIMES];
int oldest_frame;
int newest_frame;

/* ************************************************************
   Init the frametimes array */
void framerate_init() {
  int i; 
  for (i = 0; i < MAXFRAMETIMES; i++) {
    frameinfos[i].time     = TIME_NULL;
    frameinfos[i].framenum = 0;
  }
  oldest_frame = 0;
  newest_frame = 0;
}

/* ************************************************************
   Add a measurement */
void framerate_add(system_time_t time, int framenum) {
  frameinfos[oldest_frame].time     = time;
  frameinfos[oldest_frame].framenum = framenum;
  newest_frame = oldest_frame;
  oldest_frame = (newest_frame + 1) % MAXFRAMETIMES;
}

/* ************************************************************
   Get a measurement */
double framerate_get() {
  if (frameinfos[oldest_frame].time != frameinfos[newest_frame].time) {
    return ((frameinfos[oldest_frame].framenum - frameinfos[newest_frame].framenum)
	    /(frameinfos[oldest_frame].time - frameinfos[newest_frame].time));
  } else {
    return 0.0;
  }
}
