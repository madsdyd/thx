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
#include <strstream>
#include "menu_players.hh"
/* **********************************************************************
 * TPlayerMenu constructor - sets variables
 * *********************************************************************/
TPlayerMenu::TPlayerMenu(string title, unsigned int * num_players, 
			 bool * teammode, 
			 TPlayerSettings * nplayer_settings) 
  : TMenu(title) {
  NumPlayers     = num_players;
  TeamMode       = teammode;
  PlayerSettings = nplayer_settings;

}

/* **********************************************************************
 * SyncPlayerSettings - makes sure that we have enough playersettings
 * init names, etc.
 * *********************************************************************/
void TPlayerMenu::SyncPlayerSettings() {
  while(PlayerSettings->size() < *NumPlayers) {
    // cout << "Adding a playersetting" << endl;
    TPlayerSetting tmpset;
    ostrstream tmp1, tmp2, tmp3, tmp4;
    /* Set default values for name, team and color */
    tmp1.form("Player %i", PlayerSettings->size() + 1) << ends;
    tmpset.name = tmp1.str();
    tmp2.form("player-%i", PlayerSettings->size() + 1) << ends;
    tmpset.color = ColorDefinitions.GetColor(tmp2.str());
    tmp3.form("team-%i", PlayerSettings->size() + 1) << ends;
    tmpset.team  = tmp3.str();
    if (0 == PlayerSettings->size()) {
      // tmpset.name        = "Baltazar";
      // tmpset.playerclass = "baltazar";
      tmpset.name        = "Casper";
      tmpset.playerclass = "casper";
    } else {
      tmpset.playerclass = "human";
    }
    PlayerSettings->push_back(tmpset);
  }
  while(PlayerSettings->size() > *NumPlayers) {
    // cout << "Removing a playersetting" << endl;
    PlayerSettings->pop_back();
  }
}


/* **********************************************************************
 * Show - Add neccesary playersettings to the list of menuitems.
 * *********************************************************************/
void TPlayerMenu::Show() {
  /* The menuitems are added in here - first, they are all removed */
  cout << "TPlayerMenu::Show - memory leak right here" << endl;
  menuitems.clear();

  /* Add a scrollupmenuitem */
  TScrollMenuItem * tmpitem1 
    = new TScrollMenuItem(this, "Click to scroll up", true);
  TScrollMenuItem * tmpitem2 
    = new TScrollMenuItem(this, "Click to scroll down", false);
  tmpitem1->SetOtherEnd(tmpitem2);
  tmpitem2->SetOtherEnd(tmpitem1);

  OpenScrollArea(tmpitem1);

  /* For each player, currently the name and color needs to be 
     selected. 
     First, make sure that the PlayerSettings has the correct number
     of elemets.
     Then, add/remove the neccesary menu items to the list of menu items
     The layout is fixed, and setup in the constructor */
  SyncPlayerSettings();
  
  /* Create and associate a menu item with each playersetting */
  int count = 1;
  for (TPlayerSettingsIterator i = PlayerSettings->begin();
       i != PlayerSettings->end();
       i++) {
    ostrstream tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    /* fix the player names here */
    tmp1.form("Player %i name", count) << ends;
    tmp2.form("Enter the name of player %i here", count) << ends;
    AddMenuItem(new TStringMenuItem(this, tmp1.str(), tmp2.str(), 
				    &(*i).name));
    /* and the players type */
    tmp7.form("Player %i type", count) << ends;
    tmp8.form("Select the type of player %i here", count) << ends;
    TStringListMenuItem * stmpitem
      = new TStringListMenuItem(this, tmp7.str(), tmp8.str(),
			    &(*i).playerclass);
    stmpitem->AddOption("human", "human");
    stmpitem->AddOption("AI Baltazar", "baltazar");
    stmpitem->AddOption("AI Casper", "casper");
    AddMenuItem(stmpitem);

    /* and the players team */
    if (*TeamMode) {
      tmp5.form("Player %i teamname", count) << ends;
      tmp6.form("Enter the team of player %i here", count) << ends;
      AddMenuItem(new TStringMenuItem(this, tmp5.str(), tmp6.str(),
				      &(*i).team));
    }

    /* and, the players color */
    tmp3.form("Player %i color", count) << ends;
    tmp4.form("Select the color of player %i", count) << ends;
    TColorMenuItem * ctmpitem
      = new TColorMenuItem(this, tmp3.str(), tmp3.str(),
			   &(*i).color);
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-1"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-2"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-3"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-4"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-5"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-6"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-7"));
    ctmpitem->AddOption("", ColorDefinitions.GetColor("player-8"));
    AddMenuItem(ctmpitem);

    /* Count to the next player */
    count++;
  }

  /* Close the scroll area, allow 10 items to show at a time */
  CloseScrollArea(mymin(PlayerSettings->size()*2, 10), tmpitem2);
  
  /* add a return menu item */
  AddCancelMenuItem(new TReturnMenuItem(this, "Return",
					"Return to previous menu"));
  TMenu::Show();
}
