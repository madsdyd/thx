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
#include "menu_score.hh"
#include "player.hh"

#include <strstream>
#include <multimap.h>

/* Global score menu */
TScoreMenu * ScoreMenu;

/* **********************************************************************
 * The constructor simply uses the playerinfos to build a list of the
 * scores
 * *********************************************************************/
TScoreMenu::TScoreMenu(string title, TPlayerInfos * playerinfos, 
		       bool teammode, TAction CancelFunc) : TMenu(title) {
  /* The list of players is a TInfoMenuItem */
  TInfoMenuItem * tmpinfo = new TInfoMenuItem(this, "not used", "");
  
  TPlayerInfosIterator End = playerinfos->end();
  /* If it is a team game, start by listing the team scores */
  if (teammode) {
    tmpinfo->AddLine("Team scores");
    map<string, int> teamscores;
    for (TPlayerInfosIterator i = playerinfos->begin(); i != End; i++) {
      teamscores[(*i)->player->team] += (*i)->player->score;
    }
    for (map<string, int>::iterator it = teamscores.begin();
	 it != teamscores.end(); it++) {
      ostrstream tmp;
      tmp << (*it).first << " - " << (*it).second << ends;
      tmpinfo->AddLine(tmp.str());
    }
    tmpinfo->AddLine("");
    tmpinfo->AddLine("Player scores");
  }
  

  /* Need to sort this, somehow */
  multimap<int, const TPlayerInfo *> scores;
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
