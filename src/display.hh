#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <math.h>
#include <GL/glut.h>
#include "types.hh"
#include "text.hh"
/* The display class - this is kind of an abstraction of the screen or
   window we render into, and have control over the various elements
   that are on screen */
class TViewpoint;
class TConsole;
class TDisplay {
public:
  TTextRender textrender;
  unsigned int num_frames;
  /* We should probably publish these otherwise */
  unsigned int width;
  unsigned int height;
  bool flymode;
  bool clipmode;
  TViewpoint * viewpoint;
  TConsole * console;
  
  TDisplay(int argc, char** argv);
  void Update(system_time_t deltatime); 
  void Reshape(int w, int h);
  void NormalMode();
  void FlatMode();
  void Render();
  void RefreshRate();
};
extern TDisplay * Display;
#endif
