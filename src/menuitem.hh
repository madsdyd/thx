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
#ifndef __MENUITEM_HH__
#define __MENUITEM_HH__

#include <string>
#include <vector>
#include "commandconsumer.hh"
#include "gamemode.hh" 
/* This declares items that can be part of a menu */
typedef enum {menuitem_state_disabled, /* This menuitem wont accept focus */
	      menuitem_state_blurred,  /* Is blurred, can take focus */
	      menuitem_state_focused,  /* Has focus */
	      menuitem_state_selected  /* Interaction is happening with this item */
} menuitem_state_t;

/* **********************************************************************
 * TMenuItem
 * **********************************************************************
 This is a menuitem that serves as a basis for value and submenu menu
 items */
class TMenu;
class TMenuItem : public TCommandConsumer {
protected:
  menuitem_state_t state;
  string caption;
  string description;
  TMenu * Owner;
  /* Sets the render color based on the state */
  virtual void SetRenderColor();
  /* Maintain a hit area */
  int xl, xh, yl, yh;
public:
  TMenuItem(TMenu * owner, string cap, string desc);
  virtual ~TMenuItem () {};

  /* Render will call SetRenderColor then render the caption */
  virtual void Render(int xlow, int xhigh);

  /* GetDescription simply return the description for the caller to use */
  string GetDescription() {
    return description;
  };
  
  /* Enable and disable the menuitem */
  virtual bool Enable();
  virtual bool Disable();

  /* Focus and Blur */
  /* Focus let the menu know that it has focus. It will only handle
     keypresses and other events in focused state. If it wont accept
     focus, false will be returned, otherwise true */
  virtual bool Focus();
  /* If the menu item is editing, it will not allow blur */
  virtual bool Blur();
  /* Even if the menu item is editing, it must allow a cancel
     Wheter any changes are accepted or not is up to the item. */
  virtual void Cancel();
  /* Let the caller know if it is selected */
  virtual bool Selected();

  /* Hit test - hack upon hack */
  void SetHitArea(int nxl, int nxh, int nyl, int nyh);
  bool TestHit(int x, int y);
};
  
typedef std::vector<TMenuItem *> TMenuItems;
typedef TMenuItems::iterator TMenuItemsIterator;           

/* **********************************************************************
 * TSpaceMenuItem - Renders a blank line 
 * *********************************************************************/
class TSpaceMenuItem : public TMenuItem {
public:
  TSpaceMenuItem(TMenu * owner) :
    TMenuItem(owner, "", "") {
    Disable();
  }
  TSpaceMenuItem(TMenu * owner, string cap, string desc) :
    TMenuItem(owner, cap, desc) {
    Disable();
  };
  virtual bool CommandConsume(TCommand * Command) { return false; };
  virtual void Render(int xlow, int xhigh);
};

/* **********************************************************************
 * TInfoMenuItem
 * **********************************************************************
 This menuitem can show some text */
class TInfoMenuItem : public TSpaceMenuItem {
protected:
  std::vector<string> lines;
public:
  TInfoMenuItem(TMenu * owner, string cap, string desc) :
    TSpaceMenuItem(owner, cap, desc) { };
  /* Renders both the menu entry and value */
  virtual void Render(int xlow, int xhigh);
  virtual void AddLine(string line);
};

/* **********************************************************************
 * TSimpleActionMenuItem
 * **********************************************************************
 This menuitem is a base item for some very similar command handling
 code. If it receives a (menuitem, select) command, it calls DoAction */
class TSimpleActionMenuItem : public TMenuItem {
protected:
  virtual void DoAction() = 0;
public:
  TSimpleActionMenuItem(TMenu * owner, string cap, string desc) :
    TMenuItem(owner, cap, desc) {};
  virtual bool CommandConsume(TCommand * Command);
};

/* **********************************************************************
 * TSubMenuItem
 * **********************************************************************
 This menuitem can show a sub menu */
class TSubMenuItem : public TSimpleActionMenuItem {
protected:
  TMenu * SubMenu;
  virtual void DoAction();
public:
  TSubMenuItem(TMenu * owner, string cap, string desc, TMenu * subMenu) :
    TSimpleActionMenuItem(owner, cap, desc) {
    SubMenu = subMenu;
  }
};

/* **********************************************************************
 * TReturnMenuItem
 * **********************************************************************
 This menuitem can return from a sub menu */
class TReturnMenuItem : public TSimpleActionMenuItem {
protected:
  virtual void DoAction();
public:
  TReturnMenuItem(TMenu * owner, string cap, string desc) 
    : TSimpleActionMenuItem(owner, cap, desc) {};
};

/* **********************************************************************
 * TActionMenuItem
 * **********************************************************************
   This menuitem performs an action and hides the menu */
typedef void (*TAction)(void);

class TActionMenuItem : public TSimpleActionMenuItem {
protected:
  virtual void DoAction();
private:
  void (* function) (void);
public:
  TActionMenuItem(TMenu * owner, string cap, string desc, TAction func) 
    : TSimpleActionMenuItem(owner, cap, desc) {
    function = func;
  };
};

/* **********************************************************************
 * TValueMenuItem
 * **********************************************************************
   This is a menuitem that can serve as a basis for menuitems that can
   handle values. The menuitem is assumed to have some sort of
   internal state, that it "copies", and let the user "edit". When the
   user hits enter, the new state is accepted. Should the user hit
   esc, the new state is discarded. */

class TValueMenuItem : public TMenuItem {
protected:
  /* Used to store the mode in, when we go into editing. 
     This will probably always be menu, but better safe than sorry */
  gamemode_t StoredMode;
  /* This is called when we leave focus state and go into editing
     state..  Returns true if we did enter the state, false otherwise
     Will also change the game mode, and call RegisterCommand */
  virtual bool EnterEditState();
  /* Leave edit state, will change the game mode, and
     UnregisterCommand */
  virtual bool LeaveEditState();
  /* During editing, validate can be called and will return accept or
     reject on the current user entered (new) value */
  virtual bool ValidateNewValue();
  /* Will value the value, and possible accept it */
  virtual bool AcceptNewValue();
  virtual void DiscardNewValue();
  /* This handles registering and unregistering commands that are
     handled by this class.  Typically called when editing/selected
     mode is entered/left. */
  virtual bool RegisterCommands();
  virtual bool UnregisterCommands();

public:
  TValueMenuItem(TMenu * owner, string cap, string desc) 
    : TMenuItem(owner, cap, desc) {};  
  /* Special cancel for this class */
  virtual void Cancel();
  /* Returns true if the key has been handled, false otherwise */
  virtual bool CommandConsume(TCommand * Command); 
};
  
/* **********************************************************************
 * TListMenuItem
 * **********************************************************************
 This menuitem can select a value from a list. */
class TListMenuItem : public TValueMenuItem {
private:
  std::vector<string> values;
  int selected_value;
  string * storage; /* the place where the value can be read */
protected:
  virtual bool RegisterCommands();
  virtual bool UnregisterCommands();
public:
  TListMenuItem(TMenu * owner, string cap, string desc, string * store) 
    : TValueMenuItem(owner, cap, desc) {
    selected_value = -1;
    storage = store;
  };
  virtual bool CommandConsume(TCommand * Command);

  /* Add a value to be shown to the list */
  void AddValue(string nvalue);
  /* Renders both the menu entry and value */
  virtual void Render(int xlow, int xhigh);
};

/* **********************************************************************
 * TStringMenuItem
 * **********************************************************************
   This TMenuItem can edit a string.
   This could probably be done a lot smarter with templates, but... */
class TStringMenuItem : public TValueMenuItem {
protected:
  virtual bool RegisterCommands();
  virtual bool UnregisterCommands();
private:
  unsigned int cursorpos;
  string new_value;
  string * value;
  static string killed;
  virtual bool EnterEditState();
  virtual bool AcceptNewValue();
public:
  TStringMenuItem(TMenu * owner, string cap, string desc, string * vdefault) 
    : TValueMenuItem(owner, cap, desc) {
    value = vdefault;
    cursorpos = vdefault->size();
  };
  /* Renders both the menu entry and value */
  virtual void Render(int xlow, int xhigh);
  /* Returns true if the key has been handled, false otherwise */
  virtual bool CommandConsume(TCommand * Command);
};
#endif
