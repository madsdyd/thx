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
