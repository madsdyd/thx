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
#ifndef __EXPLOSION_HH__
#define __EXPLOSION_HH__
#include "entities.hh"
#include "vector.hh"
/* This should probably be a parameter on an explosion/projectile */
#define EXPLOSION_DAMAGE 50

/* Some forward defines */
class TGame; 
class TViewpoint;

/* Standard explosion */
class TExplosion : public TEntity {
private:
  float radius;
  float speed;
public:
  TVector location;
  float maxradius, damage;
  TExplosion(TPlayer * owner, TVector loc, float mradius, float espeed, float edamage);
  virtual void Update(TGame * game, system_time_t deltatime);
  virtual void Render(TViewpoint * viewpoint);
  virtual void TakeDamage(TEntity * other_entity) {
    cout << "TExplosion::TakeDamage" << endl;
  }; 
};
#endif
