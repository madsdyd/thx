#ifndef __VECTOR_HH__
#define __VECTOR_HH__
/* Defined a vector of 3 floats. */

#include <list>

class TVector {
public:
  float x, y, z;
  void Normalize();
  /* The distance between two points expressed as vectors */
  float Distance(TVector &b);
};

typedef list<TVector> TVectors;
typedef TVectors::iterator TVectorsIterator;

#endif
