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
#ifndef __SOUND_H__
#define __SOUND_H__
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef SOUND_ON
/** Include stuff that is neccesary to activate the sound system. */
//#include <sys/types.h>
#include <sys/stat.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>           

/** Record types */
#include <string>
#include <map>

using std::string;
using std::map;

extern map<string, int> names_to_nums;

typedef enum {play, loop, stop} request;

struct SoundRequest {
  request snd_request;
  int snd_request_id; /** Identifies the sound for later reference.
			  In case of stop, 
			  -1 can be used to shut down all sounds */
  unsigned int snd_num; /** The sound fix to assiciate with this request */
};

/** User functions */

/** Initialize the sound module. If 0 is returned, the call is succesfull, if
    anything !=0 is returned, the sound module could not be initialized. */ 
int sound_init(char* dev, unsigned int num_fxs);

/** Will load a sound into the array at place "num" 
    returns 0 OK, !=0 else */
int sound_load(int num, const char * filename);

/** The sounds are referred with a number (a series of constants are
    declared to use), and can be played back with */
int sound_play(unsigned int i);
/**  where i is the number of a sfx to play. Return values are 0, if no
errors are suspected, and -1 if i is outside range, or another error
ocurred.  Already playing sounds will be mixed with the new sound. (May be a
little delayed before mixing occurs). */

/** Enques a request to stop all sounds */
int sound_stopall();

/** Enter some requests. Need to use SoundRequest as the type of the ... args*/
int sound_request(int num_requests, ...);

/** Releases the sound device. Stop playing sounds 0 == OK */
int sound_off();

/** Grabs the sound device. 0 == OK */
int sound_on();

/** Already playing sounds will play to the finish. Silence will be feed the card.
    Calls to sound_request and sound_play will be ignored. */
void sound_mute();

/** Calls to sound_request and sound_play will work again. */
void sound_unmute();

/** Remember to shut the sound system down properly */
void sound_shutdown();
#endif /* Sound on */
#endif
