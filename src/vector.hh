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
#ifndef __VECTOR_HH__
#define __VECTOR_HH__

#include <list>

/* This seemed to be an OK place to put this declaration */
#define RadToDegree(r) ((r)/M_PI*180.0)

/* Defined a vector of 3 floats. */
class TVector {
public:
  TVector() {}
  TVector(float a, float b, float c);
  float x, y, z;
  void Normalize();
  /* The distance between two points expressed as vectors */
  float Distance(TVector &b);
  float Distance(TVector *b);
  void display();

  float inner(TVector a);
  TVector cross(TVector a);
  TVector operator-(TVector a);
  TVector operator+(TVector a);
  TVector operator*(double s);
  TVector operator*(float s);
  TVector operator*(int s);
  /* Rotate this vector d degrees in the xy plane */
  void rotatexy(double d);
};

typedef list<TVector> TVectors;
typedef TVectors::iterator TVectorsIterator;

#endif
