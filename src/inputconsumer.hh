/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#include "inputmouse.hh"
#include "gamemode.hh"

/* Define enough info about a command to issue it */
class TCmdArg {
public:
  string cmd;
  string arg;
  TCmdArg() {
    cmd = "ERROR";
    arg = "42";
  }
  TCmdArg(string ncmd, string narg) {
    cmd = ncmd; arg = narg;
  }
};

/* Define maps for our mappings between keyboard and mouse events */
typedef map <keyboard_inputevent_event_t, TCmdArg, lt_kiet> TKeyboardCommandMap;
typedef TKeyboardCommandMap::iterator TKeyboardCommandMapIterator;

typedef map <mouse_inputevent_event_t, TCmdArg, lt_miet> TMouseCommandMap;
typedef TMouseCommandMap::iterator TMouseCommandMapIterator;

/* This class can consume input events and fill the command queue */
class TInputToCommand {
private:
  /* These are mapping maps for all gamemodes */
  TKeyboardCommandMap KeyboardCommandMap[gamemode_count];
  TMouseCommandMap MouseCommandMap[gamemode_count];
public: 
  /* The constructor sets up the initial mappings */
  TInputToCommand();
  /* This initializes the input "systems" */
  void Init();
  /* This shutdowns the input system */
  void Shutdown();
  /* Consumes commands, and returns the number of commands added to
     the command queue */
  int Consume();
  
  /* **********************************************************************
   * Info about commands, and registering commands
   * Mostly used by the TControlsMenu and related objects.
   * *********************************************************************/
  /* Get the keys or mouse clicks - in a string - that are mapped to the command
     NOTE: Only "down" events from gamemode_game are returned by this. */
  string GetMappingsForCommand(string command, string args);
  /* Add a translation to the commandmap - true == added */
  bool AddKeyboardMapping(gamemode_t mode, keyboard_inputevent_event_t event,
			  string cmd, string arg);
  bool AddMouseMapping(gamemode_t mode, mouse_inputevent_event_t event,
		       string cmd, string arg);
};

/* The input consumer */
extern TInputToCommand InputToCommand;
#endif
