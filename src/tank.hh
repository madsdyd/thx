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
#ifndef __TANK_H__
#define __TANK_H__

#include "types.hh"
#include "entities.hh"
#include "vector.hh"
#include "color.hh"

typedef struct {
  int angle;
  int rotation;
  int force;
} cannon_t;

typedef enum { tankstate_alive,    /* Alive and well - health > 0 */
	       tankstate_dead,     /* Dead - health == 0 */
	       tankstate_animdead, /* In dead animantion */
	       tankstate_rip       /* Done with animation */
} tankstate_t;

class TExplosion;
class TMap;
class TObject;
class TTank : public TEntity {
protected:
  float levitation;               /* Used for death animation */
  tankstate_t tankstate;
public:
  TObject * model;
  TObject * barrel;
  TColor color;
  TVector location;
  cannon_t cannon;
  float health;
  TTank(TPlayer * owner);
  ~TTank();
  /* Prepare a tank for a new round, updates location, health, etc */
  void PrepareRound(TVector * loc);
  /* Adjust the force of the cannon */
  void AdjustForce(int adjust);
  /* Adjust the angle of the cannon */
  void AdjustAngle(int adjust);
  /* Adjust the rotation of the cannon */
  void AdjustRotation(int adjust);
  /* Update the tank. Not used at the moment */
  virtual void Update(TGame * game, system_time_t deltatime);
  /* Render the tank */
  virtual void Render(TViewpoint * viewpoint);
  /* Take damage from another entity */
  virtual void TakeDamage(TEntity * other_entity) {
    cout << "TTank::TakeDamage" << endl;
  }; 
  /* Register a hit by an explosion 
     TODO: Should use TakeDamage?*/
  void Explosion(TMap * map, TExplosion * Explosion);

};
#endif
