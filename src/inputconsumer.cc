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
/* The commands */
#include <string.h>

#include <stdlib.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <sstream>

using std::ostringstream;
using std::iostream;
using std::fstream;
using std::ends;


#include "inputconsumer.hh"
#include "command.hh"

/* Set to 1 to get mapping information */
#define INPUT_DEBUG 0

/* The InputToCommand object */
TInputToCommand InputToCommand;

/* **********************************************************************
 * The TInputToCommand class 
 * *********************************************************************/

/* **********************************************************************
 * Keyboard default mappings
 * *********************************************************************/
struct keymap_t {
  gamemode_t mode;
  keyboard_inputevent_event_t kev;
  char * cmd;
  char * arg;
  /* Never actually used */
  ostream & Out(ostream & out) {
    out << "bind-key " << (int) mode << " ";
    kev.Out(out) << " " << cmd << " " << arg << endl;
    return out;
  }
};

/* Kids, don't look */
struct keymap_t_in {
  gamemode_t mode;
  keyboard_inputevent_event_t kev;
  string cmd;
  string arg;
  istream & In(istream & in) {
    string check;
    in >> check;
    if ("bind-key" != check) {
      cerr << "keymap_t - expected bind-key, got " << check << endl;
      return in;
    }
    int tmp;
    in >> tmp;
    mode = (gamemode_t) tmp;
    // in >> (int) mode;
    kev.In(in) >> cmd >> arg;
    /* Strip cmd and arg */
    cmd = cmd.substr(1, cmd.size()-2);
    arg = arg.substr(1, arg.size()-2);
    // cout << "READ: -" << cmd << "- -" << arg << "-" << endl;
    return in;
  }
};

keymap_t key_map_std[] =
{
  /* GENERAL */
  {gamemode_any,  {KEY_CTRLC,     keydown}, "quit", ""},
  /* NOTE: Any "normal" keys, that are used in any mode, needs to be mapped
     to keydown 'key' in the gamemode_edit mode! Otherwise, they can not 
     be typed into names and so on */
  // TODO: Remove q?
  {gamemode_any,  {'q',           keydown}, "quit", ""},
  // {gamemode_edit, {'q',           keydown}, "keydown", "q"},
  {gamemode_any,  {'a',           keydown}, "toggle-pointer-grab", ""},
  //{gamemode_edit, {'a',           keydown}, "keydown", "a"},
  {gamemode_any, {KEY_F3,         keydown}, "console-toggle-display", ""},


  /* MENU */
  {gamemode_menu, {KEY_UP,        keydown}, "focus-change", "up"},
  {gamemode_menu, {KEY_DOWN,      keydown}, "focus-change", "down"},
  {gamemode_menu, {KEY_PAGEUP,    keydown}, "focus-change", "scroll-up"},
  {gamemode_menu, {KEY_PAGEDOWN,  keydown}, "focus-change", "scroll-down"},
  {gamemode_menu, {KEY_ESCAPE,    keydown}, "focus-change", "escape"},
  {gamemode_menu, {KEY_ENTER,     keydown}, "menuitem", "select"},
  {gamemode_menu, {' ',           keydown}, "menuitem", "select"},

  /* EDIT */
  /* itemedit - used to accept/cancel an editing operation */
  {gamemode_edit, {KEY_ENTER,     keydown}, "itemedit", "accept"},
  {gamemode_edit, {KEY_ESCAPE,    keydown}, "itemedit", "cancel"},
  /* list is used to control a TListMenuItem */
  {gamemode_edit, {KEY_UP,        keydown}, "list", "previous"},
  {gamemode_edit, {KEY_DOWN,      keydown}, "list", "next"},
  /* These are for TStringMenuItem */
  /* 1=Ctrl+A, 2=Ctrl+B, 5=Ctrl+E, 6=Ctrl+F */
  {gamemode_edit, {KEY_CTRLB,     keydown}, "point-move", "backward-char"},
  {gamemode_edit, {KEY_LEFT,      keydown}, "point-move", "backward-char"},
  {gamemode_edit, {KEY_CTRLF,     keydown}, "point-move", "forward-char"},
  {gamemode_edit, {KEY_RIGHT,     keydown}, "point-move", "forward-char"},
  {gamemode_edit, {KEY_CTRLA,     keydown}, "point-move", "beginning-of-line"},
  {gamemode_edit, {KEY_HOME,      keydown}, "point-move", "beginning-of-line"},
  {gamemode_edit, {KEY_CTRLE,     keydown}, "point-move", "end-of-line"},
  {gamemode_edit, {KEY_END,       keydown}, "point-move", "end-of-line"},

  {gamemode_edit, {KEY_BACKSPACE, keydown}, "edit", "delete-backward-char"},
  {gamemode_edit, {KEY_CTRLD,     keydown}, "edit", "delete-char"},
  {gamemode_edit, {KEY_DELETE,    keydown}, "edit", "delete-char"},
  {gamemode_edit, {KEY_CTRLK,     keydown}, "edit", "kill-line"},
  {gamemode_edit, {KEY_CTRLY,     keydown}, "edit", "yank"},
  {gamemode_edit, {KEY_CTRLT,     keydown}, "edit", "transpose-chars"},

  /* GAME */
  {gamemode_game, {' ',           keydown}, "fire", ""},
  {gamemode_game, {'\r',          keydown}, "fire", ""},
  {gamemode_game, {KEY_ESCAPE,    keydown}, "in-game-menu-show", ""},
  /* Viewpoint-move manipulation commands */
  {gamemode_game, {'r',           keydown}, "viewpoint-move", "+forward"},
  {gamemode_game, {'r',           keyup},   "viewpoint-move", "-forward"},
  {gamemode_game, {'f',           keydown}, "viewpoint-move", "+backward"},
  {gamemode_game, {'f',           keyup},   "viewpoint-move", "-backward"},
  {gamemode_game, {'d',           keydown}, "viewpoint-move", "+left"},
  {gamemode_game, {'d',           keyup},   "viewpoint-move", "-left"},
  {gamemode_game, {'g',           keydown}, "viewpoint-move", "+right"},
  {gamemode_game, {'g',           keyup},   "viewpoint-move", "-right"},
  {gamemode_game, {'t',           keydown}, "viewpoint-move", "+up"},
  {gamemode_game, {'t',           keyup},   "viewpoint-move", "-up"},
  {gamemode_game, {'b',           keydown}, "viewpoint-move", "+down"},
  {gamemode_game, {'b',           keyup},   "viewpoint-move", "-down"},
  /* Viewpoint-rotate manipulation commands */
  {gamemode_game, {'u',           keydown}, "viewpoint-rotate", "+forward"},
  {gamemode_game, {'u',           keyup},   "viewpoint-rotate", "-forward"},
  {gamemode_game, {'j',           keydown}, "viewpoint-rotate", "+backward"},
  {gamemode_game, {'j',           keyup},   "viewpoint-rotate", "-backward"},
  {gamemode_game, {'h',           keydown}, "viewpoint-rotate", "+left"},
  {gamemode_game, {'h',           keyup},   "viewpoint-rotate", "-left"},
  {gamemode_game, {'k',           keydown}, "viewpoint-rotate", "+right"},
  {gamemode_game, {'k',           keyup},   "viewpoint-rotate", "-right"},
  // TODO?: keyboard.cc : e == viewpoint->Reset(..);
  /* Tank manipulation stuff */
  {gamemode_game, {KEY_LEFT,      keydown}, "cannon", "+rotate-left"},
  {gamemode_game, {KEY_LEFT,      keyup},   "cannon", "-rotate-left"},
  {gamemode_game, {KEY_RIGHT,     keydown}, "cannon", "+rotate-right"},
  {gamemode_game, {KEY_RIGHT,     keyup},   "cannon", "-rotate-right"},
  {gamemode_game, {KEY_UP,        keydown}, "cannon", "+raise"},
  {gamemode_game, {KEY_UP,        keyup},   "cannon", "-raise"},
  {gamemode_game, {KEY_DOWN,      keydown}, "cannon", "+lower"},
  {gamemode_game, {KEY_DOWN,      keyup},   "cannon", "-lower"},
  /* Force */
  {gamemode_game, {'+',           keydown}, "cannon", "+force-increase"},
  {gamemode_game, {'+',           keyup},   "cannon", "-force-increase"},
  {gamemode_game, {'-',           keydown}, "cannon", "+force-decrease"},
  {gamemode_game, {'-',           keyup},   "cannon", "-force-decrease"},
  /* Selection of projectile */
  {gamemode_game, {'\t',          keydown},   "inventory", "next-weapon"},
  /* Render and display stuff - misplaced? */
  {gamemode_game, {'1',           keydown},   "render", "lines"},
  {gamemode_game, {'2',           keydown},   "render", "polygons"},
  {gamemode_game, {'3',           keydown},   "render", "textures"},
  {gamemode_game, {'4',           keydown},   "render", "toggle-lights"},
  {gamemode_game, {'5',           keydown},   "render", "toggle-shademode"},
  {gamemode_game, {'6',           keydown},   "render", "toggle-normals"},
  {gamemode_game, {'7',           keydown},   "render", "toggle-markers"},
  {gamemode_game, {'8',           keydown},   "display", "toggle-flymode"},
  {gamemode_game, {'9',           keydown},   "display", "toggle-clipmode"},
  {gamemode_game, {'0',           keydown},   "display", "refreshrate"},
  

  /* FINAL */
  {gamemode_any, {0, keydown}, "", ""}
};


/* **********************************************************************
 * Mouse default mappings
 * *********************************************************************/

/* To a mouse command, a TPointInfo is appended as a string.
   To a raw mouse command a TMouseInputEvent is encoded in the args. 
   (Yes, this is silly - some day I will fix it).
   This means, that a TMouseInputEvent can be constructed from 
   the args like this; 
   TMouseInputEvents tmp(Command->args);
   The string can have stuff in front and after the TMouseInputEvent - 
   it scans for a "magic". */

struct mousemap_t {
  gamemode_t mode;
  mouse_inputevent_event_t mev;
  char * cmd;
  char * arg;
};

mousemap_t mouse_map_std[] =
{
  /* GENERAL */
  {gamemode_any,  {mousemove, mouse_none},   "mouse-move", ""},
  {gamemode_any,  {mousemove, mouse_left},   "mouse-move", ""},
  {gamemode_any,  {mousemove, mouse_middle}, "mouse-move", ""},
  {gamemode_any,  {mousemove, mouse_right},  "mouse-move", ""},
  {gamemode_any,  {mousedown, mouse_left},   "mouse-down", ""},
  // {gamemode_any,  {mouseup,   mouse_left},   "mouse-up",   ""},

  /* GAME */
  {gamemode_game, {mousemove, mouse_none},   "viewpoint-rotate", "mouse"},
  {gamemode_game, {mousemove, mouse_middle}, "cannon",           "mouse"},
  {gamemode_game, {mousemove, mouse_right},  "viewpoint-move",   "mouse"},

  {gamemode_game, {mousedown, mouse_left},   "fire", ""},
  
  /* FINAL */
  {gamemode_any,  {mousemove, mouse_none}, "", ""}
};

/* **********************************************************************
 * The TInputToCommand constructor - sets up default mappings
 * *********************************************************************/
TInputToCommand::TInputToCommand() {
  int i; 
  /* TODO: Nicer way to initialize - load and save, so on */
  // TODO: This is a really hackish way to restore bindings.
  // It _will_ go
  char * home      = getenv("HOME");
  bool use_std_map = true;
  if (home) {
    string mapfile = string(home) + "/.thx/mappings.cfg";
    ifstream input(mapfile.c_str());
    if (input) {
      string version; 
      input >> version;
      if ("1" == version) {
	while(!input.eof()) {
	  /* Eat whitespace */
	  char c;
	  while(input.get(c)) {
	    if (isspace(c) == 0) {
	      input.putback(c);
	      break;
	    }
	  }
	  /* Check for a keybind */
	  if(!input.eof()) {
	    keymap_t_in tmp;
	    tmp.In(input);
	    KeyboardCommandMap[tmp.mode][tmp.kev]
	      = TCmdArg(tmp.cmd, tmp.arg);
	  }
	} // While not eof
	  use_std_map = false;
      } else {
	cerr << "Version of mappings.cfg wrong." << endl
	     << " Got " << version << " expected 1" << endl
	     << "Maybe you should delete the file " << mapfile << endl;
      }
    } else {
      cerr << "Could not open " << mapfile << endl;
    }
  }

  if (use_std_map) {
    /* Map the standard keys */
    i = 0;  
    while(strcmp(key_map_std[i].cmd, "")) {
#if (INPUT_DEBUG)
      cout << "Mapping key " << key_map_std[i].kev.key << " to ("
	   << key_map_std[i].cmd << "," << key_map_std[i].arg << ")" << endl;
#endif
      // TODO: Use proper method
      KeyboardCommandMap[key_map_std[i].mode][key_map_std[i].kev]
      = TCmdArg(string(key_map_std[i].cmd), string(key_map_std[i].arg));
      i++;
    }    
  }

  /* Map the standard mouse */
  i = 0;  
  while(strcmp(mouse_map_std[i].cmd, "")) {
#if (INPUT_DEBUG)
    cout << "Mapping mouse (" << mouse_map_std[i].cmd 
	 << "," << mouse_map_std[i].arg << ")" << endl;
#endif
    // TODO: Use proper method
    MouseCommandMap[mouse_map_std[i].mode][mouse_map_std[i].mev]
      = TCmdArg(string(mouse_map_std[i].cmd), string(mouse_map_std[i].arg));
    i++;
  }    
}

/* **********************************************************************
 * Desctructor saves mappings to disk
 * *********************************************************************/
TInputToCommand::~TInputToCommand() {
  char * home      = getenv("HOME");
  if (home) {
    string mapfile = string(home) + "/.thx/mappings.cfg";
    ofstream output(mapfile.c_str());
    if (output) {
      /* Write the version */
      output << "1" << endl;
      for (int gi = 0; gi < gamemode_count; gi++) { 
	TKeyboardCommandMapIterator End = KeyboardCommandMap[gi].end();
	TKeyboardCommandMapIterator i;
	for (i = KeyboardCommandMap[gi].begin(); i != End; i++) {
	  output << "bind-key " << (int) gi << " ";
	  /* Put the kev */
	  keyboard_inputevent_event_t tmpkev = (*i).first;
	  tmpkev.Out(output) 
	    << " \"" << (*i).second.cmd 
	    << "\" \"" << (*i).second.arg << "\" "
	    << endl;
	}
      } 
    } else {
      cerr << "Could not open "
	   << mapfile << endl
	   << "If you wish for THX to store your key mappings between games" 
	   << " then please create a directory called .thx in your home directory"
	   << endl;
    }
  }
}

/* **********************************************************************
 * Init the keyboard and mouse, and others
 * *********************************************************************/
void TInputToCommand::Init() {
  inputkeyboard_init();
  inputmouse_init();
}

/* **********************************************************************
 * Shutdown the keyboard and mouse, and others
 * *********************************************************************/
void TInputToCommand::Shutdown() {
  inputkeyboard_shutdown();
  inputmouse_shutdown();
}

/* **********************************************************************
 * The Consume method.
 * *********************************************************************/
int TInputToCommand::Consume() {
  /* The consume method grabs input events from the inputevent queue and
     figure out what command string is associated with the event.
     First the current gamemode mappings is checked, then the anymode
     If a mappings is found, it is added to the command queue */
  // cout << "TInputToCommand::Consume" << endl;
  TInputEvent * InputEvent;
  int count = Inputs.Events.size();
  // cout << "The number of input events are " << count << endl;
  while (Inputs.Events.size() > 0) {
    InputEvent = Inputs.Events.front();
    switch (InputEvent->eventtype) {
    case inputevent_type_keyboard: {
      /* Get a pointer we can use */
      TKeyboardInputEvent * KeyEvent = (TKeyboardInputEvent *) InputEvent;
      
      /* **********************************************************************
       * Raw gamemode
       * *********************************************************************/
      /* Raw is a special case, similar to automap, however, 
	 raw sends keys as numbers only, where automap maps to ascii (sort of) */
      if (gamemode_raw == GameMode.GetMode()) {
	if (keydown 
	    == KeyEvent->keyboard_inputevent_event.type 
	    && (KeyEvent->keyboard_inputevent_event.key < 128
		|| (KeyEvent->keyboard_inputevent_event.key >= KEY_FIRST &&
		    KeyEvent->keyboard_inputevent_event.key < KEY_LAST))) {
#if(INPUT_DEBUG)
	  cout << "TInputToCommand::Consume, key (down)"
	       << KeyEvent->keyboard_inputevent_event.key
	       << " in raw is" << "raw " 
	       << KeyEvent->keyboard_inputevent_event.key << endl;
#endif
	  ostringstream tmparg;
	  tmparg << KeyEvent->keyboard_inputevent_event.key << ends;
	  CommandQueue.push(new TCommand(KeyEvent->timestamp,
					 "raw-keydown", tmparg.str()));
	} 
	break;
      }
      /* **********************************************************************
       * Normal handling - find the command in one of the keyboard maps
       * *********************************************************************/

      /* **********************************************************************
       * Check the current game mode mappings
       * *********************************************************************/
      TKeyboardCommandMapIterator loc 
	= KeyboardCommandMap[GameMode.GetMode()].find(KeyEvent->keyboard_inputevent_event);
      if (loc != KeyboardCommandMap[GameMode.GetMode()].end()) {
	/* Found a "real" mapping */
#if (INPUT_DEBUG)
	cout << "TInputToCommand::Consume, current mode, key "
	     << KeyEvent->keyboard_inputevent_event.key
	     << " maps to \"" << (*loc).second.cmd << "\"" << endl;
#endif
	CommandQueue.push(new TCommand(KeyEvent->timestamp,
				       (*loc).second.cmd, (*loc).second.arg));
	break;
      };
      
      /* **********************************************************************
       * No mappings found for current mode, check the any mode mappings 
       * but only if not in gamemode_edit. (Yes, this is becomming muddy)
       * *********************************************************************/
      if (gamemode_edit != GameMode.GetMode()) {
	TKeyboardCommandMapIterator loc 
	  = KeyboardCommandMap[gamemode_any].find(KeyEvent->keyboard_inputevent_event);
	if (loc != KeyboardCommandMap[gamemode_any].end()) {
	  /* Found a "real" mapping in the any mode<*/
#if(INPUT_DEBUG)
	  cout << "TInputToCommand::Consume, any mode, key "
	       << KeyEvent->keyboard_inputevent_event.key
	       << " maps to \"" << (*loc).second.cmd << "\"" << endl;
#endif
	  CommandQueue.push(new TCommand(KeyEvent->timestamp,
					 (*loc).second.cmd, (*loc).second.arg));       
	  break;
	};
      };

      /* **********************************************************************
       * No mappings found, automap 
       * Keyboard events for "down" that are not mapped, with key <
       * 128 generate a command that has the name of the key if the
       * key is within a certain range. 
       * *********************************************************************/
      if (keydown 
	  == KeyEvent->keyboard_inputevent_event.type 
	  && KeyEvent->keyboard_inputevent_event.key < 128) {
#if(INPUT_DEBUG)
	cout << "TInputToCommand::Consume, key (down)"
	     << KeyEvent->keyboard_inputevent_event.key
	     << " automaps to " << "keydown " 
	     << KeyEvent->keyboard_inputevent_event.key << endl;
#endif
	/* I do not know how to do this properly... */
	char tmp[2];
	tmp[0] = KeyEvent->keyboard_inputevent_event.key;
	tmp[1] = 0;
	CommandQueue.push(new TCommand(KeyEvent->timestamp,
				       "keydown", tmp));
	break;
      } 

      /* **********************************************************************
       * Lastly, if debug, print that no mapping was found
       * *********************************************************************/
#if(INPUT_DEBUG)
      cout << "TInputToCommand::Consume, no mapping found for key "
	   << ((TKeyboardInputEvent *) 
	       InputEvent)->keyboard_inputevent_event.key;
      if (KeyEvent->keyboard_inputevent_event.type == 
	  keydown) {
	cout << " down";
      } else {
	cout << " up";
      }
      cout << endl;
#endif
      break;
    } /* Keyboard */

    case inputevent_type_pointer:  {
      /* Find the command in the mouse map by checking the current
	 and any modes */
      TMouseInputEvent * MouseEvent 
	= (TMouseInputEvent *) InputEvent;
      /* **********************************************************************
       * Raw gamemode
       * *********************************************************************/
      /* Raw is a special case. We only trigger mouse presses, however, 
	 so it is similar to the keyboard case 
	 Only downwards causes raw a command */
      if (gamemode_raw == GameMode.GetMode()) {
	if (mousedown == MouseEvent->mouse_inputevent_event.action) {
	  CommandQueue.push(new TCommand(MouseEvent->timestamp,
					 "raw-mouse", MouseEvent->ToString()));
	  break;
	}
      }
      /* Note, that we fall through to any mode, to allow for 
	 the mouse to be moved around in menus. This is differently than
	 the way the keyboard works. */
      /* Check the current mode */
      TMouseCommandMapIterator loc 
	= MouseCommandMap[GameMode.GetMode()].find(MouseEvent->mouse_inputevent_event);
      if (loc != MouseCommandMap[GameMode.GetMode()].end()) {
	/* Found a "real" mapping */
#if (INPUT_DEBUG)
	cout << "TInputToCommand::Consume, current mode, mouse "
	     << " maps to \"" << (*loc).second.cmd << "\"" << endl;
#endif
	CommandQueue.push(new TCommand(MouseEvent->timestamp,
				       (*loc).second.cmd, 
				       (*loc).second.arg, 
				       new TPointerInfo(MouseEvent)));
      } else {
	/* No mapping found for the current mode, check 
	   for the any mode */
	TMouseCommandMapIterator loc 
	  = MouseCommandMap[gamemode_any].find(MouseEvent->mouse_inputevent_event);
	if (loc != MouseCommandMap[gamemode_any].end()) {
	  /* Found a "real" mapping in the any mode<*/
#if(INPUT_DEBUG)
	  cout << "TInputToCommand::Consume, any mode, mouse "
	       << " maps to \"" << (*loc).second.cmd << "\"" << endl;
#endif
	  CommandQueue.push(new TCommand(MouseEvent->timestamp,
					 (*loc).second.cmd, 
					 (*loc).second.arg,
					 new TPointerInfo(MouseEvent)));
	} /* Any mode mapping */
#if(INPUT_DEBUG)
	else { /* No mappings found */
	  cout << "TInputToCommand::Consume, no mapping found for mouse event"
	       << endl;
	}
#endif
      } /* Current mode mapping */
      break; /* pointer events */
    }
    default:
      cerr << "TInputToCommand::Consume - Unknown inputeventtype" << endl;
      break;
    }
    /* Clean up */
    Inputs.Events.pop();
    delete InputEvent;
  }  
  return count;
}

/* **********************************************************************
 * GeMappingsForCommand 
 * Get a string describing what keys or mouse clicks are mapped to
 * this command.  Only return mapping for keydown and mousedown events.
 * This is pretty ugly code, but only used from the ControlsMenu
 * *********************************************************************/
string TInputToCommand::GetMappingsForCommand(string command, string args, 
					      gamemode_t mode) {
  string res = "";
  string pre = "";
  /* KeyboardMappings */
  {
    TKeyboardCommandMapIterator End = KeyboardCommandMap[mode].end();
    TKeyboardCommandMapIterator i;
    for (i = KeyboardCommandMap[mode].begin(); i != End; i++) {
      if ((*i).second.cmd == command && (*i).second.arg == args) {
	/* Found a match - add to res */
	res += pre + (*i).first.AsString();
	if ("" == pre) {
	  pre = ", ";
	}
      }
    }
  }
  /* MouseMappings */
  {
    TMouseCommandMapIterator End = MouseCommandMap[mode].end();
    TMouseCommandMapIterator i;
    for (i = MouseCommandMap[mode].begin(); i != End; i++) {
      if ((*i).second.cmd == command && (*i).second.arg == args) {
	/* Found a match - add to res */
	res += pre + (*i).first.AsString();
	if ("" == pre) {
	  pre = ", ";
	}
      }
    }
  }
  /* t << "GetKeyMappingsForCommand - (" << command << "," << args << ") maps to "
     << res << endl; */
  return res;
}

/* **********************************************************************
 * ADD mappings
 * *********************************************************************/
  
/* **********************************************************************
 * The AddKeyboardMapping method.
 * *********************************************************************/
void TInputToCommand::AddKeyboardMapping(gamemode_t mode, 
					 keyboard_inputevent_event_t event,
					 string cmd, string arg) {
  KeyboardCommandMap[mode][event] = TCmdArg(cmd, arg);
  /* Check if we need to add a - mapping to a + mapping */
  if (keydown == event.type && arg.size() > 0 && '+' == arg[0]) {
    event.type = keyup;
    arg[0] = '-';
    KeyboardCommandMap[mode][event] = TCmdArg(cmd, arg);
  }
}

/* **********************************************************************
 * The AddMouseMapping method.
 * *********************************************************************/
void TInputToCommand::AddMouseMapping(gamemode_t mode, 
				      mouse_inputevent_event_t event,
				      string cmd, string arg) {
  MouseCommandMap[mode][event] = TCmdArg(cmd, arg);
  /* Check if we need to add a - mapping to a + mapping */
  if (mousedown == event.action && arg.size() > 0 && '+' == arg[0]) {
    event.action = mouseup;
    arg[0] = '-';
    MouseCommandMap[mode][event] = TCmdArg(cmd, arg);
  }
}

/* **********************************************************************
 * REMOVE mappings
 * *********************************************************************/
/* **********************************************************************
 * The RemoveKeyboardMapping method.
 * *********************************************************************/
void TInputToCommand::RemoveKeyboardMapping(gamemode_t mode, 
					    keyboard_inputevent_event_t event) {
  
  KeyboardCommandMap[mode].erase(event);
}

/* **********************************************************************
 * The RemoveMouseMapping method.
 * *********************************************************************/
void TInputToCommand::RemoveMouseMapping(gamemode_t mode, 
					 mouse_inputevent_event_t event) {
  MouseCommandMap[mode].erase(event);
}

/* **********************************************************************
 * TOGGLE mappings
 * *********************************************************************/

/* **********************************************************************
 * The ToggleKeyboardMapping method.
 * *********************************************************************/
void TInputToCommand::ToggleKeyboardMapping(gamemode_t mode, 
					    keyboard_inputevent_event_t event,
					    string cmd, string arg) {
  if (KeyboardCommandMap[mode][event].cmd == cmd 
      && KeyboardCommandMap[mode][event].arg == arg) {
    /* Already mapped, remove mapping */
    RemoveKeyboardMapping(mode, event);
    /* Check if we need to add a - mapping to a + mapping */
    if (keydown == event.type && arg.size() > 0 && '+' == arg[0]) {
      event.type = keyup;
      arg[0] = '-';
      RemoveKeyboardMapping(mode, event);
    }
  } else {
    AddKeyboardMapping(mode, event, cmd, arg);
  }
}

/* **********************************************************************
 * The ToggleMouseMapping method.
 * *********************************************************************/
void TInputToCommand::ToggleMouseMapping(gamemode_t mode, 
					 mouse_inputevent_event_t event,
					 string cmd, string arg) {
  if (MouseCommandMap[mode][event].cmd == cmd 
      && MouseCommandMap[mode][event].arg == arg) {
    /* Already mapped, remove mapping */
    RemoveMouseMapping(mode, event);
    /* Check if we need to add a - mapping to a + mapping */
    if (mousedown == event.action && arg.size() > 0 && '+' == arg[0]) {
      event.action = mouseup;
      arg[0] = '-';
      RemoveMouseMapping(mode, event);
    }
  } else {
    AddMouseMapping(mode, event, cmd, arg);
  }
}
