#include "menu_round.hh"

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
  AddMenuItem(new TActionMenuItem(this, "Abort round", 
				  "Press return to abort current round", 
				  AbortRoundFunc));
  AddMenuItem(new TActionMenuItem(this, "Abort game", 
				  "Press return to abort current game", 
				  AbortGameFunc));
}
