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
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "tank.hh"
#include "display.hh"
#include "map.hh"
#include "player.hh"

#ifdef SOUND_ON
#include "sound.hh"
#endif

#include "types.hh"
#include "marker.hh"
#include "explosion.hh"
#include "object.hh"
#include "models.hh"

/* **********************************************************************
 * Initialize a tank
 * *********************************************************************/
TTank::TTank(TPlayer * owner) : TEntity(owner) {
  /* Setup the models */
  model  = NULL;
  barrel = NULL;
  /* Override default behaviour, we do not want to be freed be the game, 
     since players still reference us */
  free = false;
  /* Other init variables is done in prepare round */
}

/* **********************************************************************
 * Destruct a tank
 * *********************************************************************/
TTank::~TTank() {
  if (model) {
    delete model;
    delete barrel;
  }
}

/* **********************************************************************
 * Set the location of a tank and prepare it for a new round
 * *********************************************************************/
void TTank::PrepareRound(TGame * game, TVector * loc) {
  keep            = true;
  location        = *loc;
  velocity.x      = 0;
  velocity.y      = 0;
  velocity.z      = 0;
  health          = 100.0;
  tankstate       = tankstate_alive;
  levitation      = 0; 
  cannon.angle    = 45.0;
  cannon.rotation = 0.0;
  cannon.force    = 15.0;
  /* This can not be setup until we know the color */
  if (!model) {
    // model  = new TObject();
    // barrel = new TObject();
    model  = DefaultTank(&color);
    barrel = AbramsBarrel(&color);
  }

  /* Level the area */
  game->GetMap()->LevelArea(loc);
}

/* **********************************************************************
 * Adjust the strength of the current tank, within 0-100
 * *********************************************************************/
void TTank::AdjustForce(double adjust) {
  cannon.force+=adjust;
  if (cannon.force < 0.0) {
    cannon.force = 0.0;
  }
  if (cannon.force > 100.0) {
    cannon.force = 100.0;
  }
}

/* **********************************************************************
 * Set the angle of a tank within 0-90
 * *********************************************************************/
void TTank::AdjustAngle(double adjust) {
  cannon.angle+=adjust;
  if (cannon.angle < 0.0) {
    cannon.angle = 0.0;
  }
  if (cannon.angle > 90.0) {
    cannon.angle = 90.0;
  }
}

/* **********************************************************************
 * Set the angle of a tank within 0-360
 * *********************************************************************/
void TTank::AdjustRotation(double adjust) {
  cannon.rotation+=adjust;
  while (cannon.rotation < 0.0) {
    cannon.rotation += 360.0;
  }
  while (cannon.rotation >= 360.0) {
    cannon.rotation -= 360.0;
  }
}

/* **********************************************************************
 * Handle an explosion
 * *********************************************************************/
void TTank::Explosion(TMap * map, TExplosion * Explosion) {
  /* Only matters if the tank is alive */
  if (tankstate_alive != tankstate) {
    return;
  }
  float taken_damage; 
  /* Find distance to explosion use this for damage calculation */
  float dist = sqrt( (Explosion->location.x-location.x)
		     *(Explosion->location.x-location.x) +
		      (Explosion->location.y-location.y)
		     *(Explosion->location.y-location.y) +
		      (Explosion->location.z-location.z)
		     *(Explosion->location.z-location.z));
  if (dist < Explosion->maxradius) {
    dist = mymax(dist, 1.0);
    taken_damage = (1/dist * Explosion->damage);
    health -= taken_damage;
    // cout << "Took" << taken_damage << " in damage from explosion";
    if (Explosion->owner) {
      // cout << " from " << Explosion->owner->name;
      /* Grant the player that caused the damage some money, only
	 if different from self. Also update score.
      TODO: This allows for more then 100 % of damage "granting" - 
      is this OK?.*/
      if (Explosion->owner != owner) {
	Explosion->owner->money += (int) rint(taken_damage * 100) / 2;
	Explosion->owner->score += (int) rint(taken_damage * 100) / 2;
      }
    }
    // cout << endl;
  }
  
  /* Never go below 0 */
  if (health <= 0) {
    health = 0;
    tankstate = tankstate_dead;
  }
}

/* **********************************************************************
 * Update 
 * If we are alive, we may need to fall down
 * If we are dead, we float for a while, then explode...
 * *********************************************************************/
void TTank::Update(TGame * game, system_time_t deltatime) {

  /* If alive, check if we need to fall down 
     NOTE; This is down _only_ */
  if (tankstate_alive == tankstate) {
    /* If we are above the ground, we could be faling. Update position and 
       possible velocidsfsdfw3rqty */
    if (location.z > game->GetMap()->HeightAt(location.x, location.y) + EPSILON) {
      /* cout << "TTank::Update - falling" << endl;
      cout << "TTank::Update - location.z ; " << location.z 
	   << ", HeightAt ; " << game->GetMap()->HeightAt(location.x, location.y)
	   << endl; */
      if (!affect_turn) {
	game->AddAffectTurn(this);
      }
      location.z += velocity.z * deltatime;
      velocity.z -= GRAVITY * deltatime;
    }
    if (velocity.z != 0 && 
	location.z <= game->GetMap()->HeightAt(location.x, location.y)) {
      // cout << "TTank::Update - damage from fall" << endl;
      /* We are at or below the map && our velocity in the z direction is
	 != 0 -- we should take damage - possible die... 
	 The calculations here are a little uncorrect, but hey, such is life. */
      // health -= (velocity.z*velocity.z);
      health += velocity.z;
      /* Make sure we are done falling */
      velocity.z = 0;
      location.z = game->GetMap()->HeightAt(location.x, location.y);
      /* Adjust the map */
      game->GetMap()->LevelArea(&location);
      /* Never go below 0 */
      if (health <= 0) {
	health = 0;
	tankstate = tankstate_dead;
      }
      /* Make sure that we are not used for round, but add a sligth delay */
      game->RemoveAffectTurn(this);
      game->AddEntity(new TDelay(owner, 1)); 
    }
  }

  /* If death, then make the tank do a funny little stunt */
  if (tankstate_dead == tankstate) {
    /* Start the animation - make sure it does not stop until we
       have exploded. */
    tankstate = tankstate_animdead;
    game->AddAffectTurn(this);
  }

  /* Make sure we animate the death sequence */
  if (tankstate_animdead == tankstate) {
    if (levitation < 5.0) { /* run for 5 seconds */
      levitation += 1.0*deltatime;
      location.z += 2.0*deltatime; /* Lift a little faster */
    } else {
      if (levitation >= 5.0) {
	game->AddEntity(new TExplosion(owner, location, 3.0, 3.0, 25));
#ifdef SOUND_ON
	sound_play(names_to_nums["data/sounds/explosion2.raw"]);
#endif
	keep = false;
	tankstate = tankstate_rip; /* Will stop us from animating */
	game->RemoveAffectTurn(this);
      }
    }
  }
}

/* **********************************************************************
 * Render a tank
 * *********************************************************************/
void TTank::Render(TViewpoint * viewpoint) {
  /***********************************************
   ** WARNING
   ** This kind of changing the material properties
   ** are BAD - and not only for the I
   ***********************************************/
  GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 10.0 };

  /* ************************************************** */
  /* Store original matrix */
  glPushMatrix();
  /* Translate to the origion of our entity */
  glTranslatef(location.x, location.y, location.z);

  /* This is required to make the model appear correctly on the
     map. This is a mess. */
  glTranslatef(0.0, 0.0, -0.50);

  /* We want to do this with lighting */
  glEnable(GL_LIGHTING);

  /* ************************************************** */
  /* Render the canon, just above the tank */
  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.65);
  /* This is needed to make OpenGL agree with me on rotation */
  glRotatef(90.0, 0.0, 0.0, 1.0);
  glRotatef(cannon.rotation, 0.0, 0.0, 1.0);
  glRotatef(90.0-cannon.angle, 1.0, 0.0, 0.0);
  /* Scale to make a long, sleek barrel for the tank */
  // glScalef(0.15,0.15,1.5);
  glScalef(0.075,0.075,0.75);

  /* "Push" the barrel half its length to have it rotate around the bottom */
  //  glTranslatef(0.0, 0.0, 0.5);
  glColor3f(0.25 * color.data[0] + 0.15
	    , 0.25 * color.data[1] + 0.15
	    , 0.25 * color.data[2] + 0.15);
  
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  
  glutSolidCone(1.0,1.0,8,8);
  //barrel->draw();
  glPopMatrix();

  /* ************************************************** */
  /* Render the tank */
  /* Rotate back to compensate for... errrr... something...
     Now the Y-axis in the tank model is actually up... */
  glRotatef(90.0, 1.0, 0.0, 0.0);

  glTranslatef(0.0, 0.75, 0.0);
  /* This is just to make the tank appear on a diagonal */
  glRotatef(-45.0, 0.0, 1.0, 0.0);
  glColor4fv(color.data);
  //glPushMatrix();
  //  glScalef(2.0, 2.0, 2.0);
  model->Render();
  //glPopMatrix();
  
  /* ************************************************** */
  /* Remove ligthning */
  glDisable(GL_LIGHTING);
  /* Restore original state */
  glPopMatrix();
}
