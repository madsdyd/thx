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
  TDelay(TPlayer * owner, float ttl);
  virtual void Update(TGame * game, system_time_t deltatime);
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
  TMarker(TPlayer * owner, TVector loc, 
	  float nradius, float ttl);
  virtual void Render(TViewpoint * viewpoint);
  virtual void TakeDamage(TEntity * other_entity) {}; 
};
#endif
