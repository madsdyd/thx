#ifndef __ENTITIES_HH__
#define __ENTITIES_HH__
/* Define entities types that can be used within the game.
   Entities can be updated, rendered, damaged by other entities, etc */
#include <list>

#include "vector.hh"
#include "types.hh"
#include "viewpoint.hh"

/* Forward declare TGame and TPlayer - used by entities */
class TGame;
class TPlayer;
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
  /* Default is that we should be kept, but freed when done */
  TEntity(TPlayer * nowner) {
    keep        = true;
    free        = true;
    affect_turn = false;
    owner       = nowner;
  };
  virtual ~TEntity() {};
  /* The operations on an entity is declared pure virtual */
  virtual void Update(TGame * game, system_time_t deltatime) = 0;
  virtual void Render(TViewpoint * viewpoint) = 0;
  virtual void TakeDamage(TEntity * other_entity) = 0;
};

typedef std::list<TEntity *> TEntities;
typedef TEntities::iterator TEntitiesIterator;
#endif
