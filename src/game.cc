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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef SOUND_ON
#include "sound.hh"
#endif

#include "game.hh"
#include "player.hh"
#include "map.hh"
#include "projectile.hh"
#include "explosion.hh"
#include "tank.hh"
#include "inventory.hh"
#include "command.hh"

/* The deltatime min and max are used like this:

max_deltatime:
  if we have an update where there has been longer then this since the last time
  then we will divide the time into slots that have this length, and update the
  game state accordingly. 

min_deltatime:
  if the resudial from above or the time passed is greater then this, then we 
  will have an update with the time. 

The big question is wheter min and max should always be the same? If they are
the same, the game state will be updated in simulated time that equals this step.
If they are not, the game state may not be updated in equal slots, and it may be
something like this with 15-17 ms between each call to TGame::Update, and max=10
min = 5

If the first to calls are 16, 17 ms "behind", the simulated time will pass in 
intervals like this then:

10 ms, 6 ms, 10 ms, 7 ms, 

Of course, the totally rigth way to do it is to use differential and integral equations
and solve them, instead of iterating, but who cares to write the maths?

*/

float max_deltatime = 1.0/20.0; /* 20 Hz */
float min_deltatime = 1.0/100.0; /* 100 Hz */

/* **********************************************************************
 * **********************************************************************
 * Initial stuff
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
   Constructor. Initializes everything, including roundstate, etc. */
TGame::TGame(int nwidth, int nlenght, int nnum_rounds, 
	     float nmapsteepness, bool nteammode) {
  /* Setup the game state */
  width               = nwidth;
  lenght              = nlenght;
  mapsteepness        = nmapsteepness;
  teammode            = nteammode;
  gamestate           = gamestate_joining;
  num_players         = 0;
  current_player      = NULL;
  map                 = NULL;
  last_update_time    = system_gettime(); /* Probably superflous */

  /* Round state */
  num_rounds          = nnum_rounds;
  num_this_round      = 0;
  roundstate          = roundstate_not_ready;

  /* Wind init */
  wind.x = 1.0;
  wind.y = 1.0;
  wind.z = 0.0;
  
}

TGame::~TGame() {
  CommandDispatcher.UnregisterConsumer("fire");
  cout << "~TGame needs to clean up a lot of stuff" << endl;
}

/* **********************************************************************
 * Add a player to the game
 * Various values will be reset for each level
 * *********************************************************************/
bool TGame::AddPlayer(TPlayer * player) {
  if (gamestate_joining == gamestate) {
    /* Add to players */
    playerInfos.push_back(new TPlayerInfo(player));
    num_players++;
    /* Tanks are added to entities in RoundStart() */
    return true;
  } else {
    cerr << "TGame::AddPlayer called, wrong state " << endl;
    return false;
  }
}

/* **********************************************************************
 * Get a pointer to the current player
 * *********************************************************************/
TPlayer * TGame::GetCurrentPlayer() {
  if (!current_player) {
    cerr << "TGame::GetCurrentPlayer - current player is NULL!" << endl;
  }
  return current_player;
}

/* **********************************************************************
 * **********************************************************************
 * Update operations
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * DoUpdateRoundRunning()
 * called from UpdateRoundRunning() with rather "homogenous" deltatimes 
 * to ensure smooth animation on "slower" systems
 * *********************************************************************/
void TGame::DoUpdateRoundRunning(system_time_t deltatime) {
  /* Check if we need to change the turn of the players. */
  if (roundstate_no_fire == roundstate) {
    if (0 == num_stoppers) {
      /* Do the next player thingy */
      NextPlayer();
    }
  }
  
  /* Update the current player - to allow for smooth command handling, etc.  */
  current_player->Update(last_update_time + deltatime);

  /* Remove or update entities */
  TEntitiesIterator End = entities.end();
  TEntitiesIterator tmp = NULL;
  for (TEntitiesIterator i = entities.begin(); i != End;) {
    /* If we keep it, update it, otherwise remove it */
    if ((*i)->keep) {
      // cout << "Calling update on TEntity" << endl;
      (*i)->Update(deltatime);
      i++;
    } else {
      tmp = i;
      i++;
      // cout << "Removing entity from entities" << endl;
      /* This could be a stopper */
      if ((*tmp)->affect_turn) {
	num_stoppers--;
	// cout << "TGame::DoUpdate, decreased num_stoppers to " << num_stoppers << endl;
      }
      
      /* Delete it, if we do not reference it any where else */
      if ((*tmp)->free) {
	delete (*tmp);
      }
      /* Erase it from our list of entities.
         This could mean that with low fps, we could spawn an entity 
         that we would never see before it was removed again. 
         Maybe this is not so good?
      */
      entities.erase(tmp);
    }
  }
}

/* **********************************************************************
   UpdateRoundRunning() - Its role is mostly to smooth out the delta time 
   and then call DoUpdateRoundRunning() that does the actual updates on 
   entities, etc */
void TGame::UpdateRoundRunning(system_time_t updatetime) {
  system_time_t deltatime = updatetime - last_update_time;
  // cout << "TGame::Update" << endl;  
  /* This part fixes very slow delta updates */
  while (deltatime > max_deltatime) {
    DoUpdateRoundRunning(max_deltatime);
    last_update_time += max_deltatime;
    deltatime  -= max_deltatime;
  }
  /* This part takes care of the remainder and or fast updates */
  if (deltatime > min_deltatime) {
    DoUpdateRoundRunning(deltatime);
    /* Adjust the game update time */
    last_update_time = updatetime;
  }
}

/* **********************************************************************
   This is the update function used when the round is done */
void TGame::UpdateRoundDone(system_time_t updatetime) {
  cout << "TGame::UpdateRoundDone called" << endl;
}

/* **********************************************************************
   This is the update function used when the game is done */
void TGame::UpdateGameDone(system_time_t updatetime) {
  cout << "TGame::UpdateGameDone called" << endl;
}

/* **********************************************************************
   Update calls the different update functions, according to the state of 
   the game. */

void TGame::Update(system_time_t updatetime) {
  // cout << "TGame::Update(" << updatetime << ")" << endl;
  switch(gamestate) {
  case gamestate_round_running:
    UpdateRoundRunning(updatetime);
    break;
  case gamestate_round_done:
    UpdateRoundDone(updatetime);
    break;
  case gamestate_done:
    UpdateGameDone(updatetime);
    break;
  default:
    cerr << "TGame::Update called with unhandled game state" << endl;
  }


}


/* **********************************************************************
 * **********************************************************************
 * Entity stuff
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Add an entity to the game
 * *********************************************************************/
void TGame::AddEntity(TEntity * Entity) {
  // cout << "TGame::AddEntity" << endl;
  entities.push_back(Entity);
  if (Entity->affect_turn) {
    num_stoppers++;
    // cout << "TGame::AddEntity, increased num_stoppers to " << num_stoppers << endl;
  }
}

/* **********************************************************************
 * Will drop all entities, freeing those that have free=true
 * *********************************************************************/
void TGame::ClearEntities() {
  TEntitiesIterator End = entities.end();
  TEntitiesIterator tmp = NULL;
  for (TEntitiesIterator i = entities.begin(); i != End;) {
    tmp = i;
    i++;
    /* Delete it, if we do not reference it any where else */
    if ((*tmp)->free) {
      delete (*tmp);
    }
    entities.erase(tmp);
  }
  num_stoppers = 0;
}

/* **********************************************************************
 * Render each entity in the game from the viewpoint
 * *********************************************************************/
void TGame::RenderEntities(TViewpoint * viewpoint) {
  TEntitiesIterator End = entities.end();
  for (TEntitiesIterator i = entities.begin(); i != End; i++) {
    (*i)->Render(viewpoint);
  }
}

/* **********************************************************************
 * Make the entity passed as an argument affect the current turn
 * *********************************************************************/
void TGame::AddAffectTurn(TEntity * Entity) {
  // TODO: Look it up in the entities
  /* Sanity check */
  if (Entity->affect_turn) {
    cout << "TGame::AddAffectTurn - entity already counted" << endl;
    return;
  }
  /* Make sure we count it */
  num_stoppers++;
  /* Toogling the flag makes sure we catch this entity when removing it */
  Entity->affect_turn = true;
}

/* **********************************************************************
 * Make the entity passed as an argument NOT affect the current turn
 * *********************************************************************/
void TGame::RemoveAffectTurn(TEntity * Entity) {
  // TODO: Look it up in the entities
  /* Sanity check */
  if (!Entity->affect_turn) {
    cout << "TGame::AddAffectTurn - entity not counted" << endl;
    return;
  }
  /* Make sure we count it */
  num_stoppers--;
  /* Toogling the flag makes sure we don't catch this entity when
     removing it */
  Entity->affect_turn = false;
}

/* **********************************************************************
 * Fire a projectile
 * *********************************************************************/
bool TGame::FireProjectile() {
  /* The current player can insert a projectile, if we are in 
     roundstate_can_fire, and his num_fired < num_this_round.
     The state of the level then changes to roundstate_no_fire.  */
  if(roundstate_can_fire != roundstate) {
    // cerr << "TGame::FireProjectile, not in roundstate_can_fire" << endl;
    return false;
  }
  
  if(num_this_turn <= playerInfos[current_num_player]->num_turn_fired) {
    /* The player has already fired this round */
    cerr << "TGame::FireProjectile, current_player has already!" 
	 << endl;
    return false;
  }

  /* Insert a projectile */
  /* Set projectile loc/vel from the current players angle and rotation */
  float dir_x, dir_y;
  TTank * tank  = current_player->tank;
  
  TVector location, velocity;
  
  /* Initialize the projectile to the position of the tank fireing it */
  location.x = tank->location.x;  
  location.y = tank->location.y;
  /* TODO: This hack makes sure that we are always above the map */
  location.z = GetMap()->HeightAt(location.x, location.y) + 1.0;
  
  /* The velocity is determined as the direcion, angle and force */
  dir_x = cos(tank->cannon.rotation * M_PI / 180.0);
  dir_y = sin(tank->cannon.rotation * M_PI / 180.0);
  
  velocity.z = sin(tank->cannon.angle * M_PI / 180.0) 
    * tank->cannon.force;
  /* By doing a cos, the x and y directions are scaled to the angle */
  velocity.x = cos(tank->cannon.angle * M_PI / 180.0)  
    * dir_x * tank->cannon.force;
  velocity.y = cos(tank->cannon.angle * M_PI / 180.0)  
    * dir_y * tank->cannon.force;
  
  /* Insert projectile into the entities */
#ifdef SOUND_ON
  sound_play(names_to_nums["data/sounds/fire.raw"]);
#endif
  AddEntity(current_player->inventory->FireSelected(&location, &velocity));

  /* Make sure this player can not fire again this round */
  playerInfos[current_num_player]->num_turn_fired = num_this_turn;

  /* Make sure that other players can not shoot either */
  roundstate = roundstate_no_fire;

  /* Getting the next player, etc, is handled in update(s) */
  return true;
}

/* **********************************************************************
 * **********************************************************************
 * Map stuff
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Set and get the map
 * *********************************************************************/
void TGame::SetMap() {
  if (map) { 
    delete map; /* Free old map */
  }
  map = new TMap(width, lenght, mapsteepness);
  map->InitRender();
}

/* **********************************************************************
 * GetMap - simply return a pointer to the map
 * *********************************************************************/
TMap * TGame::GetMap() {
  return map;
}

/* **********************************************************************
 * An explosion 
 * Handles impact on the map, tank damage, etc.
 * *********************************************************************/
void TGame::Explosion(TExplosion * Explosion) {
  int i;
  /* Make impact on the map */
  map->Explosion(Explosion);
  /* Apply effect of explosion to each tank, then lower its position to 
     the landscape */
  for (i = 0; i < num_players; i++) {
    playerInfos[i]->player->tank->Explosion(Explosion);
  }
}

/* **********************************************************************
 * **********************************************************************
 * Turn management (Also a little round)
 * **********************************************************************
 * *********************************************************************/

/* **********************************************************************
 * Start the level
 * *********************************************************************/
bool TGame::RoundStart() {
  /* If first or following round */

  if ((gamestate_joining == gamestate
       && roundstate_not_ready == roundstate) /* First round */
      || (gamestate_round_done == gamestate 
	  && roundstate_done == roundstate)) {
    /* Clean out the entities */
    ClearEntities();

    /* Get a fresh map */
    if (GL_NO_ERROR != glGetError()) {
      cerr << "TGame::RoundStart : GL was in error condition before SetMap" << endl;
    } 
    SetMap();
    if (GL_NO_ERROR != glGetError()) {
      cerr << "TGame::RoundStart : GL was in error condition after SetMap" << endl;
    } 

    /* Randomly set the order of the players */
    int tmp1, tmp2;
    TPlayerInfo * tmppi;
    // cout << "Randomizing player order for " << num_players <<  endl;
    for (int i = 0; i < 100; i++) {
      tmp1 = (int) rint((num_players-1)*(rand()/(RAND_MAX+1.0)));
      tmp2 = (int) rint((num_players-1)*(rand()/(RAND_MAX+1.0)));
      if (tmp1 != tmp2) {
	// cout << "Swapping " << tmp1 << ", " << tmp2 << endl;
	tmppi               = playerInfos[tmp1];
	playerInfos[tmp1]   = playerInfos[tmp2];
	playerInfos[tmp2]   = tmppi;
      }
    }
    
    /* Randomly get a sufficient number of spots on the map */
    TVectors * tmp = map->RandomSpots(num_players, 10);
    TVectorsIterator spot = tmp->begin();
    /* Set each player to "active" and reset their round counts 
       Also set their tanks position */
    TPlayerInfosIterator End = playerInfos.end();
    for(TPlayerInfosIterator i = playerInfos.begin(); i != End; i++) {
      (*i)->active         = true;
      (*i)->num_turn_fired = 0;
      (*i)->player->PrepareRound(&(*spot));
      /* (Re)add the tanks to the game */
      AddEntity((*i)->player->tank);
      spot++;
    }
    delete tmp;

    /* Set next player - start with player 0 */
    current_num_player = 0;
    current_player     = playerInfos[0]->player;
    /* Tell this player it is its turn */
    current_player->BeginTurn();

    /* This turn has number 1 */
    num_this_turn      = 1;
    
    /* Update the states to reflect we are now running */
    gamestate          = gamestate_round_running;
    roundstate         = roundstate_can_fire;     /* Players may fire */
    /* Increase the round number */
    num_this_round++;
    
    /* Register the commands the game wants to consume 
       These are unregistered in the ~TGame function */
    CommandDispatcher.RegisterConsumer("fire", this);
    return true;
  } else {
    cerr << "TGame::RoundStart() : Wrong state for starting" << endl;
    return false;
  }
}



/* **********************************************************************
 * Update the active flag on players, according to their health. 
 * Returns the number of active players
 * *********************************************************************/
int TGame::UpdateActivePlayers() {
  int num_active_players = 0;
  TPlayerInfosIterator End = playerInfos.end();
  for(TPlayerInfosIterator i = playerInfos.begin(); i != End; i++) {
    // cout << "TGame::IsGameOver checking player" << endl;
    (*i)->active = ((*i)->player->tank->health > 0.0);
    if ((*i)->active) {
      num_active_players++;
    }
  }
  return num_active_players;
}

/* **********************************************************************
 * Aborts the current round, by updating the state
 * *********************************************************************/
void TGame::AbortRound() {
  roundstate = roundstate_done;
  current_player->EndTurn();
  gamestate = gamestate_round_done;
  /* Check roundnumber here for game over */
  if (num_rounds <= num_this_round) {
    gamestate = gamestate_done;
  }
}

/* **********************************************************************
 * Find the nextplayer/check for end of round
 * This is a mess
 * *********************************************************************/
void TGame::NextPlayer() {
  // cerr << "TGame::NextPlayer()" << endl;

  if (gamestate != gamestate_round_running) {
    cerr << "ERROR: NextPlayer() called, with state != "
	 << "gamestate_round_running" << endl;
  }
  if (roundstate == roundstate_not_ready) {
    cerr << "ERROR: NextPlayer() with state == roundstate_not_ready" << endl;
  }

  /* Tell the current player that his turn is done */
  current_player->EndTurn();

  /* Check if the current round is over:
     only one active */
  if (UpdateActivePlayers() < 2) {
    AbortRound();
    return;
  }
      
  /* Check if one of the active players haven't fired, or 
     if we need to start a new round */
  if (roundstate == roundstate_no_fire 
      && 0 == num_stoppers) {
    int next_player;
    bool found_one = false;
    for (next_player = 0; (next_player < num_players) && (!found_one); next_player++) {
      // cout << "TGame::NextPlayer() - check player " << next_player << endl;
      found_one = playerInfos[next_player]->active 
	&& (playerInfos[next_player]->num_turn_fired < num_this_turn);
      if(found_one) {
	break;
      }
    }
    if (!found_one) { /* Did not find active that has not shot this turn */
      // cout << "TGame:NextPlayer() - did not find active" << endl;
      num_this_turn++;
      next_player = 0;
      /* This is safe, because active players exists */
      while(!playerInfos[next_player]->active) {
	next_player++;
      }
    }
    // cout << "TGame::NextPlayer setting player to " << next_player << endl;
    /* Set the current and pointer to this player */
    current_num_player = next_player;
    current_player     = playerInfos[current_num_player]->player;
    /* And, can fire */
    roundstate = roundstate_can_fire;

    /* And, let the player know that he can go ahead */
    current_player->BeginTurn();
  }
}

/* **********************************************************************
 * IsGameOver()
 * *********************************************************************/
bool TGame::IsGameOver() {
  return (gamestate_done == gamestate);
}

/* **********************************************************************
 * TGame::CommandConsume
 * *********************************************************************/
bool TGame::CommandConsume(TCommand * Command) {
  // TODO: Maybe some more intelligent checking here. 
  // For now, the fire checking is handled elsewhere
  if ("fire" == Command->name) {
    return FireProjectile();
  }
  return false;
}

/* **********************************************************************
 * Wheter or not two players are on the same team
 * *********************************************************************/
bool TGame::OnSameTeam(TPlayer * a, TPlayer * b) {
  if (teammode) {
    /* They are on the same team, if there teams are equal */
    return a->team == b->team;
  } else {
    /* No teams, there are on same teams, if they are equal */
    return a == b;
  }
}

/* **********************************************************************
 * Wind stuff
 * *********************************************************************/

/* **********************************************************************
 * SetWind - Called to change the wind according to some rules.
 * *********************************************************************/
void TGame::SetWind() {
  wind.x += 1.0;
}

/* **********************************************************************
 * Apply the current wind to a velocity
 * This is a bit more complicated than it sounds.
 * Obviously, the wind cannot add to the velocity if the velocity in the
 * wind direction is greater than the wind velocity. In this case, 
 * we may actually apply drag.
 * TODO: Ask skjalm about how to do it. We may have to use the area of
 * the object and possible the weight.
 * *********************************************************************/
void TGame::ApplyWind(TVector &vel, system_time_t deltatime) {
  /* This is a hack at the moment */
  vel.x += wind.x * deltatime;
  vel.y += wind.y * deltatime;
  vel.z += wind.z * deltatime;
}
