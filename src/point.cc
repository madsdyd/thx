#include "point.hh"

Point::Point(float c[]) {

  coords.x = c[0];
  coords.y = c[1];
  coords.z = c[2];

  color[0] = 1.0;
  color[1] = 1.0;
  color[2] = 1.0;
  color[3] = 1.0;

}


Point::Point(float x, float y, float z) {

  coords.x = x;
  coords.y = y;
  coords.z = z;

  color[0] = 1.0;
  color[1] = 1.0;
  color[2] = 1.0;
  color[3] = 1.0;

}


Point::Point(float c[4], float col[4]) {

  coords.x = c[0];
  coords.y = c[1];
  coords.z = c[2];

  color[0] = col[0];
  color[1] = col[1];
  color[2] = col[2];
  color[3] = col[3];
  
}

Point::Point(float x, float y, float z, float col[4]) {

  coords.x = x;
  coords.y = y;
  coords.z = z;
  
  color[0] = col[0];
  color[1] = col[1];
  color[2] = col[2];
  color[3] = col[3];
  
}

void Point::setColor(float r, float g, float b, float a) {
  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = a;
}

void Point::setColor(float col[4]) {
  color[0] = col[0];
  color[1] = col[1];
  color[2] = col[2];
  color[3] = col[3];
}
