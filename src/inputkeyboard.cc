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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <map>
using std::map;

#include <GL/glut.h>
#include "inputkeyboard.hh"

/* **********************************************************************
 * AsString return a string describing the key
 * *********************************************************************/
/* Define a static array of mappings. */
static struct {
  unsigned int key;
  char * name;
} keynames[] = {
  {1, "^A"},
  {2, "^B"},
  {3, "^C"},
  {4, "^D"},
  {5, "^E"},
  {6, "^F"},
  {7, "^G"},
  {8, "BACKSPACE"},
  {9, "TAB"},
  {10, "RETURN"},
  {11, "^K"},
  {12, "^L"},
  {13, "ENTER"},
  {14, "^N"},
  {15, "^O"},
  {16, "^P"},
  {17, "^Q"},
  {18, "^R"},
  {19, "^S"},
  {20, "^T"},
  {21, "^U"},
  {22, "^V"},
  {23, "^W"},
  {24, "^X"},
  {25, "^Y"},
  {26, "^Z"},
  {27, "ESC"},
  {28, "FS"},
  {29, "GS"},
  {30, "RS"},
  {31, "US"},
  {32, "SPACE"},
  {127, "DEL"},
  {KEY_UP, "UP"},
  {KEY_DOWN, "DOWN"},
  {KEY_LEFT, "LEFT"},
  {KEY_RIGHT, "RIGHT"},
  {KEY_HOME, "HOME"},
  {KEY_END, "END"},
  {KEY_PAGEUP, "PAGEUP"},
  {KEY_PAGEDOWN, "PAGEDOWN"},
  {KEY_INSERT, "INSERT"},
  {KEY_F1, "F1"},
  {KEY_F2, "F2"},
  {KEY_F3, "F3"},
  {KEY_F4, "F4"},
  {KEY_F5, "F5"},
  {KEY_F6, "F6"},
  {KEY_F7, "F7"},
  {KEY_F8, "F8"},
  {KEY_F9, "F9"},
  {KEY_F10, "F10"},
  {KEY_F11, "F11"},
  {KEY_F12, "F12"},
  {0, 0}
};

typedef map <int, string> TKeynameMap;
typedef TKeynameMap::iterator TKeynameMapIterator;
TKeynameMap keys_to_names;

string keyboard_inputevent_event_t::AsString() const {
  /* Check if this is a normal key */
  if (key < 127 && key > 32) {
    char buf[2];
    buf[0] = (char) key;
    buf[1] = 0;
    return string(buf);
  } 
  else if (key <= 32 || (127 == key) || (key >= KEY_FIRST && key < KEY_LAST)) {
    return string(keys_to_names[key]);
  }
  else return string("??");
};

ostream & keyboard_inputevent_event_t::Out(ostream & out) {
  out << (int) key << " " << (int) type;
  return out;
}

istream & keyboard_inputevent_event_t::In(istream & in) {
  in >> (int) key >> (int) type;
  return in;
}


/* **********************************************************************
 * The TKeyboardInputEvent class (constructor) 
 * Creates an instance, sets attributes
 * *********************************************************************/
TKeyboardInputEvent::TKeyboardInputEvent(keypress_t nkeyboard_inputevent,
					 unsigned int nkey) 
  : TInputEvent(inputevent_type_keyboard) {
  keyboard_inputevent_event.key = nkey;
  keyboard_inputevent_event.type = nkeyboard_inputevent;
}
/* **********************************************************************
 * Definition of glut callback functions
 * *********************************************************************/

/* **********************************************************************
 * Keyboard{,Up}Func - called when a normal key is pressed 
 * Note that x,y is thrown away here. This may be stupid
 * *********************************************************************/
void KeyboardFunc(unsigned char key, int x, int y) {
  Inputs.Events.push(new TKeyboardInputEvent(keydown,
					     key));
}

void KeyboardUpFunc(unsigned char key, int x, int y) {
  Inputs.Events.push(new TKeyboardInputEvent(keyup,
					     key));
}

/* **********************************************************************
 * This table is used when mapping from GLUT key stuff to our format. 
 * *********************************************************************/

struct {
  int glutkey;
  int thxkey;
} glutkey_to_thxkey[] = 
{
  {GLUT_KEY_F1, KEY_F1},
  {GLUT_KEY_F2, KEY_F2},
  {GLUT_KEY_F3, KEY_F3},
  {GLUT_KEY_F4, KEY_F4},
  {GLUT_KEY_F5, KEY_F5},
  {GLUT_KEY_F6, KEY_F6},
  {GLUT_KEY_F7, KEY_F7},
  {GLUT_KEY_F8, KEY_F8},
  {GLUT_KEY_F9, KEY_F9},
  {GLUT_KEY_F10, KEY_F10},
  {GLUT_KEY_F11, KEY_F11},
  {GLUT_KEY_F12, KEY_F12},
  {GLUT_KEY_UP, KEY_UP},
  {GLUT_KEY_DOWN, KEY_DOWN},
  {GLUT_KEY_LEFT, KEY_LEFT},
  {GLUT_KEY_RIGHT, KEY_RIGHT},
  {GLUT_KEY_HOME, KEY_HOME},
  {GLUT_KEY_END, KEY_END},
  {GLUT_KEY_PAGE_UP, KEY_PAGEUP},
  {GLUT_KEY_PAGE_DOWN, KEY_PAGEDOWN},
  {GLUT_KEY_INSERT, KEY_INSERT},
  {0, 0}
};

/* This mapping is used between the two ways to specify keys */
typedef map <int, int> TIntIntMap;
typedef TIntIntMap::iterator TIntIntMapIterator;
TIntIntMap glutkey_to_thxkeys;

/* **********************************************************************
 * Special{,Up}Func - called when a special key is pressed 
 * Note that x,y is thrown away here. This may be stupid
 * *********************************************************************/
void SpecialFunc(int key, int x, int y) {
  /* Map from glut to thx keys */
  TIntIntMapIterator loc = glutkey_to_thxkeys.find(key);
  if (loc != glutkey_to_thxkeys.end()) {
    //    cout << "Sending " << (*loc).second << " down" << endl;
    Inputs.Events.push(new TKeyboardInputEvent(keydown,
					       (*loc).second));
  } else {
    cerr << "SpecialFunc - key " << key << " can not be mapped!" << endl;
  }
}

void SpecialUpFunc(int key, int x, int y) {
  /* Map from glut to thx keys */
  TIntIntMapIterator loc = glutkey_to_thxkeys.find(key);
  if (loc != glutkey_to_thxkeys.end()) {
    // cout << "Sending " << (*loc).second << " up" << endl;
    Inputs.Events.push(new TKeyboardInputEvent(keyup,
					       (*loc).second));
  } else {
    cerr << "SpecialFunc - key " << key << " can not be mapped!" << endl;
  }
}

/* **********************************************************************
 * Registering and deregistering keyboard handler functions in glut 
 * Registering also disables keyrepeat
 * *********************************************************************/
void inputkeyboard_init() {
  /* Set up the keyboard mappings */
  int i = 0;
  glutkey_to_thxkeys.clear();
  while(glutkey_to_thxkey[i].glutkey) {
    glutkey_to_thxkeys[glutkey_to_thxkey[i].glutkey]
      = glutkey_to_thxkey[i].thxkey;
    i++;
  }
  i = 0;
  keys_to_names.clear();
  while(keynames[i].key) {
    keys_to_names[keynames[i].key] = keynames[i].name;
    i++;
  }
  /* Enable global keyrepeat - used in menus */
  inputkeyboard_enablerepeat();
  /* Set the keyboard functions */
  glutKeyboardFunc(KeyboardFunc);
  glutKeyboardUpFunc(KeyboardUpFunc);
  glutSpecialFunc(SpecialFunc);
  glutSpecialUpFunc(SpecialUpFunc);
}

void inputkeyboard_shutdown() {
  /* Reset the keyboard functions */
  glutKeyboardUpFunc(NULL);
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
  glutSpecialUpFunc(NULL);
  /* Reset global keyrepeat */
  inputkeyboard_restorerepeat();
}

void inputkeyboard_disablerepeat() {
  /* Disable global keyrepeat */
  // glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glutIgnoreKeyRepeat(1);
}

void inputkeyboard_enablerepeat() {
  /* Enable global keyrepeat */
  // glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
  glutIgnoreKeyRepeat(0);
}

void inputkeyboard_restorerepeat() {
  /* Reset global keyrepeat */
  // glutSetKeyRepeat(GLUT_KEY_REPEAT_DEFAULT);
}
