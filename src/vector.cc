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

float TVector::Distance(TVector *b) {
  float xd = x-b->x;
  float yd = y-b->y;
  float zd = z-b->z;
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

TVector TVector::operator*(double s) {
  TVector tmp(x * s, y * s, z * s);
  return tmp;
}

TVector TVector::operator*(int s) {
  TVector tmp(x * s, y * s, z * s);
  return tmp;
}

void TVector::rotatexy(double d) {
  float x1 = x * cos(d) - y * sin(d);
  y = x * sin(d) + y * cos(d);
  x = x1;
}
