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
#include <string>

#include "menu_controls.hh"
#include "menuitem.hh"

#include "inputconsumer.hh"

/* This is the global controls menu */
TControlsMenu * ControlsMenu = NULL;

/* **********************************************************************
 * A special menuitem for the controls menu
 * This menuitem can bind only to keydown, and only to gamemode_game
 * *********************************************************************/
class TControlsMenuItem : public TValueMenuItem {
protected:
  string command;
  string args;
public:
  TControlsMenuItem(TMenu * owner, string cap, string desc,
		    string cmd, string arg) 
    : TValueMenuItem(owner, cap, desc) {
    command = cmd;
    args    = arg;
  }
  /* Override render */
  void Render(int xlow, int xhigh) {
    /* Sets the render color */
    SetRenderColor(); 
    /* Render everything */
    MenuTextRender.CenterLn(xlow, xhigh, 
			    caption + " : " + 
			    InputToCommand.GetKeyMappingsForCommand(command, args),
			    ":");
  }
};
/* **********************************************************************
 * The constructor for the controls menu
 * *********************************************************************/
TControlsMenu::TControlsMenu(string title) : TMenu(title) {
  /* Viewpoint related */
  AddMenuItem(new TControlsMenuItem(this, "Move forward", 
				    "Change what key moves forward",
				    "viewpoint-move", "+forward"));
  AddMenuItem(new TControlsMenuItem(this, "Move backward", 
				    "Change what key moves backward",
				    "viewpoint-move", "+backward"));
  AddMenuItem(new TControlsMenuItem(this, "Move left", 
				    "Change what key moves left",
				    "viewpoint-move", "+left"));
  AddMenuItem(new TControlsMenuItem(this, "Move right", 
				    "Change what key moves right",
				    "viewpoint-move", "+right"));
  AddMenuItem(new TControlsMenuItem(this, "Move up", 
				    "Change what key moves up",
				    "viewpoint-move", "+up"));
  AddMenuItem(new TControlsMenuItem(this, "Move down", 
				    "Change what key moves down",
				    "viewpoint-move", "+down"));

  /* Tank related */
  AddMenuItem(new TControlsMenuItem(this, "Next weapon", 
				    "Change what key selects next weapon",
				    "inventory", "next-weapon"));
  AddMenuItem(new TControlsMenuItem(this, "Fire", 
				    "Change what key fires the cannon",
				    "fire", ""));
  /* TODO: Need a dynamic way to return to the correct menu */
}
