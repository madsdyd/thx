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
#include "menu_buy.hh"

#include <iomanip> /* for setw ... sic */
using std::setw;

#include <sstream>

using std::ostringstream;
using std::ends;

#include "menuitem.hh"

/* This is our global BuyMenu */
TBuyMenu * BuyMenu = NULL;

/* **********************************************************************
   TBuyMenu uses a speciel menuitem class that is almost like a 
   TActionMenuItem */
class TBuyMenuItem : public TSimpleActionMenuItem {
private:
  TInventoryElement * element;
  TAction function;
  int * money;
public:
  /* Set the caption */
  void SetCaption() {
    /* Num  -  quantity @ price */
    ostringstream tmp;
    tmp << setw(4) << element->num << " - " << setw(5) << element->price
	<< "/" << setw(2) << element->units << setw(0) << " : " 
	<< element->name << ends;
    caption = tmp.str();
  };
  TBuyMenuItem(TMenu * owner, string desc, int * nmoney,
	       TAction nbuyaction, TInventoryElement * nelement) 
    : TSimpleActionMenuItem(owner, "", desc) {
    money    = nmoney;
    element  = nelement;
    function = nbuyaction;
    SetCaption();
  };
  virtual void DoAction() {
    if (function && element) {
      /* But it */
      element->Buy(money);
      /* Force an update */
      function();
    }
  }
  /* Budget adjust the state of the menu item */
  void Budget() {
    Blur();
    SetCaption();
    if (*money < element->price) {
      Disable();
    } else {
      Enable();
    }
  }
  void Render(int xlow, int xhigh) {
    /* Sets the color, then renders the caption centered with linechange */
    SetRenderColor();
    MenuTextRender.CenterLn(xlow, xhigh, caption, ":");
  }
};

/* **********************************************************************
   The BuyMenu constructor */
TBuyMenu::TBuyMenu(string title, TInventory * inventory, int * money,
		   TAction CancelFunc, TAction BudgetFunc) 
  : TMenu(title) {
  /* Create a lot of TBuyMenuItems */
  TInventoryElements * tmp = inventory->GetElements();
  TInventoryElementsIterator End = tmp->end();
  for (TInventoryElementsIterator i = tmp->begin(); i != End; i++) {
    AddMenuItem(new TBuyMenuItem(this, "Buy this package", money, 
				 BudgetFunc, (*i)));
  }  
  /* Remember to fix budget, if you ever add menuitems here! */
  AddMenuItem(new TSpaceMenuItem(this));
  AddCancelMenuItem(new TActionMenuItem(this, "Continue", 
					"Stop buying for this player", 
					CancelFunc));
  /* Do a Budget... */
  Budget(Title);
}

/* **********************************************************************
   The Budget function simply adjust the state of each TBuyMenuItem */
void TBuyMenu::Budget(string ntitle) {
  /* Set the new caption */
  Title = ntitle;
  /* Update all the menuitems, except the last two. (This is ugly) - very
     ugly*/
  TMenuItemsIterator End = menuitems.end();
  End--; End--;
  for (TMenuItemsIterator i = menuitems.begin(); i != End; i++) {
    ((TBuyMenuItem *)(*i))->Budget();
  };
  /* Find an entry that can be selected 
     The entry is blurred, maybe it will focus. */
  if (focuseditem >= 0) {
    if (!menuitems[focuseditem]->Focus()) {
      do {
	focuseditem = ( focuseditem - 1 + menuitems.size() ) % menuitems.size();
      } while(!menuitems[focuseditem]->Focus());
    }
  }
}
