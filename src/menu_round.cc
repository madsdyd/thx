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
#include "menu_round.hh"
#include "menu_controls.hh"

TContinueMenu * RoundOverMenu;
TContinueMenu * GameOverMenu;
TInGameMenu * InGameMenu;

/* **********************************************************************
   The construcor for the continue menu */
TContinueMenu::TContinueMenu(string title, TAction ContinueFunc) 
  : TMenu(title) {
  AddCancelMenuItem(new TActionMenuItem(this, "OK", "Press return to continue",
					ContinueFunc));
}


/* **********************************************************************
   The in game menu constructor */
TInGameMenu::TInGameMenu(string title, 
			 TAction AbortGameFunc, 
			 TAction AbortRoundFunc, 
			 TAction CancelFunc) : TMenu(title) {
  AddCancelMenuItem(new TActionMenuItem(this, "Resume game", 
					"Press return to resume game", 
					CancelFunc));
  AddMenuItem(new TSubMenuItem(this, "Controls", 
			       "Setup the controls", 
			       ControlsMenu));
  AddMenuItem(new TActionMenuItem(this, "Abort round", 
				  "Press return to abort current round", 
				  AbortRoundFunc));
  AddMenuItem(new TActionMenuItem(this, "Abort game", 
				  "Press return to abort current game", 
				  AbortGameFunc));
}
