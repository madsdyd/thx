/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#include "marker.hh"
#include <GL/glut.h>

/* **********************************************************************
   Delay */
TDelay::TDelay(TGame * game, TPlayer * owner, float ttl) 
  : TEntity(game, owner) {
  timeleft    = ttl;
  /* We wish for no turn change to happen while we are present */
  affect_turn = true;
}

void TDelay::Update(system_time_t deltatime) {
  timeleft -= deltatime;
  if (timeleft <= 0.0) {
    keep = false;
  }
}

/* **********************************************************************
   Marker - like delay, but have "presence" */
TMarker::TMarker(TGame * game, TPlayer * owner, 
		 TVector loc, 
		 float nradius, 
		 float ttl) : TDelay(game, owner, ttl) {
  location = loc;
  radius   = nradius;
};

void TMarker::Render(TViewpoint * viewpoint) {
  glPushMatrix();
  glTranslatef(location.x, location.y, location.z);
  glColor4f(1.0, 1.0, 0.0, 0.5);
  glutSolidSphere(radius, 4, 4);
  glPopMatrix();
}

