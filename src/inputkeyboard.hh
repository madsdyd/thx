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
#include <string>
/* This is for input events generated from the keyboard */
#include "inputevent.hh"
/* Subtype of event */
typedef enum {keydown, /* Key pressed */
	      keyup    /* Key released */
} keypress_t;

/* A list of "normal" keys, that are hard to type */
#define KEY_CTRLA 1
#define KEY_CTRLB 2
#define KEY_CTRLC 3
#define KEY_CTRLD 4
#define KEY_CTRLE 5
#define KEY_CTRLF 6
#define KEY_BACKSPACE 8
#define KEY_CTRLK 11
#define KEY_ENTER '\r'
#define KEY_CTRLT 20
#define KEY_CTRLY 25
#define KEY_ESCAPE 27
#define KEY_DELETE 127

/* A list of special keys */
#define KEY_FIRST 256
#define KEY_UP KEY_FIRST
#define KEY_DOWN (KEY_UP + 1)
#define KEY_LEFT (KEY_DOWN + 1)
#define KEY_RIGHT (KEY_LEFT + 1)
#define KEY_HOME (KEY_RIGHT + 1)
#define KEY_END (KEY_HOME + 1)
#define KEY_PAGEUP (KEY_END + 1)
#define KEY_PAGEDOWN (KEY_PAGEUP + 1)
#define KEY_INSERT (KEY_PAGEDOWN + 1)
#define KEY_F1 (KEY_INSERT + 1)
#define KEY_F2 (KEY_F1 + 1)
#define KEY_F3 (KEY_F2 + 1)
#define KEY_F4 (KEY_F3 + 1)
#define KEY_F5 (KEY_F4 + 1)
#define KEY_F6 (KEY_F5 + 1)
#define KEY_F7 (KEY_F6 + 1)
#define KEY_F8 (KEY_F7 + 1)
#define KEY_F9 (KEY_F8 + 1)
#define KEY_F10 (KEY_F9 + 1)
#define KEY_F11 (KEY_F10 + 1)
#define KEY_F12 (KEY_F11 + 1)
#define KEY_LAST (KEY_F12 + 1)

/* Uniqly define a keyboard event */
class keyboard_inputevent_event_t {
public:
  unsigned int key;
  keypress_t type;
  string AsString(); /* Return the current key as a string */
};

struct lt_kiet
{
  bool operator()(const keyboard_inputevent_event_t k1, 
		  const keyboard_inputevent_event_t k2) const
  {
    return ((k1.key < k2.key) 
      || (k1.key == k2.key 
	  && (k1.type == keydown 
	      &&  k2.type == keyup)));
  }
};

/* This class is a keyboard event */
class TKeyboardInputEvent : public TInputEvent {
public:
  keyboard_inputevent_event_t keyboard_inputevent_event;
  /* Constructor takes keyboard event type and key that it relates to */
  TKeyboardInputEvent(keypress_t nkeyboard_inputevent,
		      unsigned int nkey);
};
/* Declare a global keyboardxo handling setup function
   Will register OpenGL/GLUT callbacks (in this case). */
void inputkeyboard_init();
void inputkeyboard_shutdown();
void inputkeyboard_disablerepeat();
void inputkeyboard_enablerepeat();
void inputkeyboard_restorerepeat();
#endif

