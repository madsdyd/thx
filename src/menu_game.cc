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
    tmpitem->AddValue("flatlands");
    tmpitem->AddValue("bumby");
    tmpitem->AddValue("highlands");
    tmpitem->AddValue("mountains");
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
    tmpinfo->AddLine("and change values.");
    tmpinfo->AddLine("Use the standard unix readline");
    tmpinfo->AddLine("keys to modify values:");
    tmpinfo->AddLine("Ctrl+b back, Ctrl+f forward,");
    tmpinfo->AddLine("Ctrl+k cut, Ctrl+y yank");
    tmpinfo->AddLine("Backspace, delete and Ctrl+d deletes");
    tmpinfo->AddLine("Enter accepts, escape cancels.");
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
