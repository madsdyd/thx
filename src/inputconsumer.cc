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
#include <iostream>
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
 * The TInputToCommand constructor - sets up default mappings
 * *********************************************************************/
/* First default mappings */
struct keymap_t {
  gamemode_t mode;
  keyboard_inputevent_event_t kev;
  char * cmd;
  char * arg;
};

keymap_t key_map_std_keys_down[] =
{
  /* GENERAL */
  {gamemode_any,  {KEY_CTRLC,     keyboard_inputevent_type_down}, "quit", ""},
  // TODO: Remove q?
  {gamemode_any,  {'q',           keyboard_inputevent_type_down}, "quit", ""},
  
  /* MENU */
  {gamemode_menu, {KEY_UP,        keyboard_inputevent_type_down}, "focus-change", "up"},
  {gamemode_menu, {KEY_DOWN,      keyboard_inputevent_type_down}, "focus-change", "down"},
  {gamemode_menu, {KEY_ESCAPE,    keyboard_inputevent_type_down}, "focus-change", "escape"},
  {gamemode_menu, {KEY_ENTER,     keyboard_inputevent_type_down}, "menuitem", "select"},
  {gamemode_menu, {' ',           keyboard_inputevent_type_down}, "menuitem", "select"},

  /* EDIT */
  /* itemedit - used to accept/cancel an editing operation */
  {gamemode_edit, {KEY_ENTER,     keyboard_inputevent_type_down}, "itemedit", "accept"},
  {gamemode_edit, {KEY_ESCAPE,    keyboard_inputevent_type_down}, "itemedit", "cancel"},
  /* list is used to control a TListMenuItem */
  {gamemode_edit, {KEY_UP,        keyboard_inputevent_type_down}, "list", "previous"},
  {gamemode_edit, {KEY_DOWN,      keyboard_inputevent_type_down}, "list", "next"},
  /* These are for TStringMenuItem */
  /* 1=Ctrl+A, 2=Ctrl+B, 5=Ctrl+E, 6=Ctrl+F */
  {gamemode_edit, {KEY_CTRLB,     keyboard_inputevent_type_down}, "point-move", "backward-char"},
  {gamemode_edit, {KEY_LEFT,      keyboard_inputevent_type_down}, "point-move", "backward-char"},
  {gamemode_edit, {KEY_CTRLF,     keyboard_inputevent_type_down}, "point-move", "forward-char"},
  {gamemode_edit, {KEY_RIGHT,     keyboard_inputevent_type_down}, "point-move", "forward-char"},
  {gamemode_edit, {KEY_CTRLA,     keyboard_inputevent_type_down}, "point-move", "beginning-of-line"},
  {gamemode_edit, {KEY_HOME,      keyboard_inputevent_type_down}, "point-move", "beginning-of-line"},
  {gamemode_edit, {KEY_CTRLE,     keyboard_inputevent_type_down}, "point-move", "end-of-line"},
  {gamemode_edit, {KEY_END,       keyboard_inputevent_type_down}, "point-move", "end-of-line"},

  {gamemode_edit, {KEY_BACKSPACE, keyboard_inputevent_type_down}, "edit", "delete-backward-char"},
  {gamemode_edit, {KEY_CTRLD,     keyboard_inputevent_type_down}, "edit", "delete-char"},
  {gamemode_edit, {KEY_DELETE,    keyboard_inputevent_type_down}, "edit", "delete-char"},
  {gamemode_edit, {KEY_CTRLK,     keyboard_inputevent_type_down}, "edit", "kill-line"},
  {gamemode_edit, {KEY_CTRLY,     keyboard_inputevent_type_down}, "edit", "yank"},
  {gamemode_edit, {KEY_CTRLT,     keyboard_inputevent_type_down}, "edit", "transpose-chars"},

  /* GAME */
  {gamemode_game, {' ',           keyboard_inputevent_type_down}, "fire", ""},
  {gamemode_game, {KEY_ESCAPE,    keyboard_inputevent_type_down}, "escape", ""},
  /* Viewpoint-move manipulation commands */
  {gamemode_game, {'r',           keyboard_inputevent_type_down}, "viewpoint-move", "+forward"},
  {gamemode_game, {'r',           keyboard_inputevent_type_up},   "viewpoint-move", "-forward"},
  {gamemode_game, {'f',           keyboard_inputevent_type_down}, "viewpoint-move", "+backward"},
  {gamemode_game, {'f',           keyboard_inputevent_type_up},   "viewpoint-move", "-backward"},
  {gamemode_game, {'d',           keyboard_inputevent_type_down}, "viewpoint-move", "+left"},
  {gamemode_game, {'d',           keyboard_inputevent_type_up},   "viewpoint-move", "-left"},
  {gamemode_game, {'g',           keyboard_inputevent_type_down}, "viewpoint-move", "+right"},
  {gamemode_game, {'g',           keyboard_inputevent_type_up},   "viewpoint-move", "-right"},
  {gamemode_game, {'t',           keyboard_inputevent_type_down}, "viewpoint-move", "+up"},
  {gamemode_game, {'t',           keyboard_inputevent_type_up},   "viewpoint-move", "-up"},
  {gamemode_game, {'b',           keyboard_inputevent_type_down}, "viewpoint-move", "+down"},
  {gamemode_game, {'b',           keyboard_inputevent_type_up},   "viewpoint-move", "-down"},
  /* Viewpoint-rotate manipulation commands */
  {gamemode_game, {'u',           keyboard_inputevent_type_down}, "viewpoint-rotate", "+forward"},
  {gamemode_game, {'u',           keyboard_inputevent_type_up},   "viewpoint-rotate", "-forward"},
  {gamemode_game, {'j',           keyboard_inputevent_type_down}, "viewpoint-rotate", "+backward"},
  {gamemode_game, {'j',           keyboard_inputevent_type_up},   "viewpoint-rotate", "-backward"},
  {gamemode_game, {'h',           keyboard_inputevent_type_down}, "viewpoint-rotate", "+left"},
  {gamemode_game, {'h',           keyboard_inputevent_type_up},   "viewpoint-rotate", "-left"},
  {gamemode_game, {'k',           keyboard_inputevent_type_down}, "viewpoint-rotate", "+right"},
  {gamemode_game, {'k',           keyboard_inputevent_type_up},   "viewpoint-rotate", "-right"},
  // keyboard.cc : e == viewpoint->Reset(..);
  /* Tank manipulation stuff */
  {gamemode_game, {'n',           keyboard_inputevent_type_down}, "canon", "+rotate-left"},
  {gamemode_game, {'n',           keyboard_inputevent_type_up},   "canon", "-rotate-left"},
  {gamemode_game, {'m',           keyboard_inputevent_type_down}, "canon", "+rotate-right"},
  {gamemode_game, {'m',           keyboard_inputevent_type_up},   "canon", "-rotate-right"},
  {gamemode_game, {'a',           keyboard_inputevent_type_down}, "canon", "+raise"},
  {gamemode_game, {'a',           keyboard_inputevent_type_up},   "canon", "-raise"},
  {gamemode_game, {'z',           keyboard_inputevent_type_down}, "canon", "+lower"},
  {gamemode_game, {'z',           keyboard_inputevent_type_up},   "canon", "-lower"},
  {gamemode_game, {KEY_LEFT,      keyboard_inputevent_type_down}, "canon", "+rotate-left"},
  {gamemode_game, {KEY_LEFT,      keyboard_inputevent_type_up},   "canon", "-rotate-left"},
  {gamemode_game, {KEY_RIGHT,     keyboard_inputevent_type_down}, "canon", "+rotate-right"},
  {gamemode_game, {KEY_RIGHT,     keyboard_inputevent_type_up},   "canon", "-rotate-right"},
  {gamemode_game, {KEY_UP,        keyboard_inputevent_type_down}, "canon", "+raise"},
  {gamemode_game, {KEY_UP,        keyboard_inputevent_type_up},   "canon", "-raise"},
  {gamemode_game, {KEY_DOWN,      keyboard_inputevent_type_down}, "canon", "+lower"},
  {gamemode_game, {KEY_DOWN,      keyboard_inputevent_type_up},   "canon", "-lower"},
  /* Force */
  {gamemode_game, {'p',           keyboard_inputevent_type_down}, "canon", "+force-increase"},
  {gamemode_game, {'p',           keyboard_inputevent_type_up},   "canon", "-force-increase"},
  {gamemode_game, {'o',           keyboard_inputevent_type_down}, "canon", "+force-decrease"},
  {gamemode_game, {'o',           keyboard_inputevent_type_up},   "canon", "-force-decrease"},
  {gamemode_game, {'+',           keyboard_inputevent_type_down}, "canon", "+force-increase"},
  {gamemode_game, {'+',           keyboard_inputevent_type_up},   "canon", "-force-increase"},
  {gamemode_game, {'-',           keyboard_inputevent_type_down}, "canon", "+force-decrease"},
  {gamemode_game, {'-',           keyboard_inputevent_type_up},   "canon", "-force-decrease"},
  // Selection of projectile.


  /* FINAL */
  {gamemode_any, {0, keyboard_inputevent_type_down}, "", ""}
};

TInputToCommand::TInputToCommand() {
  int i; 
  /* TODO: Nicer way to initialize - load and save, so on */
  /* Map the standard keys */
  i = 0;  
  while(strcmp(key_map_std_keys_down[i].cmd, "")) {
#if (INPUT_DEBUG)
    cout << "Mapping key " << key_map_std_keys_down[i].kev.key << " to ("
	 << key_map_std_keys_down[i].cmd << "," << key_map_std_keys_down[i].arg << ")" << endl;
#endif
    KeyboardCommandMap[key_map_std_keys_down[i].mode][key_map_std_keys_down[i].kev]
      = TCmdArg(string(key_map_std_keys_down[i].cmd), string(key_map_std_keys_down[i].arg));
    i++;
  }    
}

/* **********************************************************************
 * The Consume method.
 * *********************************************************************/
int TInputToCommand::Consume() {
  /* The consume method grabs input events from the inputevent queue and
     figure out what command string is associated with the event.
     First the current gamemode mappings is checked, then the anymode
     If a mappings is found, it is added to the command queue */
  TInputEvent * InputEvent;
  int count = Inputs.Events.size();
  // cout << "The number of input events are " << count << endl;
  while (Inputs.Events.size() > 0) {
    InputEvent = Inputs.Events.front();
    switch (InputEvent->eventtype) {
    case inputevent_type_keyboard: {
      /* Find the command in the keyboard map */
      TKeyboardInputEvent * KeyEvent 
	= (TKeyboardInputEvent *) InputEvent;
      /* Check the current mode */
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
      } else {
	/* No mapping found for the current mode, check 
	   for the any mode */
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
	} else {
	  /* No mapping found -
	     Keyboard events for "down" that are not mapped,
	     with key < 256 
	     generate a command that has the name of the key 
	     if the key is within a certain range. */
	  if (keyboard_inputevent_type_down 
	      == KeyEvent->keyboard_inputevent_event.type 
	      && KeyEvent->keyboard_inputevent_event.key < 256) {
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
	  } 
#if(INPUT_DEBUG)
	  else {
	    cout << "TInputToCommand::Consume, no mapping found for key "
		 << ((TKeyboardInputEvent *) 
		     InputEvent)->keyboard_inputevent_event.key;
	    if (KeyEvent->keyboard_inputevent_event.type == 
		keyboard_inputevent_type_down) {
	      cout << " down";
	    } else {
	      cout << " up";
	    }
	    cout << endl;
	  } /* Implicit mapping */
#endif
	} /* Any mode mapping */
      } /* Current mode mapping */
      break;
    } /* Keyboard */
    case inputevent_type_pointer:
      cerr << "TInputToCommand::Consume - no code to consume pointer events" << endl;
      break;
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
 * The AddKeyboardMapping method.
 * *********************************************************************/
bool TInputToCommand::AddKeyboardMapping(gamemode_t mode, 
					 keyboard_inputevent_event_t event,
					 string cmd, string arg) {
  /* Check if a mapping is already present */
  TKeyboardCommandMapIterator loc 
    = KeyboardCommandMap[mode].find(event);
  if (loc != KeyboardCommandMap[GameMode.GetMode()].end()) {
    /* A mapping alread exists. */
    cerr << "TInputToCommand::AddKeyboardMapping - mapping for event already exists" << endl;
    return false;
  } else {
    KeyboardCommandMap[mode][event] = TCmdArg(cmd, arg);
    return true;
  }
}
