#include "menuitem.hh"
#include "menu.hh"
#include "types.hh"
#include "sound.hh"

/* **********************************************************************
 * **********************************************************************
 *  TMenuItem 
 * **********************************************************************
 * *********************************************************************/
void TMenuItem::SetRenderColor() {
  switch (state) {
  case menuitem_state_disabled:
    MenuTextRender.color = ColorDefinitions.Colors["gray"];
    break;
  case menuitem_state_blurred:
    // cout << caption << " does not have focus" << endl;
    MenuTextRender.color = ColorDefinitions.Colors["red"];
    break;
  case menuitem_state_focused:
    // cout << caption << " has focus" << endl;
    MenuTextRender.color = ColorDefinitions.Colors["green"];
    break;
  case menuitem_state_selected:
    // cout << caption << " is editing " << endl;
    MenuTextRender.color = ColorDefinitions.Colors["blue"];
    break;
  }
}

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
 *  TSubMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   The basic keyboard handler for TSubMenuItem looks for space or enter
   to do a "Owner->ShowChild(SubMenu)"*/
bool TSubMenuItem::KeyboardHandler(unsigned char key) {
  /* Only handle keypresses if we have the focus */
  if (menuitem_state_focused != state) {
    cerr << "TSubMenuItem::KeyboardHandler not in focus!" << endl;
    return false;
  };
  if (key == 10 || key == 13 || key == 32) {
    // cout << "TSubMenuItem wants to show child " << endl;
    if (SubMenu) {
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_forward_menu.raw"]);
#endif
      Owner->ShowChild(SubMenu);
    } else {
      cerr << "TSubMenuItem has no SubMenu!" << endl;
    }
    return true;
  } else {
    return false;
  } 
}


/* **********************************************************************
 * **********************************************************************
 *  TInfoMenuItem
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   Add a line of text to be displayed */
void TInfoMenuItem::AddLine(string line) {
  lines.push_back(line);
}

/* **********************************************************************
   Render the text over multiple lines */
void TInfoMenuItem::Render(int xlow, int xhigh) {
  MenuTextRender.color = ColorDefinitions.Colors["white"];
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
   The basic keyboard handler for TReturnMenuItem looks for space or enter
   to do a "Owner->ShowParent()"*/
bool TReturnMenuItem::KeyboardHandler(unsigned char key) {
  /* Only handle keypresses if we have the focus */
  if (menuitem_state_focused != state) {
    cerr << "TReturnMenuItem::KeyboardHandler not in focus!" << endl;
    return false;
  };
  if (key == 10 || key == 13 || key == 32) {
    // cout << "TReturnMenuItem wants to return " << endl;
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_back_menu.raw"]);
#endif
    Owner->ShowParent();
    return true;
  } else {
    return false;
  } 
}
/* **********************************************************************
 * **********************************************************************
 *  TActionMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   The basic keyboard handler for TReturnMenuItem looks for space or enter
   to do a "Owner->ShowParent()"*/
bool TActionMenuItem::KeyboardHandler(unsigned char key) {
  /* Only handle keypresses if we have the focus */
  if (menuitem_state_focused != state) {
    cerr << "TActionMenuItem::KeyboardHandler not in focus!" << endl;
    return false;
  };
  if (key == 10 || key == 13 || key == 32) {
    if (function) {
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_select.raw"]);
#endif
      function();
    }
    return true;
  } else {
    return false;
  } 
}

/* **********************************************************************
 * **********************************************************************
 *  TValueMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   The basic keyboard handler for TValueMenuItem will handle some state 
   changes */
bool TValueMenuItem::KeyboardHandler(unsigned char key) {
  // cout << endl << "TValueMenuItem::KeyboardHandler(" << key << ")" << endl; 
  /* Only handle keypresses if we have the focus */
  if (!(menuitem_state_focused == state || menuitem_state_selected == state)) {
    cerr << "TValueMenuItem::KeyboardHandler not in focus/selected!" << endl;
    return false;
  };
  /* By default, ENTER or SPACE will change from focus to editing
     Note: This behavoir will be overriden by "submenus". */
  if (menuitem_state_focused == state 
      && (key == 13 || key == 10 || key == ' ')) {
    // cout << "  TValueMenuItem going into edit state" << endl;
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_select.raw"]);
#endif
    EnterEditState();
    return true;
  }
  /* Also by default, we leave the editing state by hitting enter (accept)
     or esc, which will simply drop the value */
  if (menuitem_state_selected == state) {
    // cout << "  TValueMenuItem is in edit state" << endl;
    switch (key) {
    case 13:   /* Ctrl+M */
    case 10: { /* Ctrl+J */
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_deselect.raw"]);
#endif
      AcceptNewValue();
      return true;
    }
    case 3:  /* Ctrl+C */
    case 27: { /* Esc */
#ifdef SOUND_ON
      sound_play(names_to_nums["data/sounds/menu_deselect.raw"]);
#endif
      DiscardNewValue();
      return true;
    }
    default:
      return false;
    }
  }
  /* Unhandled */
  // cout << "  TValueMenuItem::KeyboardHandler not handling -" << key << "-" << endl; 
  return false;
}

/* **********************************************************************
 * **********************************************************************
 *  TListMenuItem 
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   Add a value
*/
void TListMenuItem::AddValue(string nvalue) {
  /* Defaults to first added value is selected */
  values.push_back(nvalue);
  selected_value = 0;
  *storage = values[selected_value];
}

/* **********************************************************************
   The basic keyboard handler for TListMenuItem will handle up and down 
*/
bool TListMenuItem::KeyboardHandler(unsigned char key) {
  // cout << "TStringMenuItem::KeyboardHandler(" << key << ")" << endl; 
  if (!TValueMenuItem::KeyboardHandler(key)) {
    if (state == menuitem_state_selected) {
      /* Handle delete and backspace */
      switch(key) {
      case 'a': { /* Previous value */
	selected_value = (selected_value - 1 + values.size()) % values.size();
	*storage = values[selected_value];
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif    
	return true;
      }
      case 'z': { /* Next value */
	selected_value = (selected_value + 1) % values.size();
	*storage = values[selected_value];
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/menu_move.raw"]);
#endif    
	return true;
      }
      default:
	/* We consider everything handled in this situation */
	return true;
      }
    }
    /* We are not in editing state, and not handled by TValueMenuItem - ie, not
       handled */
    return false;
  } else {
    /* It was handled by our inherited */
    return true;
  }
}

/* **********************************************************************
   The render for TStringMenuItem render both the string and the value */
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
   The basic keyboard handler for TStringMenuItem will handle some 
   editing changes */
bool TStringMenuItem::KeyboardHandler(unsigned char key) {
  // cout << "TStringMenuItem::KeyboardHandler(" << key << ")" << endl; 
  if (!TValueMenuItem::KeyboardHandler(key)) {
    if (state == menuitem_state_selected) {
      /* Make sure that the cursor is never out of bounds */
      cursorpos = mymax(0, cursorpos);
      cursorpos = mymin(cursorpos, new_value.size());
      /* Handle delete and backspace */
      switch(key) {
      case 8: { /* Backspace */
	/* If the cursor is a 0, nothing can be deleted */
	if (0 == cursorpos) {
	  return true;
	}
	/* Backspace deletes the character to the left of the cursor
	   It must be there, because otherwise it would have to be 0 */
	new_value.erase(--cursorpos, 1);
	return true;
      }
      case 4: /* Ctrl+D */
      case 127: { /* Delete */
	/* If the cursor is at the end of the string, no delete */
	if (new_value.size() == cursorpos) {
	  return true;
	}
	/* We delete rigth of the cursor */
	new_value.erase(cursorpos, 1);
	return true;
      }
      case 1: { /* Ctrl+A - Beginning of line */
	cursorpos = 0;
	return true;
      }	
      case 5: { /* Ctrl+E - End of line */
	cursorpos = new_value.size();
	return true;
      }	
      case 2: { /* Ctrl+B - Back char */
	cursorpos = mymax(0, ((int) cursorpos) - 1);
	return true;
      }
      case 6: { /* Ctrl+F - Forward char */
	cursorpos = mymin(cursorpos + 1, new_value.size());
	return true;
      }
      case 11: { /* Ctrl+K - Delete to end of line */
	killed = new_value.substr(cursorpos, new_value.size()-cursorpos);
	new_value.erase(cursorpos, new_value.size()-cursorpos);
	return true;
      }
      case 25: { /* Ctrl+Y - Insert at cursorposition */
	new_value.insert(cursorpos, killed);
	cursorpos += killed.size();
	return true;
      }
      
      default: {
	/* Characters between ' ' and '~' are just added at the cursor point*/
	if (key >= ' ' && key <= '~') {
	  char tmp[2];
	  tmp[0] = key;
	  tmp[1] = 0;
	  new_value.insert(cursorpos, tmp);
	  cursorpos++;
	}
      }
      }
      /* We consider everything handled in this situation */
      return true;
    }
    /* We are not in editing state, and not handled by TValueMenuItem - ie, not
       handled */
    return false;
  } else {
  // cout << "TStringMenuItem::KeyboardHandler not handling -" << key << "-" << endl; 
  /* It was handled by our inherited */
    return true;
  }
}

/* **********************************************************************
   The render for TStringMenuItem render both the string and the value */
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
   Entering Edit state should copy the value */
bool TStringMenuItem::EnterEditState() {
  new_value = *value;
  cursorpos = new_value.size();
  TValueMenuItem::EnterEditState();
  return true;
}

/* **********************************************************************
   Accept is ok, just copying */
bool TStringMenuItem::AcceptNewValue() {
  *value = new_value;
  TValueMenuItem::AcceptNewValue();
  return true;
}

