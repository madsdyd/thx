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
#include <strstream>

#include "player.hh"
/* Implements functions related to the players */

#include "inventory.hh"
#include "tank.hh"

#include "command.hh"
/* We need to use this, if we use the mouse */
#include "inputmouse.hh"

/* We call some stuff from the TGame class */
#include "game.hh"

/* **********************************************************************
 * The constructor 
 * *********************************************************************/
TPlayer::TPlayer(TGame * ngame, string nname, string nteam) { 
  name      = nname;
  team      = nteam;
  game      = ngame;
  money     = 500;
  score     = 0;
  inventory = new TInventory(game, this);
}

/* **********************************************************************
 * The destructor
 * *********************************************************************/
TPlayer::~TPlayer() {
  cout << "TPlayer::~TPlayer() - clean up stuff?" << endl;
  delete inventory;
}

/* **********************************************************************
 * PrepareRound sets up the player for another round
 * *********************************************************************/
void TPlayer::PrepareRound(TVector * location) {
  /* Reset the inventory */
  inventory->SelectFirst();
  /* Prepare the tank */
  tank->PrepareRound(location);
  /* Reset the viewpoint */
  viewpoint.Reset(location);
  /* Adjust a bit, to better see our tank - this backs up a little, and looks
     into the ground */
  viewpoint.translation.y--;
  viewpoint.translation.y--;
  viewpoint.rotation.x = 45;
  viewpoint.translation.z++;
}

/* **********************************************************************
 * Register and unregister the commands the player handle. 
 * This is commands to control the viewpoint and the cannon
 * *********************************************************************/
void TPlayer::RegisterCommands() {
  CommandDispatcher.RegisterConsumer("viewpoint-move", this);
  CommandDispatcher.RegisterConsumer("viewpoint-rotate", this);
  CommandDispatcher.RegisterConsumer("cannon", this);
  CommandDispatcher.RegisterConsumer("inventory", this);
  CommandDispatcher.RegisterConsumer("fire", this);
}

void TPlayer::UnregisterCommands() {
  CommandDispatcher.UnregisterConsumer("viewpoint-move");
  CommandDispatcher.UnregisterConsumer("viewpoint-rotate");
  CommandDispatcher.UnregisterConsumer("cannon");
  CommandDispatcher.UnregisterConsumer("inventory");
  CommandDispatcher.UnregisterConsumer("fire");
}
/* **********************************************************************
 * Begin and end a turn. Mostly do command registering at the moment.
 * *********************************************************************/
void TPlayer::BeginTurn() {
  RegisterCommands();
}

void TPlayer::EndTurn() {
  UnregisterCommands();
  /* Clear the commands */
  active_commands.clear();
}
/* **********************************************************************
 * PerformCommandUpdate
 * The responsibility of this function is to perform the operation 
 * stored in the "current" command.
 * This is somewhat a test, as I am not sure this is the right way to 
 * proceed.
 * *********************************************************************/
// TODO: Remove this include. ostrstream?
#include <stdio.h>
void TPlayer::PerformCommandUpdate(system_time_t timenow) {
  /* Perform action based on the stored_commands 
     The map of commands is assumed to contain only commands that are 
     valid - each of the actions associated with the commands are carried
     out. 
     The other methods may call this method freely. Since commands are
     supposed to have monotomic timestamps, the timestamp from a command
     should be OK to use for timenow.
     All this is subject to change, of course...
*/
  // cout << "TPlayer::PerformCommandUpdate(" << timenow << ")" << endl;

  /* Iterate over all commands, do any action required */
  TActiveCommandsIterator end = active_commands.end();
  TActiveCommandsIterator i;
  for (i = active_commands.begin(); i != end; i++) {
    /* Do any action, scale after time, update the timestamp of the command 
       Not updating the timestamp will make the display accellerate. */
    TCommand * Command = &((*i).second);
    /* Strip the command args for anything but the first arg. */
#ifdef NEVER
    string myargM
    if ("" == Command->args) {
      cerr << "TPlayer::PerformCommandUpdate - empty arg" << endl;
      myarg = "";
    } else {
      istrstream tmpstr(Command->args.c_str());
      tmpstr >> myarg;
    }
#endif
    /* TODO: This is probably some wrongly typecasting */
    double scale_move = timenow - Command->timestamp;
    if (scale_move > 0) {
      /* This scale_move < 0 it means that the command was entered
	 after the last update, which is not so weird. */
      /* I originally wrote the following code:
	 Command->timestamp = timenow;
	 But, this does not work (no wonder, but bugs can be hard to see 
	 I kinda like the accelaration effect - So I will leave it in! */
      if (scale_move > 5.0) { scale_move = 5.0; };
      // printf("TPlayer::PerformCommandUpdate scale_move is :%f\n", scale_move);
      /* **********************************************************************
       * (viewpoint-move)
       * *********************************************************************/
      if ("viewpoint-move" == Command->name) {
	if ("+forward" == Command->args) {
	  viewpoint.translation.x 
	    += scale_move * sin(viewpoint.rotation.z * M_PI / 180.0);
	  viewpoint.translation.y 
	    += scale_move * cos(viewpoint.rotation.z * M_PI / 180.0);
	  viewpoint.translation.z 
	    -= scale_move * sin(viewpoint.rotation.x * M_PI / 180.0);
	}
	else if ("+backward" == Command->args) {
	  viewpoint.translation.x 
	    -= scale_move * sin(viewpoint.rotation.z * M_PI / 180.0);



	  viewpoint.translation.y 
	    -= scale_move * cos(viewpoint.rotation.z * M_PI / 180.0);
	  viewpoint.translation.z 
	    += scale_move * sin(viewpoint.rotation.x * M_PI / 180.0);
	}
	else if ("+left" == Command->args) {
	  viewpoint.translation.x 
	    -= scale_move * cos(viewpoint.rotation.z * M_PI / 180.0);
	  viewpoint.translation.y 
	    += scale_move * sin(viewpoint.rotation.z * M_PI / 180.0);
	}
	else if ("+right" == Command->args) {
	  viewpoint.translation.x 
	    += scale_move * cos(viewpoint.rotation.z * M_PI / 180.0);
	  viewpoint.translation.y 
	    -= scale_move * sin(viewpoint.rotation.z * M_PI / 180.0);
	}
	else if ("+up" == Command->args) {
	  viewpoint.translation.z += scale_move;
	}
	else if ("+down" == Command->args) {
	  viewpoint.translation.z -= scale_move;
	}    
	else {
	  /* Ups, not handling this */
	  cerr << "TPlayer::PerformCommandUpdate, not handling ("
	       << Command->name << "," << Command->args << ")" << endl;
	  cout << "TPlayer::PerformCommandUpdate, need to remove command?"
	       << endl;
	}
      } /* viewpoint-move */

      /* **********************************************************************
       * (viewpoint-rotate)
       * *********************************************************************/
      else if ("viewpoint-rotate" == Command->name) {
	if ("+forward" == Command->args) {
	  viewpoint.rotation.x-=2.0 * scale_move;
	  if (viewpoint.rotation.x<0.0) 
	    viewpoint.rotation.x+=360.0;
	} 
	else if ("+backward" == Command->args) {
	  viewpoint.rotation.x+=2.0 * scale_move;
	  if (viewpoint.rotation.x>360.0) 
	    viewpoint.rotation.x-=360.0;
	  
	}
	else if ("+left" == Command->args) {
	  viewpoint.rotation.z-=2.0 * scale_move;
	  if (viewpoint.rotation.z<0.0) 
	    viewpoint.rotation.z+=360.0;
	  
	} 
	else if ("+right" == Command->args) {
	  viewpoint.rotation.z+=2.0 * scale_move;
	  if (viewpoint.rotation.z>360.0) 
	    viewpoint.rotation.z-=360.0;
	}
	else {
	  /* Ups, not handling this */
	  cerr << "TPlayer::PerformCommandUpdate, not handling ("
	       << Command->name << "," << Command->args << ")" << endl;
	  cout << "TPlayer::PerformCommandUpdate, need to remove command?"
	       << endl;
	}
      } /* viewpoint-rotate */
      /* **********************************************************************
       * cannon
       * *********************************************************************/
      else if ("cannon" == Command->name) {
	if ("+rotate-left" == Command->args) {
	  tank->AdjustRotation(scale_move);
	}
	else if ("+rotate-right" == Command->args) {
	  tank->AdjustRotation(-scale_move);
	}
	else if ("+raise" == Command->args) {
	  tank->AdjustAngle(scale_move);
	}
	else if ("+lower" == Command->args) {
	  tank->AdjustAngle(-scale_move);
	}
	else if ("+force-increase" == Command->args) {
	  tank->AdjustForce(scale_move);
	}
	else if ("+force-decrease" == Command->args) {
	  tank->AdjustForce(-scale_move);
	}
	else {
	  /* Ups, not handling this */
	  cerr << "TPlayer::PerformCommandUpdate, not handling ("
	       << Command->name << "," << Command->args << ")" << endl;
	  cout << "TPlayer::PerformCommandUpdate, need to remove command?"
	       << endl;
	}
      } /* cannon */
    } /* scale_move > 0 */
  } /* Iterate over all commands */
}
/* **********************************************************************
 * Update - mostly to propagate the timenow to PerformCommandUpdate
 * *********************************************************************/
void TPlayer::Update(system_time_t timenow) {
  PerformCommandUpdate(timenow);
}
  
/* **********************************************************************
 * Consume commands. Mostly viewpoint and cannon.
 * It may not be the best thing to let the player handle this, but
 * for now, that is how it is.
 * *********************************************************************/
bool TPlayer::CommandConsume(TCommand * Command) {
  // TODO: Some state checking?
  // cout << "TPlayer::CommandConsume" << endl;
  
  /* The fire checking is handled by the game */
  if ("fire" == Command->name) {
    return game->FireProjectile();
  }

  /* Handle inventory commands */
  if ("inventory" == Command->name) {
    if ("next-weapon" == Command->args) {
      inventory->SelectNext();
      return true;
    }
  }

  /* Viewpoint-rotate by mouse */
  if ("viewpoint-rotate" == Command->name) {
    if ("mouse" == Command->args.substr(0, 5)) {
      TPointerInfo * PInfo = Command->GetPointerInfo();
      double xd, yd;
      /* Get relative always */
      xd = (PInfo->x - PInfo->oldx)*0.15;
      yd = -(PInfo->y - PInfo->oldy)*0.15;

      /* Change the viewpoint */
      /* Left, right */
      viewpoint.rotation.z += xd;
      while (viewpoint.rotation.z < 0.0) 
	viewpoint.rotation.z += 360.0;
      while (viewpoint.rotation.z > 360.0) 
	viewpoint.rotation.z -= 360.0;
      /* Up/down */
      viewpoint.rotation.x += yd;
      while (viewpoint.rotation.x < 0.0) 
	viewpoint.rotation.x += 360.0;
      while (viewpoint.rotation.x > 360.0) 
	viewpoint.rotation.x -= 360.0;
      return true;
    }
  }
  
  /* Viewpoint-move by mouse */
  if ("viewpoint-move" == Command->name) {
    if ("mouse" == Command->args.substr(0, 5)) {
      TPointerInfo * PInfo = Command->GetPointerInfo();
      double xd, yd;
      /* Get relative always */
      xd = -(PInfo->x - PInfo->oldx) * 0.15;
      yd = (PInfo->y - PInfo->oldy) * 0.15;

      /* Change the viewpoint */
      /* Forward and back */
      viewpoint.translation.x 
	+= yd * sin(viewpoint.rotation.z * M_PI / 180.0);
      viewpoint.translation.y 
	+= yd * cos(viewpoint.rotation.z * M_PI / 180.0);
      viewpoint.translation.z 
	-= yd * sin(viewpoint.rotation.x * M_PI / 180.0);
      /* Left and right */
      viewpoint.translation.x 
	-= xd * cos(viewpoint.rotation.z * M_PI / 180.0);
      viewpoint.translation.y 
	+= xd * sin(viewpoint.rotation.z * M_PI / 180.0);
     
      return true;
    }
  }
  
  /* Cannon rotate by mouse */
  if ("cannon" == Command->name) {
    if ("mouse" == Command->args.substr(0, 5)) {
      TPointerInfo * PInfo = Command->GetPointerInfo();
      double xd, yd;

      /* Get relative always */
      xd = -(PInfo->x - PInfo->oldx) * 0.15;
      yd = (PInfo->y - PInfo->oldy) * 0.15;
      
      /* Change the cannon */
      tank->AdjustRotation(xd);
      tank->AdjustAngle(yd);
      return true;
    }
  }
  
  // cout << "TPlayer::CommandConsumer, reaching +/- ("
  //     << Command->name << "," << Command->args << ")" << endl;
  /* Handle commands that have a +/- stuff */
  {
    // Check if this is one we handle, check for +/-, 
    // Remove/insert into active commands, PerformCommandUpdate
    /* Check the first character in the args */
    string plus_minus = Command->args.substr(0, 1);
    string cmd_key    
      = Command->name + "%" + Command->args.substr(1,Command->args.size()-1);
    if ("+" == plus_minus) {
      /* This command needs to be added, unless it is a duplicate */
      TActiveCommandsIterator i = active_commands.find(cmd_key);
      if (i == active_commands.end()) {
	/* No duplicates 
	   This should use the copy constructor */
	active_commands.insert(TActiveCommandsElement(cmd_key, Command));
	// cout << "CommandConsume - inserting " << cmd_key << " into active " << endl;
      } else {
	// cout << "Duplicates?" << endl;
      }
      return true;
    }
    else if ("-" == plus_minus) {
      /* Remove this command from the active map */
      TActiveCommandsIterator i = active_commands.find(cmd_key);
      if (i != active_commands.end()) {
	/* Found one - remove it 
	   First make sure that the time is current - that is, update all commands */
	PerformCommandUpdate(Command->timestamp);
	active_commands.erase(i);
	// cout << "CommandConsume - removing " << cmd_key << " from active " << endl;
      }
      return true;
    }
  }
  cerr << "TPlayer::CommandConsumer, not handling ("
       << Command->name << "," << Command->args << ")" << endl;
  return false;
}
