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
#ifndef __MENU_GAME_HH__
#define __MENU_GAME_HH__
/* Defines various options and a global menu to display before and between games */

#include "menu.hh"
#include "menu_players.hh"
#include "color.hh"

class TGameMenu : public TMenu {
private:
  /* The game menu owns the following menus, which are built by
     the TGameMenu constructor */
  TMenu * NewGameMenu;
  TPlayerMenu * PlayerMenu; 
  TMenu * HelpMenu;
  TMenu * AboutMenu;
  TMenu * ExitMenu;
public:
  /* Storage for our variables that are set in the menus, but used in
     setting up the game */
  unsigned int mapsize;
  unsigned int numplayers;
  unsigned int numrounds;
  double maptype;
  bool teammode;

  /* Callback to let the menu update, if the playernumbber changes */
  virtual void MenuItemChange(int code);

  /* This is the storage for the information about players. 
     This list only changes while the TPlayerMenu is shown */
  TPlayerSettings PlayerSettings;

  /* Set up the menu */
  TGameMenu(string title, 
	    TAction GameStartFunc,
	    TAction GameEndFunc);
  ~TGameMenu();
};

/* The global game menu */
extern TGameMenu * GameMenu;

#endif
