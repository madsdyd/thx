#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>

#include "color.hh"
#include "viewpoint.hh"

/* A player is either a human or bot controlled participiant in the world */
class TInventory;
class TTank;
class TPlayer {
public:
  string name;            /* The players name */
  TInventory * inventory; /* Used to hold and create projectiles */
  int money;              /* The money the player have */
  int score;              /* The score */
  TTank * tank;           /* The players tank */
  TColor color;           /* Used at all? */
  TViewpoint viewpoint;   /* The players viewpoint */
  TPlayer(string nname);
  ~TPlayer();
  void PrepareRound(TVector * location);
};
#endif
