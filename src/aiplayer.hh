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
#ifndef __AIPLAYER_HH__
#define __AIPLAYER_HH__
/* This is an AI player - based on a normal player */

#include "tank.hh"
#include "player.hh"

class TAIPlayer : public TPlayer {
protected:
  /* The Target is another player, if set */
  TPlayer * Target;
  /* variable used to store the target for cannon */
  cannon_t cannon_target;
  bool has_fired; /* true if we have fired in current round */
  void RegisterCommands();
  void UnregisterCommands();
  /* Used to follow the cannon around with */
  virtual void UpdateViewpoint();
  // void PerformCommandUpdate(system_time_t timenow);
public:
  TAIPlayer(TGame * ngame, string nname, string nteam);
  virtual ~TAIPlayer();
  virtual bool NeedsInteraction(); /* True if this player needs local interaction */
  virtual void PrepareRound(TVector * location);
  virtual void BeginTurn(); /* Called, when this players turn is about to start */
  virtual void EndTurn();   /* Called, when this players turn is about to end */
  /* Follow a fired missile */
  void TrackProjectile(TVector * location, TVector * velocity);
  /* The update command mostly handle the movement stuff */
  virtual void Update(system_time_t timenow);
};
#endif
