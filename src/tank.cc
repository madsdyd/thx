#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "tank.hh"

#ifdef SOUND_ON
#include "sound.hh"
#endif

#include "player.hh"
#include "game.hh"
#include "explosion.hh"
#include "map.hh"
#include "marker.hh"

/* **********************************************************************
   Initialize a tank */
TTank::TTank(TPlayer * owner) : TEntity(owner) {
  /* Override default behaviour, we do not want to be freed be the game, 
     since players still reference us */
  free = false;
  /* Other init variables is done in prepare round */
}

/* **********************************************************************
   Set the location of a tank and prepare it for a new round */
void TTank::PrepareRound(TVector * loc) {
  keep            = true;
  location        = *loc;
  health          = 100.0;
  tankstate       = tankstate_alive;
  levitation      = 0; 
  cannon.angle    = 45;
  cannon.rotation = 0;
  cannon.force    = 15;
}

/* **********************************************************************
   Adjust the strength of the current tank, within 0-100 */
void TTank::AdjustForce(int adjust) {
  cannon.force+=adjust;
  if (cannon.force < 0) {
    cannon.force = 0;
  }
  if (cannon.force > 100) {
    cannon.force = 100;
  }
}

/* **********************************************************************
   Set the angle of a tank within 0-90 */
void TTank::AdjustAngle(int adjust) {
  cannon.angle+=adjust;
  if (cannon.angle < 0) {
    cannon.angle = 0;
  }
  if (cannon.angle > 90) {
    cannon.angle = 90;
  }
}

/* **********************************************************************
   Set the angle of a tank within 0-360 */
void TTank::AdjustRotation(int adjust) {
  cannon.rotation+=adjust;
  while (cannon.rotation < 0) {
    cannon.rotation += 360;
  }
  while (cannon.rotation >= 360) {
    cannon.rotation -= 360;
  }
}

/* **********************************************************************
   Handle an explosion */
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
  
  /* Move tank to land level take damage from falling */
  dist = location.z 
    - map->PointAt(location.x, location.y)->z;
  taken_damage = dist * dist; /* In lack of proper acceleration */
  health -= taken_damage;
  location.z 
    = map->PointAt(location.x, location.y)->z;
  // printf("Took %f in damage from faling\n", taken_damage);

  /* Never go below 0 */
  if (health <= 0) {
    health = 0;
    tankstate = tankstate_dead;
  }
  // printf("Health at : %5.2f\n", health);
}

/* **********************************************************************
   Update 
   If we are dead, we float for a while, then explode... */
void TTank::Update(TGame * game, system_time_t deltatime) {
  // cout << "TTank::Update" << endl;
  /* If health is at 0 then make the tank do a funny little stunt */
  if (tankstate_dead == tankstate) {
    /* Start the animation - make sure it does not stop until we
       have exploded. This is a kludge for now. <-- TODO fix 
       6 seconds should be enough */
    game->AddEntity(new TDelay(owner, 6));
    tankstate = tankstate_animdead;
  }
  if (tankstate_animdead == tankstate) {
    if (levitation < 5.0) { /* run for 5 seconds */
      levitation += 1.0*deltatime;
      location.z += 2.0*deltatime; /* Lift a little faster */
    } else {
      if (levitation >= 5.0) {
	game->AddEntity(new TExplosion(owner, location, 3.0, 3.0, 25));
#ifdef SOUND_ON
#warning "LAME SOUND CODE ON"
	sound_play(names_to_nums["data/sounds/explosion2.raw"]);
#endif
	keep = false;
	tankstate = tankstate_rip; /* Will stop us from animating */
      }
    }
  }
}

/* **********************************************************************
   Render a tank */
void TTank::Render(TViewpoint * viewpoint) {
  glPushMatrix();
  glTranslatef(location.x, location.y, location.z);
  //  printf("tank: translating to %i %i %f\n",
  //	 x, y, height);
  glColor4fv(color.data);  
  glutSolidCube(1.0);

  /* Just above the tank, render a canon */
  glTranslatef(0.0, 0.0, 1.0);
  /* This is needed to make OpenGL agree with me on rotation */
  glRotatef(90.0, 0.0, 0.0, 1.0);
  glRotatef(cannon.rotation, 0.0, 0.0, 1.0);
  glRotatef(90.0-cannon.angle, 1.0, 0.0, 0.0);
  glColor3f(0.3, 0.3, 0.3);
  glutSolidCone(0.5, 2.5, 8, 8);
  glPopMatrix();
}
