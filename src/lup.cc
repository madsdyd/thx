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
#include <stdlib.h>
#include <math.h>

/*************************
 * lupSolver(A,b,x,n)
 * Solve linear equation system: Ax = b
 * A is a non-singular n by n matrix
 * Result is stored in x
 *************************/

void lupSolver(float *A,float *b,float *x,int n) {
  int i,j,k,kp,ktemp;
  int pi[n];
  float p;
  float y[n];
  float L[n][n],U[n][n];

  /*
   * Initialization
   */
  for (i=0;i<n;i++) {
    pi[i] = i;
  }

  /*
   * Pivotting
   */
  for (k=0;k<n-1;k++) {
    p = 0.0;
    for (i=k;i<n;i++) {
      if (fabs(A[i*n+k])>p) {
	p = fabs(A[i*n+k]);
	kp = i;
      }
    }
    /*
     * Check for singularity
     */
    if (p<.000001) {
      /************************************************
       ****          NOTENOTENOTENOTENOTE          ****
       ************************************************
       **** THIS IS BOGUS! WE CAN ONLY DO THIS CUZ ****
       **** WE _KNOW_ _KNOW_ _KNOW_ HOW THE RETURN ****
       **** VALUES ARE CHECKED. IF YOU PLAN TO USE ****
       **** THIS LUP SOLVER FOR ANYTHING ELSE THAN ****
       **** TANK HILL EXTREME PLEASE BE SURE TO DO ****
       **** SOMETHING ABOUT THIS... THANK YOU! :-) ****
       ************************************************/
      x[0] = 1.0;
      x[1] = 1.0;
      x[2] = 1.0;
      return;
    }
    /*
     * exchange(pi[k],pi[kp])
     */
    ktemp=pi[k];
    pi[k]=pi[kp];
    pi[kp]=ktemp;
    /*
     * exchange(A[k][i],A[kp][i])
     */
    for (i=0;i<n;i++) {
      p = A[k*n+i];
      A[k*n+i] = A[kp*n+i];
      A[kp*n+i] = p;
    }
    
    /*
     * Schur's complement
     */
    for (i=k+1;i<n;i++) {
      A[i*n+k] = A[i*n+k]/A[k*n+k];
      for (j=k+1;j<n;j++) {
	A[i*n+j] = A[i*n+j] - A[i*n+k]*A[k*n+j];
      }
    }
  }

  /*
   * Create L and U
   */
  for (i=0;i<n;i++) {
    for (j=0;j<n;j++) {
      L[i][j] = ((i>j)?A[i*n+j]:((i==j)?1.0:0.0));
      U[i][j] = ((i<=j)?A[i*n+j]:0.0);
    }
  }

  /*
   * Forward solving gives us y
   */
  for (i=0;i<n;i++) {
    p = 0.0;
    for (j=0;j<i;j++) {
      p = p + L[i][j]*y[j];
    }
    y[i] = b[pi[i]] - p;
  }

  /*
   * Back solving gives us x (the solution)
   */
  for (i=n-1;i>=0;i--) {
    p = 0.0;
    for (j=i+1;j<n;j++) {
      p = p + U[i][j]*x[j];
    }
    x[i] = (y[i] - p) / U[i][i];
  }

}
