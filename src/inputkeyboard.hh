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
#ifndef __INPUTKEYBOARD_HH__
#define __INPUTKEYBOARD_HH__
/* This is for input events generated from the keyboard */
#include "inputevent.hh"
/* Subtype of event */
typedef enum {keyboard_inputevent_type_down, /* Key pressed */
	      keyboard_inputevent_type_up    /* Key released */
} keyboard_inputevent_type_t;

/* Uniqly define a keyboard event */
typedef struct {
  unsigned char key;
  keyboard_inputevent_type_t type;
} keyboard_inputevent_event_t;

struct lt_kiet
{
  bool operator()(const keyboard_inputevent_event_t k1, 
		  const keyboard_inputevent_event_t k2) const
  {
    return ((k1.key < k2.key) 
      || (k1.key == k2.key 
	  && (k1.type == keyboard_inputevent_type_down 
	      &&  k2.type == keyboard_inputevent_type_up)));
  }
};

/* This class is a keyboard event */
class TKeyboardInputEvent : public TInputEvent {
public:
  keyboard_inputevent_event_t keyboard_inputevent_event;
  /* Constructor takes keyboard event type and key that it relates to */
  TKeyboardInputEvent(keyboard_inputevent_type_t nkeyboard_inputevent,
		      unsigned char nkey);
};
/* Declare a global keyboard handling setup function
   Will register OpenGL/GLUT callbacks (in this case). */
void inputkeyboard_init();
void inputkeyboard_shutdown();
#endif

