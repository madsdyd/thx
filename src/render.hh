#ifndef __RENDER_H__
#define __RENDER_H__
/* In here goes functions to render the various stuff.
   display related stuff (stuff related to the screen almost) goes into the display
   module.
   I am not entirely sure this is the rigth way to do it, but at least it
   gets done somewhere. */

/* We need to include the header files for the various elements we wish to be
   able to render.
   When drawing elements from the game (map, players, tanks, projectiles, etc) 
   we use parameterised functions - often we may wish to draw several different
   instances and difference between them. For some stuff we need some global
   variables - like a display list for the map - and these can be found in render.c 
   We do need a way to tell if a map was modified, and this goes here.
*/
#include "types.hh"
#include "viewpoint.hh"
#include "map.hh"
#include "tank.hh"
#include "projectile.hh"
#include "explosion.hh"

/* This is how we render */
typedef enum { render_type_lines, 
	       render_type_triangles, 
               render_type_textures } render_type_t;
extern render_type_t render_type;

extern bool render_shade_smooth;
extern bool render_normals;
extern bool render_light;

#endif
