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
#include "explosion.hh"
#include <GL/glut.h>
#include "game.hh"
#include "marker.hh" /* For delays */

/* **********************************************************************
   Initialize an explosion */
TExplosion::TExplosion(TPlayer * owner, TVector loc, 
		       float mradius, float espeed, float edamage) 
  : TEntity(owner) {
  location    = loc;
  radius      = 0;
  maxradius   = mradius;
  speed       = espeed;
  damage      = edamage;
  /* We affect turn changing */
  affect_turn = true;
}

/* **********************************************************************
   Update a explosion */
void TExplosion::Update(TGame * game, system_time_t deltatime) {
  /* We start at minimum, add speed, until above max, then reverse 
     speed */
  //  printf("explosion updated\n");
  if (radius >= maxradius 
      && speed > 0.0) {
    /* Implosion is a little slower */
    speed = -0.6 * speed;
    /* This is probably the best place to let the game know an explosion
       happened. If we wait to the end, other explosions may have "moved"
       the tanks, etc. before this finish */
    /* Let the game know we had an explosion */
    game->Explosion(this);
  }
  radius += speed * deltatime;
  /* Remove the explosion if it is done */
  if (radius <= 0.0) {
    radius = 0.0;
    /* We do not need further updates */
    keep = false;
    /* Delay round changes for a second */
    game->AddEntity(new TDelay(owner, 1));
  } 
}

/* **********************************************************************
   Render an explosion 
   TODO: Use the viewpoint .. */
void TExplosion::Render(TViewpoint * viewpoint) {
  glPushMatrix();
  glTranslatef(location.x, location.y, 
	       location.z);
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glutSolidSphere(radius, (int) rint(8*(radius+1.0)), 
		  (int) rint(8*(radius+1.0)));
  glPopMatrix();
}
