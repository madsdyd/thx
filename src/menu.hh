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
  /* Construct and destruct the menu */
  TMenu(string nTitle);
  virtual ~TMenu();
  
  /* Render the menu */
  void Render(int xlow, int xhigh, int ylow, int yhigh);
  /* Enable showing of this menu */
  void Show();
  /* Sometimes we will stop showing ourselves, and show our
     parent */
  void ShowParent();
  /* If we need to show a sub menu, an menuitem will let us know */
  void ShowChild(TMenu * nChild);
  /* Hide this menu */
  void Hide();
  /* Add a TMenuItem to the menu */
  void AddMenuItem(TMenuItem * item);
  /* Add "the" TCancelMenuItem to the menu */
  void AddCancelMenuItem(TMenuItem * item);
  /* This is to get the current menu - which is a hack */
  TMenu * GetCurrentMenu();
  /* Handle commands */
  virtual bool CommandConsume(TCommand * Command);
};
#endif
