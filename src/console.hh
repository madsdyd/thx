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
#ifndef __CONSOLE_HH__
#define __CONSOLE_HH__

/* The purpose of this is - in the long run - to have a "quake" like console.
   For now, it is simply a couple of lines in the upper left corner */
#include <string>
#include <vector>
#include "types.hh"  /* system_time */

class TDisplay;
class TConsole {
private:
  unsigned int max_num_lines;
  vector<string> lines;
  system_time_t fade_time;
  int position;  /* The current position of the console - 0 if not down */
  int delta_pos; /* The way we move the console */
  int max_pos;   /* The max position  - updated from Render, to the middle 
		    of the y position */
public:
  TConsole(int num_lines);
  void Update(system_time_t deltatime);
  /* Render assumes that the display is in flat mode */
  void Render(TDisplay * display);
  /* Add a line to the console */
  void AddLine(string line);
  /* Used to add a line that will have "extratime" on the console lines 
     Used like this: Clear() - do something that takes a loong time -
     AddLine("something", long_time) - Update() */
  void AddLine(string line, system_time_t extratime);
  /* Take the console up and down */
  void Down();
  void Up();
  /* Clear the console */
  void Clear();
};
#endif
