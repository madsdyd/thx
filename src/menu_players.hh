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
#ifndef __MENU_PLAYERS_HH__
#define __MENU_PLAYERS_HH__
/* This unit defines a menu to set up the players. This does not
   include the number - which is supposed to be set in another menu */
#include <list>
#include "menu.hh"
#include "menuitem_color.hh"
/* This declares a class to hold some player information.
   This information is stored somewhere else, so that it can be
   üsed when setting up a game. */
class TPlayerSetting {
public:
  string name;
  TColor color;
};
typedef vector<TPlayerSetting> TPlayerSettings;
typedef TPlayerSettings::iterator TPlayerSettingsIterator;


class TPlayerMenu : public TMenu {
private:
  unsigned int * NumPlayers;
  TPlayerSettings * PlayerSettings;
public:
  /* Override the constructor to take a list of TPlayerSettings */
  TPlayerMenu(string title, unsigned int * num_players, 
	      TPlayerSettings * nplayer_settings);
  /* Override show to make it dynamically insert a number of menuitems */
  virtual void Show();
};
#endif
