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
#include <strstream>

#include "menu_controls.hh"
#include "menuitem.hh"
#include "command.hh"
#include "sound.hh"
#include "inputconsumer.hh"

#include "display.hh"
/* This is the global controls menu */
TControlsMenu * ControlsMenu = NULL;

/* **********************************************************************
 * A special menuitem for the controls menu
 * This menuitem can bind only to keydown and mousedown events
 * *********************************************************************/
class TControlsMenuItem : public TValueMenuItem {
protected:
  string     command;
  string     args;
  gamemode_t mode;
public:
  TControlsMenuItem(TMenu * owner, string cap, string desc,
		    string cmd, string arg, gamemode_t nmode = gamemode_game) 
    : TValueMenuItem(owner, cap, desc) {
    command = cmd;
    args    = arg;
    mode    = nmode;
  }
  /* Override render
     Renders the mappings, center about a : */
  void Render(int xlow, int xhigh) {
    /* Sets the render color */
    SetRenderColor(); 
    /* Render everything */
    MenuTextRender.CenterLn(xlow, xhigh, 
			    caption + " : " + 
			    InputToCommand.GetMappingsForCommand(command, args, mode),
			    ":");
  }
  /* Override {Unr,R}egisterCommands 
     We do not want the usual itemedit handling - only raw */
  void RegisterCommands() {
    CommandDispatcher.RegisterConsumer("raw-keydown", this);
    CommandDispatcher.RegisterConsumer("raw-mouse", this);
  };
  void UnregisterCommands() {
    CommandDispatcher.UnregisterConsumer("raw-mouse");
    CommandDispatcher.UnregisterConsumer("raw-keydown");
  };
  /* Override enter edit state - we want the raw game mode */
  bool EnterEditState() {
  /* In here, copying of the currentvalue to "new" should take place, 
     then a state change */
    StoredMode = GameMode.GetMode();
    if (!GameMode.SetMode(gamemode_raw)) {
      cerr << "TConstrolMenuItem::EnterEditState - unable to change gamemode" << endl;
      return false;
    }
    RegisterCommands();
    state = menuitem_state_selected;
    return true;
  } 
  /* Override CommandConsume - to handle raw, when we are selected */
  bool CommandConsume(TCommand * Command) {
    if (menuitem_state_selected == state) {
      // cout << "TControlsMenuItem::CommandConsume - got (" 
      //<< Command->name << ", " << Command->args << ")" << endl;
      /* Mouse down? */
      if ("raw-mouse" == Command->name) {
	/* Build mouse event */
	TMouseInputEvent tmpev(Command->args);
	/* Figure out if within */
	if (TestHit(tmpev.x, tmpev.y)) {
	  /* Accept this - add mapping */
	  InputToCommand.ToggleMouseMapping(mode, tmpev.mouse_inputevent_event,
					 command, args);
#ifdef SOUND_ON
	  sound_play(names_to_nums[THX_DATADIR "/sounds/menu_deselect.raw"]);
#endif
	  AcceptNewValue();
	} else {
	  /* Deselect this command */
#ifdef SOUND_ON
	  sound_play(names_to_nums[THX_DATADIR "/sounds/menu_deselect.raw"]);
#endif
	  DiscardNewValue();
	}
	return true;
      }
      /* Keyboard down? */
      else if ("raw-keydown" == Command->name) {
	istrstream tmpargs(Command->args.c_str());
	keyboard_inputevent_event_t tmpev;
	tmpargs >> tmpev.key;
	tmpev.type = keydown;
	/* Add a mapping between this key, and the command this item has */
	InputToCommand.ToggleKeyboardMapping(mode, tmpev, 
					  command, args);
#ifdef SOUND_ON
	sound_play(names_to_nums[THX_DATADIR "/sounds/menu_deselect.raw"]);
#endif
	AcceptNewValue();
	return true;
	}	  
      /* Fall through to TValueMenuItem */
    }
    /* Not selected */
    return TValueMenuItem::CommandConsume(Command);
  }
};

/* **********************************************************************
 * The constructor for the controls menu
 * *********************************************************************/
TControlsMenu::TControlsMenu(string title) : TMenu(title) {
  /* Setup a scroll area */
  TScrollMenuItem * tmpitem1 
    = new TScrollMenuItem(this, "Click to scroll up", true);
  TScrollMenuItem * tmpitem2 
    = new TScrollMenuItem(this, "Click to scroll down", false);
  tmpitem1->SetOtherEnd(tmpitem2);
  tmpitem2->SetOtherEnd(tmpitem1);

  OpenScrollArea(tmpitem1);

  /* Viewpoint move related */
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

  /* Viewpoint rotate related */
  AddMenuItem(new TControlsMenuItem(this, "Rotate forward", 
				    "Change what key rotates forward",
				    "viewpoint-rotate", "+forward"));
  AddMenuItem(new TControlsMenuItem(this, "Rotate backward", 
				    "Change what key rotates backward",
				    "viewpoint-rotate", "+backward"));
  AddMenuItem(new TControlsMenuItem(this, "Rotate left", 
				    "Change what key rotates left",
				    "viewpoint-rotate", "+left"));
  AddMenuItem(new TControlsMenuItem(this, "Rotate right", 
				    "Change what key rotates right",
				    "viewpoint-rotate", "+right"));

  /* Tank related */
  AddMenuItem(new TControlsMenuItem(this, "Cannon left", 
				    "Change what key rotates the cannon left",
				    "cannon", "+rotate-left"));
  AddMenuItem(new TControlsMenuItem(this, "Cannon right", 
				    "Change what key rotates the cannon right",
				    "cannon", "+rotate-right"));
  AddMenuItem(new TControlsMenuItem(this, "Cannon raise", 
				    "Change what key raises the cannon",
				    "cannon", "+raise"));
  AddMenuItem(new TControlsMenuItem(this, "Cannon lower", 
				    "Change what key lowers the cannon",
				    "cannon", "+lower"));
  AddMenuItem(new TControlsMenuItem(this, "Increase force", 
				    "Change what key increases the cannon force",
				    "cannon", "+force-increase"));  
  AddMenuItem(new TControlsMenuItem(this, "Decrease force", 
				    "Change what key decreases the cannon force",
				    "cannon", "+force-decrease"));  
  AddMenuItem(new TControlsMenuItem(this, "Next weapon", 
				    "Change what key selects next weapon",
				    "inventory", "next-weapon"));
  AddMenuItem(new TControlsMenuItem(this, "Fire", 
				    "Change what key fires the cannon",
				    "fire", ""));

  /* System */
  AddMenuItem(new TControlsMenuItem(this, "Toggle grabbing", 
				    "Change what key toggles grabbing the mouse",
				    "toggle-pointer-grab", "", gamemode_any));
  AddMenuItem(new TControlsMenuItem(this, "Render lines", 
				    "Change what key select line render mode",
				    "render", "lines"));
  AddMenuItem(new TControlsMenuItem(this, "Render polygons", 
				    "Change what key select polygon render mode",
				    "render", "polygons"));
  AddMenuItem(new TControlsMenuItem(this, "Render textures", 
				    "Change what key select texture render mode",
				    "render", "textures"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle lighting", 
				    "Change what key toggles lighting",
				    "render", "toggle-lights"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle shading", 
				    "Change what key toggles shademode",
				    "render", "toggle-shademode"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle normals", 
				    "Change what key toggles display of normals",
				    "render", "toggle-normals"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle markers", 
				    "Change what key toggles display of markers",
				    "render", "toggle-markers"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle flymode", 
				    "Change what key toggles flymode",
				    "display", "toggle-flymode"));
  AddMenuItem(new TControlsMenuItem(this, "Toggle clipmode", 
				    "Change what key toggles clipmode",
				    "display", "toggle-clipmode"));
  AddMenuItem(new TControlsMenuItem(this, "Test refreshrate", 
				    "Change what key test the refreshrate",
				    "display", "refreshrate"));
  AddMenuItem(new TControlsMenuItem(this, "Quit immediate", 
				    "Change what key quits THX",
				    "quit", "", gamemode_any));

  /* Close the scroll area, allow 10 items to show at a time */
  CloseScrollArea(10, tmpitem2);

  /* Return menu item */
  AddCancelMenuItem(new TReturnMenuItem(this, "Return", 
					"Return to previous "
					"menu"));
}
/* **********************************************************************
 * Overriding the item size
 * *********************************************************************/
void TControlsMenu::SetItemSize() {
  // MenuTextRender.size = Display->GetHeight()/34;  /* 480/34 ~= 14 */
  MenuTextRender.size = Display->GetHeight()/27;  /* 480/34 ~= 14 */
}
