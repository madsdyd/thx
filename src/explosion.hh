#ifndef __EXPLOSION_HH__
#define __EXPLOSION_HH__
#include "entities.hh"

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
