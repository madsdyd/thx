#include "vector.hh"

#include "math.h"

void TVector::Normalize() {
  float len = sqrt(x*x + y*y + z*z);
  x /= len;
  y /= len;
  z /= len;
}

float TVector::Distance(TVector &b) {
  float xd = x-b.x;
  float yd = y-b.y;
  float zd = z-b.z;
  return sqrt(xd*xd + yd*yd + zd*zd);
}
