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
