#ifndef __MENU_GAME_HH__
#define __MENU_GAME_HH__
/* Defines various options and a global menu to display before and between games */

#include "menu.hh"

class TGameMenu : public TMenu {
private:
  TMenu * NewGameMenu;
  TMenu * HelpMenu;
  TMenu * ExitMenu;
public:
  /* Storage for our variables that are set in the menus, but used in
     setting up the game */
  string mapsize;
  string numplayers;
  string numrounds;

  string playernames[4];

  /* Set up the menu */
  TGameMenu(string title, 
	    TAction GameStartFunc,
	    TAction GameEndFunc);
  ~TGameMenu() {
    delete NewGameMenu;
    delete HelpMenu;
    delete ExitMenu;
  }
};

/* The global game menu */
extern TGameMenu * GameMenu;

#endif
