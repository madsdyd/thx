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
#ifndef __GAMEMODE_HH__
#define __GAMEMODE_HH__
/* This unit define which "mode" the game is in.
   Modes are (pt) 
   menu  - a menu is shown. Inputs should map to the menu commands
   edit  - a menu item is in editing mode
   game  - the game is running. Inputs should map the game commands
   any   - this is not really a mode the game is in, but is used
           for commands, and such. 
   other possible modes:
   raw   - This is a vague idea in my head, that 
           during configuration, we need to see raw events.
   remote - Another vague idea - when a server stuffs commands, we are in
            remote mode?
*/
typedef enum {gamemode_menu = 0, 
	      gamemode_edit = 1,
              gamemode_game = 2,
              gamemode_any  = 3} gamemode_t;
const int gamemode_count = 4;
/* The gamemode class */
class TGameMode {
private:
  gamemode_t mode;
public:
  TGameMode();
  /* Changing the mode may fail.. */
  bool SetMode(gamemode_t newmode);
  gamemode_t GetMode();
};
/* The gamemode singleton */
extern TGameMode GameMode;
#endif
