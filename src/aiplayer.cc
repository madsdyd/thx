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
#include "display.hh"

/* **********************************************************************
 * Constructors and destructors
 * *********************************************************************/
TAIPlayer::TAIPlayer(TGame * ngame, string nname, string nteam) 
  : TPlayer(ngame, nname, nteam) {
  Target = NULL;
};

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
 * NeedsInteraction returns true, if the player needs local interaction
 * AI's do not, and eventually, networked players will neither
 * *********************************************************************/

bool TAIPlayer::NeedsInteraction() {
  return false;
}


/* **********************************************************************
 * Round and turn commands - the meat of this object
 * *********************************************************************/

/* **********************************************************************
 * Prepareround should set the overall goal for the round, somehow
 * *********************************************************************/
void TAIPlayer::PrepareRound(TVector * location) {
  /* It should be safe to perform the same steps as our parent. */
  TPlayer::PrepareRound(location);
  /* Now, a decision about the round strategy should be set. For now, the 
     strategy is always just to hit someone else. */
  /* This is also where the AI player buys weapons */
}

/* **********************************************************************
 * Set the goal for this turn
 * *********************************************************************/
void TAIPlayer::BeginTurn() {
  /* Calling our parents BeginTurn should be safe and register the
     commands */
  TPlayer::BeginTurn();
  /* In here, the turn goals are set - actually meeting them is done
     in "Update"
     The basic player does not set any goals */
}

/* **********************************************************************
 * EndTurn should maybe evaluate how our turn went
 * *********************************************************************/
void TAIPlayer::EndTurn() {
  /* Deregister the commands, etc */
  TPlayer::EndTurn();
  /* We should probably evaluate our strategy in here */
}

/* **********************************************************************
 * The update function simply tries to meet the round goals - adjusting
 * the angle, rotation and force, and finally firering, when all are meet
 * *********************************************************************/
void TAIPlayer::Update(system_time_t timenow) {
  /* Call our parents update */
  TPlayer::Update(timenow);
  
  /* In here, we check if our turngoal is met, and if that is the case, 
     we kinda fire */
  bool cr, ca, cf;
  
  /* Adjust to the correct rotation */
  if (fabs(cannon_target.rotation - tank->cannon.rotation) < 1.0) {
    // cout << "TAIPlayer::Update - goal met" << endl;
    /* If we are close enough, stop adjusting the cannon 
       This should work, since we are on simulated time */
    CommandConsume(new TCommand(timenow, "cannon", "-rotate-left"));
    CommandConsume(new TCommand(timenow, "cannon", "-rotate-right"));
    cr = true;
  } else {
    /* We need to rotate: If the normalized difference between 
       destination and source is greater then 180 degrees, we should
       turn right, otherwise left. */
    float tmprot = cannon_target.rotation - tank->cannon.rotation;
    while (tmprot < 0.0) {
      tmprot += 360;
    }
    if (tmprot > 180.0) {
      /* Stop any pending raises, insert lower (duplicates will not
	 be inserted) */
      CommandConsume(new TCommand(timenow, "cannon", "-rotate-left"));
      CommandConsume(new TCommand(timenow, "cannon", "+rotate-right"));
    } else {
      /* Stop any pending lowers, insert raise (duplicates will not
	 be inserted */
      CommandConsume(new TCommand(timenow, "cannon", "-rotate-right"));
      CommandConsume(new TCommand(timenow, "cannon", "+rotate-left"));
    }
    cr = false;
    UpdateViewpoint();
    return;
  }
  
  /* Adjust to the correct angle - same as rotation, really */
  if (fabs(cannon_target.angle - tank->cannon.angle) < 1.0) {
    CommandConsume(new TCommand(timenow, "cannon", "-lower"));
    CommandConsume(new TCommand(timenow, "cannon", "-raise"));
    ca = true;
  } else {
    if (cannon_target.angle < tank->cannon.angle) {
      CommandConsume(new TCommand(timenow, "cannon", "-raise"));
      CommandConsume(new TCommand(timenow, "cannon", "+lower"));
    } else {
      CommandConsume(new TCommand(timenow, "cannon", "-lower"));
      CommandConsume(new TCommand(timenow, "cannon", "+raise"));
    }
    ca = false;
    UpdateViewpoint();
    return;
  }
  
  /* Adjust the force - same as rotation, really */
  if (fabs(cannon_target.force - tank->cannon.force) < 1.0) {
    CommandConsume(new TCommand(timenow, "cannon", "-force-increase"));
    CommandConsume(new TCommand(timenow, "cannon", "-force-decrease"));
    cf = true;
  } else {
    if (cannon_target.force < tank->cannon.force) {
      CommandConsume(new TCommand(timenow, "cannon", "-force-increase"));
      CommandConsume(new TCommand(timenow, "cannon", "+force-decrease"));
    } else {
      CommandConsume(new TCommand(timenow, "cannon", "-force-decrease"));
      CommandConsume(new TCommand(timenow, "cannon", "+force-increase"));
    }
    cf = false;
    UpdateViewpoint();
    return;
  }

  UpdateViewpoint();
  /* If all our objectives are meet, fire the projectile. 
     Caveat: We do need to have a target... */
  if (ca && cr && cf && Target) {
    game->FireProjectile();
  }
}

/* **********************************************************************
 * This updates the viewpoint, to allow us to follow the cannon
 * *********************************************************************/

void TAIPlayer::UpdateViewpoint() {
  // viewpoint.rotation.x = tank->cannon.angle;
  viewpoint.rotation.x = 20;
  viewpoint.rotation.z = -tank->cannon.rotation + 90.0;
  
  viewpoint.translation = tank->location;
  double scale_move = 3.0;
  viewpoint.translation.x 
    -= scale_move * sin(viewpoint.rotation.z * M_PI / 180.0);
  viewpoint.translation.y 
    -= scale_move * cos(viewpoint.rotation.z * M_PI / 180.0);
  viewpoint.translation.z 
    += scale_move * sin(viewpoint.rotation.x * M_PI / 180.0);

  /* TODO : Adjust further, based on */
  Display->UpdateViewpoint();

}
