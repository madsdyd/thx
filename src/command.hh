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
#ifndef __COMMAND_HH__
#define __COMMAND_HH__

#include <string>
#include <queue>
#include "types.hh"

/* This is the definition of a command. 
   Typically, commands are issued from user input - that is, 
   user input is translated into a command. 
   Commands then live in the command queue, until dispatched by the 
   command consumer, and then freed.
*/
class TCommand {
public:
  system_time_t timestamp; /* The time when the command was created 
                              or, if originated in input, the time of input */
  string name;             /* The name of the command - used for dispatching */
  string args;             /* Optional args to the command */
  TCommand(system_time_t ntimestamp, string nname, string nargs = "");
};
/* This is the type of the command queue */
typedef queue <TCommand *> TCommands;
/* This is the command queue */
extern TCommands CommandQueue;
#endif
