#ifndef __GAME_HH__
#define __GAME_HH__
#include <vector>

/* Other includes */
#include "types.hh"
#include "map.hh"
#include "player.hh"

#include "entities.hh"
#include "explosion.hh"
/* This module provides the actual game and gamestate
   and includes procedure that allow clients to connect to it, 
   create a game (create a map), let other clients connect to it, 
   start the game, etc. A server could host several games 
   
   A game is a number of maps with the same participiants.
   A game also includes all players. 
   A game has several rounds, each consisting of several "turns".
*/

/* Entities: 
   Entities are inserted into a list, and any update method they may have, 
   is called during each game_update.
   The value returned by update is used to determine if an entity should be
   removed during the next update of the game state: TRUE means that the
   entity wants future updates (and still is alive), FALSE that it wants no
   future updates. The game_update method will remove any entity that has
   been marked as dead during the next update. 
   There is a method that can be used to insert entities into the game.

   A typical usage is that a projectile is inserted into the state.
   update on the projectile is called for each frame. When a collision with
   the ground is detected, the projectile adds a "explosion" entity to the 
   game, and returns false from its update. During the next update, the
   projectile is removed from the game state.
*/

/* The state of the game */
typedef enum {gamestate_not_ready,     /* Freshly started, unitialized */
	      gamestate_joining,       /* Players can still join this game */
	      //	      gamestate_round_ready,   /* We are ready to start a level */
	      gamestate_round_running, /* A level is running */
	      gamestate_round_done,    /* A level has just ended */
	      gamestate_done           /* The game is over */
} gamestate_t;

/* The state of a turn */
typedef enum {roundstate_not_ready,    /* We can not begin the level */
	      roundstate_can_fire,     /* The level is started, firering allowed */
	      roundstate_no_fire,      /* No firering is allowed */
	      roundstate_done          /* Current level has ended */
} roundstate_t;

class TPlayerInfo {
public:
  int num_turn_fired;    /* The number of the last turn where the player fired */
  bool active;           /* Wheter the player is alive this round */
  TPlayer * player;      /* Pointer to the "real" TPlayer structure */
  TPlayerInfo(TPlayer * np) {
    player = np;
  }
};

typedef std::vector<TPlayerInfo *> TPlayerInfos;
typedef TPlayerInfos::iterator TPlayerInfosIterator;

class TGame {
private:
  /* GAME related stuff */
  /* These are for each time we wish to make a new map */
  gamestate_t gamestate;
  system_time_t last_update_time;

  TMap * map;
  int width, lenght;
  void SetMap();             /* Creates a new map and stores it in map */

  /* The player infos.  */
  TPlayerInfos playerInfos;
  int num_players;

  /* The entities */
  TEntities entities;

  /* round related stuff */
  int num_rounds;            /* The total number of rounds in the game */
  int num_this_round;        /* The number of this round */
  roundstate_t roundstate;   /* The state of the current round */

  /* turn related stuff */
  int num_this_turn;
  int current_num_player;    /* -1 for no one set */
  int num_stoppers;          /* The number of "active" entities
				that stops us from ending this turn 
				This number is increased when inserting 
				entities, and decreased when removing entities
				(in update). */
  
  /* Do something about players */
  int UpdateActivePlayers(); /* Update active, return number of */
  void NextPlayer();         /* Check turn rotation and end of */

  /* These update functions are used when we are in different states */
  void DoUpdateRoundRunning(system_time_t updatetime);
  void UpdateRoundRunning(system_time_t updatetime);
  void UpdateRoundDone(system_time_t updatetime);
  void UpdateGameDone(system_time_t updatetime);

public:
  TPlayer * current_player;  /* Pointer to current player.
				Accessed from keyboard.c :-/ */
  TGame(int nwidth, int nlenght, int max_num);
  ~TGame();
  TMap * GetMap();
  bool AddPlayer(TPlayer * player);
  /* Return a list of the playerinfos - used for menu manipulation */
  TPlayerInfos * GetPlayerInfos() {
    return &playerInfos;
  }
  
  /* Adds an entity to the game. Ownership _is_ taken, unless the entities
     "free" attribute is false */
  void AddEntity(TEntity * Entity);
  void RenderEntities(TViewpoint * viewpoint);
  /* Will drop all entities, freeing those that have free=true */
  void ClearEntities();

  bool FireProjectile();
  void Explosion(TExplosion * Explosion);

  /* Start a round */
  bool RoundStart();
  /* Abort current round - may end game */
  void AbortRound();
  void Update(system_time_t updatetime);
  /* Stuff that helps us get the state of the game */
  roundstate_t GetRoundState() { return roundstate; };
  /* Return true if the game is game over 
     Also updates the "active" on the players. */
  bool IsGameOver();
};
#endif
