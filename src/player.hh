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

using std::string;
using std::map;
using std::pair;

#include "color.hh"
#include "viewpoint.hh"
#include "commandconsumer.hh"
#include "command.hh"


/* **********************************************************************
 * A player is either a human or bot controlled participiant in the
 * world
 * The base TPlayer class is supposed to be controlled by a human
 * *********************************************************************/

/* This type is used to control the active commands */
typedef map <string, TCommand> TActiveCommands;
typedef TActiveCommands::iterator TActiveCommandsIterator;
typedef pair <string, TCommand> TActiveCommandsElement;

class TInventory;
class TTank;
class TGame;
class TExplosion;
class TPlayer : public TCommandConsumer {
protected:
  /* These are the commands that are active for the 
     current client. The string is used to identify the 
     command, mostly when removing and inserting commands */
  TGame * game;
  map <string, TCommand> active_commands;
  virtual void RegisterCommands();
  virtual void UnregisterCommands();
  virtual void PerformCommandUpdate(system_time_t timenow);
public:
  string name;            /* The players name */
  string team;            /* The team the player is on, if in team mode */
  TInventory * inventory; /* Used to hold and create projectiles */
  int money;              /* The money the player have */
  int score;              /* The score */
  TTank * tank;           /* The players tank */
  TColor color;           /* Used at all? */
  TViewpoint viewpoint;   /* The players viewpoint */
  TPlayer(TGame * ngame, string nname, string nteam);
  virtual ~TPlayer();
  virtual void PrepareRound(TVector * location);
  virtual bool IsActive();        /* True if tank is alive */
  virtual bool NeedsInteraction(); /* True if this player needs local interaction */
  virtual void BeginTurn();       /* Called, when this players turn is about to 
				     start */
  virtual void EndTurn();         /* Called, when this players turn is about to end */
  /* Called when a tracking missile has been updated */
  virtual void TrackProjectile(TVector * location, TVector * velocity) {};
  /* Called, when the fired projectile has impact */
  virtual void ImpactProjectile(TVector * location, TVector * velocity) {};

  /* Called, whenever there is an explosion owned by the player */
  virtual void Explosion(TExplosion * Explosion) {}; 
  /* Called from a tank, when a players projectile hits a tank */  
  virtual void RegisterHit(TTank * Tank, TExplosion * Explosion, 
			   float dist, float damage) {}; 
  virtual bool CommandConsume(TCommand * Command);
  /* The update command mostly handle the movement stuff */
  virtual void Update(system_time_t timenow);
};
#endif
