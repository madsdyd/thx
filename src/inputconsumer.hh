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
#ifndef __INPUTCONSUMER_HH__
#define __INPUTCONSUMER_HH__
/* This unit declares a consumer of input events. Events are mapped to 
   commands, that are stuffed into the command chain. */
#include <string>
#include <map>
#include "inputkeyboard.hh"

/* This map contains the mappings between keyboard input events and
   command names */
typedef map <keyboard_inputevent_event_t, string, lt_kiet> TKeyboardCommandMap;
typedef TKeyboardCommandMap::iterator TKeyboardCommandMapIterator;
extern TKeyboardCommandMap KeyboardCommandMap;

/* This is a input command */
struct TInputCommand {
  system_time_t timestamp;
  string command;
};
/* Definition of the queue of commands */
typedef queue<TInputCommand *> TInputCommands;
/* This is the list of pending commands */
extern TInputCommands InputCommands;

/* This class can consume input events and fill the command queue */
class TInputToCommand {
public: 
  /* The constructor sets up the initial mappings */
  TInputToCommand();
  /* Returns the number of commands added to the command queue */
  int Consume();
};

/* The input consumer */
extern TInputToCommand InputToCommand;
#endif
