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
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <map>

#include "color.hh"
#include "viewpoint.hh"
#include "commandconsumer.hh"
#include "command.hh"
/* A player is either a human or bot controlled participiant in the world */

/* This type is used to control the active commands */
typedef map <string, TCommand> TActiveCommands;
typedef TActiveCommands::iterator TActiveCommandsIterator;
typedef pair <string, TCommand> TActiveCommandsElement;

class TInventory;
class TTank;
class TGame;
class TPlayer : public TCommandConsumer {
protected:
  /* These are the commands that are active for the 
     current client. The string is used to identify the 
     command, mostly when removing and inserting commands */
  map <string, TCommand> active_commands;
  bool RegisterCommands();
  bool UnregisterCommands();
  void PerformCommandUpdate(system_time_t timenow);
public:
  string name;            /* The players name */
  TInventory * inventory; /* Used to hold and create projectiles */
  int money;              /* The money the player have */
  int score;              /* The score */
  TTank * tank;           /* The players tank */
  TColor color;           /* Used at all? */
  TViewpoint viewpoint;   /* The players viewpoint */
  TPlayer(string nname);
  virtual ~TPlayer();
  void PrepareRound(TGame * game, TVector * location);
  void BeginTurn();       /* Called, when this players turn is about to start */
  void EndTurn();         /* Called, when this players turn is about to end */
  virtual bool CommandConsume(TCommand * Command);
  /* The update command mostly handle the movement stuff */
  void Update(system_time_t timenow
);
};
#endif
