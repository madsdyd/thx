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
#include "types.hh"
#include "debug.hh"

#include "menuitem.hh"
#include "menu.hh"
#include "sound.hh"
#include "command.hh"

#define DEBUG_MENUITEM 0
/* **********************************************************************
 * **********************************************************************
 *  TMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * TMenuItem constructor 
 * *********************************************************************/
TMenuItem::TMenuItem(TMenu * owner, string cap, string desc) {
  Owner       = owner;
  state       = menuitem_state_blurred;
  caption     = cap;
  description = desc;
  /* No hit */
  xl          = 1;
  xh          = 0;
  yl          = 1;
  yh          = 0;
}
/* **********************************************************************
 * TMenuItem Enable and Disable 
 * *********************************************************************/
bool TMenuItem::Enable() {
#if (DEBUG_MENUITEM)
  cout << "TMenuItem::Enable() - " << caption << endl;
#endif
  if (menuitem_state_disabled == state) {
    state = menuitem_state_blurred;
    return true;
  } else {
    return false;
  }
};

bool TMenuItem::Disable() {
#if (DEBUG_MENUITEM)
  cout << "TMenuItem::Disable() - " << caption << endl;
#endif
  if (menuitem_state_blurred == state) {
    state = menuitem_state_disabled;
    return true;
  } else {
    return false;
  }
};
  
/* **********************************************************************
 * TMenuItem Focus, Blur, Cancel, Selected
 * *********************************************************************/
bool TMenuItem::Focus() {
#if (DEBUG_MENUITEM)
  cout << "TMenuItem::Focus() - " << caption << endl;
#endif
  if (menuitem_state_blurred == state) {
    state = menuitem_state_focused;
    /* Register commands that the menuitem will handle */
    CommandDispatcher.RegisterConsumer("menuitem", this);
    return true;
  } else {
    return false;
  }
};
/* If we are in state editing, we wont allow disfocus */
bool TMenuItem::Blur() {
#if (DEBUG_MENUITEM)
  cout << "TMenuItem::Blur() - " << caption << endl;
#endif
  /* We wont blur in selected */
  if (menuitem_state_selected == state) {
    return false;
  }
  /* If we are disabled, we ok the blur, but do not change our state */
  if (menuitem_state_disabled != state) {
    state = menuitem_state_blurred;
    CommandDispatcher.UnregisterConsumer("menuitem");
  }
  return true;
};

void TMenuItem::Cancel() {
#if (DEBUG_MENUITEM)
  cout << "TMenuItem::Cancel - " << caption << endl;
#endif
  if (menuitem_state_disabled != state) {
    state = menuitem_state_blurred;
    CommandDispatcher.UnregisterConsumer("menuitem");
  }
}

bool TMenuItem::Selected() {
  return (state == menuitem_state_selected);
}

/* **********************************************************************
 * Hit testing
 * *********************************************************************/
void TMenuItem::SetHitArea(int nxl, int nxh, int nyl, int nyh) {
  xl = nxl; xh = nxh; yl = nyl; yh = nyh;
  Assert(yl < yh, "TMenuItem::SetHitArea - yl >= yh");
}

bool TMenuItem::TestHit(int x, int y) {
  /* Use < for completly within */
  return (x < xh && x > xl && y < yh && y > yl);
}

/* **********************************************************************
 * TMenuItem::SetRenderColor
 * *********************************************************************/
void TMenuItem::SetRenderColor() {
  switch (state) {
  case menuitem_state_disabled:
    MenuTextRender.color = ColorDefinitions.GetColor("menu-item-disabled");
    break;
  case menuitem_state_blurred:
    // cout << caption << " does not have focus" << endl;
    MenuTextRender.color = ColorDefinitions.GetColor("menu-item-blurred");
    break;
  case menuitem_state_focused:
    {
      // cout << caption << " has focus" << endl;
      MenuTextRender.color = ColorDefinitions.GetColor("menu-item-focused");
      /* Here I am abusing the fact that system_time_t is a double */
      system_time_t tmp = system_gettime();
      /* Reduce to 0 - M_PI */
      tmp = tmp - floor(tmp/M_PI)*M_PI;
      MenuTextRender.color.SetIntensity(fabs(sin(3*tmp))*0.7+0.5);
      break;
    }
  case menuitem_state_selected:
    // cout << caption << " is editing " << endl;
    MenuTextRender.color = ColorDefinitions.GetColor("menu-item-selected");
    break;
  }
}

/* **********************************************************************
 * TMenuItem::Render
 * *********************************************************************/

void TMenuItem::Render(int xlow, int xhigh) {
  /* Sets the color, then renders the caption centered with linechange */
  SetRenderColor();
  MenuTextRender.CenterLn(xlow, xhigh, caption);
}

/* **********************************************************************
 * **********************************************************************
 *  TSpaceMenuItem - Renders a blank line
 * **********************************************************************
 * *********************************************************************/
void TSpaceMenuItem::Render(int xlow, int xhigh) {
  MenuTextRender.PrintLn("");
}

/* **********************************************************************
 * **********************************************************************
 *  TSimpleActionMenuItem 
 * **********************************************************************
 * *********************************************************************/
bool TSimpleActionMenuItem::CommandConsume(TCommand * Command) {
  if (menuitem_state_focused != state) {
    cerr << "TSimpleActionMenuItem::CommandConsume not in focus!" << endl;
    return false;
  };
  if ("menuitem" != Command->name) {
    cerr << "TSimpleactionMenuItem::CommandConsume non registered command "
	 << Command->name << endl;
    return false;
  }
  if ("select" == Command->args) {
    DoAction();
    return true;
  } else {
    cerr << "TSimpleActionMenuItem::CommandConsume, do not know how to handle ("
	 << Command->name << ", " << Command->args << ")" << endl;
    return false;
  } 
}

/* **********************************************************************
 * **********************************************************************
 * TScrollMenuItem
 * **********************************************************************
 * *********************************************************************/
TScrollMenuItem::TScrollMenuItem(TMenu * owner, string desc, bool isup) 
  : TSimpleActionMenuItem(owner, "--", desc) {
  IsUp      = isup;
  CanScroll = !isup;
  OtherEnd  = NULL;
  if (!isup) {
    caption = "vv";
  }
};

void TScrollMenuItem::SetOtherEnd(TScrollMenuItem * other) {
  OtherEnd = other;
}

void TScrollMenuItem::SetCanScroll() {
  CanScroll = true;
  if (IsUp) {
    caption = "^^";
  } else {
    caption = "vv";
  }
}
/* **********************************************************************
 * TScrollMenuItem::DoAction - this is kinda hacky.
 * *********************************************************************/
void TScrollMenuItem::DoAction() {
  Assert(OtherEnd, "TScrollMenuItem::DoAction - no other end");
  OtherEnd->SetCanScroll();
  if (IsUp) {
    /* We scroll Up - then test if it can be done again */
    Owner->ScrollUp(true);
    if (!Owner->ScrollUp(false)) {
      caption = "--";
    } else {
      Owner->ScrollDown(false);
      caption = "^^";
    }
  } else {
    Owner->ScrollDown(true);
    if (!Owner->ScrollDown(false)) {
      caption = "--";
    } else {
      Owner->ScrollUp(false);
      caption = "vv";
    }
  }
}

/* **********************************************************************
 * **********************************************************************
 *  TSubMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * TSubMenuItem::DoAction
 * *********************************************************************/
void TSubMenuItem::DoAction() {
  if (SubMenu) {
#ifdef SOUND_ON
    sound_play(names_to_nums["data/sounds/menu_forward_menu.raw"]);
#endif
    Owner->ShowChild(SubMenu);
  } else {
    cerr << "TSubMenuItem has no SubMenu!" << endl;
  }
  return;
}

/* **********************************************************************
 * **********************************************************************
 *  TInfoMenuItem
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Add a line of text to be displayed
 * *********************************************************************/
void TInfoMenuItem::AddLine(string line) {
  lines.push_back(line);
}

/* **********************************************************************
 * Render the text over multiple lines
 * *********************************************************************/
void TInfoMenuItem::Render(int xlow, int xhigh) {
  MenuTextRender.color = ColorDefinitions.GetColor("menu-item-info");
  MenuTextRender.CenterLn(xlow, xhigh, description);
  for (unsigned int i = 0; i < lines.size(); i++) {
    MenuTextRender.CenterLn(xlow, xhigh, lines[i]);
  }
}

/* **********************************************************************
 * **********************************************************************
 *  TReturnItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * TReturnMenuItem::DoAction
 * *********************************************************************/
void TReturnMenuItem::DoAction() {
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/menu_back_menu.raw"]);
#endif
  Owner->ShowParent();
  return;
}

/* **********************************************************************
 * **********************************************************************
 *  TActionMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * TActionMenuItem::DoAction
 * *********************************************************************/
void TActionMenuItem::DoAction() {
  if (function) {
#ifdef SOUND_ON
    sound_play(names_to_nums["data/sounds/menu_select.raw"]);
#endif
    function();
  }
}

/* **********************************************************************
 * **********************************************************************
 *  TValueMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * TValueMenuItem::{R,Unr}egisterCommands
 * *********************************************************************/
void TValueMenuItem::RegisterCommands() {
  // cout << "TValueMenuItem::RegisterCommands called" << endl;
  CommandDispatcher.RegisterConsumer("itemedit", this);
}

void TValueMenuItem::UnregisterCommands() {
  // cout << "TValueMenuItem::UnregisterCommands called" << endl;
  CommandDispatcher.UnregisterConsumer("itemedit");
}

/* **********************************************************************
 * TValueMenuItem::EnterEditState, and others
 * *********************************************************************/

/* Cancel must be like receiving a cancel */
void TValueMenuItem::Cancel() {
#if (DEBUG_MENUITEM)
  cout << "TValueMenuItem::Cancel - " << caption << endl;
#endif
  if (menuitem_state_selected == state) {
    DiscardNewValue();
  }
  if (menuitem_state_disabled != state) {
    CommandDispatcher.UnregisterConsumer("menuitem");
    state = menuitem_state_blurred;
  }
}

/* This is called when we leave focus state and go into editing state..
   Returns true if we did enter the state, false otherwise */
bool TValueMenuItem::EnterEditState() {
  /* In here, copying of the currentvalue to "new" should take place, 
     then a state change */
  StoredMode = GameMode.GetMode();
  if (!GameMode.SetMode(gamemode_edit)) {
    cerr << "TValueMenuItem::EnterEditState - unable to change gamemode" << endl;
    return false;
  }
  RegisterCommands();
  state = menuitem_state_selected;
  return true;
}

bool TValueMenuItem::LeaveEditState() {
  UnregisterCommands();
  if (!GameMode.SetMode(StoredMode)) {
    RegisterCommands();
    cerr << "TValueMenuItem::LeaveEditState - unable to change gamemode" << endl;
    return false;
  }
  state = menuitem_state_focused;
  return false;
}

/* **********************************************************************
 * TValueMenuItem:: - methods to manipulate values.
 * *********************************************************************/
/* During editing, validate can be called and will return accept or
   reject on the current user entered (new) value */
bool TValueMenuItem::ValidateNewValue(){
  return true;
};

/* Will value the value, and possible accept it */
bool TValueMenuItem::AcceptNewValue() {
  if (!ValidateNewValue()) {
    return false;
  }
  /* In here, validation and copying back should take place */
  return LeaveEditState();
};

void TValueMenuItem::DiscardNewValue() {
  /* In here, freeing temporary variables should take place. 
     Then a state change */
  // TODO: This is always a succes, but LeaveEditState can fail???
  LeaveEditState();
};


/* **********************************************************************
 * TValueMenuItem::CommandConsume
 * This handles 2 different commands, in 2 different modes.
 * In menu mode, it handles a select command, which will change the mode
 * to edit mode. In edit mode, it handles accept and cancel
 * *********************************************************************/
bool TValueMenuItem::CommandConsume(TCommand * Command) {
  /* Only handle Commands if we have the focus */
  if (!(menuitem_state_focused == state || menuitem_state_selected == state)) {
    cerr << "TValueMenuItem::CommandConsume not in focus/selected!" << endl;
    return false;
  };

  /* Handle menuitem commands */
  if ("menuitem" == Command->name) {
    /* By default, ENTER or SPACE will change from focus to editing
       Note: This behavoir will be overriden by "submenus". */
    if (menuitem_state_focused == state 
	&& ("select" == Command->args)) {
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_select.raw"]);
#endif
      EnterEditState();
      return true;
    }
    cerr << "TValueMenuItem::CommandConsume not handling (" 
	 << Command->name << ", " << Command->args << ")" << endl;
    return false;
  }
    
  
  /* Handle edit commands */
  if ("itemedit" == Command->name) {
    /* Also by default, we leave the editing/selected state with
       accept or cancel */
    if (menuitem_state_selected == state) {
      if ("accept" == Command->args) { 
#ifdef SOUND_ON
	sound_play(names_to_nums["data/sounds/menu_deselect.raw"]);
#endif
	AcceptNewValue();
	return true;
      }
      if ("cancel" == Command->args) {
#ifdef SOUND_ON
	sound_play(names_to_nums["data/sounds/menu_deselect.raw"]);
#endif
	DiscardNewValue();
	return true;
      }
    }
    /* Ups, not handled/wrong state for command/something */
    cerr << "TValueMenuItem::CommandConsume not handling (" 
	 << Command->name << ", " << Command->args << ")" << endl;
    return false;
  }
  /* Yes, this is a duplicate. Will go eventually */
  /* Unhandled */
  cerr << "TValueMenuItem::CommandConsume not handling (" 
       << Command->name << ", " << Command->args << ")" << endl;
  return false;
}


/* **********************************************************************
 * **********************************************************************
 *  TListMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Add a value
 * **********************************************************************/
void TListMenuItem::AddValue(string nvalue) {
  /* Defaults to first added value is selected */
  values.push_back(nvalue);
  selected_value = 0;
  *storage = values[selected_value];
};
/* **********************************************************************
 * Register and unregister commands.
 * **********************************************************************/
void TListMenuItem::RegisterCommands() {
  TValueMenuItem::RegisterCommands();
  CommandDispatcher.RegisterConsumer("list", this);
};

void TListMenuItem::UnregisterCommands() {
  TValueMenuItem::UnregisterCommands();
  CommandDispatcher.UnregisterConsumer("list");
};

/* **********************************************************************
 * TListMenuItem::CommandConsume
 * This handles the list up and down commands, but rely on
 * TValueMenuItem to handle the rest of the commands
 * *********************************************************************/
bool TListMenuItem::CommandConsume(TCommand * Command) {
  if (state == menuitem_state_selected) {
    if ("list" == Command->name) {
      /* Handle up and down */
      if ("previous" == Command->args) {
	selected_value = (selected_value - 1 + values.size()) % values.size();
	*storage = values[selected_value];
#ifdef SOUND_ON
	sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif    
	return true;
      }
      if ("next" == Command->args) {
	selected_value = (selected_value + 1) % values.size();
	*storage = values[selected_value];
#ifdef SOUND_ON
	sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif    
	return true;
      }
    }
  }
  /* Errors and unhandled fall to here */
  return TValueMenuItem::CommandConsume(Command);
}

/* **********************************************************************
 * The render for TStringMenuItem render both the string and the value
 * *********************************************************************/
void TListMenuItem::Render(int xlow, int xhigh) {
  /* Sets the render color */
  SetRenderColor(); 
  /* Render everything */
  MenuTextRender.CenterLn(xlow, xhigh, 
			  caption + ":" + values[selected_value], ":");
}

/* **********************************************************************
 * **********************************************************************
 *  TStringMenuItem 
 * **********************************************************************
 * *********************************************************************/

string TStringMenuItem::killed = "";


/* **********************************************************************
 * Register and unregister commands.
 * **********************************************************************/
void TStringMenuItem::RegisterCommands() {
  TValueMenuItem::RegisterCommands();
  CommandDispatcher.RegisterConsumer("point-move", this);
  CommandDispatcher.RegisterConsumer("edit", this);
  CommandDispatcher.RegisterConsumer("keydown", this);
};

void TStringMenuItem::UnregisterCommands() {
  TValueMenuItem::UnregisterCommands();
  CommandDispatcher.UnregisterConsumer("edit");
  CommandDispatcher.UnregisterConsumer("point-move");
  CommandDispatcher.UnregisterConsumer("keydown");
};

/* **********************************************************************
 * TStringMenuItem::CommandConsume
 * This handles the list up and down commands, but rely on
 * TValueMenuItem to handle the rest of the commands
 * *********************************************************************/
bool TStringMenuItem::CommandConsume(TCommand * Command) {
  if (state == menuitem_state_selected) {
    /* Make sure that the cursor is never out of bounds */
    // TODO: Figure out if this is still neccesary....
    cursorpos = mymax(0, cursorpos);
    cursorpos = mymin(cursorpos, new_value.size());
    /* Point/cursor move commands */
    if ("point-move" == Command->name) {
      if("backward-char" == Command->args) {
	cursorpos = mymax(0, ((int) cursorpos) - 1);
	return true;
      }
      if("forward-char" == Command->args) {
      	cursorpos = mymin(cursorpos + 1, new_value.size());
	return true;
      }
      if("beginning-of-line" == Command->args) {
	cursorpos = 0;
	return true;
      }
      if("end-of-line" == Command->args) {
	cursorpos = new_value.size();
	return true;
      }
    }	
    /* Edit commands */
    if ("edit" == Command->name) {
      if("delete-backward-char" == Command->args) {
	/* If the cursor is at 0, nothing can be deleted */
	if (0 == cursorpos) {
	  return true;
	}
	/* Backspace deletes the character to the left of the cursor
	   It must be there, because otherwise it would have to be 0 */
	new_value.erase(--cursorpos, 1);
	return true;
      }
      if("delete-char" == Command->args) {
	/* If the cursor is at the end of the string, no delete */
	if (new_value.size() == cursorpos) {
	  return true;
	}
	/* We delete rigth of the cursor */
	new_value.erase(cursorpos, 1);
	return true;
      }
      if("kill-line" == Command->args) {
      	killed = new_value.substr(cursorpos, new_value.size()-cursorpos);
	new_value.erase(cursorpos, new_value.size()-cursorpos);
	return true;
      }
      if("yank" == Command->args) {
	new_value.insert(cursorpos, killed);
	cursorpos += killed.size();
	return true;
      }
      if("transpose-chars" == Command->args) {
	cerr << "TODO: No code to handle transpose-chars" << endl;
      }
    }
    /* Keydown "events" */
    if ("keydown" == Command->name) {
      /* Characters between ' ' and '~' are just added at the cursor point*/
      const char * tmp = Command->args.c_str();
      // cout << "Got a keypress!" << endl;
      if (tmp[0] >= ' ' && tmp[0] <= '~') {
	new_value.insert(cursorpos, Command->args);
	cursorpos++;
	return true;
      }
    }
  }
  /* Errors and unhandled fall to here */
  return TValueMenuItem::CommandConsume(Command);
}

/* **********************************************************************
 * The render for TStringMenuItem render both the string and the value
 * *********************************************************************/
void TStringMenuItem::Render(int xlow, int xhigh) {
  /* Sets the render color */
  SetRenderColor(); 

  string tmp = caption + ":";
  /* Render the value */
  switch (state) {
  case menuitem_state_disabled:
  case menuitem_state_blurred:
  case menuitem_state_focused:
  /* No cursor is rendered */
    tmp += *value;
    break;
  case menuitem_state_selected: 
    /* Simply renders the cur_value up to the cursor position, then 
       the cursor, then the rest of cur_value */
    /* Check that the cursor is not without bounds */
    cursorpos = mymax(0, cursorpos);
    cursorpos = mymin(cursorpos, new_value.size());
    tmp += new_value.substr(0, cursorpos);
    /* Really ugly hack to get something that looks like a cursor */
    char buf[] = " ";
    buf[0] = 25;
    tmp += buf;
    tmp += new_value.substr(cursorpos, new_value.size()-cursorpos);
    break;
  }
  MenuTextRender.CenterLn(xlow, xhigh, tmp, ":");
}

/* **********************************************************************
 * Entering Edit state should copy the value
 * *********************************************************************/
bool TStringMenuItem::EnterEditState() {
  new_value = *value;
  cursorpos = new_value.size();
  return TValueMenuItem::EnterEditState();
}

/* **********************************************************************
 * Accept is ok, just copying
 * *********************************************************************/
bool TStringMenuItem::AcceptNewValue() {
  *value = new_value;
  return TValueMenuItem::AcceptNewValue();
}
