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
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include "text.hh"
#include "glTexFont/text.h"
/* **********************************************************************
 * Create a TextRender
 * *********************************************************************/
TTextRender::TTextRender() {
  text_render = new text_t;
  shadow = false;
  size   = 12;
  x      = 0;
  y      = 0;
  color  = ColorDefinitions.GetColor("white");
}

/* **********************************************************************
 * Destroy it
 * *********************************************************************/
TTextRender::~TTextRender() {
  delete text_render;
}

/* **********************************************************************
 * Load a font texture
 * *********************************************************************/
GLint TTextRender::Load(const string filename) {
  GLint status = text_render->Load(filename.c_str());
  text_render->gap = 1.0/256.0;
  if (1 != status) {
    cout << "TTextRender::Load could not load " << filename << endl;
  }
  return status;
}

/* **********************************************************************
 * Pos - sets the position
 * *********************************************************************/
void TTextRender::Pos(GLint xpos, GLint ypos) {
  x = xpos; y= ypos;
} 
GLint TTextRender::PosY() {
  return y;
}
void TTextRender::PosY(GLint ypos) {
  y = ypos;
} 
void TTextRender::PosX(GLint xpos) {
  x = xpos;
} 

/* **********************************************************************
 * Draw - calls the renderer
 * *********************************************************************/
void TTextRender::Draw(const string text) {
  if (shadow) {
    text_render->Shadow();
  }
  text_render->Size(size);
  text_render->Color4fv(color.data);
  text_render->Draw(x, y, text.c_str());
}

/* **********************************************************************
 * Print
 * *********************************************************************/
void TTextRender::Print(string text) {
  Draw(text);
  x += Metric(text);
}

/* **********************************************************************
 * Center
 * *********************************************************************/
void TTextRender::Center(GLint min, GLint max, string text) {
  GLint metric = Metric(text);
  x = (max - min - metric)/2;
  Draw(text);
  x += metric;
}

/* **********************************************************************
 * Center on pivot character
 * *********************************************************************/
void TTextRender::Center(GLint min, GLint max, string text, string pivot) {
  int pivot_pos = text.find(pivot);
  string left   = text.substr(0, pivot_pos);
  // string rigth  = text.substr(pivot + pivot.size(), text.size());
  /* Position for the pivot - left*/
  x = (max - min - Metric(pivot))/2 - Metric(left);
  Draw(text);
  x += Metric(text);
}

/* **********************************************************************
 * PrintLn
 * *********************************************************************/
void TTextRender::PrintLn(string text) {
  Draw(text);
  y -= size;
}

/* **********************************************************************
 * CenterLn
 * *********************************************************************/
void TTextRender::CenterLn(GLint min, GLint max, string text) {
  Center(min, max, text);
  y -= size;
}

/* **********************************************************************
 * CenterLn
 * *********************************************************************/
void TTextRender::CenterLn(GLint min, GLint max, string text, string pivot) {
  Center(min, max, text, pivot);
  y -= size;
}

/* **********************************************************************
 * Metric   
 * TODO: Fix this up.
 * *********************************************************************/
GLint TTextRender::Metric(string text) {
  return size*text.size(); 
}
