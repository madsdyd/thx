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
