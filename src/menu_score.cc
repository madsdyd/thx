#include "menu_score.hh"

#include <strstream>
#include <multimap.h>

/* Global score menu */
TScoreMenu * ScoreMenu;

/* **********************************************************************
   The constructor simply uses the playerinfos to build a list of the 
   scores */
TScoreMenu::TScoreMenu(string title, TPlayerInfos * playerinfos, 
		       TAction CancelFunc) : TMenu(title) {
  /* The list of players is a TInfoMenuItem */
  TInfoMenuItem * tmpinfo = new TInfoMenuItem(this, "not used", "");
  

  /* Need to sort this, somehow */
  multimap<int, const TPlayerInfo *> scores;
  TPlayerInfosIterator End = playerinfos->end();
  for (TPlayerInfosIterator i = playerinfos->begin(); i != End; i++) {
    scores.insert(pair<int, const TPlayerInfo *>((*i)->player->score, (*i)));
  }
  for (multimap<int, const TPlayerInfo *>::iterator it = scores.begin();
       it != scores.end(); it++) {
    ostrstream tmp;
    TPlayer * player = (*it).second->player;
    tmp << player->name << " - " << player->score << ends;
    tmpinfo->AddLine(tmp.str());
  }
  /* Add the menu item that displays the players */
  AddMenuItem(tmpinfo);
  AddMenuItem(new TSpaceMenuItem(this));
  /* Add a cancel menu item */
  AddCancelMenuItem(new TActionMenuItem(this, "Continue", "Continue",
                                        CancelFunc));              
}
