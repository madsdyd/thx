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
      - i need to write a document on how to use this stuff...
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
};

struct glTga_t : public tga_t
{
   glTga_t();
   glTga_t(const char *name);
   glTga_t(const char *name, GLuint texId, GLenum nf = (GLenum) 0);
   ~glTga_t() { }
   GLint Load(const char *name);
   GLint Load(const char *name, GLuint texId, GLenum nf = (GLenum) 0);
   GLint LoadBuffer(byte *buff, GLuint texId, GLenum nf = (GLenum) 0);
   GLint LoadBuffer(byte *buff);
   GLint Upload(bool n = 0); // pass a 1 to delete data on upload
   void Reset(void);

   void SetMipMap(GLint  m)      {mipmap = m;}
   int GetMipMap(void)        {return mipmap;}
   void SetMinFilter(GLint m) {minFilter = m;}
   GLint GetMinFilter(void)   {return minFilter;}
   void SetMagFilter(GLint m) {magFilter = m;}
   GLint GetMagFilter(void)   {return magFilter;}
   void SetWrapS(GLint m)     {wrapS = m;}
   GLint GetWrapS(void)       {return wrapS;}
   void SetWrapT(GLint m)     {wrapT = m;}
   GLint GetWrapT(void)       {return wrapT;}
   void SetTexEnv(GLint m)    {texEnv = m;}
   GLint GetTexEnv(void)      {return texEnv;}
   void SetId(GLuint id)      {texid = id;}
   GLuint GetId(void)         {return texid;}
   GLenum GetFormat(void)     {return format;}
   void SetFormat(GLenum f)   {format = f;}
   void SetInternal(GLenum m) {internal = m;}
   GLenum GetInternal(void)   {return (GLenum) internal;}
   void SetupFormat(void); // test
   void GenId(int num, uint *dest) {glGenTextures(num, dest);}
protected:
   GLint CheckSize(int x);

   GLint lastError; 
   GLint mipmap;
   GLint minFilter; // GL_TEXTURE_MIN_FILTER
   GLint magFilter; // GL_TEXTURE_MAG_FILTER
   GLint wrapS;     // GL_TEXTURE_WRAP_S
   GLint wrapT;     // GL_TEXTURE_WRAP_T
   GLint texEnv;

   GLuint texid;    // texture id   
   GLenum format;   // texture format
   GLint internal;  // texture internal format
};

#endif
