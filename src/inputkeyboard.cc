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
/* This is the implementation of the keyboard input to event handling */
#include <GL/glut.h>
#include "inputkeyboard.hh"
/* **********************************************************************
 * The TKeyboardInputEvent class (constructor) 
 * Creates an instance, sets attributes
 * *********************************************************************/
TKeyboardInputEvent::TKeyboardInputEvent(keyboard_inputevent_type_t nkeyboard_inputevent,
					 unsigned char nkey) 
  : TInputEvent(inputevent_type_keyboard) {
  keyboard_inputevent = nkeyboard_inputevent;
  key                 = nkey;
}
/* **********************************************************************
 * Definition of glut callback functions
 * *********************************************************************/

/* **********************************************************************
 * KeyboardFunc - called when a normal key is pressed 
 * Note that x,y is thrown away here. This may be stupid
 * *********************************************************************/
void KeyboardFunc(unsigned char key, int x, int y) {
  Inputs.Events.push(new TKeyboardInputEvent(keyboard_inputevent_type_down,
					     key));
}

/* **********************************************************************
 * Registering and deregistering keyboard handler functions in glut 
 * Registering also disables keyrepeat
 * *********************************************************************/
void inputkeyboard_init() {
  /* Disable global keyrepeat */
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  
  /* Set the keyboard functions */
  glutKeyboardFunc(KeyboardFunc);
}

void inputkeyboard_shutdown() {
  /* Reset the keyboard functions */
  glutKeyboardFunc(NULL);

  /* Reset global keyrepeat */
  glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
}
