#ifndef __MAP_HH__
#define __MAP_HH__
/* This module contains stuff to manipulate a map data structure, but not
   to render it */

#include <GL/glut.h>
#include "vector.hh"
#include "viewpoint.hh"
#include "types.hh"

#include "explosion.hh"

/* **********************************************************************
   Configuration stuff that is changable from the outside */
extern float map_max_steepness;

/* I am still quite unsure about the data format */
//typedef TVector TMappoint;
typedef struct {
  bool used;
  float x, y, z; /* Location */
  TVector normal; /* Normal */
} TMappoint;

class TMap {
private:
  TMappoint * data;
  /* The landscape generating stuff */
  void lsGenerate();
  GLfloat lsRand(int step);
  void lsLowerPoint(TMappoint * dest, TMappoint * a, TMappoint * b, int step);
  void lsSplitTriangle(TMappoint * a, TMappoint * b, TMappoint * c, 
		       int step, int max_steps);
  //  void lsDraw(void);
  void lsNormals(int xlow, int xhigh, int ylow, int yhigh);

  GLuint display_list;
  /* Lowers a single map point */
  void LowerPoint(int x, int y, TVector * location, float radius);
  /* Lowers a lot of map points */
  void LowerAll(TVector * location, float radius);
  /* Slides the ground, called after lowering. */
  void Slide(int x, int y);
  void SetTexture(float h);
  /* The meat of RandomSpots */
  void DoRandomSpots(TVectors * spots, int remaining, int border);
public:
  int width, length;   /* The max steepness after a slide */
  float max_steepness; /* Set to TRUE when init, impact or rendertype changes */
  bool has_changed;
  float mapsteepness;  // Scalefactor for landscape generation.
  float bumpiness;     // How bumpy the map should be
  TMap(int w, int l);
  ~TMap();
  /* Return a random spot within the map, within border */
  TVector RandomSpot(int border);
  /* Return n random spots within the map, with min border dist from border
     and other spots */
  TVectors * RandomSpots(int n, int border);
  /* Return the center point */
  TMappoint CenterPoint() {
    return *(PointAt(width/2, length/2));
  }
  /* Check for collision with the map. Returns true if a collision would happen 
     between old and new, and returns the coordinates of the collision in 
     new (if collision).
     old_location is assumes to be _above_ the map, and new_location potentially
     below it. */
  bool CollisionDetect(TVector * old_location, TVector * new_location);
  /* Check if we are within the map. TRUE if we are, FALSE otherwise
     Only the x an y coordinates are checked. */
  bool Within(TVector * location);
  /* Take a hit at location, with damage radius radius. 
     This simply "removes" part of the landscape, leaving stuff that is no
     more then map_max_steepness steep */
  void Explosion(TExplosion * Explosion);
  //#define mappoint_at(map,x,y) (&(((map)->data[(int) (((x)*((map)->length)+(y)))])))
  TMappoint * PointAt(int x, int y) {
    return &(data[x*(length+1)+y]);
  };
  /* Init the renderer */
  void InitRender();
  /* Render the map */
  void Render(TViewpoint * viewpoint);
};  

#endif
