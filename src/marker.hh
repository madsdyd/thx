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
#ifndef __MARKER_HH__
#define __MARKER_HH__
#include "entities.hh"
#include "types.hh"
#include "viewpoint.hh"
#include "game.hh"

/* This entity type can be used to defer turn changing, etc for a number of seconds. */
class TDelay : public TEntity {
protected:
  float timeleft;
public:
  TDelay(TGame * game, TPlayer * owner, float ttl);
  virtual void Update(system_time_t deltatime);
  virtual void Render(TViewpoint * viewpoint) {};
  virtual void TakeDamage(TEntity * other_entity) {};
};

/* This is a marker, that will be visible and present for a number of seconds. */
class TMarker : public TDelay {
private:
  TVector location;
  float radius;
  float timeleft;
public:
  TMarker(TGame * game, TPlayer * owner, TVector loc, 
	  float nradius, float ttl);
  virtual void Render(TViewpoint * viewpoint);
  virtual void TakeDamage(TEntity * other_entity) {}; 
};
#endif
