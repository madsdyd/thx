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
#include <iostream>
#include "command.hh"
#include "commandconsumer.hh"

/* This is our CommandDispatcher */
TCommandDispatcher CommandDispatcher;
/* **********************************************************************
 * TCommandDispatcher methods 
 * *********************************************************************/

/* **********************************************************************
 * RegisterConsumer - maps command names to a command consumer.
 * *********************************************************************/
bool TCommandDispatcher::RegisterConsumer(string commandname, 
					  TCommandConsumer * consumer) {
  /* Check that commandname is not registered, store */
  TCommandConsumersIterator loc 
    = CommandConsumers.find(commandname);
  if (loc != CommandConsumers.end()) {
    /* Already registered */
    return false;
  } else {
    CommandConsumers[commandname] = consumer;
    cout << "TCommandDispatcher::RegisterConsumer - registered consumer"
	 << " for " << commandname << endl;
  }
}

/* **********************************************************************
 * Unregister - frees a command name
 * *********************************************************************/
bool TCommandDispatcher::UnregisterConsumer(string commandname) {
  /* Find location of command name, delete it */
  TCommandConsumersIterator loc 
    = CommandConsumers.find(commandname);
  if (loc != CommandConsumers.end()) {
    CommandConsumers.erase(loc);
    return true;
  } else {
    return false;
  }
}

/* **********************************************************************
 * Dispatch - travers queue, call consumers. 
 * *********************************************************************/
int TCommandDispatcher::Dispatch() {
  int count = 0;
  TCommand * Command;
  TCommandConsumersIterator loc;
  /* Traverse queue, locate consumers for each command, call them */
  while(CommandQueue.size() > 0) {
    Command = CommandQueue.front();
    cout << "Command: " << Command->timestamp << ", " << Command->name << endl;
    loc = CommandConsumers.find(Command->name);
    if (loc != CommandConsumers.end()) {
      /* Found a consumer */
      cout << "TCommandDispatcher::Dispatch - "
	   << "Should call consumer for " << Command->name << endl;
      count++;
    } else {
      cout << "TCommandDispatcher::Dispatch - "
	   << "No consumer for " << Command->name << endl;
    }
    CommandQueue.pop();
    delete Command;
  }
  return count;
}
