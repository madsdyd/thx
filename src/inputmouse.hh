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
#ifndef __INPUTMOUSE_HH__
#define __INPUTMOUSE_HH__
/* This is for input events generated from the mouse */
#include "inputevent.hh"

/* What was the reason for the event. 
   A mousedrag is when a move happens with a button pressed */
typedef enum {mouseup, 
	      mousedown,
	      mousemove
} mousebutton_action_t;

/* Is any button pressed */
typedef enum {mouse_none,
	      mouse_left,
	      mouse_middle, 
	      mouse_right
} mousebutton_button_t;


/* Uniqly define a mouse event for mappings. */
typedef struct {
  mousebutton_action_t action;
  mousebutton_button_t button;
} mouse_inputevent_event_t;

/* Use this for comparing */
struct lt_miet
{
  bool operator()(const mouse_inputevent_event_t m1, 
		  const mouse_inputevent_event_t m2) const
  {
    return ((m1.action < m2.action) 
      || (m1.action == m2.action 
	  && (m1.button < m2.button)));
  }
};

/* This class is a mouse event */
class TMouseInputEvent : public TInputEvent {
public:
  mouse_inputevent_event_t mouse_inputevent_event;
  /* These are used for up, down, move and drags */
  unsigned int x;
  unsigned int y;
  /* These are only used for move and drags */
  unsigned int oldx;
  unsigned int oldy;
  /* Constructor takes mouse event type and key that it relates to */
  TMouseInputEvent(mousebutton_action_t naction,
		   mousebutton_button_t nbutton,
		   unsigned int nx, unsigned int ny,
		   unsigned int noldx, unsigned int noldy);
};
/* Declare a global mouse handling setup function
   Will register OpenGL/GLUT callbacks (in this case). */
void inputmouse_init();
void inputmouse_shutdown();
#endif

