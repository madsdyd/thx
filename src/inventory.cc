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
#include "inventory.hh"

#include "projectile.hh"

/* For sprintf - sigh */
#include <stdio.h>

/* **********************************************************************
 * Implementation of stuff that has to do with the inventory
 * *********************************************************************/
TInventoryElement::TInventoryElement(string nname, TProjectile * nprojectile,
				     int nunits, int nprice, int nnum) {
  name       = nname;
  projectile = nprojectile;
  units      = nunits;
  price      = nprice;
  num        = nnum;
}

/* **********************************************************************
 * Buy reduces the money with the appropiate amount and adjust quantity
 * *********************************************************************/
void TInventoryElement::Buy(int * money) {
  if (*money < price) { /* Cant buy without money */
    return;
  }
  if ((num + units) > 99) { /* Never stock more then 99 of anything */
    return;
  }
  num += units;
  *money = *money - price;
}

/* **********************************************************************
 * This is the TInventory 
 * *********************************************************************/

/* **********************************************************************
 * Constructor and destructor
 * *********************************************************************/
TInventory::TInventory(TGame * Game, TPlayer * Owner) {
  /* This is where the projectile types that are available to the players
     are created. Every inventory gets initialized the same way. */
  data.push_back(new TInventoryElement("Baby Missile", 
				       new TProjectile(Game, Owner, 
						       2.0, 40.0),     
				       10, 400, 99));
#define NUM_PROJ_TEST 0
  data.push_back(new TInventoryElement("Missile", 
				       new TProjectile(Game, Owner, 
						       4.0, 45.0),     
				       5, 1875, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Baby Nuke", 
				       new TProjectile(Game, Owner, 
						       6.0, 50.0),     
				       3, 1000, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Nuke", 
				       new TProjectile(Game, Owner, 
						       8.0, 55.0),     
				       1, 1200, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Funky Bomb", 
				       new TSpawnProjectile(Game, Owner, 
							    4.0, 100.0),
				       2, 700, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("MIRV",  
				       new TMirvProjectile(Game, Owner, 
							   4.0, 100.0),  
				       3, 1000, NUM_PROJ_TEST));
  selected = 0; /* Always assume that baby missiles are present */
}

TInventory::~TInventory() {
  cout << "~TInventory() - have to delete stuff here" << endl;
}

/* **********************************************************************
 * The selection methods
 * *********************************************************************/
void TInventory::SelectFirst() {
  selected = 0;
}

/* Select the next that has quantity > 0 - note that baby missiles are 
   unlimited */
void TInventory::SelectNext() {
  do {
    selected = (selected + 1) % data.size();
  } while(data[selected]->num <= 0);
}

/* Select the previous - same as next, almost */
void TInventory::SelectPrevious() {
  do {
    selected = (selected - 1 + data.size()) % data.size();
  } while(data[selected]->num <= 0);
}

/* **********************************************************************
 * The fire method (kaboom)
 * *********************************************************************/
TProjectile * TInventory::FireSelected(TVector * loc, TVector * vel) {
  if (data[selected]->num <= 0) {
    cerr << "TInventory::FireSelected on selected with 0 items!" << endl;
    /* We always assume that baby missiles are plentifull */
    selected = 0;
  }
  int this_selected = selected;
  /* Baby misilles are different */
  if (0 != selected) {
    data[selected]->num--;
    /* Check if we are running out of this missile type */
    if (data[selected]->num <= 0) {
      SelectPrevious(); /* Could be next .. */
    }
  }
  /* Fire is actually a sort of copy on existing projectiles */
  return data[this_selected]->projectile->Fire(loc, vel);
}

/* **********************************************************************
 * Describes the currently selected missile
 * *********************************************************************/
string TInventory::DescribeSelected() {
  /* Argh, if I only knew some more C++ */
  char buf[256];
  sprintf(buf, "%i", data[selected]->num);
  return (data[selected]->name + " " + buf);
}

/* **********************************************************************
 * Return a pointer to the elements
 * *********************************************************************/
TInventoryElements * TInventory::GetElements() {
  return &data;
}

