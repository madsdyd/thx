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
using std::endl;

#include "console.hh"

#include "display.hh"
#include "text.hh"

#define FADE_DELTA 5.0 /* Every line is on screen for this long */
#define NUM_LINES_SHOW 4
/* The speed when moving up/down */
#define DELTA_POS 100


/* **********************************************************************
 * Constructor & destructor
 * *********************************************************************/
TConsole::TConsole(int num_lines) {
  max_num_lines = num_lines;
  fade_time     = 0;
  position      = 0;
  dposition     = 0.0;
  delta_pos     = 0;
  state         = is_up;
  max_pos       = 0;
  /* Register the command we handle */
  CommandDispatcher.RegisterConsumer("console-toggle-display", this);
}

TConsole::~TConsole() {
  CommandDispatcher.UnregisterConsumer("console-toggle-display");
}

/* **********************************************************************
 * The update function mainly makes sure the display lines time out,
 * or move the
 * *********************************************************************/
void TConsole::Update(system_time_t deltatime) {
  /*  cout << "enter: deltatime: " << deltatime << ", position : " << position << ", delta_pos : " << delta_pos 
      << ", dposition : " << dposition << endl; */
  /* Control the fade time */
  fade_time = mymax(0, fade_time - deltatime);
  /* And the position of the console */
  if (delta_pos != 0) { /* We need to move the background */
    state = is_moving;
    if (delta_pos < 0) {
      /* Moving up */
      dposition = mymax (0, dposition+delta_pos*deltatime);
      position = (int) floor(dposition);
      if (0 == position) {
	dposition = 0.0;
	delta_pos = 0;
	state = is_up;
      }
    } else {
      /* Moving down */
      dposition = mymin (max_pos, dposition+delta_pos*deltatime);
      position = (int) floor(dposition);
      if (max_pos == position) {
	dposition = max_pos;
	delta_pos = 0;
	state = is_down;
      }
    }
  }
  /*  cout << "exit: position : " << position << ", delta_pos : " << delta_pos 
      << ", dposition : " << dposition << endl; */
}

/* **********************************************************************
 * The render function renders the lines that are currently
 * visible. It can be in one of two modes, displaying only a couple of
 * lines with no background or a background with several lines of text
 * *********************************************************************/
void TConsole::Render(TDisplay * display) {
  /* Update maxpos */
  max_pos = display->GetHeight()/2;
  int i = 0;
  if (0 != position) {
    /* render with background */
    i = mymin((unsigned int) position / display->textrender->size, 
	      lines.size());
    /* Start printing from the top of the screen, or lower, if fewer lines
     */
    display->textrender->Pos(0, display->GetHeight() 
			     - position
			     + i*display->textrender->size);
  } else {
    /* render without background */
    i   = mymin((int) lines.size(), 
		(int) rint(fade_time/FADE_DELTA));
    /* Start printing from the top of the screen */
    display->textrender->Pos(0, display->GetHeight() 
			     - display->textrender->size);
  }
  while(i > 0) {
    display->textrender->PrintLn(lines[lines.size()-i--]);
  }
  if (0 != position) {
    display->textrender->PrintLn("--------------------------------------------------------");
  }
}

/* **********************************************************************
 * Add a line to all the lines
 * *********************************************************************/
void TConsole::AddLine(string line) {
  /* Check if we need to remove a line */
  while (lines.size() >= max_num_lines) {
    lines.erase(lines.begin());
  }
  /* Add it */
  lines.push_back(line);
  /* Update the timer */
  fade_time += FADE_DELTA;
  fade_time  = mymin(fade_time, NUM_LINES_SHOW * FADE_DELTA);  
#ifdef DEBUG
  cout << "Coonsole: " << line << endl;
#endif
}

void TConsole::AddLine(string line, system_time_t extratime) {
  AddLine(line);
  /* This could cause problems with too many lines beeing displayed */
  fade_time += extratime;
}

/* **********************************************************************
 * Up and down
 * *********************************************************************/
void TConsole::Down() {
  // AddLine("Going down");
  delta_pos = DELTA_POS;
}

void TConsole::Up() {
  // AddLine("Going up");
  delta_pos = -DELTA_POS;
}

/* **********************************************************************
 * Clear
 * *********************************************************************/
void TConsole::Clear() {
  fade_time = 0;
}

/* **********************************************************************
 * Handling commands
 * *********************************************************************/
bool TConsole::CommandConsume(TCommand * Command) {
  if ("console-toggle-display" == Command->name) {
    if (is_up == state) {
      Down();
    } else if (is_down == state) {
      Up();
    } else {
      // AddLine("Toogling delta");
      delta_pos = -delta_pos;
    }
    return true;
  }
  return false;
}
