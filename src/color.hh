/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arrøe <skjalm@arroe.dk>
    Copyright (C) 2001 contributers of the THX project
    Please see the file "AUTHORS" for a list of contributers

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef __COLOR_HH__
#define __COLOR_HH__

#include <GL/gl.h>
#include <string>
#include <map>

class TColor {
public:
  GLfloat data[4];
  TColor(GLfloat r = 1.0, GLfloat g = 1.0, 
	 GLfloat b = 1.0, GLfloat a = 1.0) {
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
  }
  void Set(GLfloat r = 1.0, GLfloat g = 1.0, 
	   GLfloat b = 1.0, GLfloat a = 1.0) {
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
  /* Scale the color by a double */
  void SetIntensity(double intensity);
  /* Test for Equal */
  bool Equal(TColor & cref);
  bool operator==(TColor & cref) {
    return Equal(cref);
  }
};

typedef std::map<string, TColor> TColors;
typedef TColors::iterator TColorsIterator;

class TColorDefinitions {
private:
  TColors Colors;
public:
  TColorDefinitions();
  TColor GetColor(string color);
};

extern TColorDefinitions ColorDefinitions;

#endif
