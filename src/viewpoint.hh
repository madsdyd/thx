#ifndef __VIEWPOINT_HH__
#define __VIEWPOINT_HH__
#include "vector.hh"

/* This module includes the various parameters that set ups a clients viewpoint
   Maybe the FOV should also go in here? and other parameters?
 */

/* Forward declare map */
class TMap;

class TViewpoint {
public:
  TVector translation;
  TVector rotation;
  TViewpoint() {};
  ~TViewpoint() {};
  void Reset(TMap * map);
  void Reset(TVector * location);
};
#endif
