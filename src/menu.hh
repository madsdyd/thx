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
#ifndef __MENU_HH_
#define __MENU_HH_

/* This module is a class that can display menus on the display.
   TMenu is not especially intelligent. New menus can be made by inheriting.
   Relations between the menus need to be set up at some later point. */

#include "menuitem.hh"
#include "commandconsumer.hh"
#include "text.hh"

/* This renders text for the menus - also used by menu items */
extern TTextRender MenuTextRender;

class TMenu : public TCommandConsumer{
protected:
  string Title;
  /* The menuitems */
  TMenuItems menuitems;
  /* The number of the menuitem with focus. */
  int focuseditem;
  /* This is to allow for scrolling a (single) section of the menu items 
     up and down. The fragility is starting to show.... 
     The arrow up and down items are always shown. Only a limited 
     number of items between these are rendered.
     Clicking the up arrow should allow an entry hidden at the top 
     to be shown, and hide an entry at the bottom 
     Clicking the down arrow should do the opposite */
  bool hasscrollarea; 
  int scrollmin;  /* The number of the arrow up item */
  int scrollmax;  /* The number of the arrow down item */
  int scrollsize; /* The number of visible items in the scroll area */
  int scrollpos;  /* The number of the lowest item shown in the scroll
		    area, assert 
		    scrollpos > scrollmin, 
		    scrollpos <= (scrollmax - scrollsize) */
  /* New methods to encapsulate focus move logic */
  void FocusFirst();    /* Finds an item to focus initially */
  bool FocusMoveUp();   /* Attempts to move the focus "up" */
  bool FocusMoveDown(); /* Attempts to move the focus "down" */
  bool IsHiddenInScrollArea(int position);
  /* Wheter or not the menu is visible */
  bool visible;
  /* The Menu that needs to know, when this menu looses focus.
     This entry is updated with the ShowParent method */
  TMenu * Parent;
  /* If we have a child, then all handling and rendering goes 
     to it. Updated with the ShowChild method */
  TMenu * Child;
  /* If we have a "return" or "cancel" menu item, then esc may go to this */
  int cancelitem;
  /* This is a hack, because the way I do menus are borken */
  static TMenu * CurrentMenu;
  /* Enable showing and setting parent */
  void Show(TMenu * nParent);
  /* Our Child will stop showing it selv at some point */
  void HideChild(TMenu * oChild); 
  /* Set the menuitem size. Used when we do not want to override
     all of render ... */
  virtual void SetItemSize();
public:
  /* **********************************************************************
   * Construct and destroy the menu
   * *********************************************************************/
  TMenu(string nTitle);
  virtual ~TMenu();

  /* **********************************************************************
   * Control display of the menu
   * *********************************************************************/
  /* Render the menu */
  void Render(int xlow, int xhigh, int ylow, int yhigh);
  /* Enable showing of this menu */
  virtual void Show();
  /* Sometimes we will stop showing ourselves, and show our
     parent */
  void ShowParent();
  /* If we need to show a sub menu, an menuitem will let us know */
  void ShowChild(TMenu * nChild);
  /* Hide this menu */
  void Hide();
  
  /* **********************************************************************
   * Add "normal" items
   * *********************************************************************/
  /* Add a TMenuItem to the menu */
  void AddMenuItem(TMenuItem * item);
  /* Add "the" TCancelMenuItem to the menu */
  void AddCancelMenuItem(TMenuItem * item);

  /* **********************************************************************
   * Support for the scroll area 
   * NOTE: Almost no checking is performed - use at own risk
   * *********************************************************************/
  /* Open adds the topmost menuitem */
  void OpenScrollArea(TMenuItem * item);
  /* Close add the bottom menuitem, and indicates the number of visible 
     items */
  void CloseScrollArea(int size, TMenuItem * item);
  /* Move the scroll area, so the visible items goes the opposite direction */  
  bool ScrollUp(bool playsound); 
  bool ScrollDown(bool playsound);

  /* This is to get the current menu - which is a hack */
  TMenu * GetCurrentMenu();
  /* Handle commands */
  virtual bool CommandConsume(TCommand * Command);
};
#endif
