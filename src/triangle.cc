#include <iostream.h>

#include "triangle.hh"

Triangle::Triangle(int q0, int q1, int q2) {

  p0 = q0;
  p1 = q1;
  p2 = q2;

}

void Triangle::getPoints(int* ret) {

  ret[0] = p0;
  ret[1] = p1;
  ret[2] = p2;

}
