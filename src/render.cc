#include <stdio.h>
#include "render.hh"
#include "textures.hh"

/* This is how we render */
render_type_t render_type = render_type_lines;

/* This is how we shade */
bool render_shade_smooth = true;

/* Should we render normals */
bool render_normals = false;

/* Should we render light */
bool render_light = true;
