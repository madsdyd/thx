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
#define TANK_MODEL 0
#endif


void default_tank(TObject *model, TColor *color) {
  switch(TANK_MODEL) {
  case 0:
    cube_tank(model, color);
    break;
  case 1:
    pyramid_tank(model, color);
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
