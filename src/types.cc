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

#ifdef NEVER
/* ************************************************************
   Set a color value */
void color_setcolor(color_t * color, GLfloat red, GLfloat green, 
		    GLfloat blue, GLfloat alpha) {
  color->red   = red; color->green = green;
  color->blue  = blue; color->alpha = alpha;
} 

/* ************************************************************
   Set a color to white */
void color_setwhite(color_t * color) {
  color->red   = 1.0; color->green = 1.0;
  color->blue  = 1.0; color->alpha = 1.0;
}

/* ************************************************************
   Set a color to yellow */
void color_setyellow(color_t * color) {
  color->red   = 1.0; color->green = 1.0;
  color->blue  = 0.0; color->alpha = 1.0;
}

/* ************************************************************
   Set a color to black */
void color_setblack(color_t * color) {
  color->red   = 0.0; color->green = 0.0;
  color->blue  = 0.0; color->alpha = 0.0;
}

/* ************************************************************
   Set a color to red */
void color_setred(color_t * color) {
  color->red   = 1.0; color->green = 0.0;
  color->blue  = 0.0; color->alpha = 1.0;
}

/* ************************************************************
   Set a color to green */
void color_setgreen(color_t * color) {
  color->red   = 0.0; color->green = 1.0;
  color->blue  = 0.0; color->alpha = 1.0;
}

/* ************************************************************
   Set a color to blue */
void color_setblue(color_t * color) {
  color->red   = 0.0; color->green = 0.0;
  color->blue  = 1.0; color->alpha = 1.0;
}
#endif
