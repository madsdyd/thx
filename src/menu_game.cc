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
#include "menu_controls.hh"
#include "menuitem_color.hh"

TGameMenu * GameMenu;

/* **********************************************************************
 * The destructor for TGameMenue
 * *********************************************************************/
TGameMenu::~TGameMenu() {
  delete NewGameMenu;
  delete PlayerMenu;
  delete HelpMenu;
  delete AboutMenu;
  delete ExitMenu;
};

/* **********************************************************************
   The constructor for our game menu.
   The reason to use a class is to get some code called when 
   instantiating the class */
TGameMenu::TGameMenu(string title,
		     TAction GameStartFunc,
		     TAction GameEndFunc) : TMenu(title) {
  NewGameMenu = new TMenu("New Game");
  PlayerMenu  = new TPlayerMenu("Player Settings", &numplayers, 
				&teammode, &PlayerSettings);
  HelpMenu    = new TMenu("Help");
  AboutMenu   = new TMenu("About Tank Hill eXtreme");
  ExitMenu    = new TMenu("Really Exit?");

  /* Initialize default values */
  numplayers = 2;
  numrounds  = 3;
  mapsize    = 32;
  maptype    = 1.4;
  teammode   = false;
  
  /* ************************************************************
     The Main menu (this) */
  {
    // TMenuItem * tmpitem;
    AddMenuItem(new TSubMenuItem(this, "New Game", 
				 "Starts a new game", 
				 NewGameMenu));
    AddMenuItem(new TSubMenuItem(this, "Controls", 
				 "Setup the controls", 
				 ControlsMenu));
    AddMenuItem(new TSubMenuItem(this, "Help", 
				 "Help on Tank Hill eXtreme", 
				 HelpMenu));
    AddMenuItem(new TSubMenuItem(this, "About", 
				 "Display info about Tank Hill eXtreme", 
				 AboutMenu));
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
			     
    /* Player related stuff */
    TUIntListMenuItem * tmpitem 
      = new TUIntListMenuItem(NewGameMenu, "Number of players", 
			      "Set the number of players",
			      &numplayers, this, 42); 
    tmpitem->AddOption("2", 2);
    tmpitem->AddOption("3", 3);
    tmpitem->AddOption("4", 4);
    tmpitem->AddOption("5", 5);
    tmpitem->AddOption("6", 6);
    tmpitem->AddOption("7", 7);
    tmpitem->AddOption("8", 8);
    NewGameMenu->AddMenuItem(tmpitem);
    /* Make sure we are synced */
    PlayerMenu->SyncPlayerSettings();

    /* Team option */
    TBoolListMenuItem * btmpitem 
      = new TBoolListMenuItem(NewGameMenu, 
			      "Team mode", 
			      "Enable or disable team mode",
			      &teammode);
    btmpitem->AddOption("on", true);
    btmpitem->AddOption("off", false);
    NewGameMenu->AddMenuItem(btmpitem);
      
    NewGameMenu->AddMenuItem(new TSubMenuItem(NewGameMenu, 
					      "Player Settings", 
					      "Set name, color, etc for "
					      "each player", 
					      PlayerMenu));

    /* Map related stuff */
    tmpitem 
      = new TUIntListMenuItem (NewGameMenu, "Map size", 
			       "Change the size of the map",
			       &mapsize);
    tmpitem->AddOption("small", 16);
    tmpitem->AddOption("medium", 32);
    tmpitem->AddOption("large", 64);
    NewGameMenu->AddMenuItem(tmpitem);

    /* The Map steepness - decides its type */
    //    TDoubleListMenuItem * d
    TDoubleListMenuItem * dtmpitem = 
      new TDoubleListMenuItem(NewGameMenu, "Map type", 
			      "Select the topology of the map", 
			      &maptype);
    dtmpitem->AddOption("flatlands", 0.2);
    dtmpitem->AddOption("bumby", 0.8);
    dtmpitem->AddOption("highlands", 1.4);
    dtmpitem->AddOption("mountains", 2.0);
    NewGameMenu->AddMenuItem(dtmpitem);

    /* Number of rounds */
    tmpitem 
      = new TUIntListMenuItem(NewGameMenu, "Number of rounds", 
			      "Set the number of rounds of the game",
			      &numrounds);
    tmpitem->AddOption("2", 2);
    tmpitem->AddOption("3", 3);
    tmpitem->AddOption("4", 4);
    tmpitem->AddOption("5", 5);
    tmpitem->AddOption("6", 6);
    tmpitem->AddOption("7", 7);
    tmpitem->AddOption("8", 8);
    tmpitem->AddOption("9", 9);
    NewGameMenu->AddMenuItem(tmpitem);



#ifdef NEVER
    TColorMenuItem * ctmpitem 
      = new TColorMenuItem(NewGameMenu, 
			   "Player 1 color", 
			   "Choose the color of player 1", &testcolor);
    ctmpitem->AddOption("Yellow", ColorDefinitions.GetColor("yellow"));
    ctmpitem->AddOption("Blue",   ColorDefinitions.GetColor("blue"));
    ctmpitem->AddOption("Red",    ColorDefinitions.GetColor("red"));
    ctmpitem->AddOption("Green",  ColorDefinitions.GetColor("green"));
    NewGameMenu->AddMenuItem(ctmpitem);
#endif



    NewGameMenu->AddMenuItem(new TSpaceMenuItem(NewGameMenu));
    NewGameMenu->AddCancelMenuItem(new TReturnMenuItem(NewGameMenu, "Return", 
						       "Return to previous "
						       "menu"));
  }
  /* ************************************************************
     The HelpMenu menu */
  {
    TInfoMenuItem * tmpinfo = new TInfoMenuItem(HelpMenu, "Info", 
						"Welcome to Tank Hill eXtreme");
    tmpinfo->AddLine("Tank Hill eXtreme is a 3D version");
    tmpinfo->AddLine("of the mother of all games");
    tmpinfo->AddLine("Scorched Earth");
    tmpinfo->AddLine("The objective is to destroy your");
    tmpinfo->AddLine("opponents tanks in any way possible");
    tmpinfo->AddLine("");
    tmpinfo->AddLine("Use the Controls menu");
    tmpinfo->AddLine("to view and change controls.");
    tmpinfo->AddLine("Escape shows the in game menu.");
    tmpinfo->AddLine("q will quit the game at any point.");
    HelpMenu->AddMenuItem(tmpinfo);    
    HelpMenu->AddMenuItem(new TSpaceMenuItem(this));
    HelpMenu->AddCancelMenuItem(new TReturnMenuItem(HelpMenu, "Return", 
					      "Return to previous menu"));
  }
  /* ************************************************************
     The AboutMenu menu */
  {
    TInfoMenuItem * tmpinfo = new TInfoMenuItem(AboutMenu, "Info", 
						"");
    //tmpinfo->AddLine("");
    tmpinfo->AddLine("Lead programmer");
    tmpinfo->AddLine("Mads Bondo Dydensborg");
    tmpinfo->AddLine("<mads@dydensborg.dk>");
    tmpinfo->AddLine("");
    tmpinfo->AddLine("OpenGL, models, and map coding");
    tmpinfo->AddLine("Skjalm Arroe");
    tmpinfo->AddLine("<skjalm@arroe.dk>");
    tmpinfo->AddLine("");
    tmpinfo->AddLine("THX is released under the GPL");
    tmpinfo->AddLine("Visit the project homepage at");
    tmpinfo->AddLine("sourceforge.net/projects/thx");
    
    AboutMenu->AddMenuItem(tmpinfo);    
    AboutMenu->AddMenuItem(new TSpaceMenuItem(this));
    AboutMenu->AddCancelMenuItem(new TReturnMenuItem(AboutMenu, "Return", 
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

/* **********************************************************************
 * Callback override
 * *********************************************************************/
void TGameMenu::MenuItemChange(int code) {
  /* This is only called when the number of players are changed */
  PlayerMenu->SyncPlayerSettings();
}
