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
#include "debug.hh"
#include "command.hh"
#include "inputmouse.hh"
/* The command queue */
TCommands CommandQueue;

#define DEBUG_COMMAND 0

/* **********************************************************************
 * Stuff for PointerInfo
 * *********************************************************************/
#if (DEBUG_COMMAND)
static int TP_count = 0;
#endif
TPointerInfo::TPointerInfo(int nx, int ny, int noldx, int noldy) {
#if (DEBUG_COMMAND)
  TP_count++;
  cout << "Creating TPointerInfo() " << TP_count << endl;
#endif
  x = nx; y = ny; oldx = noldx; noldy = noldy;
};
/* Maybe this is quite silly - maybe a mouseevent should have 
   a pointerinfo field, sort of .. */
TPointerInfo::TPointerInfo(TMouseInputEvent * MouseEvent) {
  Assert(MouseEvent!=NULL, "TPointerInfo::TPointerInfo(TMouseInputEvent) NULL!");
#if (DEBUG_COMMAND)
  TP_count++;
  cout << "Creating TPointerInfo(MouseEvent) " << TP_count << endl;
#endif
  x    = MouseEvent->x; 
  y    = MouseEvent->y;
  oldx = MouseEvent->oldx; 
  oldy = MouseEvent->oldy;
}

TPointerInfo::TPointerInfo(TPointerInfo * nPInfo) {
  Assert(nPInfo!=NULL, "TPointerInfo::TPointerInfo(TPointerInfo) NULL!");
#if (DEBUG_COMMAND)
  TP_count++;
  cout << "Creating TPointerInfo(PInfoEvent) " << TP_count << endl;
#endif
  x    = nPInfo->x; 
  y    = nPInfo->y;
  oldx = nPInfo->oldx; 
  oldy = nPInfo->oldy;
}

TPointerInfo::~TPointerInfo() {
#if (DEBUG_COMMAND)
  TP_count--;
  cout << "Destroying TPointerInfo " << TP_count << endl;
#endif
}

/* **********************************************************************
 * Constructors for TCommand 
 * *********************************************************************/
#include <stdio.h>
TCommand::TCommand(system_time_t ntimestamp, string nname, string nargs,
		   TPointerInfo * nPointerInfo) {
  timestamp   = ntimestamp;
  name        = nname;
  args        = nargs;
  PointerInfo = nPointerInfo;
  /* printf("TCommand::TCommand - time is %f\n", ntimestamp);
  cout << "TCommand::TCommand(" << ntimestamp << ", " << nname 
  << ", " << nargs << ")" << endl; */
};

TCommand::TCommand(TCommand * cmd) {
  timestamp   = cmd->timestamp;
  name        = cmd->name;
  args        = cmd->args;
  if (cmd->PointerInfo) {
    PointerInfo = new TPointerInfo(cmd->PointerInfo);
  } else {
    PointerInfo = NULL;
  }
}

TCommand::TCommand(const TCommand & cmd) {
  timestamp   = cmd.timestamp;
  name        = cmd.name;
  args        = cmd.args;
  if (cmd.PointerInfo) {
    PointerInfo = new TPointerInfo(cmd.PointerInfo);
  } else {
    PointerInfo = NULL;
  }
}

TCommand::~TCommand() {
  if (PointerInfo) {
    delete PointerInfo;
  }
}

/* **********************************************************************
 * Get the pointerinfo
 * *********************************************************************/
TPointerInfo * TCommand::GetPointerInfo() {
  Assert(PointerInfo != NULL, "TCommand::GetPointerInfo on NULL!");
  return PointerInfo;
}
