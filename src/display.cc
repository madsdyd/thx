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
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <strstream>

#include "display.hh"
#include "text.hh"
#include "types.hh"
#include "viewpoint.hh"
#include "map.hh"
#include "tank.hh"
#include "inventory.hh"

#include "console.hh"
#include "framerate.hh"
#include "server.hh"

#include "command.hh"
#include "commandconsumer.hh"
#include "render.hh"
#include "projectile.hh"

TDisplay * Display;

#define BITMAP_FONT GLUT_BITMAP_9_BY_15
/* **********************************************************************
 * Put a line somewhere in the world - not really used anymore
 * *********************************************************************/
void display_putline(int x, int y, int z, char * line) {
  int len, i;
  glRasterPos3i(x, y, z);
  len = (int) strlen(line);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(BITMAP_FONT, line[i]);
  }
}

/* **********************************************************************
 * This is our display reshape function that I can not figure out how
 * to put into the display class directly.
 * *********************************************************************/
void display_reshape(int w, int h) {
  Display->Reshape(w, h);
}

/* **********************************************************************
 * **********************************************************************
 * TDisplay, constructors and destructors
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Initialize GL and the display class
 * *********************************************************************/
TDisplay::TDisplay(int argc, char** argv) {
  textrender = new TTextRender;

  /* Default values */
  width       = 320;
  height      = 200;
  flymode     = true; 
  clipmode    = false;
  grabpointer = true;
  cout << "Warping pointer - hit 'a' to disable (default mapping)" 
       << endl;
  num_frames  = 0;
  viewpoint   = NULL;
  console     = new TConsole(1000); /* 1000 lines is stored */

  /* Let glut initialize a display and window */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Tank Hill eXtreme");
  
  /* Testing game mode stuff */
  /*  glutGameModeString("1280x1024");
  // enter full screen
  if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
    glutEnterGameMode();
  else {
    printf("The selected mode is not available\n");
    exit(1);        
    } */
	

  /* Light colors, placements etc */
  GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 10.0 };

  GLfloat light_position[] = { 16.0, 16.0, 10.0, 1.0 };
  //  GLfloat light1_position[] = { 0.0, 56.0, 16.0, 0.0 };

  //  GLfloat black_light[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat white1_light[] = { 0.4, 0.4, 0.4, 0.4 };

  //  GLfloat yellow_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
  //  GLfloat yellow_light[] = { 0.15, 0.15, 0.3, 1.0 };

  glClearColor(0.05, 0.05, 0.15, 0.0);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white1_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);

  /* Old lighting stuff
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow_light);
  glLightfv(GL_LIGHT1, GL_SPECULAR, yellow_light);
  glLightfv(GL_LIGHT1, GL_AMBIENT, black_light);
  */

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //  glEnable(GL_LIGHT1);
  
  /* Enable depth buffering test */
  glEnable(GL_DEPTH_TEST);

  /* Enable culling */
  //  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);

  /* Shade model (default) */
  glShadeModel(GL_SMOOTH);

  /* This is our display reshape function */
  glutReshapeFunc(display_reshape);
  /* Oddly enough, the client does the display at the moment */
  // glutDisplayFunc(display_display); 

  /* Enable texturing - DECAL replaces every pixel*/
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  /* The textrender can not be initialized until OpenGL is running */
  textrender->Load("data/graphics/fonts/fontGray.tga");

  /* Testing fog */
#ifdef NEVER
  {
    glEnable(GL_FOG);
    GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.05);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 10.0);
    glFogf(GL_FOG_END, 50.0);
    glClearColor(0.5, 0.5, 0.5, 1.0);
  }
#endif

  if (GL_NO_ERROR != glGetError()) {
    cerr << "TDisplay::TDisplay: GL was in error condition on exit" << endl;
  } 

  /* Register our commands */
  CommandDispatcher.RegisterConsumer("display", this);
  CommandDispatcher.RegisterConsumer("render", this);
  CommandDispatcher.RegisterConsumer("toggle-pointer-grab", this);
}



/* **********************************************************************
 * Clean up
 * *********************************************************************/
TDisplay::~TDisplay() {
  CommandDispatcher.UnregisterConsumer("display");
  CommandDispatcher.UnregisterConsumer("render");
  CommandDispatcher.UnregisterConsumer("toggle-pointer-grab");
  delete textrender;
  delete console;
}

/* **********************************************************************
 * **********************************************************************
 * TDisplay, Information about the display
 * **********************************************************************
 * *********************************************************************/
unsigned int TDisplay::GetWidth() {
  return width;
}
unsigned int TDisplay::GetHeight() {
  return height;
}
unsigned int TDisplay::GetNumFrames() {
  return num_frames;
}

bool TDisplay::GrabbingPointer() {
  return grabpointer;
}


/* **********************************************************************
 * **********************************************************************
 * TDisplay, Call backs
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Update just updates the console rigth now
 * *********************************************************************/
void TDisplay::Update(system_time_t deltatime) {
  console->Update(deltatime);
}

/* **********************************************************************
 * Reshape
 * *********************************************************************/
void TDisplay::Reshape(int w, int h) {
  /* We store these for our own use */
  width  = w;
  height = h;
  
  /* Set the textrender size */
  textrender->size = h/42; /* 480/40 == 12, slightly less */

  /* Set the viewport - only needed on change */
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  /* Set 3d mode */
  NormalMode();
}

/* **********************************************************************
 * **********************************************************************
 * TDisplay, Render related functions
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * To actually draw textlines, we need to be in "flatmode" where
 * drawing happes "on screen" (sort of).
 * *********************************************************************/
void TDisplay::FlatMode() {
  //  set_gl_options( TEXT );
  if (GL_NO_ERROR != glGetError()) {
    cerr << "TDisplay::FlatMode: GL was in error condition" << endl;
  } 
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  // glDisable(GL_FOG);
  // Set parallelprojection mode to create "correct" menus
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho( -0.5, width-0.5, -0.5, height-0.5, -1.0, 1.0 );

  // Back to modifying objects
  glMatrixMode(GL_MODELVIEW);
  glScalef(1.0, -1.0, 1.0);
  glLoadIdentity();
}

/* **********************************************************************
 * Set up our projection and model mode to have a coordinate system
 * with z going up, x going from left to rigth and y going into the
 * screen, which is the same way our world is layed out
 * *********************************************************************/
void TDisplay::NormalMode() {
  /* To draw in normal mode, we need this setup */
  if (GL_NO_ERROR != glGetError()) {
    cerr << "TDisplay::NormalMode: GL was in error condition" << endl;
  } 
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_FOG);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, (GLfloat) width/(GLfloat) height, 0.8, 100.0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef(-90.0, 1.0, 0.0, 0.0);
}  

/* **********************************************************************
 * Called each frame to actually draw the stuff the display knows about
 * *********************************************************************/
void TDisplay::Render(void) {
  if (!viewpoint) {
    cerr << "display_display with NULL viewpoint" << endl;
    return;
  } 
  if (GL_NO_ERROR != glGetError()) {
    cerr << "TDisplay::Render: GL was in error condition" << endl;
  } 
  //  GLenum gl_error;

  /*Push the current (ie normal viewing) matrix so
    we get the _correct_ rotation */
  glPushMatrix();

  /* Rotate scene according to user input
     clamp xrotate so that it is never between 80-100 or 260-280  */
  if (260 < viewpoint->rotation.x && viewpoint->rotation.x < 280) {
    if (viewpoint->rotation.x < 270) {
      viewpoint->rotation.x = 260;
    } else {
      viewpoint->rotation.x = 280;
    }
  }
  if (80 < viewpoint->rotation.x && viewpoint->rotation.x < 100) {
    if (viewpoint->rotation.x < 90) {
      viewpoint->rotation.x = 80;
    } else {
      viewpoint->rotation.x = 100;
    }
  }
  glRotatef(viewpoint->rotation.x, 1.0, 0.0, 0.0);
  glRotatef(viewpoint->rotation.z, 0.0, 0.0, 1.0);
  /* Force us to be inside and above the map.
     The viewpoint is otherwise changed by the TPlayer class */
#define TRANSOFFSET 3
#define ZOFFSET 3.0
  if (viewpoint->translation.y < TRANSOFFSET) {
    viewpoint->translation.y = TRANSOFFSET;
  };
  if (viewpoint->translation.y > Game->GetMap()->GetLength()-TRANSOFFSET) {
    viewpoint->translation.y = Game->GetMap()->GetLength()-TRANSOFFSET;
  }
  if (viewpoint->translation.x < TRANSOFFSET) {
    viewpoint->translation.x = TRANSOFFSET;
  };
  if (viewpoint->translation.x > Game->GetMap()->GetWidth()-TRANSOFFSET) {
    viewpoint->translation.x = Game->GetMap()->GetWidth()-TRANSOFFSET;
  }
  /* Set our z viewpoint to never go below the landscape and
     go 3 above it in non fly mode (unless we are not clipping */
  if (!clipmode) {
    if (!flymode) {
      /* if "walking" on the landscape, dump us on it */
      viewpoint->translation.z = ZOFFSET
	+ Game->GetMap()->HeightAt(viewpoint->translation.x, 
				   viewpoint->translation.y);
    } else {
      /* if flying, never go below */
      if (viewpoint->translation.z < ZOFFSET 
	  + Game->GetMap()->HeightAt(viewpoint->translation.x, 
				     viewpoint->translation.y)) {
	viewpoint->translation.z = ZOFFSET
	  + Game->GetMap()->HeightAt(viewpoint->translation.x, 
				    viewpoint->translation.y);
      }
    }
  }
  
  /* Translate to our viewpoint. The world is translated to this place, so we
     use -translate. */
  glTranslatef(-viewpoint->translation.x, 
	       -viewpoint->translation.y, 
	       -viewpoint->translation.z);

  /* Draw map */
  Game->GetMap()->Render(viewpoint);
  /* Never render this part with lighting */
  glDisable(GL_LIGHTING);
  /* Draw all other entities in the world */
  Game->RenderEntities(viewpoint);
  
#ifdef NEVEREVER
  /* Draw X, Y and Z axes for reference purposes */
  glColor3f(1.0,1.0,1.0);
  glBegin(GL_LINE_STRIP);
    glVertex3f(0.0,2.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(2.0,0.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,2.0);
  glEnd();
  /* Print coordinates at corners, etc */
  display_putline(0, 0, 0, "origo");
  display_putline(0, 0, 2, "(0,0,2)");
  display_putline(2, 0, 0, "(2,0,0)");
  display_putline(0, 2, 0, "(0,2,0)");
#endif

  /* Restore original (ie normal viewing matrix) */
  glPopMatrix();

  /* Go into flat mode, that will render in 2d to the front of the display */
  FlatMode();
  glColor3f(1.0,1.0,1.0);
  /* Draw the console */
  console->Render(this);


  /* ************************************************************
     Draw translation information stuff - mostly for debug */
#ifdef NEVER
  {
    ostrstream tmp;
    tmp.form("POS: %4.1f, %4.1f, %4.1f", viewpoint->translation.x,
	     viewpoint->translation.y, viewpoint->translation.z)
	       << ends; 
    textrender->Pos(0, 2*textrender->size);
    textrender->Print(tmp.str());
    fcount++;
    cout << fcount << " : " 
	 << viewpoint->translation.x << " " 
	 << viewpoint->translation.y << " " 
	 << viewpoint->translation.z << " " 
	 << endl;
  }
#endif  

  /* ************************************************************
     Draw fps stuff */
  {
    ostrstream tmp;
    tmp.form("FPS: %4.1f", framerate_get()) << ends;
    textrender->Pos(Display->width-10*textrender->size, 2*textrender->size);
    textrender->Print(tmp.str());
  }
  
  /* ************************************************************
     Draw information about turn, etc (HUD) */
  {
    /* Background colored the same way as the players tank */

    /* Argh, I can not make alpha blending work? Why not? */
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glColor4f(Game->GetCurrentPlayer()->tank->color.data[0],
	      Game->GetCurrentPlayer()->tank->color.data[1],
	      Game->GetCurrentPlayer()->tank->color.data[2],
	      0.90);
    glBegin(GL_QUADS);
    glVertex2i(0,2*textrender->size);
    glVertex2i(0,0*textrender->size);
    glVertex2i(width,0*textrender->size);
    glVertex2i(width,2*textrender->size);
    glEnd();
    // glDisable(GL_BLEND); 

    /* Foreground colored complementary
    glColor3f(1.0 - Game->GetCurrentPlayer()->tank->color.red,
	      1.0 - Game->GetCurrentPlayer()->tank->color.green,
	      1.0 - Game->GetCurrentPlayer()->tank->color.blue);
    */
    
    /* Print name money and health */
    {
      ostrstream tmp;
      tmp << Game->GetCurrentPlayer()->name;
      tmp.form("'s turn. Score:%i Health:%0.1f",
	       Game->GetCurrentPlayer()->score,
	       Game->GetCurrentPlayer()->tank->health);
      tmp << ends;
      textrender->Pos(0, textrender->size);
      textrender->PrintLn(tmp.str());
    }
    {
      /* Print selected projectile, rotation, angle and force */
      ostrstream tmp;
      tmp.form("Rotation:%1.0f Angle:%1.0f Force:%1.0f Weapon:", 
	       Game->GetCurrentPlayer()->tank->cannon.rotation,
	       Game->GetCurrentPlayer()->tank->cannon.angle,
	       Game->GetCurrentPlayer()->tank->cannon.force);
      tmp << Game->GetCurrentPlayer()->inventory->DescribeSelected() 
	  << ends;
      textrender->Print(tmp.str());
    }
  }
  
  /* Increment frame counter */
  num_frames++;

  /* Debug, write viewpoint, rotation.z */
  /* cout << "TDisplay::Render() - viewpoint.rotation.z " 
     << viewpoint->rotation.z << endl; */
}

/* **********************************************************************
 * This is simple refreshrate benchmark - like Quake.
 * *********************************************************************/
void TDisplay::RefreshRate() {
  if (!viewpoint) {
    cerr << "display_refreshrate with NULL viewpoint" << endl;
  }
  int i, tmp;
  system_time_t start, end, total;
  //  sprintf(bud, "refreshrate called, numframes %i",
  //	  num_frames);
  tmp = num_frames;
  // display_add
  /* Clear the printline buffer */
  console->Clear();
  int tmp_diff = 2;
  start = system_gettime();
  for(i = 0; i < 360/tmp_diff; i++) {
    viewpoint->rotation.z+=tmp_diff;
    if (viewpoint->rotation.z>360.0) viewpoint->rotation.z-=360.0;
    framerate_add(system_gettime(), num_frames);
    /*Clear screen and set appropriate options */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* Set up a rendermode */
    NormalMode();
    /* Render */
    Render();
    /* Swap the buffers */
    glutSwapBuffers();
  }
  end = system_gettime();
  total = end - start;
  ostrstream otmp;
  otmp.form("%i frames in %5.2f secs, %4.1f frames pr. sec",
	    num_frames - tmp, total, (num_frames-tmp)/total);
  otmp << ends;
  console->AddLine(otmp.str(), total);
  // cout << otmp.str() << endl;
}

/* **********************************************************************
 * **********************************************************************
 * TDisplay, Handling commands
 * **********************************************************************
 * *********************************************************************/
bool TDisplay::CommandConsume(TCommand * Command) {
  /* **********************************************************************
   * (display)
   * *********************************************************************/
  if ("display" == Command->name) {
    if ("toggle-flymode" == Command->args) {
      if(flymode) {
	console->AddLine("Flymode off");
      } else {
	console->AddLine("Flymode on");
      }
      flymode = !flymode;
      return true;
    } 
    else if ("toggle-clipmode" == Command->args) {
      if(clipmode) {
	console->AddLine("Clipmode off");
      } else {
	console->AddLine("Clipmode on");
      }
      clipmode = !clipmode;
      return true;
    } 
    else if ("refreshrate" == Command->args) {
      RefreshRate();
      return true;
    } 
  } /* (display) */
  /* **********************************************************************
   * (render)
   * *********************************************************************/
  else if ("render" == Command->name) {
    if ("lines" == Command->args) {
      render_type = render_type_lines;
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("polygons" == Command->args) {
      render_type = render_type_triangles;
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("textures" == Command->args) {
      render_type = render_type_textures;
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("toggle-lights" == Command->args) {
      render_light = !render_light;
      if (render_light) {
	console->AddLine("Lights ON");
      } else {
	console->AddLine("Lights OFF");
      }
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("toggle-shademode" == Command->args) {
      render_shade_smooth = !render_shade_smooth;
      if (render_shade_smooth) {
	console->AddLine("Shademode : smooth");
      } else {
	console->AddLine("Shademode : flat");
      }
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("toggle-normals" == Command->args) {
      render_normals = !render_normals;
      if (render_normals) {
	console->AddLine("Normals ON");
      } else {
	console->AddLine("Normals OFF");
      }
      Game->GetMap()->Invalidate();
      return true;
    }
    else if ("toggle-markers" == Command->args) {
      spawn_markers = !spawn_markers;
      if (spawn_markers) {
	console->AddLine("Markers ON");
      } else {
	console->AddLine("Markers OFF");
      }
      return true;
    // Game->GetMap()->Invalidate();
    }
  } /* (render) */
  /* **********************************************************************
   * (toggle-pointer-grab)
   * *********************************************************************/
  else if ("toggle-pointer-grab" == Command->name) {
    grabpointer = !grabpointer;
    return true;
  }
  cerr << "TDisplay::CommandConsume, not handling ("
       << Command->name << "," << Command->args << ")" << endl;
  return false;
}
