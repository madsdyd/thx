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

/* Set to 0 to avoid messages about dispatching stuff */
#define DISPATCH_DEBUG 0

/* This is our CommandDispatcher */
TCommandDispatcher CommandDispatcher;
/* **********************************************************************
 * TCommandDispatcher methods 
 * *********************************************************************/

/* **********************************************************************
 * RegisterConsumer - maps command names to a command consumer.
 * *********************************************************************/
void TCommandDispatcher::RegisterConsumer(string commandname, 
					  TCommandConsumer * consumer) {
  CommandConsumers[commandname] = consumer;
#ifdef NEVER
  /* Check that commandname is not registered, store */
  TCommandConsumersIterator loc 
    = CommandConsumers.find(commandname);
  if (loc != CommandConsumers.end()) {
    /* Already registered */
    cerr << "TCommandDispatcher::RegisterConsumer - \"" << commandname
	 << "\" already have a consumer" << endl;
    return false;
  } else {
    CommandConsumers[commandname] = consumer;
#if (DISPATCH_DEBUG)
    cout << "TCommandDispatcher::RegisterConsumer - registered consumer"
	 << " for \"" << commandname << "\"" << endl;
#endif
  }
#endif

}

/* **********************************************************************
 * Unregister - frees a command name
 * *********************************************************************/
void TCommandDispatcher::UnregisterConsumer(string commandname) {
  CommandConsumers.erase(commandname);
#ifdef NEVER
  /* Find location of command name, delete it */
  TCommandConsumersIterator loc 
    = CommandConsumers.find(commandname);
  if (loc != CommandConsumers.end()) {
    CommandConsumers.erase(loc);
#if (DISPATCH_DEBUG)
    cout << "TCommandDispatcher::UnregisterConsumer - unregistered consumer"
	 << " for \"" << commandname << "\"" << endl;
#endif
    return true;
  } else {
    return false;
  }
#endif

}

/* **********************************************************************
 * Dispatch - travers queue, call consumers. 
 * *********************************************************************/
int TCommandDispatcher::Dispatch() {
  // cout << "TCommandDispatcher::Dispatch" << endl;
  int count = 0;
  TCommand * Command;
  TCommandConsumersIterator loc;
  /* Traverse queue, locate consumers for each command, call them */
  while(CommandQueue.size() > 0) {
    Command = CommandQueue.front();
    //cout << "Command: " << Command->timestamp << ", " << Command->name << ", " << Command->args << endl;
    loc = CommandConsumers.find(Command->name);
    if (loc != CommandConsumers.end()) {
      /* Found a consumer */
#if (DISPATCH_DEBUG)
      cout << "TCommandDispatcher::Dispatch - "
	   << "Calling consumer for \"" << Command->name << "\"" << endl;
#endif
      ((*loc).second)->CommandConsume(Command);
      count++;
    } 
#if (DISPATCH_DEBUG)
    else {
      cout << "TCommandDispatcher::Dispatch - "
	   << "No consumer for \"" << Command->name << "\"" << endl;
    }
#endif
    /* Iterate - we free the command */
    CommandQueue.pop();
    delete Command;
  }
  return count;
}
