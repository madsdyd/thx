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
#include "menu_game.hh"

TGameMenu * GameMenu;


/* **********************************************************************
   The constructor for our game menu.
   The reason to use a class is to get some code called when 
   instantiating the class */
TGameMenu::TGameMenu(string title,
		     TAction GameStartFunc,
		     TAction GameEndFunc) : TMenu(title) {
  NewGameMenu = new TMenu("New Game");
  HelpMenu    = new TMenu("Help");
  ExitMenu    = new TMenu("Really Exit?");

  /* ************************************************************
     The Main menu (this) */
  {
    // TMenuItem * tmpitem;
    AddMenuItem(new TSubMenuItem(this, "Help", 
				 "Hit enter for navigation help", 
				 HelpMenu));
    AddMenuItem(new TSubMenuItem(this, "New Game", 
				 "Starts a new game", 
				 NewGameMenu));
    /*    tmpitem = new TSubMenuItem(this, "Config", 
			       "Edit config options", 
			       NULL);
			       tmpitem->Disable();
    AddMenuItem(tmpitem);
    */
    /* Cancel/Exit */
    AddCancelMenuItem(new TSubMenuItem(this, "Exit", 
				       "Exit the game", 
				       ExitMenu));
  }
  /* ************************************************************
     The NewGameMenu menu */
  {
    /* Setup the NewGameMenu */
    NewGameMenu->AddMenuItem(new TActionMenuItem(NewGameMenu, "Start", 
						 "Starts the game with "
						 "current options", 
						 GameStartFunc));
    NewGameMenu->AddMenuItem(new TSpaceMenuItem(NewGameMenu));
			     
      TListMenuItem * tmpitem 
      = new TListMenuItem(NewGameMenu, "Map size", 
			  "Change the size of the map",
			  &mapsize);
    //    tmpitem->AddValue("small");
    tmpitem->AddValue("medium");
    tmpitem->AddValue("large");
    NewGameMenu->AddMenuItem(tmpitem);
    tmpitem 
      = new TListMenuItem(NewGameMenu, "Number of players", 
			  "Set the number of players",
			  &numplayers);
    tmpitem->AddValue("2");
    tmpitem->AddValue("3");
    tmpitem->AddValue("4");
    NewGameMenu->AddMenuItem(tmpitem);
    tmpitem 
      = new TListMenuItem(NewGameMenu, "Number of rounds", 
			  "Set the number of rounds of the game",
			  &numrounds);
    tmpitem->AddValue("2");
    tmpitem->AddValue("3");
    tmpitem->AddValue("4");
    tmpitem->AddValue("5");
    tmpitem->AddValue("6");
    tmpitem->AddValue("7");
    tmpitem->AddValue("8");
    tmpitem->AddValue("9");
    NewGameMenu->AddMenuItem(tmpitem);
    /* The Map steepness - decides its type */
    tmpitem = 
      new TListMenuItem(NewGameMenu, "Map type", 
			"Select the topology of the map", 
			&maptype);
    tmpitem->AddValue("highlands");
    tmpitem->AddValue("mountains");
    tmpitem->AddValue("flatlands");
    tmpitem->AddValue("bumby");
    NewGameMenu->AddMenuItem(tmpitem);
    /* The player names TODO: Should be dynamic */
    playernames[0] = "Player 1";
    playernames[1] = "Player 2";
    playernames[2] = "Player 3";
    playernames[3] = "Player 4";
    NewGameMenu->AddMenuItem(new TStringMenuItem(NewGameMenu, "Player 1 name", 
						"Enter the name of player 1 "
						 "here",
						 &playernames[0]));
    NewGameMenu->AddMenuItem(new TStringMenuItem(NewGameMenu, "Player 2 name", 
						"Enter the name of player 2 "
						 "here",
						 &playernames[1]));
    NewGameMenu->AddMenuItem(new TStringMenuItem(NewGameMenu, "Player 3 name", 
						"Enter the name of player 3 " 
						 "here",
						 &playernames[2]));
    NewGameMenu->AddMenuItem(new TStringMenuItem(NewGameMenu, "Player 4 name", 
						 "Enter the name of player 4 "
						 "here",
						 &playernames[3]));

    NewGameMenu->AddMenuItem(new TSpaceMenuItem(NewGameMenu));
    NewGameMenu->AddCancelMenuItem(new TReturnMenuItem(NewGameMenu, "Return", 
						       "Return to previous "
						       "menu"));
  }
  /* ************************************************************
     The HelpMenu menu */
  {
    TInfoMenuItem * tmpinfo = new TInfoMenuItem(HelpMenu, "Info", 
						"Use the arrow keys to select");
    tmpinfo->AddLine("and change values in menus.");
    tmpinfo->AddLine("In the game, default controls");
    tmpinfo->AddLine("are d, g,f,r,t,b for moving");
    tmpinfo->AddLine("your viewpoint, and h,j,k,u for");
    tmpinfo->AddLine("rotating your viewpoint.");
    tmpinfo->AddLine("The cannon is controlled with");
    tmpinfo->AddLine("the arrow keys,+,-, and ENTER.");
    tmpinfo->AddLine("Escape shows the in game menu.");
    tmpinfo->AddLine("q will quit the game at any point.");
    HelpMenu->AddMenuItem(tmpinfo);    
    HelpMenu->AddMenuItem(new TSpaceMenuItem(this));
    HelpMenu->AddCancelMenuItem(new TReturnMenuItem(HelpMenu, "Return", 
					      "Return to previous menu"));
  }
  /* ************************************************************
     The ExitMenu menu */
  {
    ExitMenu->AddCancelMenuItem(new TReturnMenuItem(ExitMenu, "No", 
						    "Play some more"));
    ExitMenu->AddMenuItem(new TActionMenuItem(ExitMenu, "Yes", 
					      "Really exit the game", 
					      GameEndFunc));
  }
};
