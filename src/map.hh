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
  int width, length;   
  bool has_changed;    /* Set to TRUE when init, impact or rendertype changes */
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
  /* Return a random spot within the map, within border */
  TVector RandomSpot(int border);
  /* The meat of RandomSpots */
  void DoRandomSpots(TVectors * spots, int remaining, int border);
  //#define mappoint_at(map,x,y) (&(((map)->data[(int) (((x)*((map)->length)+(y)))])))
  TMappoint * PointAt(int x, int y);
public:
  float max_steepness; /* The max steepness after a slide */
  float mapsteepness;  // Scalefactor for landscape generation.
  float bumpiness;     // How bumpy the map should be
  TMap(int w, int l, float nmapsteepness);
  ~TMap();
  /* Get the width and length of the map */
  int GetWidth(); 
  int GetLength();
  /* Make the map invalidate its internal representation */
  void Invalidate();
  /* Return n random spots within the map, with min border dist from border
     and other spots */
  TVectors * RandomSpots(int n, int border);
  /* Return the center point */
  TMappoint CenterPoint();
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
  /* Return the height at a given point - within a triangle */
  float HeightAt(float x, float y);
  /* Init the renderer */
  void InitRender();
  /* Render the map */
  void Render(TViewpoint * viewpoint);
};  

#endif
