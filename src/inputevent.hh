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
#ifndef __INPUTEVENT_HH__
#define __INPUTEVENT_HH__
/* A queue of input events are defined here. Later these are meant to
   be converted into commands, that are to be interprented by the game
   engine This may or may not be a good design - I have no clue */
#include <queue>
#include "types.hh"

/* This defines the type of input, keyboard and pointer (mouse) for
   now */
typedef enum {inputevent_type_keyboard,   /* Keyboard, obviously */
	      inputevent_type_pointer     /* Mouse most likely */
} inputevent_type_t;

/* This is an actual input event. Both keyboard and mouse are supposed
   to extend this class */
class TInputEvent { 
public:
  system_time_t timestamp;
  inputevent_type_t eventtype;
  TInputEvent(inputevent_type_t neventtype);
};

/* This is a definition of the input queue */
typedef queue<TInputEvent *> TInputEvents;
struct TInputs {
  TInputEvents Events;
};

extern TInputs Inputs; /* In inputevent.cc */
#endif
