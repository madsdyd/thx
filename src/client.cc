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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <strstream>
#include "client.hh"
#include "color.hh"
#include "types.hh"
#include "server.hh"
#include "tank.hh"
#include "player.hh"
#include "aiplayer.hh"
#include "display.hh"
#include "framerate.hh"
#include "menu.hh"
#include "menu_game.hh"
#include "menu_round.hh"
#include "menu_buy.hh"
#include "menu_score.hh"
#include "menu_controls.hh"
#include "inputconsumer.hh"
#include "command.hh"
#include "commandconsumer.hh"
#include "sound.hh"
#include "aifactory.hh"

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
 * All fiddle with the Client objexct.
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
  /* Make sure the first update uses 0 time */
  if (0 == last_time) { last_time = time_now; };
  system_time_t delta_time = time_now - last_time;
  last_time                = time_now;
  /* Empty the command chain */
  InputToCommand.Consume();
  CommandDispatcher.Dispatch();

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
	  if (!GameMode.SetMode(gamemode_menu)) {
	    cerr << "TClient_Idle - unable to change mode!" << endl;
	  }
	  GameOverMenu->Show();
	} else {
	  Client->has |= CLIENT_HAS_ROUNDOVERMENU;
	  if (!GameMode.SetMode(gamemode_menu)) {
	    cerr << "TClient_Idle - unable to change mode!" << endl;
	  }
	  RoundOverMenu->Show();
	}
	/* in any case, do not update the game now */
	Client->has &= ~CLIENT_HAS_GAME;
      }
      
    }
    /* Make frames count. TODO: Should go else where? */
    framerate_add(time_now, Display->GetNumFrames());
  } 
  /* Always post a redisplay - either the menu or game needs to update */
  glutPostRedisplay();
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
	  BuyMenu->Render(0, Display->GetWidth(), 0, Display->GetHeight());
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
	  ScoreMenu->Render(0, Display->GetWidth(), 0, Display->GetHeight());
	  glutSwapBuffers();
	} else {
	  cerr << "Client::Render - client has scoremenu that is NULL!" << endl;
	}
	/* Do not render background or anything */
	return;
      }
      
      /* Normal mode */
      Display->NormalMode();
      Display->viewpoint = &(Game->GetCurrentPlayer()->viewpoint);
      Display->Render();
      
      /* The different menus are only displayed if we have them */
      if (Client->has & 
	  CLIENT_HAS_ROUNDOVERMENU 
	  | CLIENT_HAS_GAMEOVERMENU 
	  | CLIENT_HAS_INGAMEMENU) {
	Display->FlatMode();
      }

      // TODO: Fix 100 offset dependencies
      if (Client->has & CLIENT_HAS_ROUNDOVERMENU) {
	RoundOverMenu->Render(0, Display->GetWidth(), 100, Display->GetHeight()-100);
      } else {
	if (Client->has & CLIENT_HAS_GAMEOVERMENU) {
	  GameOverMenu->Render(0, Display->GetWidth(), 100, Display->GetHeight()-100);
	}
      }
      
      /* The in game menu is only displayed if we are .. in the game */
      if (Client->has & CLIENT_HAS_GAME && 
	  Client->has & CLIENT_HAS_INGAMEMENU) {
	InGameMenu->Render(0, Display->GetWidth(), 0, Display->GetHeight());
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
    GameMenu->Render(0, Display->GetWidth(), 0, Display->GetHeight());
    
    /* Swap the buffers to show what we've just drawn */
    // cout << "Swapping buffers" << endl;
    glutSwapBuffers();
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
// TODO: Convert this to command structure?
void GameMenu_StartFunc() {
  /* We set our state accordingly */
  /* First a sanity check */
  if (GameMode.GetMode() == gamemode_game) {
    cerr << "GameMenu_StartFunc - already in game mode!" << endl;
    return;
  }
  if (!GameMode.SetMode(gamemode_game)) {
    cerr << "GameMenu_StartFunc - error changing gamemode!" << endl;
    return;
  }
  GameMenu->GetCurrentMenu()->Hide();
  if (Client->game_running || Client->has) {
    cerr << "GameMenu_StartFunc() called with wrong state!" << endl;
  }
  if (GL_NO_ERROR != glGetError()) {
    cerr << "GameMenu_StartFunc() : GL was in error condition on entering"
	 << endl;
  } 

  /* ************************************************************
     Initialize the game. This should maybe go somewhere else */
  
  /* Setup a game, with values from the GameMenu */
  Game = new TGame(GameMenu->mapsize, GameMenu->mapsize, 
		   GameMenu->numrounds, GameMenu->maptype,
		   GameMenu->teammode);

  /* Add the players to the game */
  TTank * tank;
  TPlayer * player;
  for (unsigned int i = 0; i < GameMenu->numplayers; i++) {
    /* cout << "adding a player; i = " << i << "numplayers" 
       << GameMenu->numplayers << endl;  */
    if (GameMenu->PlayerSettings[i].playerclass == "human") {
      player     = new TPlayer(Game, GameMenu->PlayerSettings[i].name,
			       GameMenu->PlayerSettings[i].team);
      cout << "Creating a normal player (TPlayer)" << endl;
    } else {
      player     = AIFactory.CreateAIPlayer(GameMenu->PlayerSettings[i].playerclass, 
					    Game, 
					    GameMenu->PlayerSettings[i].name,
					    GameMenu->PlayerSettings[i].team);
      cout << "Creating an AI player" << endl;
    }
    tank         = new TTank(Game, player);
    player->tank = tank;
    tank->color  = GameMenu->PlayerSettings[i].color;
    if (!Game->AddPlayer(player)) {
      cerr << "Error adding player " << player->name << endl;
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
  Display->viewpoint = &(Game->GetCurrentPlayer()->viewpoint);

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
  /* Only show stuff for interactive players */
  while ((PlayerInfosIterator != Game->GetPlayerInfos()->end())
	 &&  (!(*PlayerInfosIterator)->player->NeedsInteraction())) {
    PlayerInfosIterator++;
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
    if (!GameMode.SetMode(gamemode_game)) {
      cerr << "BuyMenu_Continue - unable to change gamemode!" << endl;
    }
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
  GameMenu->Show();
}

/* This sets up a score menu */
void ScoreMenu_Go() {
  if (ScoreMenu) {
    delete ScoreMenu;
    ScoreMenu = NULL;
  }
  ScoreMenu = new TScoreMenu("Score", Game->GetPlayerInfos(), 
			     Game->GetTeamMode(), ScoreMenuFunc);
  ScoreMenu->Show();
  Client->has |= CLIENT_HAS_SCORE;
}


/* **********************************************************************
 * RoundMenu Callback functions
 * *********************************************************************/

/* This gets called, when the client continues after a round is over */ 
void RoundOverMenuFunc() {
  Client->has &= ~CLIENT_HAS_ROUNDOVERMENU;
  RoundOverMenu->Hide();
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
  GameOverMenu->Hide();
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
  InGameMenu->Hide();
}

/* Abort a game - remove the client, then stop the game */
void InGameMenu_AbortGameFunc() {
  /* TODO: Should the score board be displayed? */
  Game->AbortRound();
  Client->game_running = false;
  Client->has          = CLIENT_HAS_NOTHING;
  InGameMenu->Hide();
  delete Game;
  GameMenu->Show();
  glutPostRedisplay();
}

/* Cancel simply removes the menu */
void InGameMenu_CancelFunc() {
  Client->has &= ~CLIENT_HAS_INGAMEMENU;
  InGameMenu->Hide();
  if (!GameMode.SetMode(gamemode_game)) {
    cerr << "InGameMenu_CancelFunc - unable to change gamemode!" << endl;
  }
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
  if (sound_init("/dev/dsp", 8)) { /* 1 == NUMSOUNDS */
    cerr << "Could not initialize sound" << endl;
  }
  /* load a single sound */
  char * sounds[] = {
    THX_DATADIR "/sounds/explosion.raw",
    THX_DATADIR "/sounds/explosion2.raw",
    THX_DATADIR "/sounds/fire.raw",
    THX_DATADIR "/sounds/menu_back_menu.raw",
    THX_DATADIR "/sounds/menu_deselect.raw",
    THX_DATADIR "/sounds/menu_forward_menu.raw",
    THX_DATADIR "/sounds/menu_move.raw",
    THX_DATADIR "/sounds/menu_select.raw",
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
  MenuTextRender.Load(THX_DATADIR "/graphics/fonts/fontGray.tga");
  /* Init the controls menu */
  ControlsMenu = new TControlsMenu("Controls");

  /* Init the Game Menu */
  GameMenu = new TGameMenu("Tank Hill eXtreme", 
			   GameMenu_StartFunc, 
			   GameMenu_EndFunc);

  // TODO: This needs fixing wrt to menus
  /* The Menus are always visible, but will not always be rendered */
  GameMenu->Show();

  /* The round over menu */
  RoundOverMenu = new TContinueMenu("Round over", RoundOverMenuFunc);
  // RoundOverMenu->Show();

  /* The Game over menu */
  GameOverMenu = new TContinueMenu("Game over", GameOverMenuFunc);
  // GameOverMenu->Show();

  /* The in game menu */
  InGameMenu = new TInGameMenu("Options", InGameMenu_AbortGameFunc, 
			       InGameMenu_AbortRoundFunc, 
			       InGameMenu_CancelFunc);
  // InGameMenu->Show();

  /* The BuyMenu and ScoreMenu is not constructed here */

  /* Register the commands we handle */
  CommandDispatcher.RegisterConsumer("quit", this);
  CommandDispatcher.RegisterConsumer("in-game-menu-show", this);
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
  glutIdleFunc(NULL);

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
  /* Unregister commands */
  CommandDispatcher.UnregisterConsumer("quit");
  CommandDispatcher.UnregisterConsumer("in-game-menu-show");

  /* Stop the input system */
  InputToCommand.Shutdown();
}

/* **********************************************************************
   Run - Setup the GLUT s
 * *********************************************************************/
void TClient::Run() {
  /* Set up glut */
  glutDisplayFunc(Client_Display);
  glutIdleFunc(Client_Idle);

  /* Initializing the input event system */
  InputToCommand.Init();
  
  /* Request fullscreen */
  // If fullscreen is enabled, my 3dfx breaks.
  // glutFullScreen();
  
  /* Start GLUT mainloop */
  glutMainLoop();
}

/* **********************************************************************
 * The Client command handler
 * *********************************************************************/
bool TClient::CommandConsume(TCommand * Command) {
  /*  cout  << "TClient::CommandConsumer called for command (" 
      << Command->name << "," << Command->args << ")" << endl; */
  if ("quit" == Command->name) {
    // TODO: Is this the right way to leave the game?
    delete this;
    exit(0);
  } else if ("in-game-menu-show" == Command->name) {
    if (GameMode.SetMode(gamemode_menu)) {
      // TODO: Maintaining 2 status fields?? This is needed pt. for rendering
      has |= CLIENT_HAS_INGAMEMENU;
      InGameMenu->Show();
#ifdef SOUND_ON
      sound_play(names_to_nums[THX_DATADIR "/sounds/menu_forward_menu.raw"]);
#endif
      return true;
    }
    cerr << "TClient::CommandConsume - error setting gamemode!" << endl;
    return false;
  } else {
    cerr << "TClient::CommandConsume non registered command! " 
	 << Command->name << endl;
    return false;
  } 
} 
