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
