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
#ifndef __COMMANDCONSUMER_HH__
#define __COMMANDCONSUMER_HH__
/* This unit defines a class that consume commands, and call functions 
   that are registered to handle the command. I do not know if this is generic
   enough, but assuming the game state is global and all functions works on 
   the gamestate, it should be OK */
#include <string>
#include <map>
/* The first class defines a base class for classes that wants to consume 
   commands. */
class TCommand;
class TCommandConsumer {
public:
  virtual bool CommandConsume(TCommand * Command) = 0;
};
/* Define the mappings from command names to consumers */
typedef map <string, TCommandConsumer *> TCommandConsumers;
typedef TCommandConsumers::iterator TCommandConsumersIterator;

/* This class is the actual dispatcher of commands */
class TCommandDispatcher {
private: 
  TCommandConsumers CommandConsumers;
public:
  /* Register a consumer of commands */
  bool RegisterConsumer(string commandname, TCommandConsumer * consumer);
  /* Unregister a consumer of commands */
  bool UnregisterConsumer(string commandname);
  /* Dispatch and return the number of commands dispatched */
  int Dispatch();
};

/* And, this is the instance */
extern TCommandDispatcher CommandDispatcher;
#endif
