#ifndef __MENU_SCORE_HH__
#define __MENU_SCORE_HH__

#include "menu.hh"
#include "game.hh"

/* This module defines the score board that is displayed at the end of
   each game */

class TScoreMenu : public TMenu {
public:
  TScoreMenu(string title, TPlayerInfos * playerinfos, 
	     TAction CancelFunc);
};

extern TScoreMenu * ScoreMenu;
#endif
