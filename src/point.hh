/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#ifndef __POINT_HH__
#define __POINT_HH__

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
};
typedef Point* PointPtr;
#endif
