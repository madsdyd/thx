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
#include "aibaltazar.hh"

/* **********************************************************************
 * Constructors and destructors
 * *********************************************************************/
TAIPlayerBaltazar::TAIPlayerBaltazar(TGame * ngame, string nname, string nteam) 
  : TAIPlayer(ngame, nname, nteam) {};

TAIPlayerBaltazar::~TAIPlayerBaltazar() {};

/* **********************************************************************
 * Round and turn commands - the meat of this object
 * *********************************************************************/

/* **********************************************************************
 * Set the goal for this turn
 * *********************************************************************/
void TAIPlayerBaltazar::BeginTurn() {
  /* Calling our parents BeginTurn should be safe and register the
     commands */
  TAIPlayer::BeginTurn();
  /* Baltazar is totally random in its goals, and is 
     modelled after the way my son played, when he was 4 months old. ;-) */
  cannon_target.angle    =  90.0*rand()/(RAND_MAX+1.0);
  cannon_target.rotation = 359.0*rand()/(RAND_MAX+1.0);
  cannon_target.force    = 100.0*rand()/(RAND_MAX+1.0);
  /* cout << "TAIPlayerBaltazar::BeginTurn - setting goals " 
     << cannon_target.angle << ", " << cannon_target.rotation 
     << ", " << cannon_target.force << endl; */
}



