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
#ifndef __TEXT_HH__
#define __TEXT_HH__
#include <GL/gl.h>
#include <string>

#include "color.hh"

/* Unit to make text wrapper class that maintains state and position, etc. */
class text_t;
class TTextRender {
private:
  text_t * text_render;
  GLint x, y;
  void Draw(const string text);
public:
  bool shadow;
  int size;
  TColor color;
  TTextRender(); 
  ~TTextRender();
  GLint Load(const string filename);
  void Pos(GLint xpos, GLint ypos);
  void PosY(GLint ypos);
  void PosX(GLint xpos);
  /* Draw line at current position, advances position */
  void Print(string text);
  /* Center line on current y position */
  void Center(GLint min, GLint max, string text);
  /* Center line about pivot character */
  void Center(GLint min, GLint max, string text, string pivot);
  /* Center line on current y position, then set y += size */
  void CenterLn(GLint min, GLint max, string text);
  /* Center line about pivot character, then set y += size */
  void CenterLn(GLint min, GLint max, string text, string pivot);
  /* Draw line at current position, set position to y + size */
  void PrintLn(string text);
  /* Returns the x size needed to render the string. 
     The height is the same as size */
  GLint Metric(string text);
};
#endif
