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
#include "commandconsumer.hh"
/* The display class - this is kind of an abstraction of the screen or
   window we render into, and have control over the various elements
   that are on screen */
class TViewpoint;
class TConsole;
class TTextRender;
class TDisplay : public TCommandConsumer {
private:
  /* Wheter or not we grab the pointer */
  bool grabpointer; 
  unsigned int width;
  unsigned int height;
  bool flymode;
  bool clipmode;
  unsigned int num_frames;
public:
  TTextRender * textrender;

  /* We should probably publish these in another way */  
  TViewpoint * viewpoint;
  TConsole * console;

  /* **********************************************************************
   * Construction and such.
   * *********************************************************************/
  
  TDisplay(int argc, char** argv);
  virtual ~TDisplay();
  
  /* **********************************************************************
   * Information about the display
   * *********************************************************************/
  unsigned int GetWidth();
  unsigned int GetHeight();
  unsigned int GetNumFrames();
  bool GrabbingPointer();

  /* **********************************************************************
   * This can be used be the players to enforce rules about viewpoint 
   * placement, onto the current viewpoint. 
   * *********************************************************************/
  void UpdateViewpoint();

  /* **********************************************************************
   * Call backs
   * *********************************************************************/

  void Update(system_time_t deltatime); 
  void Reshape(int w, int h);

  /* **********************************************************************
   * Render related functions
   * *********************************************************************/

  void NormalMode(); /* Sets up a 3D projection */
  void FlatMode();   /* Sets up a 2D projection for menus, etc */
  void Render();     /* Actually render everything the display knows about */
  void RefreshRate();/* Perform a refreshrate test */

  /* **********************************************************************
   * CommandConsume
   * *********************************************************************/
  virtual bool CommandConsume(TCommand * Command);
};
extern TDisplay * Display;
#endif
