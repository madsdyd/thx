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
/* This is the implementation of the mouse input to event handling */
#include <GL/glut.h>
#include <map>
#include "debug.hh"
#include "inputmouse.hh"

/* Sadly, this unit needs to know stuff about the display - I could
   retrieve it from glut, but this is easier */
#include "display.hh"

/* This is the centermode toggle */
static bool centermode = false;

/* **********************************************************************
 * The TMouseInputEvent class (constructor) 
 * Creates an instance, sets attributes
 * *********************************************************************/
TMouseInputEvent::TMouseInputEvent(mousebutton_action_t naction,
				   mousebutton_button_t nbutton,
				   unsigned int nx, unsigned int ny,
				   unsigned int noldx, unsigned int noldy)
  : TInputEvent(inputevent_type_pointer) {
  mouse_inputevent_event.action = naction;
  mouse_inputevent_event.button = nbutton;
  x                             = nx;
  y                             = ny;
  oldx                          = noldx;
  oldy                          = noldy;
}
/* **********************************************************************
 * Definition of glut callback functions
 * *********************************************************************/

/* **********************************************************************
 * This is a global variable, that holds the last known state of the mouse
 * *********************************************************************/
static unsigned int oldx = 0;
static unsigned int oldy = 0;
static mousebutton_button_t last_button = mouse_none;

/* **********************************************************************
 * MouseFunc - called when a mouse button is clicked
 * *********************************************************************/
void MouseFunc(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    last_button = mouse_left;
    break;
  case GLUT_MIDDLE_BUTTON:
    last_button = mouse_middle;
    break;
  case GLUT_RIGHT_BUTTON:
    last_button = mouse_right;
    break;
  default:
    Assert(0, "MouseFunc: Unknown GLUT button pressed");
  }
  /* Push an event, store the new values as old */
  if (GLUT_DOWN == state) {
    Inputs.Events.push(new TMouseInputEvent(mousedown, last_button, 
					    x, y, oldx, oldy));
  } else {
    Inputs.Events.push(new TMouseInputEvent(mouseup, last_button, 
					    x, y, oldx, oldy));
  }
  oldx = x;
  oldy = y;
}

/* **********************************************************************
 * MouseFunc - called by both passive and motion func
 * *********************************************************************/
void MouseFunc(unsigned int x, unsigned int y, 
	       mousebutton_button_t button) {
  bool center = centermode && Display->GrabbingPointer;
  int x2 = Display->GetWidth()/2;
  int y2 = Display->GetHeight()/2;

  /* This is a hack - if we are in centermode, and the source and
     destination is the center of the screen, then ignore the event */
  if (center
      && x == oldx && y == oldy && x == x2 && y == y2) {
    return;
  }
  /* If we are in centermode, push event that is relative to center,
     otherwise push true event*/
  /*  if (center) {
      Inputs.Events.push(new TMouseInputEvent(mousemove, button, 
      x, y, oldx, oldy));  
      } else { */
  Inputs.Events.push(new TMouseInputEvent(mousemove, button, 
					  x, y, oldx, oldy));  
  //  }
  oldx = x;
  oldy = y;
  /* Make sure we warp back */
  if (center) {
    glutWarpPointer(x2, y2);
  }
}
/* **********************************************************************
 * PassiveMotionFunc - called when the pointer is moved, with no
 * buttons pressed
 * *********************************************************************/
void PassiveMotionFunc(int x, int y) {
  MouseFunc((unsigned int) x, (unsigned int) y, mouse_none);
}

/* **********************************************************************
 * MotionFunc - called when the pointer is moved, with a button pressed.
 * Note, this function will not work with multiple buttons pressed. 
 * *********************************************************************/
void MotionFunc(int x, int y) {
  MouseFunc((unsigned int) x, (unsigned int) y, last_button);
}

/* **********************************************************************
 * EntryFunc - called when the mouse enter/leaves our window
 * *********************************************************************/
void EntryFunc(int state) {
  /* If the mouse left us, quickly return it to the last known state. */
  if (Display->GrabbingPointer() && GLUT_LEFT == state) {
    glutWarpPointer(oldx, oldy);
  }
}

/* ***************************************************************a*******
 * Registering and deregistering mouse handler functions in glut 
 * Registering also warps the pointer to the center of the display
 * *********************************************************************/
void inputmouse_init() {
  /* Set the mouse functions */
  glutMouseFunc(MouseFunc);
  glutPassiveMotionFunc(PassiveMotionFunc);
  glutMotionFunc(MotionFunc);
  glutSetCursor(GLUT_CURSOR_NONE);
  glutEntryFunc(EntryFunc);
  /* Warp the pointer to the center of our display to start with */
  glutWarpPointer(Display->GetWidth()/2, Display->GetHeight()/2);
}

void inputmouse_shutdown() {
  /* Reset the mouse functions */
  glutEntryFunc(NULL);
  glutSetCursor(GLUT_CURSOR_INHERIT);
  glutMotionFunc(NULL);
  glutPassiveMotionFunc(NULL);
  glutMouseFunc(NULL);
}
/* **********************************************************************
 * Center or normal mode
 * *********************************************************************/
void inputmouse_centermode() {
  centermode = true;
};
void inputmouse_normalmode() {
  centermode = false;
};
