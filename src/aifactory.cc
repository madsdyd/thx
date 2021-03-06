/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#include "aifactory.hh"
#include "aibaltazar.hh"
#include "aicasper.hh"

#include <iostream>
using std::cerr;
/* **********************************************************************
 * The factory
 * *********************************************************************/
TAIFactory AIFactory;

/* **********************************************************************
 * Constructor - does nothing now
 * *********************************************************************/
TAIFactory::TAIFactory() {
};

/* **********************************************************************
 * Destructor - equally empty
 * *********************************************************************/
TAIFactory::~TAIFactory() {
};

/* **********************************************************************
 * Creates a name AI player
 * *********************************************************************/
TAIPlayer * TAIFactory::CreateAIPlayer(string ainame, TGame * game, 
			   string playername, string teamname) {
  if ("casper" == ainame) {
    return new TAIPlayerCasper(game, playername, teamname);
  } else if ("baltazar" == ainame) {
    return new TAIPlayerBaltazar(game, playername, teamname);
  } else {
    cerr << "TAIFactory::CreateAIPlayer - Unknown AT \"" << ainame
	 << "\" - creating Baltazar AI" << endl;
    return new TAIPlayerBaltazar(game, playername, teamname);
  }
}
