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
#ifndef __OBJECT_HH__
#define __OBJECT_HH__
#include <list>
#include <vector>

using std::list;
using std::vector;

#include <GL/glut.h>
#include "vector.hh"
#include "triangle.hh"
#include "point.hh"
#include "render.hh"

class TObject {
private:
  GLuint display_list;
  bool changed;
  render_type_t myrendertype;
public:
  TObject(int numPts, int numTris);
  // TObject() {}
  ~TObject();
  //  TVector getVelocity();
  void setVelocity(TVector v);
  void addVelocity(TVector v);

  void move(float s); // Add (vel*s) to each point in the TObject

  void addTriangle(int q0, int q1, int q2);

  void addPoint(float p0, float p1, float p2);
  void addPoint(float pts[]);
  void addPoint(float p0, float p1, float p2, float col[]);
  void addPoint(float pts[], float col[]);

  void calcBbox();

  void setColor(float *col);
  
  bool CollisionDetect(TVector * old_location, TVector * new_location);

  void setCollision() { collision = true; }
  void clearCollision() { collision = false; }

  void Render();
  void drawBbox();

  void print();
  void printBbox();
  void calcNormals();

  // Rotate theta degrees around Q-P
  void rotate(TVector P, TVector Q, float theta);
  // Translate with vector T
  void translate(TVector T);
  // Scale around origo, S holds scalefactors for each axis
  void scale(TVector S);

  float bbox[3][2],vel[3];
  bool collision;

  PointPtr points;
  TrianglePtr triangles;
  TVector *normals,*ptsnormals;

  int numPoints, maxPoints;
  int numTriangles, maxTriangles;

private:

  bool bboxused;
  void matrixmult(float *A, float *B, float *C);

  /*
   * Transformation matrix
   * IMPORTANT!
   * This matrix does NOT conform with the OpenGL standard
   * for matrices, so we transpose the matrix of all
   * the transformations!
   */
  float transform[16];

  vector<TrianglePtrList> ptsNbour;

};
#endif
