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
#include "menuitem_color.hh"
#include "menu.hh"
void TColorMenuItem::Render(int xlow, int xhigh) {
  /* Sets the render color */
  SetRenderColor(); 
  /* Render everything */
  char buf[6];
  buf[0] = 4;
  buf[1] = 4;
  buf[2] = 4;
  buf[3] = 4;
  buf[4] = 4;
  buf[5] = 0; // xlow, xhigh, 
  /* MenuTextRender.PrintLn(
			 buf + caption + ":" 
			 + options[selected_value].caption); */
  MenuTextRender.Center(xlow, xhigh, 			 
			caption + ":" , ":");
  TColor tmp = MenuTextRender.color;
  MenuTextRender.color = options[selected_value].value; 
  MenuTextRender.PrintLn(buf);
  MenuTextRender.color = tmp;
  // MenuTextRender.PrintLn(" (" + options[selected_value].caption + ")");
  // MenuTextRender.color = tmp;
}
