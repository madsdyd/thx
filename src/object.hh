#ifndef __OBJECT_HH__
#define __OBJECT_HH__
#include <list>
#include <vector>

#include "vector.hh"
#include "triangle.hh"
#include "point.hh"
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
