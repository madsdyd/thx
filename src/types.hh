#ifndef __TYPES_H__
#define __TYPES_H__
/* Various common stuff */
#include <math.h>
#include <list>
#include <vector>

#ifndef M_PI
#define M_PI 3.1415926535897931160
#endif
#include <GL/glut.h>

/* Not so common stuff, used by SUC (Skjalm's Ugly Code ;-) */
#include "triangle.hh"
#include "point.hh"

/* Min and max */
#define mymin(a,b) (((a)<(b))?(a):(b))
#define mymax(a,b) (((a)>(b))?(a):(b))
#define mymax3(a,b,c) (mymax((a),mymax((b),(c))))

#ifndef NULL
#define NULL 0
#endif

#define GRAVITY 9.82
//#define GRAVITY 15

/* OpenGL color */
typedef struct {
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} color_t;

/* Vector is 3 floats */
typedef struct {
  float x, y, z;
} vector_t;

/* Time measurement. Under Linux/BSD this will be gettimeofday
   On other systems it should be redefined */
#if 1
#define system_time_t double
#define TIME_NULL 0.0
#else
#error "Non Linux system and system_time_t is not defined"
#endif

void color_setcolor(color_t * color, GLfloat red, GLfloat green, 
		    GLfloat blue, GLfloat alpha);

/* Methods that sets a color to a specific color */
void color_setwhite(color_t * color);
void color_setblack(color_t * color);
void color_setyellow(color_t * color);
void color_setred(color_t * color);
void color_setgreen(color_t * color);
void color_setblue(color_t * color);
void color_setmagenta(color_t * color);

system_time_t system_gettime();
 

/* Typedefs used by TObject (i.e. more stuff from Skjalm's Ugly Code) */
typedef Triangle* TrianglePtr;
typedef Point* PointPtr;
typedef list<TrianglePtr> TrianglePtrList;
typedef list<TrianglePtr>::iterator CI;

#endif
