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
#include "menu.hh"

#include <GL/glut.h>
#include "sound.hh"

TTextRender MenuTextRender;

/* **********************************************************************
   Create the menu with appropiate init of variables. */
TMenu::TMenu(string nTitle) {
  Title          = nTitle;
  Parent         = NULL;
  Child          = NULL;
  visible        = false;
  focuseditem    = -1;
  cancelitem     = -1;
}

/* **********************************************************************
   Destroy the menu. Deallocate dynamic data */
TMenu::~TMenu() {
  for (unsigned int i = 0; i < menuitems.size(); i++) {
    delete menuitems[i];
  }
}

/* **********************************************************************
   Enable showing of this menu */
void TMenu::Show() {
  if (focuseditem < 0) {
    /* Locate element that can take focus */
    focuseditem = 0;
    while(focuseditem < (int) menuitems.size()) {
      if (menuitems[focuseditem]->Focus()) {
	break;
      }
      focuseditem++;
    }
    if (focuseditem >= (int) menuitems.size()) { /* Should never happen */
      cerr << "TMenu::Show() - no menuitems want focus!" << endl;
      focuseditem = 0;
    }
  }
  visible = true;
}

/* **********************************************************************
   Enable showing and setting parent */
void TMenu::Show(TMenu * nParent) {
  Parent = nParent;
  Show();
}

/* **********************************************************************
   Hide this menu */
void TMenu::Hide() {
  visible = false;
}

/* **********************************************************************
   The render function simpy moves a point, and render all our lines */
void TMenu::Render(int xlow, int xhigh, int ylow, int yhigh) {
  if (visible) {
    /* Render the menu title at the top (offset 2*size), centered in magenta */
    MenuTextRender.color = ColorDefinitions.Colors["magenta"];
    MenuTextRender.size = 24;
    MenuTextRender.PosY(yhigh-2*MenuTextRender.size);
    MenuTextRender.Center(xlow, xhigh, Title);
    
    /* Render a descrition of the focused items description
       at the bottom of the screen in yellow */
    MenuTextRender.color = ColorDefinitions.Colors["yellow"];
    MenuTextRender.size = 14;
    MenuTextRender.PosY(ylow+4*MenuTextRender.size);
    MenuTextRender.Center(xlow, xhigh, menuitems[focuseditem]->GetDescription());
    
    /* Render the menu items */
    /* Set the size for the "normal" menu entries */
    MenuTextRender.size = 18;
    /* We render them between position 120 and the position where the 
       description is.
       The Menuitem updates the MenuTextPosition themselves;
       they prepare for the next menuitem to write by calling println */
    MenuTextRender.PosY(yhigh-120);
    TMenuItemsIterator End = menuitems.end();
    for (TMenuItemsIterator i = menuitems.begin(); i != End; i++) {
      /* The menuitems simply writes as if it was a textterminal,
	 but updates the position in MenuTextRender */
      (*i)->Render(xlow, xhigh);
    }

  } else {
    /* We render a child, if we have one */
    if (Child) {
      Child->Render(xlow, xhigh, ylow, yhigh);
    }
  }
}
/* **********************************************************************
   The keyboard handler sends everything to the focused item. If
   it does not handle it, it checks for "up" and "down". If not handled,
   returns false */
bool TMenu::KeyboardHandler(unsigned char key) {
  if (visible) {
    // printf"TMenu::KeyboardHandler %i\n", key);
    if (!menuitems[focuseditem]->KeyboardHandler(key)) {
      /* Check for up and down */
      switch(key) {
      case 'a': { /* Up */
	if (menuitems[focuseditem]->Blur()) {
	  do {
	    focuseditem = ( focuseditem - 1 + menuitems.size() ) % menuitems.size();
	  } while(!menuitems[focuseditem]->Focus());
#ifdef SOUND_ON
	  sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif
	}
	return true;
      }
      case 'z': { /* Down */
	if (menuitems[focuseditem]->Blur()) {
	  do {
	    focuseditem = (focuseditem + 1) % menuitems.size();
	  } while(!menuitems[focuseditem]->Focus());
#ifdef SOUND_ON
	  sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif
	}
	return true;
      }
      case 27: { /* Escape */
	bool handled = false;
	if (-1 != cancelitem) {
	  /* Send an enter to it */
	  if (menuitems[focuseditem]->Blur()) {
	    /* Try and handle it */
	    if (!(menuitems[cancelitem]->Focus())) {
	      cerr << "TMenu::KeyboardHandler - could not focus CancelMenuItem" 
		   << endl;
	    };
	    /* This is now the focused item */
	    int olditem = focuseditem;
	    focuseditem = cancelitem;
	    handled = menuitems[cancelitem]->KeyboardHandler(10);
	    if (!handled) { /* We only refocus if not handled */
	      if (!(menuitems[cancelitem]->Blur())) {
		cerr << "TMenu::KeyboardHandler - could not Blur CancelMenuItem" << endl;
	      };
	      /* Always try and refocus the old one - should never fail */
	      if (!menuitems[olditem]->Focus()) {
		cerr << "TMenu::KeyboardHandler - could not refocus on cancel" << endl;
	      } else {
		focuseditem = olditem;
	      }
	    }
	    /* Return the result of handling */
	    return handled;
	  }
	}
      }
      default:
	return false;
      }
    }
    /* Was handled by item */
    return true;
  } else { /* We can't handle everything, because we still have
	      focused items */
    if (Child) {
      return Child->KeyboardHandler(key);
    } else {
      return false;
    }
  }
}

/* **********************************************************************
   Add a normal menu item */
void TMenu::AddMenuItem(TMenuItem * item) {
  menuitems.push_back(item);
}

/* **********************************************************************
   Add "the" Cancel menu item */
void TMenu::AddCancelMenuItem(TMenuItem * item) {
  menuitems.push_back(item);
  cancelitem = menuitems.size() - 1;
}

