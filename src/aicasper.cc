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
#include "aicasper.hh"
#include "game.hh"
#include "debug.hh"
#include "display.hh" /* To use the console. This may be stupid */
#include "console.hh"
/* **********************************************************************
 * Constructors and destructors
 * *********************************************************************/
TAIPlayerCasper::TAIPlayerCasper(TGame * ngame, string nname, string nteam) 
  : TAIPlayer(ngame, nname, nteam) {
  Target = NULL;
  
};

TAIPlayerCasper::~TAIPlayerCasper() {};

/* **********************************************************************
 * Round and turn commands - the meat of this object
 * *********************************************************************/

/* **********************************************************************
 * Prepareround should set the overall goal for the round, somehow
 * *********************************************************************/
void TAIPlayerCasper::PrepareRound(TVector * location) {
  /* It should be safe to perform the same steps as our parent. */
  TAIPlayer::PrepareRound(location);
  /* Now, a decision about the round strategy should be set. For now, the 
     strategy is always just to hit someone else. */
  /* This is also where the AI player buys weapons */

  /* Let us try and locate a target */
  Target = game->NotMe(this);
  Assert(Target, "TAIPlayerCasper::PrepareRound - no other player found!");
  Display->console->AddLine("Casper found a target!");
}

/* **********************************************************************
 * Set the goal for this turn
 * *********************************************************************/
void TAIPlayerCasper::BeginTurn() {
  /* Calling our parents BeginTurn should be safe and register the
     commands */
  TAIPlayer::BeginTurn();
  /* In here, the turn goals are set - actually meeting them is done
     in "Update" 
     Please note that the basic AI player is totally random in its goals,
     modelled after the way my son played, when he was 4 months old. ;-) */
  Display->console->AddLine("TIAPlayerCasper::BeginTurn needs to do something");
  cannon_target.angle    =  90.0*rand()/(RAND_MAX+1.0);
  cannon_target.rotation = 359.0*rand()/(RAND_MAX+1.0);
  cannon_target.force    = 100.0*rand()/(RAND_MAX+1.0);
  /* cout << "TAIPlayerCasper::BeginTurn - setting goals " 
     << cannon_target.angle << ", " << cannon_target.rotation 
     << ", " << cannon_target.force << endl; */

}

/* **********************************************************************
 * EndTurn should maybe evaluate how our turn went
 * *********************************************************************/
void TAIPlayerCasper::EndTurn() {
  /* Deregister the commands, etc */
  TPlayer::EndTurn();
  /* We should probably evaluate our strategy in here */
}


