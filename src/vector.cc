#include "vector.hh"

#include "math.h"

TVector::TVector(float a, float b, float c) {
  x = a;
  y = b;
  z = c;
}

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

void TVector::display() {
  cout << "("
       << x << " , " 
       << y << " , " 
       << z 
       << ")\n";
}

float TVector::inner(TVector a) {
  return ( x * a.x + y * a.y + z * a.z);
}

TVector TVector::cross(TVector a) {
  return TVector (y * a.z - z * a.y , z * a.x - x * a.z , x * a.y - y * a.x );
}

TVector TVector::operator-(TVector a) {
  TVector tmp(x - a.x , y - a.y, z - a.z);
  return tmp;
}

TVector TVector::operator+(TVector a) {
  TVector tmp(x + a.x , y + a.y , z + a.z);
  return tmp;
}

TVector TVector::operator*(float s) {
  TVector tmp(x * s, y * s, z * s);
  return tmp;
}

TVector TVector::operator*(int s) {
  TVector tmp(x * s, y * s, z * s);
  return tmp;
}
