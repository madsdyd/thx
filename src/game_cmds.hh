#ifndef __GAME_CMDS_H__
#define __GAME_CMDS_H__
/* This file includes declarations for the commands and variables that can be 
   used to modify the game state.
   This file is temporary and is meant to be replaced by a more clever approach. */

#define CMDNAMELENGTH 32
#define CMDDESCRIPTIONLENGTH 128

#define MAXNUMCMDS 32

typedef enum { cmd_type_game, cmd_type_client } cmd_type_t;

typedef struct {
  int id;
  cmd_type_t cmd_type;
  char name[CMDNAMELENGTH];
  char description[CMDDESCRIPTIONLENGTH];
} cmd_t;

extern cmd_t cmds[MAXNUMCMDS];
   

/* A number of constants we use */
/* Attack is like "fire" */
#define CMD_ATTACK           0

/* Rotate up and down */
#define CMD_ROTATE_UP_1      1
#define CMD_ROTATE_UP_5      2
#define CMD_ROTATE_DOWN_1    3
#define CMD_ROTATE_DOWN_5    4

/* Rotate left and rigth */
#define CMD_ROTATE_LEFT_1    5
#define CMD_ROTATE_LEFT_5    6
#define CMD_ROTATE_RIGTH_1   7
#define CMD_ROTATE_RIGTH_5   8

void cmds_init();

#endif
