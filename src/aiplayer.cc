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
#include "aiplayer.hh"
#include "game.hh"

/* **********************************************************************
 * Constructors and destructors
 * *********************************************************************/
TAIPlayer::TAIPlayer(TGame * ngame, string nname, string nteam) 
  : TPlayer(ngame, nname, nteam) {};

TAIPlayer::~TAIPlayer() {};

/* **********************************************************************
 * Register and unregister commands
 * This player does not handle commands that go the canon or inventory
 * *********************************************************************/
void TAIPlayer::RegisterCommands() {
  CommandDispatcher.RegisterConsumer("viewpoint-move", this);
  CommandDispatcher.RegisterConsumer("viewpoint-rotate", this);
}

void TAIPlayer::UnregisterCommands() {
  CommandDispatcher.UnregisterConsumer("viewpoint-move");
  CommandDispatcher.UnregisterConsumer("viewpoint-rotate");
}

/* **********************************************************************
 * Round and turn commands - the meat of this object
 * *********************************************************************/
void TAIPlayer::PrepareRound(TVector * location) {
  /* It should be safe to perform the same steps as our parent. */
  TPlayer::PrepareRound(location);
  /* Now, a decision about the round strategy should be set. For now, the 
     strategy is always just to hit someone else. */
  /* This is also where the AI player buys weapons */
}

void TAIPlayer::BeginTurn() {
  /* Calling our parents BeginTurn should be safe and register the
     commands */
  TPlayer::BeginTurn();
  /* In here, the turn goals are set - actually meeting them is done
     in "Update" */
  cannon_target.angle    = 15;
  cannon_target.rotation = 170;
  cannon_target.force    = 8;
  cout << "TAIPlayer::BeginTurn - setting goals" << endl;

}

void TAIPlayer::EndTurn() {
  /* Deregister the commands, etc */
  TPlayer::EndTurn();
  /* We should probably evaluate our strategy in here */
}

void TAIPlayer::Update(system_time_t timenow) {
  /* Call our parents update */
  TPlayer::Update(timenow);
  
  /* In here, we check if our turngoal is met, and if that is the case, 
     we kinda fire */
  bool ca, cr, cf;
  if (fabs(cannon_target.angle - tank->cannon.angle) < 1.0) {
    // cout << "TAIPlayer::Update - goal met" << endl;
    /* If we are close enough, stop adjusting the cannon 
       This should work, since we are on simulated time */
    CommandConsume(new TCommand(timenow, "cannon", "-lower"));
    CommandConsume(new TCommand(timenow, "cannon", "-raise"));
    ca = true;
  } else {
    if (cannon_target.angle < tank->cannon.angle) {
      cout << "TAIPlayer::Cannon too high " << tank->cannon.angle << endl;
      /* Stop any pending raises, insert lower (duplicates will not
	 be inserted) */
      CommandConsume(new TCommand(timenow, "cannon", "-raise"));
      CommandConsume(new TCommand(timenow, "cannon", "+lower"));
    } else {
      cout << "TAIPlayer::Cannon too low: " << tank->cannon.angle << endl;
      /* Stop any pending lowers, insert raise (duplicates will not
	 be inserted */
      CommandConsume(new TCommand(timenow, "cannon", "-lower"));
      CommandConsume(new TCommand(timenow, "cannon", "+raise"));
    }
    ca = false;
  }

  /* If all our objectives are meet, fire the projectile. */
  if (ca) {
    game->FireProjectile();
  }
}

