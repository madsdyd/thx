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
#include "lup.hh"

/*************************
 * checkPointPoly(*P, *Q, *P0, *P1, *P2)
 * Checks if the line (PQ) intersects the
 * triangle P0P1P2 and returns the "collisiontime",
 * e.g. a number between 0 and 1, that can be used
 * to determine the collisionpoint:
 *     collision = P + t * (Q - P)
 * If no collision is detected 2.0 is returned. We do
 * not return 1.0 since we could have a collision at
 * this time.
 *************************/
float checkPointPoly(TVector *P, TVector *Q, TVector *P0, TVector *P1, TVector *P2) {

  float A[3][3], b[3], uvt[3];

  /*
   * Coefficients for 3x3 linear equation system checking for
   * a hit/cross/collision between plane (P0P1P2) and line (PQ) :
   * (P1 - P0) * u + (P2 - P0) * v + (P - Q) * t = P - P0
   */

  A[0][0] = P1->x - P0->x; A[0][1] = P2->x - P0->x; A[0][2] = P->x - Q->x;
  A[1][0] = P1->y - P0->y; A[1][1] = P2->y - P0->y; A[1][2] = P->y - Q->y;
  A[2][0] = P1->z - P0->z; A[2][1] = P2->z - P0->z; A[2][2] = P->z - Q->z;

  b[0] = P->x - P0->x;
  b[1] = P->y - P0->y;
  b[2] = P->z - P0->z;

  /*
   * Solve the system:
   * A * uvt = b
   */
  lupSolver(A[0],b,uvt,3);

  /*
   * Constraints:
   *   u,v,t >= 0
   *   t <= 1
   *   u+v <= 1.0
   * This makes sure that we're inside the triangle and
   * between P and Q.
   */
  if (
      (uvt[2]<=1.0) && // t <= 1.0
      (uvt[2]>=0.0) && // t >= 0.0
      (uvt[0]>=0.0) && // u >= 0.0
      (uvt[1]>=0.0) && // v >= 0.0
      (uvt[0]+uvt[1]<=1.0)) { // u + v <= 1.0
    /*
     * Collision detected, time of collision returned
     */
    return uvt[2];
  } else {
    /*
     * No collision, 2 returned to indicate we move full timestep
     * We use 2 because, theoretically, we _can_ have a collision
     * at t = 1.0 so we need something else...
     */
    return 2.0;
  }
}
