#ifndef __MENU_HH_
#define __MENU_HH_

/* This module is a class that can display menus on the display.
   TMenu is not especially intelligent. New menus can be made by inheriting.
   Relations between the menus need to be set up at some later point. */

#include "menuitem.hh"

#include "text.hh"
extern TTextRender MenuTextRender;

class TMenu {
protected:
  string Title;
  /* The menuitems */
  TMenuItems menuitems;
  /* The number of the menuitem with focus.
     Keyboard events go to this item */
  int focuseditem;
  bool visible;
  /* The Menu that needs to know, when this menu looses focus.
     This entry is updated with the ShowParent method */
  TMenu * Parent;
  /* If we have a child, then all handling and rendering goes 
     to it. Updated with the ShowChild method */
  TMenu * Child;
  /* If we have a "return" or "cancel" menu item, then esc may go to this */
  int cancelitem;
public:
  /* Construct and destruct the menu */
  TMenu(string nTitle);
  virtual ~TMenu();
  
  /* Render the menu */
  void Render(int xlow, int xhigh, int ylow, int yhigh);

  /* Enable showing of this menu */
  void Show();
  /* Enable showing and setting parent */
  void Show(TMenu * nParent);
  /* Hide this menu */
  void Hide();

  /* Handle the keyboard presses */
  bool KeyboardHandler(unsigned char key);

  /* If we need to show a sub menu, an menuitem will let us know */
  void ShowChild(TMenu * nChild) {
    Child = nChild;
    Hide();
    Child->Show(this);
  };
  /* Our Child will stop showing it selv at some point */
  void HideChild(TMenu * oChild) {
    if (oChild != Child) {
      cerr << "TMenu::HideChild, with unknown child" << endl;
    } else {
      /* Redundant */
      Child->Hide();
      /* Needs to be here */
      Show();
    }
  }
  /* Sometimes we will stop showing ourselves, and show our
     parent */
  void ShowParent() {
    Hide();
    if (Parent) {
      Parent->HideChild(this);
    }
  }
  /* Add a TMenuItem to the menu */
  void AddMenuItem(TMenuItem * item);
  /* Add "the" TCancelMenuItem to the menu */
  void AddCancelMenuItem(TMenuItem * item);
};
#endif
