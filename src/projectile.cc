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
#include <stdlib.h>
#include "projectile.hh"

#include "explosion.hh"
#include "marker.hh"
#include "map.hh"

#include "display.hh"
#include "console.hh"

#ifdef SOUND_ON
#include "sound.hh"
#endif

/* **********************************************************************
   Configuration stuff, accessible from the outside */
bool spawn_markers = false;

/* **********************************************************************
 * Constructors
 * *********************************************************************/
TProjectile::TProjectile(TGame * game, TPlayer * owner, 
			 float rad, float str) : TEntity(game, owner) {
  radius   = rad;
  strength = str;
  /* We affect turn changing */
  affect_turn = true;
};

TProjectile::TProjectile(TGame * game, TPlayer * owner, 
			 float rad, float str, 
			 TVector loc, TVector vel) : TEntity(game, owner) {
  radius   = rad;
  strength = str;
  location = loc;
  velocity = vel;
  /* We affect turn changing */
  affect_turn = true;
};

/* **********************************************************************
 * CopyThis method
 * *********************************************************************/
TProjectile * TProjectile::CopyThis() {
  return new TProjectile(game, owner, radius, strength);
}

/* **********************************************************************
 * Fire method
 * *********************************************************************/
TProjectile * TProjectile::Fire(TVector * loc, TVector * vel) {
  TProjectile * tmp = CopyThis();
  tmp->location = *loc;
  tmp->velocity = *vel;
  return tmp;
}

/* **********************************************************************
 * OnOrbit - default spawns little explosion
 * *********************************************************************/
void TProjectile::OnOrbit(system_time_t deltatime) {
  Display->console->AddLine("Projectile went into orbit");
  /* Insert small delay */
  game->AddEntity(new TDelay(game, owner, 1)); 
  /* Insert small explosion entity */
  /*  game->AddEntity(new TExplosion(owner, location, radius/4.0, 1.0, strength/4.0));
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/explosion.raw"]);
#endif
  */
  /* Remove this projectile during next game update */
  keep = false;
}

/* **********************************************************************
 * OnCollision - default spawns explosion
 * *********************************************************************/
void TProjectile::OnCollision(system_time_t deltatime, 
			      TVector loc) {
  // cout << "TProjectile impact detected" << endl;
  game->AddEntity(new TExplosion(game, owner, loc, radius, 6.0, strength));
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/explosion.raw"]);
#endif
  keep = false;
}

/* **********************************************************************
 * OnPosUpdate - called on succesfull update. The position _has_ been
 * changed, velocity updated
 * *********************************************************************/
void TProjectile::OnPosUpdate(system_time_t deltatime) {
  /* Do nothing */
}

/* **********************************************************************
 * Update - explanation goes here
 * *********************************************************************/
void TProjectile::Update(system_time_t deltatime) {
  TVector new_location = location;
  
  /* We update the location according to the time spend */
  new_location.x += velocity.x * deltatime;
  new_location.y += velocity.y * deltatime;
  new_location.z += velocity.z * deltatime;
  
  /* Checking within map is done by calling map */
  if (!game->GetMap()->Within(&(new_location))) {
    /* Oops, outside map, call OnOrbit */
    OnOrbit(deltatime);
    return;
  }
  
  /*****************************************************
   *****************************************************

   This is probably a good place to make a kind of:
    for (i = 0 ; i < numPlayers; i++) {
      if (players[i].tank->model.CollisionDetect) {
        players[i].explode();
      }
    }

   *****************************************************/


  /* Impact checking is done by calling the map. */
  if (game->GetMap()->CollisionDetect(&(location), &new_location)) {
    /* IMPACT! - call OnImpact */
    OnCollision(deltatime, new_location);
    return;
  };

  /* Update according to gravity */
  location = new_location;
  velocity.z -= GRAVITY * deltatime;

  /* Update according to wind */
  game->ApplyWind(velocity, deltatime);

  /* Set a marker if the option is on. */
  if (spawn_markers)
    game->AddEntity(new TMarker(game, owner, location, 0.05, 2.0));
  /* Call OnPosUpdate */
  OnPosUpdate(deltatime);
}

/* **********************************************************************
   Render a projectile */
void TProjectile::Render(TViewpoint * viewpoint) {
  // cout << "TProjectile::Render" << endl;
  glPushMatrix();
  glTranslatef(location.x, location.y, location.z);
  glColor4f(0.8, 0.8, 0.8, 1.0);
  /* This used to be a radius measurement, but should change to some kind 
     of model later on */
  glutSolidSphere(0.20, 8, 8);
  glPopMatrix();
};


/* **********************************************************************
 * **********************************************************************
 * TSpawnProjectile - spawns several kids 
 * **********************************************************************
 * *********************************************************************/
TProjectile * TSpawnProjectile::CopyThis() {
  return new TSpawnProjectile(game, owner, radius, strength);
}

void TSpawnProjectile::OnCollision(system_time_t deltatime, 
				   TVector loc) {
  TVector vel;
  vel.x = 1.0;
  vel.y = 1.0;
  vel.z = 10.0;

  /* TODO: This is a hack - need to make sure the spawned projectiles do not 
     explode on the first round
     When the collision detection is fixed, this should probably go. */
  loc.z += 1.5;
  game->AddEntity(new TProjectile(game, owner, radius, 
				  strength/4.0, loc, vel));
  vel.x = -vel.x;
  game->AddEntity(new TProjectile(game, owner, radius, 
				  strength/4.0, loc, vel));
  vel.y = -vel.y;
  game->AddEntity(new TProjectile(game, owner, radius, 
				  strength/4.0, loc, vel));
  vel.x = -vel.x;
  game->AddEntity(new TProjectile(game, owner, radius, 
				  strength/4.0, loc, vel));
  /* Add a minor explosion */
  game->AddEntity(new TExplosion(game, owner, loc, 
				 radius/4.0, 6.0, strength/4.0));
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/explosion.raw"]);
#endif

  keep = false;
}

/* **********************************************************************
 * **********************************************************************
 *    TMirvProjectile - Spawn at top 
 * **********************************************************************
 * *********************************************************************/
TProjectile * TMirvProjectile::CopyThis() {
  return new TMirvProjectile(game, owner, radius, strength);
}

void TMirvProjectile::OnPosUpdate(system_time_t deltatime) {
  /* if vel.z < 0, spawn, do not keep */
  if (velocity.z <= 0) {
    TVector vel;
    vel.x = velocity.x;
    vel.y = velocity.y;
    vel.z = 0.0;
    /* Add projectile that continues same track */
    game->AddEntity(new TProjectile(game, owner, radius, strength/4.0, 
				    location, vel));

    /* Add front, left, behind and back */
    vel = vel*0.8F;
    game->AddEntity(new TProjectile(game, owner, radius, strength/4.0, 
				    location, vel));
    vel = vel*1.25F*1.25F;
    game->AddEntity(new TProjectile(game, owner, radius, strength/4.0, 
				    location, vel));
    vel = vel*0.8F;
    vel.rotatexy(M_PI/8.0);
    game->AddEntity(new TProjectile(game, owner, radius, strength/4.0, 
				    location, vel));
    vel.rotatexy(2.0*M_PI-M_PI/4.0);
    game->AddEntity(new TProjectile(game, owner, radius, strength/4.0, 
				    location, vel)); 

    /* Add a minor explosion */
    game->AddEntity(new TExplosion(game, owner, location, 
				   radius/4.0, 6.0, strength/4.0)); 
#ifdef SOUND_ON
    sound_play(names_to_nums["data/sounds/explosion.raw"]);
#endif    
    /* Remove this projectile on next update */
    keep = false;
  }
}

