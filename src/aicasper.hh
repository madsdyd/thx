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
#ifndef __AICASPER_HH__
#define __AICASPER_HH__
/* This is Casper - slightly more sophisticated than Baltazar
   He will 
   - try to obtain a target in PrepareRound
   - try to hit the target in each turn
   - accept feedback and try to adjust to it 

   Casper will not/is not
   - a teamplayer
   - able to buy or use different weapons
   - sensitive to wind

*/

#include "aiplayer.hh"

class TAIPlayerCasper : public TAIPlayer {
protected:
  /* Variables that tracks the "best" settings so far. */
  bool best_valid; /* True if the best_location is valid */
  TVector best_location; /* Used to keep track of current best location for hitting
			    target - updated each turn */
  float lowest_dist; /* Used to keep track of the impact that gave the most damage */
  /* Update the best_location, if apropiate */
  void UpdateBest(TVector * Hit);
public:
  TAIPlayerCasper(TGame * ngame, string nname, string nteam);
  virtual ~TAIPlayerCasper();
  void PrepareRound(TVector * location);
  void BeginTurn(); /* Called, when this players turn is about to start */
  void EndTurn();   /* Called, when this players turn is about to end */

  /* Called when we have an explosion */
  void Explosion(TExplosion * Explosion);   
  /* Called when we have a hit on a tank */
  void RegisterHit(TTank * Tank, TExplosion * Explosion, 
		   float dist, float damage);   
};
#endif
