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
/***************************************************************************
                          sound_test.c  -  description
 
                             -------------------
 
    begin                : Mon Jun 21 00:55:10 CEST 1999
 
    copyright            : (C) 1999 by Mads Bondo Dydensborg
                           (Parts may be (C) ECC)
 
    email                : madsdyd@challenge.dk
 
 ***************************************************************************/
 
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *

***************************************************************************/
#include "sound.hh"
#include <stdio.h>
#include "old_sound_types.h"

void ExitProc() {
  printf("In exitproc\n");
  //  vga_setmode(TEXT); 
  printf("Text mode set, shutting down sound");
  sound_shutdown();
  printf("Leaving exitproc\n");
}

int main() {
  int i;
  struct SoundRequest snd1, snd2;
  if (0 != sound_init("", SFX_NUM_SFX)) {
    printf("Error initializing sound\n");
    exit(1);
  }
  for (i = 0; i < SFX_NUM_SFX; i++) {
    printf("Loading %i %s\n", i, SoundFiles[i]);
    if (0 != sound_load(i, SoundFiles[i])) {
      fprintf(stderr, "Error loading sound %i %s\n",
	      i, SoundFiles[i]);
    } else {
      printf("Loaded %i %s\n", i, SoundFiles[i]);
    }
  }              
  
  /** Register an exit function */
  /**  if ( atexit( ExitProc ) == -1 ) {
    perror("atexit");
    ExitProc();
    } */
  /**  
       for (i = 0; i < SFX_NUM_SFX; i++) {
       if (0 != sound_play(i)) {
       printf("Error playing sound %i\n", i);
       exit(1);
       } 
  }
  */
  
  snd1.snd_request = loop;
  snd1.snd_request_id      = 0;
  snd1.snd_num     = 1;
  snd2.snd_request = play;
  snd2.snd_request_id      = 1;
  snd2.snd_num     = 1;
  sound_request(1, snd1); 
  snd1.snd_request = loop;
  snd1.snd_request_id      = 0;
  snd1.snd_num     = 2;
  sound_request(1, snd1); 

  printf("Sleeping 3 minutes... \n");
  sleep(180);
  
printf("Starting eating CPU. Kill me... \n");
  
  while(1){;};


  printf("Muting sound stuff\n"); 
  sound_mute();
  //  sleep(5); 
  printf("Unmuting soundstuff\n");
  sound_unmute();

  printf("off sound stuff, returns %i\n", sound_off());
  sleep(5); 
  printf("on sound stuff, returns %i\n", sound_on());
  sleep(5); 
  printf("off sound stuff, returns %i\n", sound_off());
  sleep(5); 

  printf("Stopping\n");
  sound_shutdown();
  return 0;

  printf("on sound stuff, returns %i\n", sound_on());
  /**
  snd1.snd_request = loop;
  snd1.snd_request_id      = 5;
  snd1.snd_num     = 5;
  sound_request(1, snd1); sleep(1);
  snd2.snd_request = stop;
  snd2.snd_request_id      = -1;
  snd2.snd_num     = 5;
  sound_request(1, snd2);  */

  sound_shutdown();
  printf("Sound shut down\n");

  
  if (0 != sound_init("", SFX_NUM_SFX)) {
    printf("Error initializing sound\n");
    exit(1);
  }
  for (i = 0; i < SFX_NUM_SFX; i++) {
    if (0 != sound_load(i, SoundFiles[i])) {
      fprintf(stderr, "Error loading sound %i %s\n",
	      i, SoundFiles[i]);
    }
  }              
  sound_request(1, snd1); sleep(1);
  sound_shutdown();
  
  /**  for (i = 0; i < SFX_NUM_SFX; i++) {
    if (0 != sound_play(i)) {
      printf("Error playing sound %i\n", i);
      exit(1);
    }
    } */
  //  sound_shutdown();

  return 0;
}










