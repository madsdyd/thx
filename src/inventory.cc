#include "inventory.hh"

#include <strstream>

#include "projectile.hh"

/* For sprintf - sigh */
#include <stdio.h>

/* **********************************************************************
   Implementation of stuff that has to do with the inventory */
TInventoryElement::TInventoryElement(string nname, TProjectile * nprojectile,
				     int nunits, int nprice, int nnum) {
  name       = nname;
  projectile = nprojectile;
  units      = nunits;
  price      = nprice;
  num        = nnum;
}

/* **********************************************************************
   Buy reduces the money with the appropiate amount and adjust quantity */
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
   Constructor and destructor */
TInventory::TInventory(TPlayer * Owner) {
  owner    = Owner;
  /* This is where the projectile types that are available to the players
     are created. Every inventory gets initialized the same way. */
  data.push_back(new TInventoryElement("Baby Missile", 
				       new TProjectile(owner, 2.0, 40.0),     
				       10, 400, 99));
#define NUM_PROJ_TEST 0
  data.push_back(new TInventoryElement("Missile", 
				       new TProjectile(owner, 4.0, 45.0),     
				       5, 1875, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Baby Nuke", 
				       new TProjectile(owner, 6.0, 50.0),     
				       3, 10000, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Nuke", 
				       new TProjectile(owner, 8.0, 55.0),     
				       1, 12000, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("Funky Bomb", 
				       new TSpawnProjectile(owner, 4.0, 100.0),
				       2, 7000, NUM_PROJ_TEST));
  data.push_back(new TInventoryElement("MIRV",  
				       new TMirvProjectile(owner, 4.0, 100.0),  
				       3, 10000, NUM_PROJ_TEST));
  selected = 0; /* Always assume that baby missiles are present */
}

TInventory::~TInventory() {
  cout << "~TInventory() - have to delete stuff here" << endl;
}

/* **********************************************************************
   The selection methods */
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
   The fire method (kaboom) */
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
   Describes the currently selected missile */
string TInventory::DescribeSelected() {
  ostrstream tmp;
  tmp << data[selected]->name;
  tmp.form(" %i", data[selected]->num) << ends;
  return tmp.str();
}

/* **********************************************************************
   Return a pointer to the elements */
TInventoryElements * TInventory::GetElements() {
  return &data;
}

