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
