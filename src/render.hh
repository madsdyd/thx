#ifndef __RENDER_H__
#define __RENDER_H__
/* This file holds some rendering related options.
   It will probably be removed, whenever I get around to writing
   some console code. Only the map actually uses this stuff at 
   moment
*/
typedef enum { render_type_lines, 
	       render_type_triangles, 
               render_type_textures } render_type_t;
extern render_type_t render_type;

extern bool render_shade_smooth;
extern bool render_normals;
extern bool render_light;
#endif
