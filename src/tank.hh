#ifndef __TANK_H__
#define __TANK_H__

#include "types.hh"
#include "entities.hh"
#include "explosion.hh"

#include "map.hh"

#include "object.hh"

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

class TTank : public TEntity {
protected:
  float levitation;               /* Used for death animation */
  tankstate_t tankstate;
public:
  TObject model;
  color_t color;
  TVector location;
  cannon_t cannon;
  float health;
  TTank(TPlayer * owner);
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
