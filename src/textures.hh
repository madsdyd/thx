#ifndef __TEXTURES_H__
#define __TEXTURES_H__
/* This module manages textures.
   Methods are available to load a set of textures and select a single one, 
   and only having to bind a texture when it have too. */

#define TEXTUREMOD 8.0
#define TEXCOORD(x) ((float) ((x)/TEXTUREMOD))

/* Initialize the texture system to allow loading of num_textures textures */
void texture_init(int num);

/* There is currently no way to shut down our texture system */

/* Load a single texture from filename 
   Filename is the name, x and y is the size
   Returns the number it has been assigned 
   This function also allocates space, etc, 
   and creates a texture object. */
int texture_load(char * filename, unsigned int x, unsigned int y);

/* Reset the texture set mechanism - this has _nothing_ to with
   freeing textures. */
void texture_reset();

/* Set the texture to num */
void texture_set(int num);


#endif
