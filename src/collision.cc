#include "vector.hh"
#include "lup.hh"

/*************************
 * checkPointPoly
 * Check point, P, moving at velocity V against
 * triangle P0,P1,P2 moving at velocity W
 * Currently W is unused!
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
