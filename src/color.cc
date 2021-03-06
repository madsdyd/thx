/*
    Tank Hill eXtreme
    Copyright (C) 2000-2001 Mads Bondo Dydensborg <mads@dydensborg.dk> and 
    Skjalm Arr�e <skjalm@arroe.dk>
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include "types.hh"
#include "color.hh"

/* The colordefinitions */
TColorDefinitions ColorDefinitions;

/* **********************************************************************
 * TColor::SetIntensity - change the scale of rgb values
 * *********************************************************************/
void TColor::SetIntensity(double intensity) {
  /* Clamp to 0-1 */
  double realint = mymin(1.0, mymax(0.0, intensity));
  data[0] *= realint;
  data[1] *= realint;
  data[2] *= realint;
}

/* **********************************************************************
 * Initialize TColorDefinitions
 * *********************************************************************/
TColorDefinitions::TColorDefinitions() {
  Colors["white"]   = TColor(1.0, 1.0, 1.0);
  Colors["red"]     = TColor(1.0, 0.0, 0.0);
  Colors["green"]   = TColor(0.0, 1.0, 0.0);
  Colors["blue"]    = TColor(0.0, 0.0, 1.0);
  Colors["black"]   = TColor(0.0, 0.0, 0.0);

  Colors["yellow"]  = TColor(1.0, 1.0, 0.0);
  Colors["cyan"]    = TColor(0.0, 1.0, 1.0);
  Colors["magenta"] = TColor(1.0, 0.0, 1.0);

  Colors["navy"]    = TColor(0.0, 0.0, 0.5);
  

  Colors["gray"]  = TColor(0.75, 0.75, 0.75);
  
  /* Menu colors */
  Colors["menu-title"]         = Colors["magenta"];
  Colors["menu-title"].SetIntensity(0.8);
  Colors["menu-description"]   = Colors["yellow"];
  Colors["menu-description"].SetIntensity(0.65);
  /* Menu item colors */
  Colors["menu-item-blurred"]  = Colors["red"];
  Colors["menu-item-blurred"].SetIntensity(0.65);
  Colors["menu-item-disabled"] = Colors["gray"];
  Colors["menu-item-focused"]  = Colors["red"];
  Colors["menu-item-selected"] = Colors["red"];
  Colors["menu-item-info"]     = Colors["white"];
	
  Colors["player-1"]           = Colors["blue"];
  Colors["player-2"]           = Colors["red"];
  Colors["player-3"]           = Colors["green"];
  Colors["player-4"]           = Colors["yellow"];
  Colors["player-5"]           = Colors["cyan"];
  Colors["player-6"]           = Colors["magenta"];
  Colors["player-7"]           = Colors["black"];
  Colors["player-8"]           = Colors["navy"];
  


}
/* **********************************************************************
 * Return a color - return white if the name passed is not found
 * *********************************************************************/
TColor TColorDefinitions::GetColor(string color) {
  TColorsIterator loc = Colors.find(color);
  if (loc != Colors.end()) {
    return (*loc).second; 
  } else {
    cerr << "TColorDefinitions::GetColor(" << color << ") - not found" 
	 << endl;
    return TColor(1.0, 1.0, 1.0);
  }
}

/* **********************************************************************
 * Test for equality
 * *********************************************************************/
bool TColor::Equal(TColor & cref) {
  /*  cout << "TColor::Equal, comparing ("
       << data[0] << ", "
       << data[1] << ", "
       << data[2] << ", "
       << data[3] << ") to ("
       << cref.data[0] << ", "
       << cref.data[1] << ", "
       << cref.data[2] << ", "
       << cref.data[3] << ") - they "; */
  if (data[0] == cref.data[0] &&
      data[1] == cref.data[1] &&
      data[2] == cref.data[2] &&	
      data[3] == cref.data[3]) {
    // cout << "are equal" << endl;
    return true;
  } else {
    // cout << "differ" << endl;
    return false;
  }
}
