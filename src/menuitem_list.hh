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
#ifndef __MENUITEM_LIST_HH__
#define __MENUITEM_LIST_HH__

#include <iostream>
using std::cout;
using std::endl;

#include "menuitem.hh"



/* **********************************************************************
 * TListMenuItem
 * **********************************************************************
 This menuitem can select a value from a list. */
template <typename value_t> 
class TListMenuItem : public TValueMenuItem {
protected:
  class storage_t {
  public:
    string caption;
    value_t value;
    storage_t(string ncap, value_t nval) {
      caption = ncap;
      value   = nval;
    }
  };
  std::vector<storage_t> options;
  int selected_value;
  value_t * storage; /* the place where the value can be read */
  virtual void RegisterCommands();
  virtual void UnregisterCommands();
public:
  TListMenuItem(TMenu * owner, string cap, string desc, value_t * store, 
		TMenu * ntarget = NULL, int ncode = -1)
    : TValueMenuItem(owner, cap, desc, ntarget, ncode) {
    selected_value = -1;
    storage        = store;
  };
  ~TListMenuItem();
  virtual bool CommandConsume(TCommand * Command);
  /* Add a value to be shown to the list */
  void AddOption(string ncaption, value_t nvalue);
  /* Renders both the menu entry and value */
  virtual void Render(int xlow, int xhigh);
};

typedef TListMenuItem<int> TIntListMenuItem;
typedef TListMenuItem<unsigned int> TUIntListMenuItem;
typedef TListMenuItem<double> TDoubleListMenuItem;
typedef TListMenuItem<bool> TBoolListMenuItem;
typedef TListMenuItem<string> TStringListMenuItem;

/* **********************************************************************
 * The TListMenuItem stuff - need to go here, because it uses templates
 * *********************************************************************/

/* **********************************************************************
 * Desctructor
 * *********************************************************************/
template <typename value_t>
TListMenuItem <value_t> ::~TListMenuItem() {
  cout << "~TListMenuItem - Gotta clean up some stuff" << endl;
}

/* **********************************************************************
 * Add a value
 * **********************************************************************/
template <typename value_t>
void TListMenuItem <value_t> ::AddOption(string ncaption, 
					 value_t nvalue) {
  /* Defaults to first added value is selected */
  options.push_back(storage_t(ncaption, nvalue));
  if (nvalue == *storage) {
    selected_value = options.size() - 1;
  } else {
    /* If not matched and first value */
    if (1 == options.size()) {
      selected_value = 0;
    }
  }
};
/* **********************************************************************
 * Register and unregister commands.
 * **********************************************************************/
template <typename value_t>
void TListMenuItem <value_t>::RegisterCommands() {
  TValueMenuItem::RegisterCommands();


  CommandDispatcher.RegisterConsumer("list", this);
};

template <typename value_t>
void TListMenuItem <value_t>::UnregisterCommands() {
  TValueMenuItem::UnregisterCommands();
  CommandDispatcher.UnregisterConsumer("list");
};

/* **********************************************************************
 * TListMenuItem::CommandConsume
 * This handles the list up and down commands, but rely on
 * TValueMenuItem to handle the rest of the commands
 * *********************************************************************/
template <typename value_t>
bool TListMenuItem <value_t>::CommandConsume(TCommand * Command) {
#ifdef OLDLISTMENUITEMSELECTSTUFF
  if (state == menuitem_state_selected) {
    if ("list" == Command->name) {
      /* Handle up and down */
      if ("previous" == Command->args) {
	selected_value 
	  = (selected_value - 1 + options.size()) % options.size();
	*storage = options[selected_value].value;
#ifdef SOUND_ON
	sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif    
	return true;
      }
      if ("next" == Command->args) {
	selected_value = (selected_value + 1) % options.size();
	*storage = options[selected_value].value;
#ifdef SOUND_ON
	sound_play(names_to_nums[THX_DATADIR "/sounds/menu_move.raw"]);
#endif    
	return true;
      }
    }
  }
#endif
  /* Handle (menuitem,select) as "down" in liste */
  if (menuitem_state_focused == state && 
      "menuitem" == Command->name  &&
      "select" == Command->args) {
    selected_value = (selected_value + 1) % options.size();
    *storage = options[selected_value].value;
#ifdef SOUND_ON
    sound_play(names_to_nums[THX_DATADIR "/sounds/menu_select.raw"]);
#endif
    /* Remember to call any registered function */
    if (NotifyTarget) {
      NotifyTarget->MenuItemChange(code);
    }
    return true;
  }

  /* Errors and unhandled fall to here */
  return TValueMenuItem::CommandConsume(Command);
}

/* **********************************************************************
 * The render for TListMenuItem render both the string and the value
 * *********************************************************************/
template <typename value_t>
void TListMenuItem<value_t>::Render(int xlow, int xhigh) {
  /* Sets the render color */
  SetRenderColor(); 
  /* Render everything */
  MenuTextRender.CenterLn(xlow, xhigh, 
			  caption + ":" + options[selected_value].caption,
			  ":");
}

#endif
