#ifndef __3DOBJECT_H__
#define __3DOBJECT_H__

#include <list>
#include <vector>
#include "types.hh"
#include "vector.hh"

class TObject {

public:

  TObject(int numPts, int numTris);
  TObject() {}
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

  void draw();
  void drawBbox();

  void print();
  void printBbox();

  float bbox[3][2],vel[3];
  bool collision;

  PointPtr points;
  TrianglePtr triangles;

  int numPoints, maxPoints;
  int numTriangles, maxTriangles;

private:

  bool bboxused;

  vector<TrianglePtrList> ptsNbour;

};

#endif

