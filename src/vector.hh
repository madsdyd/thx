#ifndef __VECTOR_HH__
#define __VECTOR_HH__
/* Defined a vector of 3 floats. */

#include <list>

class TVector {
public:
  TVector() {}
  TVector(float a, float b, float c);
  float x, y, z;
  void Normalize();
  /* The distance between two points expressed as vectors */
  float Distance(TVector &b);
  void display();

  float TVector::inner(TVector a);
  TVector TVector::cross(TVector a);
  TVector TVector::operator-(TVector a);
  TVector TVector::operator+(TVector a);
  TVector TVector::operator*(float s);
  TVector TVector::operator*(int s);

};

typedef list<TVector> TVectors;
typedef TVectors::iterator TVectorsIterator;

#endif
