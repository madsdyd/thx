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

/* **********************************************************************
 * The TMouseInputEvent class (constructor) 
 * Creates an instance, sets attributes
 * *********************************************************************/
TMouseInputEvent::TMouseInputEvent(mousebutton_button_t nbutton,
				   mousebutton_state_t nstate,
				   unsigned int x, unsigned int y)
  : TInputEvent(inputevent_type_pointer) {
  mouse_inputevent_event.x      = x;
  mouse_inputevent_event.y      = y;
  mouse_inputevent_event.button = nbutton;
  mouse_inputevent_event.state  = nstate;
}
/* **********************************************************************
 * Definition of glut callback functions
 * *********************************************************************/

/* **********************************************************************
 * MouseFunc - called when a mouse button is clicked
 * *********************************************************************/
void MouseFunc(int button, int state, int x, int y) {
  mousebutton_button_t tmpbutton;
  mousebutton_state_t  tmpstate;
  switch (button) {
  case GLUT_LEFT_BUTTON:
    tmpbutton = mouse_left;
    break;
  case GLUT_MIDDLE_BUTTON:
    tmpbutton = mouse_middle;
    break;
  case GLUT_RIGHT_BUTTON:
    tmpbutton = mouse_right;
    break;
  default:
    Assert(0, "Unknown GLUT state");
  }
  switch (state) {
  case GLUT_UP:
    tmpstate = mouseup;
      break;
  case GLUT_DOWN:
    tmpstate = mousedown;
    break;
  default:
    Assert(0, "Unknown GLUT state");
  }
  Inputs.Events.push(new TMouseInputEvent(tmpbutton, tmpstate, x, y));
}

/* **********************************************************************
 * This table is used when mapping from GLUT key stuff to our format. 
 * *********************************************************************/


/* **********************************************************************
 * Registering and deregistering mouse handler functions in glut 
 * Registering also disables keyrepeat
 * *********************************************************************/
void inputmouse_init() {
  /* Set the mouse functions */
  glutMouseFunc(MouseFunc);
}

void inputmouse_shutdown() {
  /* Reset the mouse functions */
  glutMouseFunc(NULL);
}

