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
#include "models.hh"
#include "types.hh"

// If -DPYRAMID_MODEL use a pyramid as the tank
#ifdef PYRAMID_MODEL
#define TANK_MODEL 1
#endif

// If -DCUBE_MODEL use a cube as the tank
#ifdef CUBE_MODEL
#define TANK_MODEL 0
#endif

#ifndef TANK_MODEL
#define TANK_MODEL 2
#endif


void default_tank(TObject *model, TColor *color) {
  switch(TANK_MODEL) {
  case 0:
    cube_tank(model, color);
    break;
  case 1:
    pyramid_tank(model, color);
    break;
  case 2:
    abrams_chassis(model, color);
    break;
  default:
    cube_tank(model, color);
    break;
  }
}

void cube_tank(TObject *model, TColor *color) {
  float col[4];

  (*model)  = TObject(8,12);

  col[0] = color->data[0];
  col[1] = color->data[1];
  col[2] = color->data[2];
  col[3] = color->data[3];

  // Lowest four points
  model->addPoint(-0.5,-0.5,-0.5,col);
  model->addPoint(0.5,-0.5,-0.5,col);
  model->addPoint(-0.5,-0.5,0.5,col);
  model->addPoint(0.5,-0.5,0.5,col);

  // Highest four points
  model->addPoint(-0.5,0.5,-0.5,col);
  model->addPoint(0.5,0.5,-0.5,col);
  model->addPoint(-0.5,0.5,0.5,col);
  model->addPoint(0.5,0.5,0.5,col);

  // Bottom
  model->addTriangle( 0, 1, 2);
  model->addTriangle( 2, 1, 3);
  
  // Top
  model->addTriangle( 4, 6, 5);
  model->addTriangle( 5, 6, 7);

  // Left
  model->addTriangle( 0, 2, 4);
  model->addTriangle( 4, 2, 6);

  // Right
  model->addTriangle( 1, 7, 3);
  model->addTriangle( 1, 5, 7);

  // Front
  model->addTriangle( 2, 3, 6);
  model->addTriangle( 3, 7, 6);

  // Back
  model->addTriangle( 1, 0, 5);
  model->addTriangle( 0, 4, 5);

}

void pyramid_tank(TObject *model, TColor *color) {
  float col[4];

  (*model)  = TObject(8,12);

  col[0] = color->data[0];
  col[1] = color->data[1];
  col[2] = color->data[2];
  col[3] = color->data[3];

  // Top point
  model->addPoint(0.0,0.75,0.0,col);

  // Other points along X- and Z-axis
  model->addPoint(0.75,0.0,0.0,col);
  model->addPoint(0.0,0.0,-0.75,col);
  model->addPoint(-0.75,0.0,0.0,col);
  model->addPoint(0.0,0.0,0.75,col);

  // Sides
  model->addTriangle( 0, 1, 2);
  model->addTriangle( 0, 2, 3);
  model->addTriangle( 0, 3, 4);
  model->addTriangle( 0, 4, 1);

  // Bottom
  model->addTriangle( 1, 3, 2);
  model->addTriangle( 1, 4, 3);

}

void abrams_chassis(TObject *model, TColor *color) {
  int i;
  float col[4];

  (*model)  = TObject(48,74);

  col[0] = color->data[0];
  col[1] = color->data[1];
  col[2] = color->data[2];
  col[3] = color->data[3];

  /*********************************************
   ** Add the chassis
   *********************************************/
  model->addPoint( 0.401095,-0.070808,0.127213,col);
  model->addPoint( 0.457116,-0.128811,0.127213,col);
  model->addPoint( 0.457439,-0.159642,0.127213,col);
  model->addPoint( 0.228164,-0.217503,0.127213,col);
  model->addPoint( -0.400442,-0.216827,0.127213,col);
  model->addPoint( -0.461345,-0.128411,0.127213,col);
  model->addPoint( -0.460668,-0.011985,0.127213,col);
  model->addPoint( -0.197648,-0.056244,0.127213,col);
  model->addPoint( -0.197648,-0.056244,-0.126894,col);
  model->addPoint( -0.460668,-0.011985,-0.126894,col);
  model->addPoint( -0.461345,-0.128411,-0.126894,col);
  model->addPoint( -0.400442,-0.216827,-0.126894,col);
  model->addPoint( 0.228164,-0.217503,-0.126894,col);
  model->addPoint( 0.457439,-0.159642,-0.126894,col);
  model->addPoint( 0.457116,-0.128811,-0.126894,col);
  model->addPoint( 0.401095,-0.070808,-0.126894,col);


  model->addTriangle(2, 13, 14);
  model->addTriangle(1, 2, 14);
  model->addTriangle(1, 14, 15);
  model->addTriangle(0, 1, 15);
  model->addTriangle(0, 15, 8);
  model->addTriangle(0, 8, 7);
  model->addTriangle(7, 8, 9);
  model->addTriangle(6, 7, 9);
  model->addTriangle(6, 9, 10);
  model->addTriangle(5, 6, 10);
  model->addTriangle(5, 10, 11);
  model->addTriangle(4, 5, 11);
  model->addTriangle(4, 11, 12);
  model->addTriangle(3, 4, 12);
  model->addTriangle(3, 12, 13);
  model->addTriangle(2, 3, 13);
  model->addTriangle(5, 7, 6);
  model->addTriangle(8, 10, 9);

  /*********************************************
   ** Add the left thread
   *********************************************/
  model->addPoint( 0.400590,-0.056361,-0.126806);
  model->addPoint( 0.478656,-0.135155,-0.126806);
  model->addPoint( 0.478062,-0.159287,-0.126806);
  model->addPoint( 0.381580,-0.254640,-0.126806);
  model->addPoint( -0.386759,-0.254607,-0.126806);
  model->addPoint( -0.461003,-0.146061,-0.126806);
  model->addPoint( -0.460810,-0.089634,-0.126806);
  model->addPoint( -0.356364,-0.055805,-0.126806);
  model->addPoint( -0.356364,-0.055805,-0.170824);
  model->addPoint( -0.460810,-0.089634,-0.170824);
  model->addPoint( -0.461003,-0.146061,-0.170824);
  model->addPoint( -0.386759,-0.254607,-0.170824);
  model->addPoint( 0.381580,-0.254640,-0.170824);
  model->addPoint( 0.478062,-0.159287,-0.170824);
  model->addPoint( 0.478656,-0.135155,-0.170824);
  model->addPoint( 0.400590,-0.056361,-0.170824);

  i = 16;
  model->addTriangle(i+1, i+2, i+13);
  model->addTriangle(i+1, i+13, i+14);
  model->addTriangle(i+0, i+1, i+14);
  model->addTriangle(i+0, i+14, i+15);
  model->addTriangle(i+0, i+15, i+7);
  model->addTriangle(i+7, i+15, i+8);
  model->addTriangle(i+6, i+7, i+8);
  model->addTriangle(i+6, i+8, i+9);
  model->addTriangle(i+5, i+6, i+9);
  model->addTriangle(i+5, i+9, i+10);
  model->addTriangle(i+4, i+5, i+10);
  model->addTriangle(i+4, i+10, i+11);
  model->addTriangle(i+3, i+4, i+11);
  model->addTriangle(i+3, i+11, i+12);
  model->addTriangle(i+2, i+3, i+12);
  model->addTriangle(i+2, i+12, i+13);
  model->addTriangle(i+13, i+15, i+14);
  model->addTriangle(i+12, i+15, i+13);
  model->addTriangle(i+8, i+15, i+12);
  model->addTriangle(i+8, i+12, i+11);
  model->addTriangle(i+8, i+11, i+10);
  model->addTriangle(i+8, i+10, i+9);
  model->addTriangle(i+5, i+7, i+6);
  model->addTriangle(i+4, i+7, i+5);
  model->addTriangle(i+0, i+7, i+4);
  model->addTriangle(i+0, i+4, i+3);
  model->addTriangle(i+0, i+2, i+1);
  model->addTriangle(i+0, i+3, i+2);

  /*********************************************
   ** Add the right thread
   *********************************************/
  model->addPoint( 0.400590,-0.056361,0.171211);
  model->addPoint( 0.478656,-0.135155,0.171211);
  model->addPoint( 0.478062,-0.159287,0.171211);
  model->addPoint( 0.381580,-0.254640,0.171211);
  model->addPoint( -0.386759,-0.254607,0.171211);
  model->addPoint( -0.461003,-0.146061,0.171211);
  model->addPoint( -0.460810,-0.089634,0.171211);
  model->addPoint( -0.356364,-0.055805,0.171211);
  model->addPoint( -0.356364,-0.055805,0.127193);
  model->addPoint( -0.460810,-0.089634,0.127193);
  model->addPoint( -0.461003,-0.146061,0.127193);
  model->addPoint( -0.386759,-0.254607,0.127193);
  model->addPoint( 0.381580,-0.254640,0.127193);
  model->addPoint( 0.478062,-0.159287,0.127193);
  model->addPoint( 0.478656,-0.135155,0.127193);
  model->addPoint( 0.400590,-0.056361,0.127193);

  i = 32;
  model->addTriangle( i+1, i+2, i+13);
  model->addTriangle( i+1, i+13, i+14);
  model->addTriangle( i+0, i+1, i+14);
  model->addTriangle( i+0, i+14, i+15);
  model->addTriangle( i+0, i+15, i+7);
  model->addTriangle( i+7, i+15, i+8);
  model->addTriangle( i+6, i+7, i+8);
  model->addTriangle( i+6, i+8, i+9);
  model->addTriangle( i+5, i+6, i+9);
  model->addTriangle( i+5, i+9, i+10);
  model->addTriangle( i+4, i+5, i+10);
  model->addTriangle( i+4, i+10, i+11);
  model->addTriangle( i+3, i+4, i+11);
  model->addTriangle( i+3, i+11, i+12);
  model->addTriangle( i+2, i+3, i+12);
  model->addTriangle( i+2, i+12, i+13);
  model->addTriangle( i+13, i+15, i+14);
  model->addTriangle( i+12, i+15, i+13);
  model->addTriangle( i+8, i+15, i+12);
  model->addTriangle( i+8, i+12, i+11);
  model->addTriangle( i+8, i+11, i+10);
  model->addTriangle( i+8, i+10, i+9);
  model->addTriangle( i+5, i+7, i+6);
  model->addTriangle( i+4, i+7, i+5);
  model->addTriangle( i+0, i+7, i+4);
  model->addTriangle( i+0, i+4, i+3);
  model->addTriangle( i+0, i+2, i+1);
  model->addTriangle( i+0, i+3, i+2);

  model->calcNormals();

}

void abrams_barrel(TObject *model, TColor *color) {
  // int i;
  float col[4];

  (*model)  = TObject(16,22);

  col[0] = color->data[0];
  col[1] = color->data[1];
  col[2] = color->data[2];
  col[3] = color->data[3];

  /*********************************************
   ** Add the barrel
   *********************************************/
  model->addPoint( 0.000040,0.017295,-0.016964,col);
  model->addPoint( 0.000040,0.000074,-0.024097,col);
  model->addPoint( 0.000040,-0.017146,-0.016964,col);
  model->addPoint( 0.000040,-0.024279,0.000257,col);
  model->addPoint( 0.000040,-0.017146,0.017478,col);
  model->addPoint( 0.000040,0.000074,0.024611,col);
  model->addPoint( 0.000040,0.017295,0.017478,col);
  model->addPoint( 0.000040,0.024428,0.000257,col);
  model->addPoint( 0.574063,0.017122,0.000257,col);
  model->addPoint( 0.574063,0.012129,0.012311,col);
  model->addPoint( 0.574063,0.000074,0.017305,col);
  model->addPoint( 0.574063,-0.011980,0.012311,col);
  model->addPoint( 0.574063,-0.016973,0.000257,col);
  model->addPoint( 0.574063,-0.011980,-0.011797,col);
  model->addPoint( 0.574063,0.000074,-0.016791,col);
  model->addPoint( 0.574063,0.012129,-0.011797,col);

  model->addTriangle(3, 13, 12);
  model->addTriangle(2, 13, 3);
  model->addTriangle(2, 14, 13);
  model->addTriangle(1, 14, 2);
  model->addTriangle(1, 15, 14);
  model->addTriangle(0, 15, 1);
  model->addTriangle(0, 8, 15);
  model->addTriangle(0, 7, 8);
  model->addTriangle(7, 9, 8);
  model->addTriangle(6, 9, 7);
  model->addTriangle(6, 10, 9);
  model->addTriangle(5, 10, 6);
  model->addTriangle(5, 11, 10);
  model->addTriangle(4, 11, 5);
  model->addTriangle(4, 12, 11);
  model->addTriangle(3, 12, 4);
  model->addTriangle(8, 9, 10);
  model->addTriangle(8, 10, 11);
  model->addTriangle(8, 11, 15);
  model->addTriangle(11, 14, 15);
  model->addTriangle(11, 12, 14);
  model->addTriangle(12, 13, 14);

  model->calcNormals();

}
