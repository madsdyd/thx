#ifndef __CLIENT_HH__
#define __CLIENT_HH__

#include <set>

/* The purpose of the client is to be the link between the server, game and 
   other parts of the system. Granted, there are no server support, but one is 
   allowed to hack, isn't one? Here is how I think it should be viewed:

   client <--talks to--> server <--has--> game
    
   client <--talks to and has-->display, menu, sound, keyboard

   client <--talks to--> GLUT
   
   As said, there are no server, and no talking, but since there is no design 
   either that figures :-). As of now, the client talks directly to the game.

   Oh, and of course it very little sense to put the client into a class, 
   but it makes it easier to refer to.
*/



#define CLIENT_HAS_NOTHING        0x00000000 /* The client has no menu/game */
#define CLIENT_HAS_GAME           0x00000001 /* The Game object needs stuff */
#define CLIENT_HAS_INGAMEMENU     0x00000002 /* In-between handling */
#define CLIENT_HAS_ROUNDOVERMENU  0x00000004 /* In-between handling */
#define CLIENT_HAS_BUYMENU        0x00000008 /* Menu for buying, displayed between games */
#define CLIENT_HAS_GAMEOVERMENU   0x00000010 /* In-between handling */
#define CLIENT_HAS_SCORE          0x00000020 /* Score board */

//#define CLIENT_HAS_CONSOLE    0x00000008 /* Nonexistant console :-) */

class TClient {
public:
  bool game_running; /* If we do not have a game running, we show the 
			game menu */
  int has;           /* has covers the logical ors of elements that needs 
			update/keyboard/display */
public:
  /* Construct the client object 
     Stuff that are related to the client gets constructed and initialized as
     well. This includes the menu, sound and display system, but not the 
     (nonpresent) server */
  TClient(int argc, char ** argv);
  /* Run the game - Used to dispatch the glut events, and so on... */
  void Run();
};
 
extern TClient * Client;
#endif
