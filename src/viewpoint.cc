#include "viewpoint.hh"
#include "map.hh"

/* ************************************************************
   Reset the viewpoint to the center of the map */
void TViewpoint::Reset(TMap * map) {
  /* Center at map */
  translation.x = map->CenterPoint().x;
  translation.y = map->CenterPoint().y;
  translation.z = map->PointAt(translation.x, translation.y)->z+3.0;
  
  /* Set the rotation apropiate */
  rotation.x = 0.0;
  rotation.y = 0.0;
  rotation.z = 0.0;
}

/* **********************************************************************
   Reset the viewpoint to a location */
void TViewpoint::Reset(TVector * location) {
  translation = *location;
  rotation.x = 0.0;
  rotation.y = 0.0;
  rotation.z = 0.0;
}
