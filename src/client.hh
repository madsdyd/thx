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
#ifndef __CLIENT_HH__
#define __CLIENT_HH__

#include <set>

/* The purpose of the client is to be the link between the server, game and 
   other parts of the system. Granted, there are no server support, but one is 
   allowed to hack, isn't one? Here is how I think it should be viewed:

   client <--talks to--> server <--has--> game
    
   client <--talks to and has-->display, menu, sound, keyboard

   client <--talks to--> GLUT
   
   As said, there are no server, and no talking, but since there is no design 
   either that figures :-). As of now, the client talks directly to the game.

   Oh, and of course it very little sense to put the client into a class, 
   but it makes it easier to refer to.
*/
#include "commandconsumer.hh"
class TCommand;
class TClient : public TCommandConsumer {
public:
  bool game_running; /* If we do not have a game running, we show the 
			game menu */
  int has;           /* has covers the logical ors of elements that needs 
			update/keyboard/display */
public:
  /* Construct the client object 
     Stuff that are related to the client gets constructed and initialized as
     well. This includes the menu, sound and display system, but not the 
     (nonpresent) server */
  TClient(int argc, char ** argv);
  virtual ~TClient();
  /* Run the game - Used to dispatch the glut events, and so on... */
  void Run();
  /* The Commands the client consume */
  virtual bool CommandConsume(TCommand * Command); 
};
 
extern TClient * Client;
#endif
