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
#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;


#include "entities.hh"
#include "vector.hh"

/* This controls wheter the projectile base class will spawn markers 
   during flight or not */
extern bool spawn_markers;

/* If only I knew a little bit more about "class factories" in C++, I
   could probably write this code a lot more elegant. Basically, I
   want to be able to create an object of any TProjectile type with
   different parameters, then later on copy this - along with its
   basic parameters - and get a "fired" TProjectile.  I am sure there
   are more elegant ways to do this then the approach below */

/* Standard projectile */
class TGame;
class TPlayer;
class TViewpoint;
class TProjectile : public TEntity {
protected:
  TVector location;
  TVector velocity;
  bool  track;    /* This is used to indicate wheter or not the missile should be
		     tracked - meaning that the missile will let the owner know
		     of its position */
  float radius;   /* This is used mostly to set the radius of the explosion */
  float strength; /* This is used mostly to set the strength of the explosion */
  virtual void OnPosUpdate(system_time_t deltatime);
  virtual void OnCollision(system_time_t deltatime, TVector loc);
  virtual void OnOrbit(system_time_t deltatime);

  /* This is overridden in each inherited class and actually 
     creates the object we want to return from fire
     The variables passed to the constructor 
     is assumed to be set up, so this is a simple object copy 
     I wish I knew more C++ :-(*/
  virtual TProjectile * CopyThis();
public:
  /* This constructor is used only for instantiating a "template" */
  TProjectile(TGame * game, TPlayer * owner, float rad, float str);  
  /* This constructor is used to create entities that can be used in the game */
  TProjectile(TGame * game, TPlayer * owner, float rad, float str, 
	      TVector loc, TVector vel);  
  /* The Fire method is used to create entities that can be used in the game */
  virtual TProjectile * Fire(TVector * loc, TVector * vel);
  virtual void Update(system_time_t deltatime);
  virtual void Render(TViewpoint * viewpoint);
  virtual void TakeDamage(TEntity * other_entity) {
    cout << "TProjectile::TakeDamage" << endl;
  }; 
};


/* Spawns several TProjectil on impact */
class TSpawnProjectile : public TProjectile {
protected:
  virtual void OnCollision(system_time_t deltatime, TVector loc);
  virtual TProjectile * CopyThis();
public:
  TSpawnProjectile(TGame * game, TPlayer * owner, float rad, float str) 
    : TProjectile(game, owner, rad, str) {};
  TSpawnProjectile(TGame * game, TPlayer * owner, float rad, float str, 
		   TVector loc, TVector vel) :
    TProjectile(game, owner, rad, str, loc, vel) {};
};

/* Spawn several Projectiles on orbit top (MIRV) */
class TMirvProjectile : public TProjectile {
protected:
  virtual void OnPosUpdate(system_time_t deltatime);
  virtual TProjectile * CopyThis();
public:
  TMirvProjectile(TGame * game, TPlayer * owner, float rad, float str) 
    : TProjectile(game, owner, rad, str) {};
  TMirvProjectile(TGame * game, TPlayer * owner, float rad, float str,
		  TVector loc, TVector vel) :
    TProjectile(game, owner, rad, str, loc, vel) {};
};

#endif
