/*
   Copyright (C) 2000 Nate Miller nkmiller@calpoly.edu

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   See gpl.txt for more information regarding the GNU General Public License.
*/
/*
   TODO
      - no support for GL_TEXTURE_BORDER_COLOR, GL_TEXTURE_PRIORITY yet...
      - still a work in progress, things will change
*/
#ifndef __GLTGAH__
#define __GLTGAH__

#include "tga.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

enum
{
   badUpload = badData + 1,
   badDimension,
   maxTexSize = 1024, // dont hardcode
   imageRelease = 1,
};

struct glTga_t : public tga_t
{
   glTga_t();
   glTga_t(char *name);
   glTga_t(char *name, int texId, GLenum nf = (GLenum) 0);
   ~glTga_t() { }
   int Load(char *name);
   int Load(char *name, GLint texId, GLenum nf = (GLenum) 0);
   int Upload(int n = 0);
   void Reset(void);
   void Delete(void) {delete [] data; data = 0;}

   void MipMap(int m)      {mipmap = m;}
   int MipMap(void)        {return mipmap;}
   void MinFilter(GLint m) {minFilter = m;}
   GLint MinFilter(void)   {return minFilter;}
   void MagFilter(GLint m) {magFilter = m;}
   GLint MagFilter(void)   {return magFilter;}
   void WrapS(GLint m)     {wrapS = m;}
   GLint WrapS(void)       {return wrapS;}
   void WrapT(GLint m)     {wrapT = m;}
   GLint WrapT(void)       {return wrapT;}
   void TexEnv(GLint m)    {texEnv = m;}
   GLint TexEnv(void)      {return texEnv;}
   void Id(GLint id)       {texid = id;}
   GLint Id(void)          {return texid;}
   GLenum Format(void)     {return format;}
   void Format(GLenum f)   {format = f;}
   void Internal(GLenum m) {internal = m;}
   GLenum Internal(void)   {return (GLenum) internal;}
   void GenId(int num, uint *dest) {glGenTextures(num, dest);}

protected:
   int CheckSize(int x);

   int lastError; 
   int mipmap;
   GLint minFilter; // GL_TEXTURE_MIN_FILTER
   GLint magFilter; // GL_TEXTURE_MAG_FILTER
   GLint wrapS;     // GL_TEXTURE_WRAP_S
   GLint wrapT;     // GL_TEXTURE_WRAP_T
   GLint texEnv;

   GLint texid;     // texture id   
   GLenum format;   // texture format
   GLint internal;  // texture internal format
};

#endif
