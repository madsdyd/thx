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
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <math.h>
#include <GL/glut.h>
#include "types.hh"
/* The display class - this is kind of an abstraction of the screen or
   window we render into, and have control over the various elements
   that are on screen */
class TViewpoint;
class TConsole;
class TTextRender;
class TDisplay {
public:
  TTextRender * textrender;
  unsigned int num_frames;
  /* We should probably publish these otherwise */
  unsigned int width;
  unsigned int height;
  bool flymode;
  bool clipmode;
  TViewpoint * viewpoint;
  TConsole * console;
  
  TDisplay(int argc, char** argv);
  ~TDisplay();
  void Update(system_time_t deltatime); 
  void Reshape(int w, int h);
  void NormalMode();
  void FlatMode();
  void Render();
  void RefreshRate();
};
extern TDisplay * Display;
#endif
