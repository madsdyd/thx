#ifndef __COLOR_HH__
#define __COLOR_HH__

#include <GL/gl.h>
#include <string>
#include <map>

class TColor {
public:
  GLfloat data[4];
  TColor(GLfloat r = 1.0, GLfloat g = 1.0, GLfloat b = 1.0, GLfloat a = 1.0) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
  }
  void Set(GLfloat r = 1.0, GLfloat g = 1.0, GLfloat b = 1.0, GLfloat a = 1.0) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
  }
  GLfloat * Red() {
    return &(data[0]);
  }
  GLfloat * Green() {
    return &(data[1]);
  }
  GLfloat * Blue() {
    return &(data[2]);
  }
  GLfloat * Alpha() {
    return &(data[3]);
  }
};

typedef std::map<string, TColor> TColors;

class TColorDefinitions {
  public:
  TColors Colors;
  TColorDefinitions();
};

extern TColorDefinitions ColorDefinitions;

#endif
