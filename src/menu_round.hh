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
