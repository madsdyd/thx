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
#ifndef __INVENTORY_HH_
#define __INVENTORY_HH_
/* This is an inventory that each player can have. It is also used to
   construct instances of projectiles with. */
#include <vector>
#include <string>
#include "vector.hh"

/* Forward declare player and projectile */
class TPlayer;
class TProjectile;

class TInventoryElement {
public:
  string name;
  TProjectile * projectile;
  int units; /* Number of units in package */
  int price; /* Price pr. package */
  int num;   /* Number in player storage */
  TInventoryElement(string nname, TProjectile * nprojectile,
		    int nunits, int nprice, int nnum);
  void Buy(int * money);
  string MenuLine();
};

typedef vector<TInventoryElement *> TInventoryElements;
typedef TInventoryElements::iterator TInventoryElementsIterator;

class TGame;
class TInventory {
private:
  TInventoryElements data;
  int selected;
public:
  TInventory(TGame * Game, TPlayer * Owner);
  ~TInventory();
  /* Selection routines - only selects projectiles that the player
     actually have*/
  void SelectFirst();
  void SelectNext();
  void SelectPrevious();
  /* Instantiate a selected missile */
  TProjectile * FireSelected(TVector * loc, TVector * vel);
  /* Returns the name of the currently selected element */
  string DescribeSelected();
  /* Get access to the elements - used to fill menus and buy stuff */
  TInventoryElements * GetElements();
};

#endif
