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
#ifndef __MENU_ROUND_HH__
#define __MENU_ROUND_HH__
/* Define the menus to display between rounds */

#include "menu.hh"

/* The continuemenu is very simple, and only have one menu item, that
   "continues", by calling the function passed to the menu */
class TContinueMenu : public TMenu {
public:
  TContinueMenu(string title, TAction ContinueFunc);
};

/* This is the "in game" menu, that can do various things, like resuming, 
   canceling, etc */
class TInGameMenu : public TMenu {
public:
  TInGameMenu(string title, 
	      TAction AbortGameFunc, 
	      TAction AbortRoundFunc, 
	      TAction CancelFunc);
};

/* The global menues defined in this module */
extern TContinueMenu * RoundOverMenu;
extern TContinueMenu * GameOverMenu;
extern TInGameMenu   * InGameMenu;
#endif
