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
#include "client.hh"
#include "color.hh"
#include "types.hh"
#include "server.hh"
#include "tank.hh"
#include "player.hh"

#include "display.hh"
#include "framerate.hh"
#include "keyboard.hh"

#include "menu.hh"
#include "menu_game.hh"
#include "menu_round.hh"
#include "menu_buy.hh"
#include "menu_score.hh"

#include <strstream>

#ifdef SOUND_ON
#include "sound.hh"
#endif

TClient * Client;

static int selfcount = 0;

#define CLIENT_HAS_NOTHING        0x00000000 /* The client has no menu/game */
#define CLIENT_HAS_GAME           0x00000001 /* The Game object needs stuff */
#define CLIENT_HAS_INGAMEMENU     0x00000002 /* In-between handling */
#define CLIENT_HAS_ROUNDOVERMENU  0x00000004 /* In-between handling */
#define CLIENT_HAS_BUYMENU        0x00000008 /* Menu for buying, displayed between games */
#define CLIENT_HAS_GAMEOVERMENU   0x00000010 /* In-between handling */
#define CLIENT_HAS_SCORE          0x00000020 /* Score board */

//#define CLIENT_HAS_CONSOLE    0x00000008 /* Nonexistant console :-) */


/* **********************************************************************
 * Callback function - need to be global or static.
 * All fiddle with the Client object.
 * *********************************************************************/

/* **********************************************************************
 * GLUT CallBack functions. 
 * *********************************************************************/

/* **********************************************************************
 * Idle typically updates the game/menu system, then posts a redisplay
 * *********************************************************************/
system_time_t last_time = 0;
void Client_Idle() {
  /* Get the current time */
  system_time_t time_now   = system_gettime();
  system_time_t delta_time = time_now - last_time;
  last_time                = time_now;
  /* The display should always be updated */
  Display->Update(delta_time);
  /* Main switch on wheter or not a game is running */
  if (Client->game_running) {
    /* Check the different elements */
    if (Client->has & CLIENT_HAS_GAME) {
      Game->Update(time_now);
      /* Check the game state, update accordingly */
      switch(Game->GetRoundState()) {
      case roundstate_not_ready:
      case roundstate_can_fire:
      case roundstate_no_fire:
	/* Do not show in-between menus */
	// Client->has &= ~(CLIENT_HAS_ROUNDOVER | CLIENT_HAS_ROUNDMENU);
	break;
      case roundstate_done:
	if (Game->IsGameOver()) {
	  Client->has |= CLIENT_HAS_GAMEOVERMENU;
	} else {
	  Client->has |= CLIENT_HAS_ROUNDOVERMENU;
	}
	/* in any case, do not update the game now */
	Client->has &= ~CLIENT_HAS_GAME;
      }
      
    }
    /* Make frames count. TODO: Should go else where? */
    framerate_add(time_now, Display->num_frames);
    
    /* Always post a redisplay - we need it to get back here, etc. */
    glutPostRedisplay();
  } else {
    /* Sleep 50 ms = 50000 usec */
    usleep(50000);
    /* TODO: Update the GameMenu */
  }
}
  

/* **********************************************************************
 * Display typically calls all elements that needs to be displayed.
 * *********************************************************************/
void Client_Display() {
  if (Client->game_running) {
    // cout << "Client state " << Client->has << endl;

    /* Display game elements */
    if (Client->has & CLIENT_HAS_GAME 
	| CLIENT_HAS_ROUNDOVERMENU 
	| CLIENT_HAS_GAMEOVERMENU
	| CLIENT_HAS_BUYMENU) {
      /*Clear screen and set appropriate options */
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* If buy menu, do not render background */
      if (Client->has & CLIENT_HAS_BUYMENU) {
	if (BuyMenu) {
	  Display->FlatMode();
	  BuyMenu->Render(0, Display->width, 0, Display->height);
	  glutSwapBuffers();
	} else {
	  cerr << "Client::Render - client has buymenu that is NULL!" << endl;
	}
	/* Do not render background or anything */
	return;
      }

      /* If score board, do not render background */
      if (Client->has & CLIENT_HAS_SCORE) {
	if (ScoreMenu) {
	  Display->FlatMode();
	  ScoreMenu->Render(0, Display->width, 0, Display->height);
	  glutSwapBuffers();
	} else {
	  cerr << "Client::Render - client has scoremenu that is NULL!" << endl;
	}
	/* Do not render background or anything */
	return;
      }
      
      /* Normal mode */
      Display->NormalMode();
      Display->viewpoint = &(Game->current_player->viewpoint);
      Display->Render();
      
      /* The different menus are only displayed if we have them */
      if (Client->has & 
	  CLIENT_HAS_ROUNDOVERMENU 
	  | CLIENT_HAS_GAMEOVERMENU 
	  | CLIENT_HAS_INGAMEMENU) {
	Display->FlatMode();
      }

      if (Client->has & CLIENT_HAS_ROUNDOVERMENU) {
	RoundOverMenu->Render(0, 640, 100, 380);
      } else {
	if (Client->has & CLIENT_HAS_GAMEOVERMENU) {
	  GameOverMenu->Render(0, 640, 100, 380);
	}
      }
      
      /* The in game menu is only displayed if we are .. in the game */
      if (Client->has & CLIENT_HAS_GAME && 
	  Client->has & CLIENT_HAS_INGAMEMENU) {
	InGameMenu->Render(0, Display->width, 0, Display->height);
      }
      /* Swap the buffers to show what we've just drawn */
      glutSwapBuffers();

      /* Print error information */
      if (GL_NO_ERROR != glGetError()) {
	cout << "GL was in error condition after swapping" << endl;
      } 
    } else {
      cerr << "TClient::Display() - Client hasn't got anything to display?" << endl;
    }  
  } else {
    /* Could be in GameMenu, or BuyMenu */
    /*Clear screen and set appropriate options */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Display->FlatMode();
    GameMenu->Render(0, Display->width, 0, Display->height);
    
    /* Swap the buffers to show what we've just drawn */
    // cout << "Swapping buffers" << endl;
    glutSwapBuffers();
  }
}

/* **********************************************************************
 * Keyboard also triggers the relevant keyboard handler
 * The keyboard stuff needs to be replaced by some kind of event handling
 * mechanism.
 * *********************************************************************/
void Client_Keyboard(unsigned char key, int x, int y) {
  if (Client->game_running) {
    if (Client->has & CLIENT_HAS_GAME) {
      /* Check for showing the in game menu */
      if (Client->has & CLIENT_HAS_INGAMEMENU) {
	/* Call menu handler */
	InGameMenu->KeyboardHandler(key);
      } else {
	if (27 == key) { /* Escape displays in game menu */
	  Client->has |= CLIENT_HAS_INGAMEMENU;
	} else {
	  /* Call game specific keyboard handler */
	  keyboard_handler(key, x, y);
	}
      }
    } else {
      bool handled;
      if (Client->has & CLIENT_HAS_ROUNDOVERMENU) {
	handled = RoundOverMenu->KeyboardHandler(key);
      } else {
	if (Client->has & CLIENT_HAS_GAMEOVERMENU) {
	  handled = GameOverMenu->KeyboardHandler(key);
	} else {
	  if (Client->has & CLIENT_HAS_BUYMENU) {
	    handled = BuyMenu->KeyboardHandler(key);
	  } else {
	    if (Client->has & CLIENT_HAS_SCORE) {
	      handled = ScoreMenu->KeyboardHandler(key);
	    }
	  }
	}
      }
      if (!handled) {
	keyboard_handler(key, x, y);
      }
      glutPostRedisplay();
    }
  } else { /* no game running */
    if (GameMenu->KeyboardHandler(key)) {
      /* The menu needs update if it handled the key */
      glutPostRedisplay();
    }
  }
  if ('q' == key) {
    cout << "TODO: Clean up stuff" << endl;
    exit(0);
  }
}

/* This is a quick hack to make up, down, left, right work in the menus */
void Client_Special(int key, int x, int y) {
  /* If displaying any kind of menu, then stuff characters */
  if (!Client->game_running 
      || (Client->has & 
	  (CLIENT_HAS_INGAMEMENU || CLIENT_HAS_ROUNDOVERMENU 
	   || CLIENT_HAS_GAMEOVERMENU || CLIENT_HAS_BUYMENU 
	   || CLIENT_HAS_SCORE))) {
    switch(key) {
    case GLUT_KEY_UP:
      Client_Keyboard('a', x, y);
      break;
    case GLUT_KEY_DOWN:
      Client_Keyboard('z', x, y);
      break;
    case GLUT_KEY_LEFT:
      Client_Keyboard(2, x, y); /* Ctrl+B */
      break;
    case GLUT_KEY_RIGHT:
      Client_Keyboard(6, x, y); /* Ctrl+F */
      break;
    }
  }
}


/* **********************************************************************
 * Mouse testing stuff
 * *********************************************************************/

void Client_Mouse(int button, int state, int x, int y) {
  cout << "Client_Mouse; button:" << button << ", state:" << state
       << ", x:" << x << ", y:" << y << endl;
}

void Client_PassiveMotion(int x, int y) {
  cout << "Client_PassiveMotion; x:" << x << ", y:" << y << endl;
}

/* **********************************************************************
 * GameMenu Callback functions
 * *********************************************************************/

void GameMenu_StartFunc() {
  /* We set our state accordingly */
  /* First a sanity check */
  if (Client->game_running || Client->has) {
    cerr << "GameMenu_StartFunc() called with wrong state!" << endl;
  }
  if (GL_NO_ERROR != glGetError()) {
    cerr << "GameMenu_StartFunc() : GL was in error condition on entering" << endl;
  } 

  /* ************************************************************
     Initialize the game. This should maybe go somewhere else */
  
  /* Initialize stuff as selected from the menu system */
  int game_numrounds = atoi(GameMenu->numrounds.c_str());
  
  int game_mapsize;
  float game_mapsteepness;
  /* Find the mapsize. 3 fixed values for now */
  if ("small" == GameMenu->mapsize) {
    game_mapsize = 16;
  } else {
    if ("medium" == GameMenu->mapsize) {
      game_mapsize = 32;
    } else {
      game_mapsize = 64;
    }
  }

  /* Find the map type */
  if ("flatlands" == GameMenu->maptype) {
    game_mapsteepness = 0.2;
  } else {
    if ("bumby" == GameMenu->maptype) {
      game_mapsteepness = 0.8;
    } else {
      if ("highlands" == GameMenu->maptype) {
	game_mapsteepness = 1.4;
      } else {
	if ("mountains" == GameMenu->maptype) {
	  game_mapsteepness = 2.0;
	}
      }
    }
  }
  

  /* Setup a game. This is pretty simple at this point. 
     max 4 players allowed */
  Game = new TGame(game_mapsize, game_mapsize, game_numrounds, game_mapsteepness);

  TTank * tank;
  TPlayer * player;

  /* There are better ways to do this */
  switch(atoi(GameMenu->numplayers.c_str())) {
    
  case 4:
    player = new TPlayer(GameMenu->playernames[3]);
    tank   = new TTank(player);
    tank->color      =  ColorDefinitions.Colors["yellow"];
    player->tank     = tank;
    if (!Game->AddPlayer(player)) {
      cerr << "Error adding player " << player->name << endl;
      //    sound_shutdown();
      exit(1);
    }

  case 3:
    player = new TPlayer(GameMenu->playernames[2]);
    tank   = new TTank(player);
    tank->color      = ColorDefinitions.Colors["blue"];;
    player->tank     = tank;
    if (!Game->AddPlayer(player)) {
      cerr << "Error adding player " << player->name << endl;
      //    sound_shutdown();
      exit(1);
    }

  default:
    player = new TPlayer(GameMenu->playernames[1]);
    tank   = new TTank(player);
    tank->color      = ColorDefinitions.Colors["green"];;
    player->tank     = tank;
    if (!Game->AddPlayer(player)) {
      cerr << "Error adding player " << player->name << endl;
      //    sound_shutdown();
      exit(1);
    }

    player = new TPlayer(GameMenu->playernames[0]);
    tank   = new TTank(player);
    tank->color      = ColorDefinitions.Colors["red"];;
    player->tank     = tank;
    if (!Game->AddPlayer(player)) {
      cerr << "Error adding player " << player->name << endl;
      //    sound_shutdown();
      exit(1);
    }
  }
      
  /* Initialize framerate counter */
  framerate_init();

  if (GL_NO_ERROR != glGetError()) {
    cerr 
      << "GameMenu_StartFunc() : GL was in error condition before Game->RoundStart" 
      << endl;
  } 
  /* Start the game/round - also set up the map */
  Game->RoundStart();
  if (GL_NO_ERROR != glGetError()) {
    cerr 
      << "GameMenu_StartFunc() : GL was in error condition after Game->RoundStart" 
      << endl;
  } 

  /* Change the state of the Client  */
  Client->game_running  = true;
  Client->has           = CLIENT_HAS_GAME;

  /* TODO: The global viewpoint should probably go */
  Display->viewpoint = &(Game->current_player->viewpoint);

  /* Actually get us going */
  if (GL_NO_ERROR != glGetError()) {
    cerr << "GameMenu_StartFunc() : GL was in error condition before exit" << endl;
  } 
  glutPostRedisplay();
  if (GL_NO_ERROR != glGetError()) {
    cerr << "GameMenu_StartFunc() : GL was in error condition on exit" << endl;
  } 
}

/* This function should shut everything down properly. Since our stuff
   is actually in the Client object, let us shut it down, then exit */
void GameMenu_EndFunc() {
  delete Client;
  cout << "GameMenu_EndFunc - need to clean up" << endl;
  exit(0);
}

/* **********************************************************************
 * BuyMenu stuff
 * This creates and show a menu for each player
 * Each time the player buys something, the menu is "budgeted"
 * Each time we change players, we need to recreate the menu
 * *********************************************************************/

/* Variables used for these menuts */
TPlayerInfosIterator PlayerInfosIterator;
bool FirstDone;

/* Update the buymenu */
void BuyMenu_Budget() {
  // cout << "BuyMenu_Budget" << endl;
  if (BuyMenu) {
    /* Need to construct new caption for this */
    ostrstream tmp;
    tmp << (*PlayerInfosIterator)->player->name << " - " 
	<< (*PlayerInfosIterator)->player->money << ends; 
    BuyMenu->Budget(tmp.str());
  } else {
    cerr << "BuyMenu_Budget - no buymenu!" << endl;
  }
}
/* Go to next - may stop it all if displaying last player */
void BuyMenu_Continue() {
  // cout << "BuyMenu_Continue" << endl;
  if (BuyMenu) {
    delete BuyMenu;
    BuyMenu = NULL;
  }
  /* Prepare for the first player, if not this one */
  if (FirstDone) {
    PlayerInfosIterator++;
  } else {
    FirstDone = true;
  }
  if (PlayerInfosIterator != Game->GetPlayerInfos()->end()) {
    /* Create new BuyMenu, based on player information */
    ostrstream tmp;
    tmp << (*PlayerInfosIterator)->player->name << " - " 
	<< (*PlayerInfosIterator)->player->money << ends; 
    BuyMenu = new TBuyMenu(tmp.str(), 
			   (*PlayerInfosIterator)->player->inventory,
			   &((*PlayerInfosIterator)->player->money), 
			   BuyMenu_Continue, 
			   BuyMenu_Budget);
    BuyMenu->Show();
    Client->has |= CLIENT_HAS_BUYMENU;
  } else {
    /* Last player - start another round*/
    Game->RoundStart();
    Client->has |= CLIENT_HAS_GAME;  
    Client->has &= ~CLIENT_HAS_BUYMENU;
  }
} 

/* Init the client iteration */
void BuyMenu_Go() {
  // cout << "BuyMenu_Go" << endl;
  PlayerInfosIterator = Game->GetPlayerInfos()->begin();
  FirstDone = false; /* First one is special */
  BuyMenu_Continue();
}

/* **********************************************************************
 * The Score board menu
 * *********************************************************************/
/* This shuts down the game */
void ScoreMenuFunc() {
  Client->game_running = false;
  Client->has          = CLIENT_HAS_NOTHING;
  if (BuyMenu) {
    delete BuyMenu;
    BuyMenu = NULL;
  }
  if (ScoreMenu) {
    delete ScoreMenu;
    ScoreMenu = NULL;
  }
  delete Game;
}

/* This sets up a score menu */
void ScoreMenu_Go() {
  if (ScoreMenu) {
    delete ScoreMenu;
    ScoreMenu = NULL;
  }
  ScoreMenu = new TScoreMenu("Score", Game->GetPlayerInfos(), ScoreMenuFunc);
  ScoreMenu->Show();
  Client->has |= CLIENT_HAS_SCORE;
}


/* **********************************************************************
 * RoundMenu Callback functions
 * *********************************************************************/

/* This gets called, when the client continues after a round is over */ 
void RoundOverMenuFunc() {
  Client->has &= ~CLIENT_HAS_ROUNDOVERMENU;
  /* Need to set up stuff to iterating through players in the buying section */
  BuyMenu_Go();
}

/* **********************************************************************
 * GameOverMenu Callback functions
 * *********************************************************************/

/* This gets called, when the client continues after a game is over, and 
   will display the score board */ 
void GameOverMenuFunc() {
  // cout << "GameOverMenuFunc - displaying game menu" << endl;
  Client->has &= ~CLIENT_HAS_GAMEOVERMENU;
  /* Set up the score board menu */
  ScoreMenu_Go();
}

/* **********************************************************************
 * InGameMenu Callback functions
 * *********************************************************************/

/* Abort a round - let the game do it, then remove the menu */
void InGameMenu_AbortRoundFunc() {
  Game->AbortRound();
  Client->has &= ~CLIENT_HAS_INGAMEMENU;
}

/* Abort a game - remove the client, then stop the game */
void InGameMenu_AbortGameFunc() {
  /* TODO: Should the score board be displayed? */
  Client->game_running = false;
  Client->has          = CLIENT_HAS_NOTHING;
  delete Game;
  glutPostRedisplay();
}

/* Cancel simply removes the menu */
void InGameMenu_CancelFunc() {
  Client->has &= ~CLIENT_HAS_INGAMEMENU;
}



/* **********************************************************************
 * Client - Abstracts the client side of the game 
 * *********************************************************************/
TClient::TClient(int argc, char ** argv) {
  // cout << "TClient::TClient" << endl;
  
  /* Set our variables */
  game_running = false;
  has          = CLIENT_HAS_NOTHING;

  /* Check that we never get instantiated more the once */
  if (selfcount != 0) {
    cerr << "TClient must never be instantiated more then once" << endl;
    exit(1);
  }
  selfcount++;

  /* Initialize SOUND system */
#ifdef SOUND_ON
#warning "LAME SOUND CODE ON"
  if (sound_init("/dev/dsp", 8)) { /* 1 == NUMSOUNDS */
    cerr << "Could not initialize sound" << endl;
  }
  /* load a single sound */
  char * sounds[] = {
    "data/sounds/explosion.raw",
    "data/sounds/explosion2.raw",
    "data/sounds/fire.raw",
    "data/sounds/menu_back_menu.raw",
    "data/sounds/menu_deselect.raw",
    "data/sounds/menu_forward_menu.raw",
    "data/sounds/menu_move.raw",
    "data/sounds/menu_select.raw",
  };
  for (int i = 0; i < 8; i++) {
    if (0 != sound_load(i, sounds[i])) {
      cerr << "Error loading sound " << sounds[i] << endl;
    }
  }
#endif
  
  /* Initialize DISPLAY system */
  Display = new TDisplay(argc, argv);
  
  /* Initialize MENU system */
  /* First the Text render system */
  MenuTextRender.Load("data/graphics/fonts/fontGray.tga");
  /* Init the Game Menu */
  GameMenu = new TGameMenu("Tank Hill eXtreme", 
			   GameMenu_StartFunc, 
			   GameMenu_EndFunc);
  
  /* The Menus are always visible, but will not always be rendered */
  GameMenu->Show();

  /* The round over menu */
  RoundOverMenu = new TContinueMenu("Round over", RoundOverMenuFunc);
  RoundOverMenu->Show();

  /* The Game over menu */
  GameOverMenu = new TContinueMenu("Game over", GameOverMenuFunc);
  GameOverMenu->Show();

  /* The in game menu */
  InGameMenu = new TInGameMenu("Options", InGameMenu_AbortGameFunc, 
			       InGameMenu_AbortRoundFunc, 
			       InGameMenu_CancelFunc);
  InGameMenu->Show();

  /* The BuyMenu and ScoreMenu is not constructed here */
}

/* **********************************************************************
   Phony NULL function 
 * *********************************************************************/

void MyNULL() {};

/* **********************************************************************
   Destructor shuts stuff down 
 * *********************************************************************/
TClient::~TClient() {
  /* Stop GLUT */
  glutDisplayFunc(MyNULL);
  glutKeyboardFunc(NULL);
  glutSpecialFunc(NULL);
  glutIdleFunc(NULL);
  glutReshapeFunc(NULL);
  glutMouseFunc(NULL);
  glutPassiveMotionFunc(NULL);

  /* Free the menus */
  delete InGameMenu; InGameMenu = NULL;
  delete GameOverMenu; GameOverMenu = NULL;
  delete RoundOverMenu; RoundOverMenu = NULL;
  delete GameMenu; GameMenu = NULL;
  if (NULL != BuyMenu) {
    delete BuyMenu; BuyMenu = NULL;
  }
  /* Free the displays */
  delete Display; Display = NULL;
#ifdef SOUND_ON
  /* Shutdown the sound */
  sound_shutdown();
#endif
}

/* **********************************************************************
   Reshape the display 
 * *********************************************************************/ 
void Client_Reshape(int w, int h) {
  Display->width=w;
  Display->height=h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

/* **********************************************************************
   Run - Setup the GLUT s
 * *********************************************************************/
void TClient::Run() {
  /* Set up glut */
  glutDisplayFunc(Client_Display);
  glutKeyboardFunc(Client_Keyboard);
  glutSpecialFunc(Client_Special);
  glutIdleFunc(Client_Idle);
  glutReshapeFunc(Client_Reshape);
  //  glutMouseFunc(Client_Mouse);
  //  glutPassiveMotionFunc(Client_PassiveMotion);
  
  /* Request fullscreen */
  // If fullscreen is enabled, my 3dfx breaks.
  // glutFullScreen();
  
  /* Start GLUT mainloop */
  glutMainLoop();
}
