#ifndef __POINT_H__
#define __POINT_H__

#include "vector.hh"

class Point {

public:

  // Constructors
  Point() {}
  Point(float coords[]);
  Point(float x, float y, float z);
  Point(float coords[], float col[]);
  Point(float x, float y, float z, float col[]);
  void setColor(float col[]);
  void setColor(float r, float g, float b, float a);

  TVector coords; // Coordinates of the point {x,y,z,w}
  float color[4]; // Color of the point {r,g,b,a}
  TVector normal; // Vertex normal


private:


};







#endif


