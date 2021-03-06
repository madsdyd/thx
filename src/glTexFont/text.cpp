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
   This is just a quick port of my glTexfont Library to C++.  Need to change 
some things, should redo the inner loop for text rendering. This thing is 
too SLOW, need to change that.

CHANGES
   06/04/00
      - added some Get* functions
      - fixed not drawing \ 
   06/03/00
      - fixed stupid hang bug when you just have \c or \a \b \i
   06/02/00
      -  !!!!! font size is held over multiple draws, its annoying if not !!!!!
   04/04/00
      - fixed stupid region bug
*/
#include "text.h"

GLfloat white[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat gray[4] = {0.5, 0.5, 0.5, 1.0};

GLint text_t::fontBlockCol = 16;
GLint text_t::fontBlockRow = 16;
GLint text_t::fontGetModes = 1; 
GLint text_t::fontRestoreModes = 2; 
const GLint text_t::fontMaxLen = 1024 * 5;
GLint text_t::fontTabSpace = 4;
GLfloat text_t::fontITOF = (GLfloat) pow (255, -1);
GLint text_t::fontItalic = 8;

GLfloat tPoints[257][2]; 

text_t::text_t()
{
   ColorCopy(fgColor, white); 
   ColorCopy(gdColor, gray);
   ColorCopy(bgColor, gray);
   size = 12;
   shadow = 0;
   gradient = 0;
   italic = 0;
   bold = 0;
   region = 0;;         
   regionX = 0;
   regionY = 0;
   regionW = 0;
   regionH = 0;
   tIncX = 0.0f;
   tIncY = 0.0f;
   blockRow = fontBlockRow;
   blockCol = fontBlockCol;
   texId = 0;
   /* Interspacing */
   gap = 0;
}

void text_t::Reset(void)
{
   //size = 12;
   shadow = 0;
   region = 0;
   gradient = 0;
   italic = 0;
   bold = 0;
   regionX = 0;
   regionY = 0;
   regionW = 0;
   regionH = 0;
   ForeColorReset();
   ShadowColorReset();
   GradientColorReset();
}

GLint text_t::Load(const char *name)
{
   glTga_t image;
   GLint ret;

   image.GenId(1, &texId);

   blockRow = fontBlockRow;
   blockCol = fontBlockCol;
   
   MakeMap();

   if ((ret = image.Load(name, texId, GL_ALPHA)) != 1) 
      return ret;

   return image.Upload(1);
}

GLint text_t::LoadEx (const char *name, GLint row, GLint col)
{
   /* make sure we have valid dimensions */
   if (row * col != 256)
      return 0;
   
   glTga_t image;
   GLint ret;

   image.GenId(1, &texId);

   blockRow = row;
   blockCol = col;

   MakeMap ();

   if ((ret = image.Load(name, texId, GL_ALPHA)) != 1)
      return ret;

   return image.Upload(1);
}

void text_t::DrawChar(char c, GLint x, GLint y, GLint size, GLint shadow)
{
   if (!gradient && !shadow)
      glColor4fv(fgColor);
   else if (!gradient && shadow)
      glColor4fv(bgColor);
   else if (gradient && !shadow)
      glColor4fv(gdColor);

   glBegin (GL_QUADS);
      glTexCoord2f(tPoints[(GLint) c][0] + gap, 
		   tPoints[(GLint) c][1] + gap);
      glVertex2i(x, y);

      glTexCoord2f(tPoints[(GLint) c][0] + tIncX - gap, 
		   tPoints[(GLint) c][1] + gap);
      glVertex2i(x + size, y);

      if (!shadow)
         glColor4fv(fgColor);
      else glColor4fv(bgColor);

      glTexCoord2f(tPoints[(GLint) c][0] + tIncX - gap, 
		   tPoints[(GLint) c][1] + tIncY - gap);
      glVertex2i(x + size + italic, y + size);

      glTexCoord2f(tPoints[(GLint) c][0] + gap, 
		   tPoints[(GLint) c][1] + tIncY - gap);
      glVertex2i(x + italic, y + size);
   glEnd ();
}

void text_t::RenderChar (char c, GLint x, GLint y, GLint size)
{
   if (shadow)
   {
      if (!bold)
         DrawChar(c, x + 1,  y + 1, size, 1);
      else DrawChar(c, x + 2,  y + 1, size, 1);
   }

   DrawChar(c, x, y, size, 0);

   if (bold)
      DrawChar(c, x + 1, y, size, 0);
}

void text_t::ScissorNormal(GLint xpos, GLint ypos, GLint tabs, GLint carrage, GLint size, GLint len)
{
   GLint sy, ex, ey;
   
   ex = len * size * tabs;

   if (carrage)
   {
      sy = ypos - (size * carrage);
      ey = size * (carrage + 1);
   } 
   else
   {
      sy = ypos;
      ey = size;
   }
   
   glScissor(xpos, sy, ex, ey);
}

GLint text_t::SlashParser(char *buffPtr, GLint *x, GLint *y)
{
   GLint ret = 0;

   buffPtr++;

   if (!*buffPtr)
      return ret;

   switch (*buffPtr)
   {
      case 'a':
      case 'c':   
      {
         /*x -= size;
         return SetColorFromToken(buffPtr);*/
         int ret = SetColorFromToken(buffPtr);
         if (ret != -1)
            *x -= size;
         return ret == -1 ? 0 : ret;
      }
      break;
      case 'i':   
      {
         /**x -= size;
         return ItalicsMode(buffPtr);*/
         int ret = ItalicsMode(buffPtr);
         if (ret != -1)
            *x -= size;
         return ret == -1 ? 0 : ret;
      }
      break;
      case 'b':
      {
         /**x -= size;
         return BoldMode(buffPtr);*/
         int ret = BoldMode(buffPtr);
         if (ret != -1)
            *x -= size;
         return ret == -1 ? 0 : ret;
      }
      break;
      default:
         buffPtr--;
         RenderChar(*buffPtr, *x, *y, size);
         return ret;
      break;
   }
}

void text_t::WalkString(char *buffPtr, GLint xpos, GLint ypos, GLint *vPort)
{
   GLint x = xpos;
   GLint y = ypos;
   GLint carrage = 0;
   GLint tabs = 0;
   GLint len = strlen(buffPtr);
   GLint xMax;

   xMax = vPort[0] + vPort[2];

   carrage = GetCharHits(buffPtr, '\n');
   tabs = GetCharHits(buffPtr, '\t');

   if (!tabs)
      tabs = 1;
   else tabs *= fontTabSpace;

   if (region)
   {
      ScissorTextRegion();
      x = regionX;
      y = (regionY + regionH) - size;
   } else ScissorNormal(xpos, ypos, tabs, carrage, size, len);

   /* lets draw! */
   for ( ; *buffPtr; buffPtr++, x += size)
   {
      if (x > xMax)
         break;
   
      if (region)
      {
         if (x + size > (regionX + regionW))
         {
            y -= size;
            x = regionX;
         }
         if (y < regionY)
            break;
      }

      switch (*buffPtr)
      {
         case '\n':
            y -= size;
            if(region)
               x = regionX - size;
            else
               x = xpos - size;
            continue; 
         break;
         
         case '\t':
            x += (size * fontTabSpace);
            continue; 
         break;
      
         case '\\':
         {
            int ret = SlashParser(buffPtr, &x, &y);
            
            buffPtr += ret;

            if (*buffPtr == '\n' || *buffPtr == '\t')
            {
               buffPtr -= 1;
               continue;
            }
      
            if (!ret)
               RenderChar(*buffPtr, x, y, size);
         }
         break;
         default:
            RenderChar(*buffPtr, x, y, size);
         break;
      }
   }
}

void text_t::Draw(GLint xpos, GLint ypos, const char *s, ...)
{
   va_list	msg;
   char buffer[fontMaxLen] = {'\0'};
   GLint vPort[4];

   va_start (msg, s);
#ifdef _WIN32
   _vsntprintf (buffer, fontMaxLen - 1, s, msg);	
#else /* linux */
   vsnprintf (buffer, fontMaxLen - 1, s, msg);	
#endif
   va_end (msg);

   /* get current viewport */
   glGetIntegerv(GL_VIEWPORT, vPort);
   /* setup various opengl things that we need */
   SetModes(fontGetModes);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glBindTexture(GL_TEXTURE_2D, texId);

   /* draw the string */
   WalkString(buffer, xpos, ypos, vPort);

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();   
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   SetModes (fontRestoreModes);
   Reset ();
}

GLint text_t::SetColorFromToken (char *s)
{
   GLint clr[4];
   GLint ret = 1;

   if (*s == 'c')
   {
      s += 1;
      if (sscanf(s, "(%d %d %d)", &clr[0], &clr[1], &clr[2]) != 3)
         return -1;
      Color3f(clr[0] * fontITOF, clr[1] * fontITOF, clr[2] * fontITOF);
   }
   else if (*s == 'a')
   {
      s += 1;
      if (sscanf(s, "(%d %d %d %d)", &clr[0], &clr[1], &clr[2], &clr[3]) != 4)
         return -1;
      Color4f(clr[0] * fontITOF, clr[1] * fontITOF, clr[2] * fontITOF, clr[3] * fontITOF);
   }

   while (*s != ')' && ret ++)
      s += 1;

   return ret + 1;
}

GLint text_t::ItalicsMode (char *s)
{
   s += 1;

   if (*s == '+')
      italic = fontItalic;
   else if (*s == '-')
      italic = 0;
   else return -1;

   return 2;
}

GLint text_t::BoldMode (char *s)
{
   s += 1;

   if (*s == '+')
      bold = 1;
   else if (*s == '-')
      bold = 0;
   else return -1;

   return 2;
}

GLint text_t::GetCharHits(char *s, char f)
{
  GLint hits;
  for (hits = 0; *s; s++)
    if (*s == f)
      hits ++;
  return hits;
}

void text_t::MakeMap(void)
{
#define vCopy(d,x,y) {d[0] = x; d[1] = y;}
   GLint i = 0;
   GLfloat x, y;

   tIncX = (GLfloat)pow (blockCol, -1);
   tIncY = (GLfloat)pow (blockRow, -1);


   for (y = 1 - tIncY; y >= 0; y -= tIncY)
      for (x = 0; x <= 1 - tIncX; x += tIncX, i ++)
         vCopy(tPoints[i], x, y);
#undef vCopy
}

void text_t::SetModes(GLint state)
{
   static GLint matrixMode;
   static GLint polyMode[2];
   static GLint lightingOn;
   static GLint blendOn;
   static GLint depthOn;
   static GLint textureOn;
   static GLint scissorOn;
   static GLint blendSrc;
   static GLint blendDst;

   /* grab the modes that we might need to change */
   if (state == fontGetModes)
   {
      glGetIntegerv(GL_POLYGON_MODE, polyMode);

      if (polyMode[0] != GL_FILL)
         glPolygonMode(GL_FRONT, GL_FILL);
      if (polyMode[1] != GL_FILL)
         glPolygonMode(GL_BACK, GL_FILL);

      textureOn = glIsEnabled(GL_TEXTURE_2D);
         
      if (!textureOn)
         glEnable(GL_TEXTURE_2D);

      depthOn = glIsEnabled(GL_DEPTH_TEST);
   
      if (depthOn)
         glDisable(GL_DEPTH_TEST);

      lightingOn = glIsEnabled(GL_LIGHTING);        

      if (lightingOn) 
         glDisable(GL_LIGHTING);

      scissorOn= glIsEnabled(GL_SCISSOR_TEST);        

      if (!scissorOn) 
         glEnable(GL_SCISSOR_TEST);

      glGetIntegerv(GL_MATRIX_MODE, &matrixMode); 

      /* i don't know if this is correct */
      blendOn= glIsEnabled(GL_BLEND);        
      glGetIntegerv(GL_BLEND_SRC, &blendSrc);
      glGetIntegerv(GL_BLEND_DST, &blendDst);
   
      if (!blendOn)
         glEnable(GL_BLEND);

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }
   else if (state == fontRestoreModes)
   {
      /* put everything back where it was before */
      if (polyMode[0] != GL_FILL)
         glPolygonMode(GL_FRONT, (GLenum) polyMode[0]);
      if (polyMode[1] != GL_FILL)
         glPolygonMode(GL_BACK, (GLenum) polyMode[1]);

      if (lightingOn)
         glEnable(GL_LIGHTING);

      /* i don't know if this is correct */
      if (!blendOn)
      {
         glDisable(GL_BLEND);
         glBlendFunc(blendSrc, blendDst);
      }
      else glBlendFunc(blendSrc, blendDst);

      if (depthOn)
         glEnable(GL_DEPTH_TEST);

      if (!textureOn)
         glDisable(GL_TEXTURE_2D);

      if (!scissorOn) 
         glDisable(GL_SCISSOR_TEST);

      glMatrixMode((GLenum) matrixMode);
   }
}
