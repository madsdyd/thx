/*
   Copyright (C) 2000 Nate Miller nkmiller@calpoly.edu

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License,                                                                   or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   See gpl.txt for more information regarding the GNU General Public License.
*/

#ifndef __TEXTH__
#define __TEXTH__

#include "gltga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#endif
#include <GL/gl.h>

extern GLfloat white[4];
extern GLfloat gray[4];

struct text_t
{
   /* Interspacing */
   GLfloat gap;
   text_t();
   GLint Load(const char *name);
   GLint LoadEx(const char *name, GLint row, GLint col);
   void Draw(GLint xpos, GLint ypos, const char *s, ...);
   void Region(GLint x, GLint y, GLint w, GLint h)
    {region = 1; regionX = x; regionY = y - h; regionW = w; regionH = h;}
   void Size(GLint sze) {size = sze;}
   void Shadow(void) {shadow = 1;}
   void Gradient(void) {gradient = 1;}
   void Color3f(GLfloat r, GLfloat g, GLfloat b)
    {fgColor[0] = r; fgColor[1] = g; fgColor[2] = b; fgColor[3] = 1.0;}
   void Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {fgColor[0] = r; fgColor[1] = g; fgColor[2] = b; fgColor[3] = a;}
   void Color3fv(GLfloat *clr)
    {fgColor[0] = clr[0]; fgColor[1] = clr[1]; fgColor[2] = clr[2];
     fgColor[3] = 1.0;}
   void Color4fv(GLfloat *clr)
    {fgColor[0] = clr[0]; fgColor[1] = clr[1]; fgColor[2] = clr[2];
     fgColor[3] = clr[3];}
   void ShadowColor3f(GLfloat r, GLfloat g, GLfloat b)
    {bgColor[0] = r; bgColor[1] = g; bgColor[2] = b; bgColor[3] = 1.0;}
   void ShadowColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {bgColor[0] = r; bgColor[1] = g; bgColor[2] = b; bgColor[3] = a;}
   void ShadowColor3fv(GLfloat *clr)
    {bgColor[0] = clr[0]; bgColor[1] = clr[1]; bgColor[2] = clr[2];
     bgColor[3] = 1.0;}
   void ShadowColor4fv(GLfloat *clr)
    {bgColor[0] = clr[0]; bgColor[1] = clr[1]; bgColor[2] = clr[2];
     bgColor[3] = clr[3];}
   void GradientColor3f(GLfloat r, GLfloat g, GLfloat b)
    {gdColor[0] = r; gdColor[1] = g; gdColor[2] = b; gdColor[3] = 1.0;}
   void GradientColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {gdColor[0] = r; gdColor[1] = g; gdColor[2] = b; gdColor[3] = a;}
   void GradientColor3fv(GLfloat *clr)
    {gdColor[0] = clr[0]; gdColor[1] = clr[1]; gdColor[2] = clr[2];
     gdColor[3] = 1.0;}
   void GradientColor4fv(GLfloat *clr)
    {gdColor[0] = clr[0]; gdColor[1] = clr[1]; gdColor[2] = clr[2];
     gdColor[3] = clr[3];}

   GLint GetSize(void) {return size;}

protected:
   void Reset(void);
   void ColorCopy(GLfloat *dest, GLfloat *src)
    {dest[0] = src[0]; dest[1] = src[1]; dest[2] = src[2]; dest[3] = src[3];}
   void DrawChar(char c, GLint x, GLint y, GLint size, GLint shadow);
   void RenderChar(char c, GLint x, GLint y, GLint size);
   void ScissorNormal(GLint xpos, GLint ypos, GLint tabs, GLint carrage,
    GLint size, GLint len);
   void ScissorTextRegion(void)
    {glScissor(regionX, regionY, regionW, regionH);}
   void ForeColorReset(void) {ColorCopy(fgColor, white);}
   void ShadowColorReset(void) {ColorCopy(bgColor, gray);}
   void GradientColorReset(void) {ColorCopy(gdColor, gray);}
   GLint SlashParser (char *buffPtr, GLint *x, GLint *y);
   void WalkString(char *buffPtr, GLint xpos, GLint ypos, GLint *vPort);
   GLint SetColorFromToken(char *s);
   GLint ItalicsMode(char *s);
   GLint BoldMode(char *s);
   GLint GetCharHits(char *s, char f);
   void MakeMap(void);
   void SetModes(GLint state);

   static GLint fontBlockCol;
   static GLint fontBlockRow;
   static GLint fontGetModes; 
   static GLint fontRestoreModes; 
   static const GLint fontMaxLen;
   static GLint fontTabSpace;
   static GLfloat fontITOF;
   static GLint fontItalic;

   GLfloat fgColor[4];   /* foreground color, default white */
   GLfloat gdColor[4];   /* gradient color, default gray */
   GLfloat bgColor[4];   /* background color, default gray */
   GLint size;           /* size of text, default 12 */
   GLint shadow;         /* shadow text? default 0 */
   GLint gradient;       /* gradient? default 0 */
   GLint italic;         /* italic amount, defaul 0 */
   GLint bold;           /* bold text? */
   GLint region;         /* do we have a text region */
   GLint regionX;        /* lower left x */
   GLint regionY;        /* lower left y */
   GLint regionW;        /* text region w */
   GLint regionH;        /* text region h */
   GLfloat tIncX;        /* used for texture coords, x axis amount to move */
   GLfloat tIncY;        /* used for texture coords, y axis amount to move */
   GLint blockRow;       /* characters per row */
   GLint blockCol;       /* characters per col */
   GLuint texId;         /* texture id */
   
};

#endif
