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
#ifndef __ENTITIES_HH__
#define __ENTITIES_HH__
/* Define entities types that can be used within the game.
   Entities can be updated, rendered, damaged by other entities, etc */
#include <list>

#include "types.hh"

class TPlayer;
class TGame;
class TViewpoint;
/* Entities are used for all elements in the game that needs to be
   dynamically updated. The base class can not be instiantiated due to
   the abstract methods */
class TEntity {
public:
  /* Used by the game to filter out entities that are "dead" */
  bool keep;          
  /* Used by the game to determine if it need to delete this, when 
     dead */
  bool free;
  /* Used by the game to synchronize player turn changes.
     Projectiles and explosions should set this to true*/
  bool affect_turn; 
  /* Used when we want to figure out who hit us, and stuff */
  TPlayer * owner; /* ALWAYS check this for null, as entities not
		      owned by players will have this set to NULL */
  /* Used, when updating and eventually for accessing the global 
     environment */
  TGame * game;
  /* Default is that we should be kept, but freed when done */
  TEntity(TGame * ngame, TPlayer * nowner) {
    keep        = true;
    free        = true;
    affect_turn = false;
    owner       = nowner;
    game        = ngame;
  };
  virtual ~TEntity() {};
  /* The operations on an entity is declared pure virtual */
  virtual void Update(system_time_t deltatime) = 0;
  virtual void Render(TViewpoint * viewpoint) = 0;
  virtual void TakeDamage(TEntity * other_entity) = 0;
};

typedef std::list<TEntity *> TEntities;
typedef TEntities::iterator TEntitiesIterator;
#endif
