#ifndef __MENUITEM_HH__
#define __MENUITEM_HH__

#include <string>
#include <vector>

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

class TMenuItem {

protected:
  menuitem_state_t state;
  string caption;
  string description;
  TMenu * Owner;
  /* Sets the render color based on the state */
  virtual void SetRenderColor();

public:
  TMenuItem(TMenu * owner, string cap, string desc) {
    Owner       = owner;
    state       = menuitem_state_blurred;
    caption     = cap;
    description = desc;
  };
  virtual ~TMenuItem () {};

  /* Render will call SetRenderColor then render the caption */
  virtual void Render(int xlow, int xhigh);

  /* GetDescription simply return the description for the caller to use */
  string GetDescription() {
    return description;
  };

  /* Disable, disables the menuitem */
  virtual bool Disable() {
    if (menuitem_state_blurred == state) {
      state = menuitem_state_disabled;
      return true;
    } else {
      return false;
    }
  }

  /* Enable, enables the menuitem */
  virtual bool Enable() {
    if (menuitem_state_disabled == state) {
      state = menuitem_state_blurred;
      return true;
    } else {
      return false;
    }
  }
  
  /* Focus let the menu know that it has focus. It 
     will only handle keypresses and other events in 
     focused state. If it wont accept focus, false 
     will be returned, otherwise true */
  virtual bool Focus() {
    if (menuitem_state_blurred == state) {
      state = menuitem_state_focused;
      return true;
    } else {
      return false;
    }
  }

  /* If we are in state editing, we wont allow disfocus */
  virtual bool Blur() {
    if (menuitem_state_selected == state 
	|| menuitem_state_disabled) {
      return false;
    }
    state = menuitem_state_blurred;
    return true;
  }

  /* Returns true if the key has been handled, false otherwise */
  virtual bool KeyboardHandler(unsigned char key) = 0;
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
  virtual bool KeyboardHandler(unsigned char key) { return true; };
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
 * TSubMenuItem
 * **********************************************************************
   This menuitem can show a sub menu */
class TSubMenuItem : public TMenuItem {
protected:
  TMenu * SubMenu;
public:
  virtual bool KeyboardHandler(unsigned char key);
  TSubMenuItem(TMenu * owner, string cap, string desc, TMenu * subMenu) :
    TMenuItem(owner, cap, desc) {
    SubMenu = subMenu;
  }
};

/* **********************************************************************
 * TReturnMenuItem
 * **********************************************************************
 This menuitem can return from a sub menu */
class TReturnMenuItem : public TMenuItem {
public:
  TReturnMenuItem(TMenu * owner, string cap, string desc) 
    : TMenuItem(owner, cap, desc) {};
  virtual bool KeyboardHandler(unsigned char key);
};

/* **********************************************************************
 * TActionMenuItem
 * **********************************************************************
   This menuitem performs an action and hides the menu */
typedef void (*TAction)(void);

class TActionMenuItem : public TMenuItem {
private:
  void (* function) (void);
public:
  TActionMenuItem(TMenu * owner, string cap, string desc, TAction func) 
    : TMenuItem(owner, cap, desc) {
    function = func;
  };
  virtual bool KeyboardHandler(unsigned char key);
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
  /* This is called when we leave focus state and go into editing state..
     Returns true if we did enter the state, false otherwise */
  virtual bool EnterEditState() {
    /* In here, copying of the currentvalue to "new" should take place, 
       then a state change */
    state = menuitem_state_selected;
    return true;
  }
  /* During editing, validate can be called and will return accept or
     reject on the current user entered (new) value */
  virtual bool ValidateNewValue(){
    return true;
  };
  /* Will value the value, and possible accept it */
  virtual bool AcceptNewValue() {
    if (!ValidateNewValue()) {
      return false;
    }
    /* In here, validation and copying back should take place */
    state = menuitem_state_focused;
    return true;
  };
  virtual void DiscardNewValue() {
    /* In here, freeing temporary variables should take place. 
       Then a state change */
    state = menuitem_state_focused;
  }
public:
  TValueMenuItem(TMenu * owner, string cap, string desc) 
    : TMenuItem(owner, cap, desc) {};
  
  /* Returns true if the key has been handled, false otherwise */
  virtual bool KeyboardHandler(unsigned char key);
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
public:
  TListMenuItem(TMenu * owner, string cap, string desc, string * store) 
    : TValueMenuItem(owner, cap, desc) {
    selected_value = -1;
    storage = store;
  };
  virtual bool KeyboardHandler(unsigned char key);
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
  virtual bool KeyboardHandler(unsigned char key);
};

#endif
