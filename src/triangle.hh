#ifndef __TRIANGLE_HH__
#define __TRIANGLE_HH__

class Triangle {

public:

  // Constructors
  Triangle() {}
  Triangle(int q0, int q1, int q2);

  void getPoints(int* ret); // Put Point array indices in ret

  int p0,p1,p2; // Point reference to an array of Points

private:

};

#endif
