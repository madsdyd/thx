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
