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

/* The mappings from inputs to commands */
TKeyboardCommandMap KeyboardCommandMap;
/* The InputToCommand object */
TInputToCommand InputToCommand;

/* **********************************************************************
 * The TInputToCommand class 
 * *********************************************************************/

/* **********************************************************************
 * The TInputToCommand constructor - sets up default mappings
 * *********************************************************************/

/* First some nice default mappings 
   TODO: Something is rotten here? */
struct keymap_t {
  keyboard_inputevent_event_t kev;
  char * cmd;
};

keymap_t key_map_std_keys_down[] =
{
  {{'a', keyboard_inputevent_type_down}, "a"},
  /* FINAL */
  {{'z', keyboard_inputevent_type_down}, ""}
};

TInputToCommand::TInputToCommand() {
  int i; 
  /* TODO: Nicer way to initialize - load and save, so on */
  /* Map the standard keys */
  i = 0;  
  
  while(strcmp(key_map_std_keys_down[i].cmd, "")) {
    cout << "i = " << i << endl;
    KeyboardCommandMap[key_map_std_keys_down[i].kev] 
      = key_map_std_keys_down[i].cmd;
    i++;
  }
}

/* **********************************************************************
 * The Consume method.
 * *********************************************************************/
int TInputToCommand::Consume() {
  /* The consume method grabs input events from the inputevent queue and
     figure out what command string is associated with the event. 
     This is then added to the command queue */
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
      TKeyboardCommandMapIterator loc 
	= KeyboardCommandMap.find(KeyEvent->keyboard_inputevent_event);
      if (loc != KeyboardCommandMap.end()) {
	/* Found a "real" mapping */
	cout << "TInputToCommand::Consume, key "
	     << KeyEvent->keyboard_inputevent_event.key
	     << " maps to " << (*loc).second << endl;
	CommandQueue.push(new TCommand(KeyEvent->timestamp,
				       (*loc).second));	       
      } else {
	/* No mapping found -
	   Keyboard events for "down" that are not mapped, 
	   generate a command that has the name of the key 
	   if the key is within a certain range. */
	if (keyboard_inputevent_type_down 
	    == KeyEvent->keyboard_inputevent_event.type) {
	  // TODO: Actually distpatch a command 
	  cout << "TODO: Actually dispatch a command (automapped)" << endl;
	  cout << "TInputToCommand::Consume, key (down)"
	       << KeyEvent->keyboard_inputevent_event.key
	       << " automaps to " << "keydown " 
	       << KeyEvent->keyboard_inputevent_event.key << endl;
	  CommandQueue.push(new TCommand(KeyEvent->timestamp,
					 "keydown"));
	  // TODO: Fix argument.
	  //					 string(KeyEvent->keyboard_inputevent_event.key)));	
	} else {
	  cout << "TInputToCommand::Consume, no mapping found for key "
	       << ((TKeyboardInputEvent *) 
		   InputEvent)->keyboard_inputevent_event.key 
	       << " and this type (up)" << endl;
	}
      }
      break;
    }
    case inputevent_type_pointer:
      cout << "TInputToCommand::Consume - no code to consume pointer events" << endl;
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

