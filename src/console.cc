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
#include "console.hh"

#include "display.hh"
#include "text.hh"

#define FADE_DELTA 5.0 /* Every line is on screen for this long */
#define NUM_LINES_SHOW 4
#define DELTA_POS 50

/* **********************************************************************
   Constructor */
TConsole::TConsole(int num_lines) {
  max_num_lines = num_lines;
  fade_time     = 0;
  position      = 0;
  delta_pos     = 0;
  max_pos       = 240;
}

/* **********************************************************************
   The update function mainly makes sure the display lines time out,
   or move the */
void TConsole::Update(system_time_t deltatime) {
  /* Control the fade time */
  fade_time = mymax(0, fade_time - deltatime);
  /* And the position of the console */
  if (delta_pos != 0) { /* We need to move the background */
    if (delta_pos < 0) {
      position = mymin (0, position+delta_pos);
    } else {
      position = mymax (max_pos, position+delta_pos);
    }
    if (0 == position || max_pos) {
      delta_pos = 0;
    }
  }
}

/* **********************************************************************
   The render function renders the lines that are currently visible
   It can be in one of two modes, displaying only a couple of lines with
   no background or a background with several lines of text */
void TConsole::Render(TDisplay * display) {
  if (0 != position) {
    /* render with background */
    cout << "TConsole::Render with background" << endl;
  } else {
    /* render without background */
    int i   = mymin((int) lines.size(), 
		    (int) rint(fade_time/FADE_DELTA));
    // cout << "TConsole::Render without background i = " << i << endl;
    display->textrender->Pos(0, display->height-display->textrender->size);
    while(i > 0) {
      display->textrender->PrintLn(lines[lines.size()-i--]);
    }
  }
}

/* **********************************************************************
   Add a line to all the lines */
void TConsole::AddLine(string line) {
  /* Check if we need to remove a line */
  if (lines.size() >= max_num_lines) {
    lines.erase(lines.begin());
  }
  /* Add it */
  lines.push_back(line);
  /* Update the timer */
  fade_time += FADE_DELTA;
  fade_time  = mymin(fade_time, NUM_LINES_SHOW * FADE_DELTA);  
}

void TConsole::AddLine(string line, system_time_t extratime) {
  AddLine(line);
  /* This could cause problems with too many lines beeing displayed */
  fade_time += extratime;
}

/* **********************************************************************
   Up and down */
void TConsole::Down() {
  delta_pos = DELTA_POS;
}

void TConsole::Up() {
  delta_pos = -DELTA_POS;
}

/* **********************************************************************
   Clear */
void TConsole::Clear() {
  fade_time = 0;
}
