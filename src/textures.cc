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
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "textures.hh"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

/* Data structures for the texture subsystem 
   We load textures as unsigned bytes. */
typedef struct {
  int x, y;
  GLuint gl_name; /* OpenGL name for the texture */
  GLubyte * data; /* Data is unsigned bytes */
} texture_t;

/* Our textures */
texture_t * textures = NULL;

/* The number of textures and the max number of textures */
int num_textures, max_num_textures;

/* The last texture we used */
int last_tex = -1;

/* **********************************************************************
   Initialize the texture system to allow loading of num_textures textures */
void texture_init(int num) {
  /* Set up our normal stuff */
  textures         = (texture_t *) malloc(num*sizeof(texture_t));
  last_tex         = -1;
  num_textures     = 0;
  max_num_textures = num;
  /* Enable textures */
  //  glEnable(GL_TEXTURE_2D);

  /* Allocate OpenGL texture names. */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  {
    int i;
    for (i = 0; i < num; i++) {
      // GLuint * texnames = (GLuint *) malloc(num * sizeof(GLuint));
      // glGenTextures(num, texnames);
      
      /* Set the name of the textures */
      //textures[i].gl_name = texnames[i];
      glGenTextures(1, &(textures[i].gl_name));
      // printf("Setting texture %i to %i\n", i, textures[i].gl_name);
      // glBindTexture(GL_TEXTURE_2D, textures[i].gl_name);
    }
    //free(texnames);
  }
}

/* **********************************************************************
   Load a single texture from filename 
   Filename is the name, x and y is the size
   Returns the number it has been assigned 
   This function also allocates space, etc, 
   and creates a texture object. */
int texture_load(char * filename, unsigned int x, unsigned int y) {
  /* Test if we have space for a texture */
  if (num_textures >= max_num_textures) {
    return -1;
  }
  /*
  printf("Load texture into texture number %i, named %i = %s\n",
	 num_textures, 
	 textures[num_textures].gl_name, 
	 filename);
  */  
  /* Allocate space for data - bytes are rgb */
  textures[num_textures].data = (GLubyte *) malloc(x*y*3*sizeof(GLubyte));

  /* Try and open the filename */
  {
    FILE *fh;
    if (!(fh = fopen(filename, "rb")) 
	|| fread(textures[num_textures].data, 3, x*y, fh) != x*y) {
      fprintf(stderr, "Error reading file %s\n", filename);
      free(textures[num_textures].data);
      return -2;
    }
    /* Close the filehandle */
    fclose(fh);
  }

  /* Set up the OpenGL stuff */
  // printf("Binding to texture named %i\n", textures[num_textures].gl_name);

  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition before glBindTexture" << endl;
  } 
  glBindTexture(GL_TEXTURE_2D, textures[num_textures].gl_name);
  
  // printf("Setting parameters for texture named %i\n", textures[num_textures].gl_name);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition before wrapping" << endl;
  } 

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition before filtering" << endl;
  } 
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition after mag filter" << endl;
  } 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition after min filter" << endl;
  } 

  /* TODO: MipMap textures (of course) */
  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 
  //	       0, GL_RGB, GL_UNSIGNED_BYTE, textures[num_textures].data);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition before gluBuild2DMipmaps" << endl;
  } 
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, x, y, GL_RGB, GL_UNSIGNED_BYTE,
		    textures[num_textures].data);
  if (GL_NO_ERROR != glGetError()) {
    cerr << "texture_load : GL was in error condition after gluBuild2DMipmaps" << endl;
  } 
  
  /* Raise the number of loaded textures and return the current number */
  num_textures++;
  return (num_textures-1);
}

/* **********************************************************************
   Reset the texture set mechanism */
void texture_reset() {
  last_tex = -1;
}

/* **********************************************************************
   Set the texture to num. */ 
void texture_set(int num) {
  if (last_tex != num) {
    glBindTexture(GL_TEXTURE_2D, textures[num].gl_name);
    last_tex = num;
    // printf("Setting texture to %i\n", num);
  }
}
