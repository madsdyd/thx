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

class TInventory {
private:
  TInventoryElements data;
  int selected;
  TPlayer * owner;
public:
  TInventory(TPlayer * Owner);
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
