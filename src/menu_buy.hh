#ifndef __MENU_BUY_HH__
#define __MENU_BUY_HH__

/* This module defines the menu interface for buying stuff.
   Each time an element is bought, one should call the budget
   method
*/

#include "menu.hh"

#include "inventory.hh"

// typedef void (*TBuyAction)(TInventoryElement * nelement); 

class TBuyMenu : public TMenu {
public:
  TBuyMenu(string title, TInventory * inventory, int * money,
	   TAction CancelFunc, TAction BudgetFunc);
  /* Updates menuitems */
  void Budget(string ntitle);
};

extern TBuyMenu * BuyMenu;
#endif
