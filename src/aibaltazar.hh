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
#ifndef __AIBALTAZAR_HH__
#define __AIBALTAZAR_HH__
/* This is an AI player - based on a normal player */

#include "aiplayer.hh"

class TAIPlayerBaltazar : public TAIPlayer {
public:
  TAIPlayerBaltazar(TGame * ngame, string nname, string nteam);
  virtual ~TAIPlayerBaltazar();
  void BeginTurn(); /* The only thing Baltazar do */
};
#endif
