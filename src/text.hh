#ifndef __TEXT_HH__
#define __TEXT_HH__
#include <GL/gl.h>
#include <string>

#include "color.hh"

/* Unit to make text wrapper class that maintains state and position, etc. */
class text_t;
class TTextRender {
private:
  text_t * text_render;
  GLint x, y;
  void Draw(const string text);
public:
  bool shadow;
  int size;
  TColor color;
  TTextRender(); 
  ~TTextRender();
  GLint Load(const string filename);
  void Pos(GLint xpos, GLint ypos);
  void PosY(GLint ypos);
  void PosX(GLint xpos);
  /* Draw line at current position, advances position */
  void Print(string text);
  /* Center line on current y position */
  void Center(GLint min, GLint max, string text);
  /* Center line about pivot character */
  void Center(GLint min, GLint max, string text, string pivot);
  /* Center line on current y position, then set y += size */
  void CenterLn(GLint min, GLint max, string text);
  /* Center line about pivot character, then set y += size */
  void CenterLn(GLint min, GLint max, string text, string pivot);
  /* Draw line at current position, set position to y + size */
  void PrintLn(string text);
  /* Returns the x size needed to render the string. 
     The height is the same as size */
  GLint Metric(string text);
};
#endif
