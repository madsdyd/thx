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
#include "explosion.hh"
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
  best_valid = false;
  /* Now, a decision about the round strategy should be set. For now, the 
     strategy is always just to hit someone else. */
  /* This is also where the AI player buys weapons */
  
  /* Set the target to nothing. We will locate a target in begin turn */
  Target= NULL;
}

/* **********************************************************************
 * Set the goal for this turn
 * *********************************************************************/
void TAIPlayerCasper::BeginTurn() {
  /* Calling our parents BeginTurn should be safe and register the
     commands */
  TAIPlayer::BeginTurn();
  /* TODO: In here, the turn goals are set - actually meeting them is done
     in "Update" 
     Currently, there is no real "goals" - only hitting someone... */

  /* **********************************************************************
   * Make sure that we have an interessting target.
   * *********************************************************************/

  /* Store a pointer to the current target, such that we can perform
     operations that are only relevant, when we change the target */
  TPlayer * OldTarget = Target;

  /* If we have a target, it might not be active. Let us test that */
  if (Target) {
    if (!Target->IsActive()) {
      Display->console->AddLine("Caspers target is not active");
      Target = NULL;
    }
  }
  if (!Target) {
    /* Need to find an active target. If this is not possible, something
       is surely wrong. */
    Display->console->AddLine("Casper locates a new target");
    
    Target = game->NotMe(this);
    Assert(Target != NULL, 
	   "TAIPlayerCasper::BeginTurn - no other player found!");
  }
  
  /* **********************************************************************
   * At this point, we have a valid target.
   * If it is different from the old target, we should make a
   * kind of initial guess on the status of the cannon
   * *********************************************************************/
  
  if (OldTarget != Target) {
    /* Center the cannon angle, vary it sligthly */
    cannon_target.angle    =  40.0 + 10.0*rand()/(RAND_MAX+1.0);
    /* We perform a rough estimate on the target */
    /* Hopefully this is right, I do not have a math textbook here */
    /* cout << "Target : " << Target->tank->location.x << ", "
	 << Target->tank->location.y << endl;
    cout << "Casper : " << tank->location.x << ", "
    << tank->location.y << endl; */
    double dx = Target->tank->location.x - tank->location.x;
    double dy = Target->tank->location.y - tank->location.y;
    // cout << "dx : " << dx << ", dy : " << dy << endl;
    if (fabs(dx) < EPSILON) {
      Display->console->AddLine("Target is straight up/down");
      /* Either straight above us, or straight below us */
      if (dy > 0) {
	cannon_target.rotation = 90.0;
      } else {
	cannon_target.rotation = -90.0;
      } 
    } else {
      /* atan should do the trick, when we adjust for < 0 */
      cannon_target.rotation = RadToDegree(atan(dy/dx));
      if (dx < 0) {
	/* Ups, adjust */
	Display->console->AddLine("Adjusting tangent angle");
	cannon_target.rotation += 180.0;
      }
    }
    while (cannon_target.rotation < 0.0) {
      cannon_target.rotation += 360.0;
    }
    cout << "Rotation : " << cannon_target.rotation << endl;
    /* TODO: We should add some variation to the angle */
    /* Set the force to a suitable value.
       TODO: This should depend on the distance */
    cannon_target.force    = 8.0 + 8.0*rand()/(RAND_MAX+1.0);
    /* We have changed targets. The best_valid can not be true */
    best_valid = false;
  } else {    
    Display->console->AddLine("Casper is keeping his target");
    /* Clear the best valid, since we are about to shoot again. */
    best_valid = false;
  }
}

/* **********************************************************************
 * EndTurn should maybe evaluate how our turn went
 * *********************************************************************/
void TAIPlayerCasper::EndTurn() {
  /* Deregister the commands, etc */
  TPlayer::EndTurn();
  /* TODO: We should probably evaluate our strategy in here */
  Display->console->AddLine("TIAPlayerCasper::EndTurn");

  /* Try and get closer to hitting */

  /* TODO: Currently, we are right on spot, and there is no wind.
     This will change, and we need to compensate for it */

  /* First, check that we got an explosion at all */
  if (!best_valid) {
    Display->console->AddLine("Casper does not have a valid best");
    /* No hit - we probably shot outside the map */
    cannon_target.force = cannon_target.force / 2.0;
    cannon_target.angle += 1.0;
    return;
  }

  /* TODO: Scale the adjustment */

  /* Since we are on a line, we will need to go further, if we
     are short, and shorter, if we are too long */
  if (tank->location.Distance(Target->tank->location) <
      tank->location.Distance(&best_location)) {
    /* We shot too long */
    Display->console->AddLine("Casper shot too long");
    /* This adjustment needs to work better... */
    if (cannon_target.angle < 80) {
      cannon_target.angle += 5.0;
    }
    if (cannon_target.force > 2.0) {
      cannon_target.force -= 1.0;
    }
  } else {
    /* we shot to short... */
    Display->console->AddLine("Casper shot too short");
    /* This adjustment needs to work better... */
    if (cannon_target.angle > 30) {
      cannon_target.angle -= 2.0;
    }
    if (cannon_target.force < 25.0) {
      cannon_target.force += 2.0;
    }
  }
}

/* **********************************************************************
 * Explosion is called, when we register an explosion on the map
 * In here, we should record this turns best match.
 * *********************************************************************/
void TAIPlayerCasper::Explosion(TExplosion * Explosion) {
  Display->console->AddLine("TAIPlayerCasper::Explosion");
  UpdateBest(&(Explosion->location));
}

/* **********************************************************************
 * This is called, when a tank is hit. Please note, that the tank can be
 * our own (which is not good) 
 * *********************************************************************/
void TAIPlayerCasper::RegisterHit(TTank * Tank, TExplosion * Explosion, 
		 float dist, float damage) {
  if (tank != Tank) {
    Display->console->AddLine("TAIPlayerCasper::Explosion - hit someone");
  } else {
    Display->console->AddLine("TAIPlayerCasper::Explosion - hit ourselves!");
  }
  UpdateBest(&(Explosion->location));
}
/* **********************************************************************
 * Update the best_location as appropiate
 * *********************************************************************/
void TAIPlayerCasper::UpdateBest(TVector * Hit) {
  if (!best_valid ||
      (Target->tank->location.Distance(&best_location) >
       Target->tank->location.Distance(Hit))) {
    /* Update best location */
    best_location = *Hit;
    best_valid = true;
    Display->console->AddLine("Casper updated best valid");
  }
}
