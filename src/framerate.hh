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
#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__
/* Functions to calculate the framerate (and maybe display it in various ways */
#include "types.hh"


/* Init the frametime measurements */
void framerate_init();

/* Add a frame time and number
   The oldest time will be replaced*/
void framerate_add(system_time_t time, int framenum);

/* Get a framerate measurement, defined as the difference in framenum between
   the oldest and newest, divided by the time.
   Note: it is assume we _can_ divide by the time. */
double framerate_get();
#endif
