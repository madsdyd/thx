/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include <GL/glut.h>
#include <sstream>
using std::ostringstream;

#include "debug.hh"

#include "menu.hh"

#include "sound.hh"
#include "command.hh"

#include "display.hh"
#include "text.hh"

#define DEBUG_MENU 0

/* This renders text for the menus */
TTextRender MenuTextRender;

/* This keeps track of the menu that has or lastly had focus */
TMenu * TMenu::CurrentMenu;

/* This keeps track of the mouse position - in display coordinates ... 
   sigh */
#include "inputmouse.hh"
static int mouse_x = 0;
static int mouse_y = 0;

/* **********************************************************************
 * Create the menu with appropiate init of variables. 
 * *********************************************************************/
TMenu::TMenu(string nTitle) {
  Title          = nTitle;
  Parent         = NULL;
  Child          = NULL;
  visible        = false;
  focuseditem    = -1;
  cancelitem     = -1;
  hasscrollarea  = false;
}

/* **********************************************************************
 * Destroy the menu. Deallocate dynamic data
 * *********************************************************************/
TMenu::~TMenu() {
  /* First thing to do is to hide - as this unregisters our commands */
  Hide();
  for (unsigned int i = 0; i < menuitems.size(); i++) {
    delete menuitems[i];
  }
  if (CurrentMenu == this) {
    CurrentMenu = NULL;
  }
}

/* **********************************************************************
 * Enable showing of this menu 
 * *********************************************************************/
void TMenu::Show() {
  // cout << "TMenu::Show - " << Title << endl;
  if (focuseditem < 0) {
    FocusFirst();
  }
  visible = true;
  CurrentMenu = this;
  /* Force possible old focuseditem to focus. */
  menuitems[focuseditem]->Focus();
  /* Register our commands */
  CommandDispatcher.RegisterConsumer("focus-change", this);
  CommandDispatcher.RegisterConsumer("mouse-move", this);
  CommandDispatcher.RegisterConsumer("mouse-down", this);
}

/* **********************************************************************
 * Enable showing and setting parent
 * *********************************************************************/
void TMenu::Show(TMenu * nParent) {
  Parent = nParent;
  Show();
}

/* **********************************************************************
 * Hide this menu
 * *********************************************************************/
void TMenu::Hide() {
  if (!visible) { return; };
  if (focuseditem >= 0) {
    /* Force possible focused item to blur */
    if (!menuitems[focuseditem]->Blur()) {
      cerr << "TMenu::Hide - " << Title << " - unable to blur focuseditem!" << endl;
    }
  }
  visible = false;
  /* This is a kind of testing of the command structure. */
  // cout << "TMenu::Hide - " << Title << " - unregistering commands" << endl;
  CommandDispatcher.UnregisterConsumer("mouse-down");
  CommandDispatcher.UnregisterConsumer("mouse-move");
  CommandDispatcher.UnregisterConsumer("focus-change");
}

/* **********************************************************************
 * If we need to show a sub menu, an menuitem will let us kno
 * *********************************************************************/
void TMenu::ShowChild(TMenu * nChild) {
  // cout << "TMenu::ShowChild - " << Title << " showing " 
  // << nChild->Title << endl;
  Child = nChild;
  Hide();
  Child->Show(this);
};
/* **********************************************************************
 * Our Child will stop showing it selv at some point
 * *********************************************************************/
void TMenu::HideChild(TMenu * oChild) {
  if (oChild != Child) {
    cerr << "TMenu::HideChild, with unknown child" << endl;
  } else {
    /* Redundant */
    Child->Hide();
    /* Needs to be here */
    Show();
  }
}

/* **********************************************************************
 * Functions to handle moving the Focus item
 * *********************************************************************/
/* **********************************************************************
 * FocusFirst - find an element to focus, start from the top
 * *********************************************************************/
void TMenu::FocusFirst() {
  /* Locate element that can take focus */
  focuseditem = 0;
  while(focuseditem < (int) menuitems.size()) {
    if (menuitems[focuseditem]->Focus()) {
      break;
    }
    focuseditem++;
  }
  if (focuseditem >= (int) menuitems.size()) { /* Should never happen */
    cerr << "TMenu::FocusFirst() - no menuitems want focus!" << endl;
    focuseditem = 0;
  }
}

/* **********************************************************************
 * IsHiddenInScrollArea - checks if a given position is hidden in the 
 * scrollarea
 * *********************************************************************/
bool TMenu::IsHiddenInScrollArea(int position) {
  if (hasscrollarea) {
    return ((position > scrollmin && position < scrollpos) 
	    || (position < scrollmax && position >= scrollpos + scrollsize));
  } else {
    return false;
  }
}

/* **********************************************************************
 * FocusMoveUp - tries to move up the focus, while avoiding
 * items in the scroll area that are hidden
 * *********************************************************************/
bool TMenu::FocusMoveUp() {
  int tmp = focuseditem;
  do {
    focuseditem = ( focuseditem - 1 + menuitems.size() ) % menuitems.size();
  } while (IsHiddenInScrollArea(focuseditem) 
	   || !menuitems[focuseditem]->Focus());
  return (tmp != focuseditem);
}

/* **********************************************************************
 * FocusMoveDown - tries to move down the focus, while avoiding
 * items in the scroll area that are hidden
 * *********************************************************************/
bool TMenu::FocusMoveDown() {
  int tmp = focuseditem;
  do {
    focuseditem = (focuseditem + 1) % menuitems.size();
  } while (IsHiddenInScrollArea(focuseditem) 
	   || !menuitems[focuseditem]->Focus());
  return (tmp != focuseditem);
}

/* **********************************************************************
 * Set the menuitem size - called from Render
 * *********************************************************************/
void TMenu::SetItemSize() {
  MenuTextRender.size = Display->GetHeight()/27; /* 480/27 ~= 18 */
}

/* **********************************************************************
 * Sometimes we will stop showing ourselves, and show our parent
 * *********************************************************************/
void TMenu::ShowParent() {
  Hide();
  if (Parent) {
    Parent->HideChild(this);
  }
}

/* **********************************************************************
 * This is to get the current menu, which is kind of a hack
 * *********************************************************************/
TMenu * TMenu::GetCurrentMenu() {
  if (!CurrentMenu) {
    CurrentMenu = this;
  }
  return CurrentMenu;
}

/* **********************************************************************
 * This is to get the current menu - kind of a hack
 * *********************************************************************/

/* **********************************************************************
 * The render function simpy moves a point, and render all our lines
 * *********************************************************************/
void TMenu::Render(int xlow, int xhigh, int ylow, int yhigh) {
  if (visible) {
    /* Render the menu title at the top (offset 2*size), centered in magenta */
    MenuTextRender.color = ColorDefinitions.GetColor("menu-title");
    MenuTextRender.size = Display->GetHeight()/20; /* 480/20 == 24 */
    MenuTextRender.PosY(yhigh-2*MenuTextRender.size);
    MenuTextRender.Center(xlow, xhigh, Title);
    
    /* Render a descrition of the focused items description
       at the bottom of the screen in yellow */
    MenuTextRender.color = ColorDefinitions.GetColor("menu-description");
    MenuTextRender.size = Display->GetHeight()/34;  /* 480/34 ~= 14 */
    MenuTextRender.PosY(ylow+4*MenuTextRender.size);
    MenuTextRender.Center(xlow, xhigh, menuitems[focuseditem]->GetDescription());
    
    /* Render the menu items */
    /* Set the size for the "normal" menu entries */
    SetItemSize();
    /* We render them between position 25 % down and the position where the 
       description is.
       The Menuitem updates the MenuTextPosition themselves;
       they prepare for the next menuitem to write by calling println */
    MenuTextRender.PosY(yhigh-(Display->GetHeight()/4));
    int py = MenuTextRender.PosY()+MenuTextRender.size;
    TMenuItemsIterator End = menuitems.end();
    int count = 0;
    for (TMenuItemsIterator i = menuitems.begin(); i != End; i++) {
      if (IsHiddenInScrollArea(count)) {
	(*i)->SetHitArea(-2, -1, -2, -1);
      } else {
	/* The menuitems simply writes as if it was a textterminal,
	   but updates the position in MenuTextRender */
	(*i)->Render(xlow, xhigh);
	// This is if I had the coordinates I would like to 
	// (*i)->SetHitArea(xlow, xhigh, py, MenuTextRender.PosY());
	(*i)->SetHitArea(xlow, xhigh, MenuTextRender.PosY()+MenuTextRender.size, py);
	py = MenuTextRender.PosY()+MenuTextRender.size;
      }
      count++;
    }
    /* Render the mouse - this is a temporary way to do it, needs to be a texture, 
       or something I reckon */
    MenuTextRender.color = ColorDefinitions.GetColor("white");
    MenuTextRender.PosY(mouse_y - MenuTextRender.size/2);
    MenuTextRender.PosX(mouse_x - MenuTextRender.size/2);
    MenuTextRender.Print("@");

  } else {
    /* We render a child, if we have one */
    if (Child) {
      Child->Render(xlow, xhigh, ylow, yhigh);
    }
  }
}
/* **********************************************************************
 * The command consumer handles commands destined for this menu instance 
 * *********************************************************************/
bool TMenu::CommandConsume(TCommand * Command) {
  /* cout  << "TMenu::CommandConsumer called for command (" 
     << Command->name << "," << Command->args << ")" << endl; */
  if (!visible) {
    cerr << "TMenu::CommandConsume on non-visible menu!" << endl;
    return false;
  }

  /* Handle the commands */
  /* **********************************************************************
   * focus-change - changing focus one step at a time
   * *********************************************************************/
  if ("focus-change" == Command->name) {
    if ("up" == Command->args) {
      if (menuitems[focuseditem]->Blur()) {
	if (FocusMoveUp()) {
#ifdef SOUND_ON
	  sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif
	}
      }
      return true;
    }
    if ("down" == Command->args) {
      if (menuitems[focuseditem]->Blur()) {
	if (FocusMoveDown()) {
#ifdef SOUND_ON
	  sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif
	}
      }
      return true;
    }
    /* Scroll stuff */
    if ("scroll-up" == Command->args) {
      if (hasscrollarea) {
	if (menuitems[focuseditem]->Blur()) {
	  ((TScrollMenuItem *) menuitems[scrollmin])->DoAction();
	  if (IsHiddenInScrollArea(focuseditem)) {
	    FocusMoveUp();
	  }
	  menuitems[focuseditem]->Focus();
	}
      }
      return true;
    }
    if ("scroll-down" == Command->args) {
      if (hasscrollarea) {
	if (menuitems[focuseditem]->Blur()) {
	  ((TScrollMenuItem *) menuitems[scrollmax])->DoAction();
	  if (IsHiddenInScrollArea(focuseditem)) {
	    FocusMoveDown();
	  }
	  menuitems[focuseditem]->Focus();
	}
      }
      return true;
    }
    /* Escape */
    if ("escape" == Command->args) { 
      bool handled = false;
      if (-1 != cancelitem) {
	/* Send an enter to it */
	if (menuitems[focuseditem]->Blur()) {
	  /* Try and handle it */
	  if (!(menuitems[cancelitem]->Focus())) {
	    cerr << "TMenu::CommandConsumer - could not focus CancelMenuItem" 
		 << endl;
	  };
	  /* This is now the focused item */
	  int olditem = focuseditem;
	  focuseditem = cancelitem;
	  /* Construct a temporary command to send to the item */
	  TCommand * tmpcmd = new TCommand(Command->timestamp, "menuitem", "select");
	  handled = menuitems[cancelitem]->CommandConsume(tmpcmd);
	  if (!handled) { /* We only refocus if not handled */
	    if (!(menuitems[cancelitem]->Blur())) {
	      cerr << "TMenu::CommandConsumer - could not Blur CancelMenuItem" << endl;
	    };
	    /* Always try and refocus the old one - should never fail */
	    if (!menuitems[olditem]->Focus()) {
	      cerr << "TMenu::CommandConsume - could not refocus on cancel" << endl;
	    } else {
	      focuseditem = olditem;
	    }
	  }
	  delete tmpcmd;
	}
	return handled;
      }
    }
  } /* focus-change */

  /* **********************************************************************
   * mouse-move - uses the mouse to change focus
   * *********************************************************************/
  else if ("mouse-move" == Command->name) {
    /* First, update the mouse position */
    mouse_x = Command->GetPointerInfo()->x; 
    mouse_y = Command->GetPointerInfo()->y; 
#if (DEBUG_MENU)
    cout << "mouse-move " << mouse_x << ", " << mouse_y << endl;
#endif    
    /* Now, check if any hits... */
    TMenuItemsIterator End = menuitems.end();
    int count = 0;
    for (TMenuItemsIterator i = menuitems.begin(); i != End; i++) {
      if ((*i)->TestHit(mouse_x, mouse_y)) {
	/* Got a hit - check if this is different from the focuseditem */
	// cout << "Got a hit on " << (*i)->GetDescription() << endl;
	if (count != focuseditem) {
	  /* This is not the focuseditem - we should try to change focus */
#if (DEBUG_MENU)
	  cout << "mouse-move : this is not the focused item" << endl;
#endif
	  if (menuitems[focuseditem]->Blur()) {
#if (DEBUG_MENU)
	    cout << "mouse-move : blurred focused item" << endl;
#endif
	    if ((*i)->Focus()) {
#if (DEBUG_MENU)
	      cout << "mouse-move : focused item under mouse" << endl;
#endif
	      focuseditem = count;
#ifdef SOUND_ON
	      sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif
	    } else {
	      /* Ups, refocus */
#if (DEBUG_MENU)
	      cout << "mouse-move : could not focus new, refocus old" << endl;
#endif
	      menuitems[focuseditem]->Focus();
	    }
	  }
	}
      } else {
	// cout << "No hit for " << (*i)->GetDescription() << endl;
      }
      count++;
    }
    /* We handled this command */
    return true;
  } /* mouse-move */

  /* **********************************************************************
   * mouse-down - uses the mouse to enter selected state on menuitems
   * *********************************************************************/
  else if ("mouse-down" == Command->name) {
    /* The funny thing about a mouse-down is that it is like a
       mouse-move to this position, and then a down. If another item
       is in editing, however, we want this state to be left */
    /* First, update the mouse position */
    mouse_x = Command->GetPointerInfo()->x; 
    mouse_y = Command->GetPointerInfo()->y; 
#if (DEBUG_MENU)
    cout << "mouse-down " << mouse_x << ", " << mouse_y << endl;
#endif
    /* Now, check if any hits... */
    TMenuItemsIterator End = menuitems.end();
    int count = 0;
    for (TMenuItemsIterator i = menuitems.begin(); i != End; i++) {
      if ((*i)->TestHit(mouse_x, mouse_y)) {
	/* Got a hit - Cancel the current */
	if (focuseditem == count && (*i)->Selected()) {
	  /* Ignore this, the user clicked a selected entry */
#if (DEBUG_MENU)
	  cout << "mouse-down : ignoring event - item already selected" << endl;
#endif
	  return true;
	} else {
	  if (focuseditem != count) {
	    /* Have to make sure the old one is canceled, this one fucosed */
#if (DEBUG_MENU)
	    cout << "mouse-down : trying to force focus change" << endl
		 << "mouse-down : canceling current" << endl;
#endif
	    menuitems[focuseditem]->Cancel();
	    if (!(*i)->Focus()) {
	      /* Uh, oh, we can not focus this one. Refocus the old one */
#if (DEBUG_MENU)
	      cout << "mouse-down : item under mouse did not want focus. Refocusing" 
		   << endl;
#endif
	      menuitems[focuseditem]->Focus();
	      return true;
	    } else {
#if (DEBUG_MENU)
	      cout << "mouse-down : succesfully focused item under mouse" << endl;
#endif
	      focuseditem = count;
	    }
	  } /* Check or set focus to the one the mouse is above */

	  /* At this point, the item the mouse is above is focused. */
	  /* Now try to select it with a temporary command */
	  TCommand * tmpcmd = new TCommand(Command->timestamp, "menuitem", "select");
	  if ((*i)->CommandConsume(tmpcmd)) {
#if (DEBUG_MENU)
	    cout << "mouse-down : succesfully selected item under mouse" << endl;
#endif
	  } else {
	    /* It did not accept the command */
	    /* Make a focus sound */
#ifdef SOUND_ON
	    sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif
	    // This is most likely an error?
	    cerr << "TMenu, mouse command not accepted" << endl;
	  }
	  delete tmpcmd;
	  return true;
	} /* User clicked on something else */
      } /* Testhit */
      count++;
    }
    /* We handled this command anyway */
    return true;
  } /* mouse-down */

  /* **********************************************************************
   * Unhandled commands
   * *********************************************************************/

  /* If we reach here, we did not handle the command */
  cerr << "TMenu::CommandConsumer, not handling ("
       << Command->name << "," << Command->args << ")" << endl;
  return false;
}

/* **********************************************************************
 * Adding stuff to the menu
 * *********************************************************************/

/* **********************************************************************
 * Add a normal menu item
 * *********************************************************************/
void TMenu::AddMenuItem(TMenuItem * item) {
  menuitems.push_back(item);
}

/* **********************************************************************
 * Add "the" Cancel menu item
 * *********************************************************************/
void TMenu::AddCancelMenuItem(TMenuItem * item) {
  AddMenuItem(item);
  cancelitem = menuitems.size() - 1;
}

/* **********************************************************************
 * Controlling the scroll area
 * *********************************************************************/

/* **********************************************************************
 * OpenScrollArea - this adds the item, and stores its number scrollmin
 * *********************************************************************/
void TMenu::OpenScrollArea(TMenuItem * item) {
  AddMenuItem(item);
  scrollmin = menuitems.size() - 1;
}

/* **********************************************************************
 * CloseScrollArea - adds the item, sets the size, and stores the max 
 * *********************************************************************/
void TMenu::CloseScrollArea(int size, TMenuItem * item) {
  AddMenuItem(item);
  scrollsize = size;
  scrollmax  = menuitems.size() - 1;
  Assert(scrollmin < scrollmax, 
	 "TMenu::CloseScrollArea - scrollmin >= scrollmax");
  Assert((scrollmax - scrollmin) > scrollsize, 
	 "TMenu::ClosescrollArea - size to large for number of items regged");
  scrollpos = scrollmin + 1;
  hasscrollarea = true;  
}

/* **********************************************************************
 * Scroll up. Decrease scrollpos, if room.
 * *********************************************************************/
bool TMenu::ScrollUp(bool playsound) {
  Assert(hasscrollarea, "TMenu::ScrollUp - no scrollarea");
  if (scrollpos > (scrollmin + 1)) {
    scrollpos--;
#ifdef SOUND_ON
    if (playsound) {
      sound_play(names_to_nums[THX_DATADIR "/sounds/menu_deselect.raw"]);      
    }
#endif
    return true;
  }
  return false;
}

/* **********************************************************************
 * Scroll down. Increase scrollpos, if room.
 * *********************************************************************/
bool TMenu::ScrollDown(bool playsound) {
  Assert(hasscrollarea, "TMenu::ScrollDown - no scrollarea");
  if (scrollpos < (scrollmax - scrollsize)) {
    scrollpos++;
#ifdef SOUND_ON
    if (playsound) {
      sound_play(names_to_nums[THX_DATADIR "/sounds/menu_deselect.raw"]);      
    }
#endif
    return true;
  }
  return false;
}

