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
#include <math.h>
#include <GL/glut.h>

#include "keyboard.hh"
#include "display.hh"
#include "console.hh"
#include "tank.hh"
#include "player.hh"
#include "inventory.hh"
#include "viewpoint.hh"
#include "server.hh"
#include "render.hh"
#include "map.hh"        /* To be able to invalidate the map */
#include "projectile.hh" /* For spawn markers */

/* **********************************************************************
   keyboard_handler
   Called when a key on the keyboard is pressed - modifies most of the 
   game state */
void keyboard_handler(unsigned char key, int x, int y) {
  if (!Display->viewpoint) {
    cerr << "keyboard_handler with NULL Display->viewpoint" << endl;
  }

  int canon_adjust = 1; /* Used for changing the canon) */
  /* Scale our move */
  double scale_move = 0.6;

  switch (key) {
    /*
     * q/Q: Quit
     */
  case 'q':case 'Q':
    exit(0);
    break;

    /* **********************************************************************
       Functions to _move_ location 
       Bound as Mads's Quake functions */
  case 'r': case 'R':
    /* +forward */
    Display->viewpoint->translation.x 
      += scale_move * sin(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.y 
      += scale_move * cos(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.z 
      -= scale_move * sin(Display->viewpoint->rotation.x * M_PI / 180.0);
    break;
  case 'f': case 'F':
    /* +backward */
    Display->viewpoint->translation.x 
      -= scale_move * sin(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.y 
      -= scale_move * cos(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.z 
      += scale_move * sin(Display->viewpoint->rotation.x * M_PI / 180.0);
    break;
  case 'd': case 'D':
    /* +move_left */
    Display->viewpoint->translation.x 
      -= scale_move * cos(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.y 
      += scale_move * sin(Display->viewpoint->rotation.z * M_PI / 180.0);
    break;
  case 'g': case 'G':
    /* +move_rigth */
    Display->viewpoint->translation.x 
      += scale_move * cos(Display->viewpoint->rotation.z * M_PI / 180.0);
    Display->viewpoint->translation.y 
      -= scale_move * sin(Display->viewpoint->rotation.z * M_PI / 180.0);
    break;
  case 't': case 'T':
    /* +up */
    Display->viewpoint->translation.z += 1.0;
    break;
  case 'b': case 'B':
    /* +down */
    Display->viewpoint->translation.z -= 1.0;
    break;
    
    /* **********************************************************************
       Functions to _rotation. Display->viewpoint
       Bound as Mads's Quake functions */
  case 'k': case 'K':
    /* +right */
    Display->viewpoint->rotation.z+=2.0;
    if (Display->viewpoint->rotation.z>360.0) 
      Display->viewpoint->rotation.z-=360.0;
    break;
  case 'h': case 'H':
    /* +left */
    Display->viewpoint->rotation.z-=2.0;
    if (Display->viewpoint->rotation.z<0.0) 
      Display->viewpoint->rotation.z+=360.0;
    break;

  case 'u': case 'U':
    /* Tilt away from viewer */
    Display->viewpoint->rotation.x-=2.0;
    if (Display->viewpoint->rotation.x<0.0) 
      Display->viewpoint->rotation.x+=360.0;
    break;
  case 'j': case 'J':
    /* Tilt towards viewer */
    Display->viewpoint->rotation.x+=2.0;
    if (Display->viewpoint->rotation.x>360.0) 
      Display->viewpoint->rotation.x-=360.0;
    break;

    /* Reset Display->viewpoint */
  case 'e': case 'E':
    Display->viewpoint->Reset(Game->GetMap());
    break;

    /* **********************************************************************
       Functions to manipulate tank */

    /* Rotate stuff */    
  case 'N':
    canon_adjust = 10;
  case 'n':
    Game->current_player->tank->AdjustRotation(canon_adjust);
    break;

  case 'M':
    canon_adjust = 10;
  case 'm':
    Game->current_player->tank->AdjustRotation(-canon_adjust);
    break;

    /* Angle stuff */    
  case 'A':
    canon_adjust = 10;
  case 'a':
    Game->current_player->tank->AdjustAngle(canon_adjust);
    break;
  case 'Z':
    canon_adjust = 10;
  case 'z':
    Game->current_player->tank->AdjustAngle(-canon_adjust);
    break;

    /* Force stuff */    
  case 'O':
    canon_adjust = 10;
  case 'o': 
    Game->current_player->tank->AdjustForce(-canon_adjust);
    break;
  case 'P':
    canon_adjust = 10;
  case 'p': 
    Game->current_player->tank->AdjustForce(canon_adjust);
    break;

    /* Selection of projectile */
  case '\t':
    Game->current_player->inventory->SelectNext();
    break;

    /* Tank fire */
  case ' ':
    if (Game->FireProjectile()) {
      // Display->console->AddLine("Incomiiiing");
    };
    break;

    /* **********************************************************************
       Misc */
    
    /* time display */
  case 'c': case 'C':
    Display->RefreshRate();
    break;

    /* toggle flymode */
  case 'y': case 'Y':
    if(Display->flymode) {
      Display->console->AddLine("Flymode off");
    } else {
      Display->console->AddLine("Flymode on");
    }
    Display->flymode = !Display->flymode;
    break;

    /* toggle clipmode */
  case 'i': case 'I':
    if(Display->clipmode) {
      Display->console->AddLine("Clipmode off");
    } else {
      Display->console->AddLine("Clipmode on");
    }
    Display->clipmode = !Display->clipmode;
    break;

    /* Change between different render modes */
  case '1':
    render_type = render_type_lines;
    Game->GetMap()->has_changed = true;
    break;

  case '2':
    render_type = render_type_triangles;
    Game->GetMap()->has_changed = true;
    break;

  case '3':
    render_type = render_type_textures;
    Game->GetMap()->has_changed = true;
    break;

  case '4':
    render_light = !render_light;
    if (render_light) {
      Display->console->AddLine("Lights ON");
    } else {
      Display->console->AddLine("Lights OFF");
    }
    Game->GetMap()->has_changed = true;
    break;

  case '5':
    render_shade_smooth = !render_shade_smooth;
    if (render_shade_smooth) {
      Display->console->AddLine("Shademode : smooth");
    } else {
      Display->console->AddLine("Shademode : flat");
    }
    Game->GetMap()->has_changed = true;
    break;

  case '6':
    render_normals = !render_normals;
    if (render_normals) {
      Display->console->AddLine("Normals ON");
    } else {
      Display->console->AddLine("Normals OFF");
    }
    Game->GetMap()->has_changed = true;
    break;

  case '7':
    spawn_markers = !spawn_markers;
    if (spawn_markers) {
      Display->console->AddLine("Markers ON");
    } else {
      Display->console->AddLine("Markers OFF");
    }
    Game->GetMap()->has_changed = true;
    break;

  default:
    /*
     * Everything else: do nothing
     */
    break;
  }
}
