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
