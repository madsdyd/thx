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
/* The mappings from inputs to commands */
TKeyboardCommandMap KeyboardCommandMap;
/* The command queue */
TInputCommands InputCommands;
/* The InputToCommand object */
TInputToCommand InputToCommand;

/* **********************************************************************
 * The TInputToCommand class 
 * *********************************************************************/

/* **********************************************************************
 * The TInputToCommand constructor - sets up default mappings
 * *********************************************************************/
TInputToCommand::TInputToCommand() {
  keyboard_inputevent_event_t tmp;
  cout << "InputToCommand constructed" << endl;
  tmp.key  = 'a';
  tmp.type = keyboard_inputevent_type_down;
  KeyboardCommandMap[tmp] = "a down";
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
      TKeyboardCommandMapIterator loc 
	= KeyboardCommandMap.find(((TKeyboardInputEvent *) 
				   InputEvent)->keyboard_inputevent_event);
      if (loc != KeyboardCommandMap.end()) {
	cout << "TInputToCommand::Consume, key "
	     << ((TKeyboardInputEvent *) 
		 InputEvent)->keyboard_inputevent_event.key
	     << "maps to " << (*loc).second << endl;
      } else {
	/* No mapping found */
	cout << "TInputToCommand::Consume, no mapping found for key "
	     << ((TKeyboardInputEvent *) 
		 InputEvent)->keyboard_inputevent_event.key << endl;
      }
      break;
    }
    case inputevent_type_pointer:
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

