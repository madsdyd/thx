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
#include "player.hh"
/* Implements functions related to the players */

#include "inventory.hh"
#include "tank.hh"

/* **********************************************************************
   The constructor */
TPlayer::TPlayer(string nname) { 
  name  = nname;
  money = 500;
  score = 0;
  inventory = new TInventory(this);
}

/* **********************************************************************
   The destructor */
TPlayer::~TPlayer() {
  cout << "TPlayer::~TPlayer() - clean up stuff?" << endl;
  delete inventory;
}

/* **********************************************************************
   PrepareRound sets up the player for another round */
void TPlayer::PrepareRound(TVector * location) {
  /* Reset the inventory */
  inventory->SelectFirst();
  /* Prepare the tank */
  tank->PrepareRound(location);
  /* Reset the viewpoint */
  viewpoint.Reset(location);
  /* Adjust a bit, to better see our tank - this backs up a little, and looks
     into the ground */
  viewpoint.translation.y--;
  viewpoint.translation.y--;
  viewpoint.rotation.x = 45;
}
