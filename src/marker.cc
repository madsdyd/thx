#include "marker.hh"

/* **********************************************************************
   Delay */
TDelay::TDelay(TPlayer * owner, float ttl) : TEntity(owner) {
  timeleft    = ttl;
  /* We wish for no turn change to happen while we are present */
  affect_turn = true;
}

void TDelay::Update(TGame * game, system_time_t deltatime) {
  timeleft -= deltatime;
  if (timeleft <= 0.0) {
    keep = false;
  }
}

/* **********************************************************************
   Marker - like delay, but have "presence" */
TMarker::TMarker(TPlayer * owner, 
		 TVector loc, 
		 float nradius, 
		 float ttl) : TDelay(owner, ttl) {
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

