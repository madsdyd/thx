#include "text.hh"

/* **********************************************************************
   Create an object */
TTextRender::TTextRender() {
  shadow = false;
  size   = 12;
  x      = 0;
  y      = 0;
  color  = ColorDefinitions.Colors["white"];
}

/* **********************************************************************
   Create an object */
GLint TTextRender::Load(const string filename) {
  GLint status = text_render.Load(filename.c_str());
  text_render.gap = 1.0/256.0;
  if (1 != status) {
    cout << "TTextRender::Load could not load " << filename << endl;
  }
  return status;
}

/* **********************************************************************
   Pos - sets the position */
void TTextRender::Pos(GLint xpos, GLint ypos) {
  x = xpos; y= ypos;
} 
void TTextRender::PosY(GLint ypos) {
  y= ypos;
} 
void TTextRender::PosX(GLint xpos) {
  x = xpos;
} 

/* **********************************************************************
   Draw - calls the renderer */
void TTextRender::Draw(const string text) {
  if (shadow) {
    text_render.Shadow();
  }
  text_render.Size(size);
  text_render.Color4fv(color.data);
  text_render.Draw(x, y, text.c_str());
}

/* **********************************************************************
   Print */
void TTextRender::Print(string text) {
  Draw(text);
  x += Metric(text);
}

/* **********************************************************************
   Center */
void TTextRender::Center(GLint min, GLint max, string text) {
  GLint metric = Metric(text);
  x = (max - min - metric)/2;
  Draw(text);
  x += metric;
}

/* **********************************************************************
   Center on pivot character */
void TTextRender::Center(GLint min, GLint max, string text, string pivot) {
  int pivot_pos = text.find(pivot);
  string left   = text.substr(0, pivot_pos);
  // string rigth  = text.substr(pivot + pivot.size(), text.size());
  /* Position for the pivot - left*/
  GLint metric = Metric(pivot);
  x = (max - min - Metric(pivot))/2 - Metric(left);
  Draw(text);
  x += metric;
}

/* **********************************************************************
   PrintLn */
void TTextRender::PrintLn(string text) {
  Draw(text);
  y -= size;
}

/* **********************************************************************
   CenterLn */
void TTextRender::CenterLn(GLint min, GLint max, string text) {
  Center(min, max, text);
  y -= size;
}

/* **********************************************************************
   CenterLn */
void TTextRender::CenterLn(GLint min, GLint max, string text, string pivot) {
  Center(min, max, text, pivot);
  y -= size;
}

/* **********************************************************************
   Metric 
   TODO: Fix this up. */
GLint TTextRender::Metric(string text) {
  return size*text.size(); 
}
