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
                          sound.c  -  description
 
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
#include <string.h>  /** memset */
#include <pthread.h> /** threads, mutexes, etc. */
#include <signal.h>  /** signal to the thread to stop.. */
#include <stdarg.h>  /** variable list functions */

/**
   (Linux) Sound module for Xarman.
   By Mads Dydensborg <madsdyd@challenge.dk> - 19990605.

*/

/** Module wide declarations and variables */

/** Added to maintain mapping between names and sound numbers */
#include <string>
#include <map.h>
map<string, int> names_to_nums;

/** ********************************************************************** */
/** SOUND STUFF */
/** Information about a single sound */
struct SoundInfo {
  int size; 
  void * data;
};

/** An array of the sounds that are loaded */
//struct SoundInfo LoadedSounds[SFX_NUM_SFX];
struct SoundInfo * LoadedSounds;
/** The number of sounds we will allocate */
unsigned int NumSoundsAllocated;

/** Sound of/on - defaults to off, until initialized */
int SoundOn = 0;

/** Sound initialized? */
int SoundInitOK = 0;

/** Sound device */
char * SoundDevice;

/** Hardware specifics, here for overview */
/** The handle to the sound device */
#define DefSoundFormat AFMT_U8
#define FRAGMENTSIZE 512 /** Loaded files will be padded to this */
#define THREADSLEEP 12500 /** 12.5 ms for 512 bytes 
 The interessting thing is to aviod underrun to the audio device, 
 while not getting so far ahead, that sounds are too delayed compared
 to their events. 
 The number of fragments may be usefull to manipulate this. */
#define DefSoundSetting 0x00030009 /** 512 bytes, 11025 / 512 ~= 21.5 ms */
#define DefSoundChannels 0
#define DefSoundRate 11025
int SoundDevHandle = -1;
int SoundSetting = DefSoundSetting;   /** any fragments C = 4096, D = 8192 buffer */
int SoundChannels = DefSoundChannels; /** 0=mono 1=stereo */
int SoundFormat = DefSoundFormat;     /** unsigned 8 bit...  */
int SoundRate = DefSoundRate;         /** Low sample rate */


/** ********************************************************************** */
/** THREAD STUFF */

/** The mutex - this protects the stuff we write into sfxlist */
pthread_mutex_t SoundMutex = PTHREAD_MUTEX_INITIALIZER;

/** Our sound server thread - not strictly necc. */
pthread_t SoundThread;
/** An exit flag */
/** int ExitThread = 0; */

/** A prototype of the thread function to run */
void* SoundThreadFunction(void * data);

/** A list of sfx, used to store the sfxs to play from the main thread */
struct sfx_elem {
  struct SoundRequest sfx; /** The sfx to be inserted into the play queue */
  struct sfx_elem* next;   /** More elements, NULL terminated */
};
struct sfx_elem * sfxlist;

/** A list of the sounds that are currently in queue to be played. This really
    should be done in hardware, I think, but I do not know how. */
struct sound_elem {
  int sfx;        /** The sfx we are playing. */
  int id;         /** The id of the sound */
  int pos;        /** Where in the sfx we have mixed upto */
  int loop;       /** 0 = noloop, 1 = loop */
  int size;       /** Just for convenience */
  struct sound_elem* next;
};
struct sound_elem * soundlist;

/** ********************************************************************** */
/** USER FUNCTIONS */

/** Initialize the hardware, initialize memory structures,
    start a thread, setup a mutex, etc. */
int sound_init(char * dev, unsigned int num_fxs) {
  unsigned int i;

  /** Check if already initialized */
  if (1 == SoundInitOK) {
    return -1;
  }

  /** ********************************************************************** */
  /** Initialize the sound hardware */
  if (0 == strcmp("", dev)) {
    dev = "/dev/dsp";
  }
  SoundDevice = dev;

  if (0 != (i = sound_on())) {
    /** Someting went wrong! */
    return i;
  }

  /** ********************************************************************** */
  /** Set up the memory stuff, the thread, mutex, etc. */
  /** Initialize the LoadedSounds array */
  NumSoundsAllocated = num_fxs;
  LoadedSounds 
    = (struct SoundInfo *) malloc(NumSoundsAllocated*sizeof(struct SoundInfo));
  if (NULL == LoadedSounds) {
    fprintf(stderr, "Error allocating buffer space for sound info - shutting down sound\n");
    sound_shutdown();
    return -1;
  }
  for (i = 0; i < NumSoundsAllocated; i++) {
    LoadedSounds[i].size = 0;
    LoadedSounds[i].data = NULL;
  }
  
  /** The sfxlist and soundlist should be empty */
  sfxlist = NULL;  soundlist = NULL;
  
  /** Start the sound server thread */
  /**  ExitThread = 0; */
  if (0 != pthread_create(&SoundThread, NULL, SoundThreadFunction, NULL)) {
    perror("Error spawning soundthread. Sound module disabled");
    sound_shutdown();
    return -1;
  }
  
  /** ********************************************************************** */
  /** Everything is great */
  SoundOn = 1;
  SoundInitOK = 1;
  return 0;
}

/** ********************************************************************** */
/** Load a file */
int sound_load(int num, const char * filename) {
  int j;  /** Counters */
  int status;
  int readbytes;
  int fd; /** Filedescriptor */
  char * buf; /** Pointer to area to load into */
  struct stat fdstat; /* stat structure */
  struct SoundInfo TmpInfo;
  /** If not inited, return */
  if (!SoundInitOK) {
    return -1;
  }

  /** Stat before allocating memory */
  fd = open(filename, O_RDONLY);
  if (-1 == fd) {
    // sound_shutdown();
    perror("sound_load - open");
    return errno;
  }
  /** Stat the file */
  if (-1 == fstat(fd, &fdstat)) {
    perror("sound_load - fstat ");
    close(fd);
    // sound_shutdown();
    return errno;
  }

  /** Save the size */
  TmpInfo.size = fdstat.st_size;
  TmpInfo.data = NULL;
  
  readbytes = 0;
  fd = open(filename, O_RDONLY);
  TmpInfo.data = malloc(TmpInfo.size);
  if (NULL == TmpInfo.data) {
    return -1;
  }
  /** Now, load the file into the memory */
  buf = (char *) TmpInfo.data;
  j = TmpInfo.size;
  while( (status = read(fd, buf, j)) > 0) {
    readbytes += status;
    buf = buf + status;
    j = j - status;
  }
  if (-1 == status) {
    close(fd);
    free(TmpInfo.data);
    return -1;
  }
  /** Close the file */
  close(fd);

  /** Get the mutex, just in case */
  /** Note, not protecting this, means that the user is trusted to not load
      sounds, while sounds with the same index is playing */
  /** If a sound was already loaded, release it */
  if (NULL != LoadedSounds[num].data) {
    free(LoadedSounds[num].data);
  }
  LoadedSounds[num].size = TmpInfo.size;
  LoadedSounds[num].data = TmpInfo.data;
  /* Insert into names */
  names_to_nums.insert(pair<string, int>(filename, num));

  /** printf("Loaded %s, at %i -  %i bytes\n", filename, num, readbytes); */
  return 0;
} /** Done loading file */

/** ********************************************************************** */
/** Playback the sound - actually que it to the sound module */
int sound_play(unsigned int i) {
  struct SoundRequest snd;
  snd.snd_request = play;
  snd.snd_request_id      = -1;
  snd.snd_num     = i;
  return sound_request(1, snd);
}

/** ********************************************************************** */
/** Stop all sounds */
int sound_stopall() {
  struct SoundRequest snd;
  snd.snd_request    = stop;
  snd.snd_request_id = -1;
  snd.snd_num        = ~((unsigned int)0);
  return sound_request(1, snd);
}

void sound_mute() {
  SoundOn = 0;
}

void sound_unmute() {
  SoundOn = 1;
}

/** ********************************************************************** */
/** Will release a list of sfx_elem */
void free_sfx_elem(struct sfx_elem * elem) {
  if (NULL != elem) {
    free_sfx_elem(elem->next);
    free(elem);
  }
}

/** ********************************************************************** */
/** Will release a list of sound_elem */
void free_sound_elem(struct sound_elem * elem) {
  if (NULL != elem) {
    free_sound_elem(elem->next);
    free(elem);
  }
}

/** ********************************************************************** */
/** Enqueue the reque */
int sound_request(int num_requests, ...) {
  int i = num_requests;
  struct sfx_elem * tmp_elem;
  struct sfx_elem * tmp_tail = NULL;
  va_list ap;
  /** Used to hold the values, until everything is OK. */
  struct sfx_elem * tmp_head = NULL;

  /** If not running, leave */
  if (1 != SoundInitOK || 1 != SoundOn) {
    return 0;
  }
  
  /** Iterate over all sounds */
  va_start(ap, num_requests);
  while(i > 0) {
    /** Add the int to the sfxlist */
    tmp_elem = (struct sfx_elem *) malloc(sizeof(struct sfx_elem));
    if (NULL == tmp_elem) {
      /** Uh oh - no memory? */
      /** Cleanup tmp_sfxlist */
      free_sfx_elem(tmp_head);
      return -1;
    }
    
    /** Fill variables, from arguments */
    tmp_elem->sfx = va_arg(ap, struct SoundRequest);
    tmp_elem->next = NULL;
    /** Checking for valid data is deffered to the soundthread, as this 
	-could- change between calls and the thread beeing scheduled */
    /** Insert into back of tmp list. */
    if (NULL == tmp_head) {
      /** Set head and tail */
      tmp_tail = tmp_elem;
      tmp_head = tmp_elem;
    } else {
      /** Insert at tail (keep order) */
      tmp_tail->next = tmp_elem;
      tmp_tail = tmp_elem;
    }
    i--;
  }
  
  /** Insert into the real soundlist at the back */
  pthread_mutex_lock(&SoundMutex);
  if (NULL != sfxlist) {
    tmp_tail = sfxlist;
    while(NULL != tmp_tail->next) { tmp_tail = tmp_tail->next; };
    tmp_tail->next = tmp_head;
  } else {
    sfxlist = tmp_head;
  }

  /**  printf("Added %i to sfx_list\n", i); */
  /** printf("------------sfx_list---------------\n");
    for(tmp_elem = sfxlist; tmp_elem != NULL; tmp_elem = tmp_elem->next) {
    fprintf(stderr, "Dump stuff :  %i %i %i\n", tmp_elem->sfx.snd_request,
	    tmp_elem->sfx.snd_request_id, tmp_elem->sfx.snd_num);
	    } */
  pthread_mutex_unlock(&SoundMutex);
  va_end(ap);
  return 0;
}

/** ********************************************************************** */
/** Shut off the sound in sound_play, pause thread, release sound hardware */
int sound_off(){ 
  if (1 != SoundInitOK) {
    /** Has never been initialized/is already off */
    return -1;
  }
  /** Pause the thread, by grabbing the mutex. 
   Deadlock is avoided, since SoundOn = 0 before we leave this procedure. 
   Not safe for threads, other then the same that does sound_request/sound_play */
  pthread_mutex_lock(&SoundMutex);
  /** Release the sounddevice. This will play remaining buffers to the end. */
  if (SoundDevHandle != -1) {
    // printf("Closing sound device\n");
    if ( ioctl(SoundDevHandle, SNDCTL_DSP_SYNC) == -1 ) {
      perror("ioctl SNDCTL_DSP_SYNC");
    } 
    close(SoundDevHandle);
    // printf("Sound device closed\n");
    SoundDevHandle = -1;
  }                      
  /** Set some variables to know our state */
  SoundInitOK = 0;
  return 0;
}

/** ********************************************************************** */
/** Turn on the sound in sound_play */
int sound_on() {
  /** This procedure opens and sets up the interface */
  if (0 != SoundInitOK) {
    /** Already running. */
    return -1;
  } 

  if ( (SoundDevHandle = open(SoundDevice,O_WRONLY)) == -1 ) {
    fprintf(stderr, "Error opening %s, num %i\n", SoundDevice, errno);  
    return -1;
  }                            

  /** Device opened, set some options. */
  /** Setup the buffer we will use */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SETFRAGMENT, &SoundSetting) == -1 ) {
    perror("ioctl set fragment");
    sound_shutdown();
    return errno;
  }              
  if (DefSoundSetting != SoundSetting) {
    printf("WARNING: SoundSetting; wanting %x got %x\n", 
	   DefSoundSetting, SoundSetting);    
  }

  /** Set channels */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_STEREO, &SoundChannels) == -1 ) {
    perror("ioctl mono");
    sound_shutdown();
    return errno;
  }              
  if (DefSoundChannels != SoundChannels) {
    printf("ERROR: SoundChannels; wanting %x got %x - shutting down sound\n", 
	   DefSoundChannels, SoundChannels);
     sound_shutdown();
     return -1;
  }
                      
  /** Set the format */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SETFMT, &SoundFormat) == -1 ) {
    perror("ioctl format");
    sound_shutdown();
    return errno;
  }
  if (DefSoundFormat != SoundFormat) {
    printf("ERROR: DefSoundFormat not set - shutting down sound\n");
    sound_shutdown();
    return -1;
  }

  /** Set the speed/rate */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SPEED, &SoundRate) == -1 ) {
    perror("ioctl sample rate");
    sound_shutdown();
    return errno;
  }                        
  if (DefSoundRate != SoundRate) {
    printf("WARNING: SoundRate; wanting %x got %x\n", 
	   DefSoundFormat, SoundFormat);
  }
  /** We use the "fast" semantics */
  pthread_mutex_unlock(&SoundMutex);
  SoundInitOK = 1;
  return 0;
}

/** ********************************************************************** */
/** Kill the thread, release the hardware */
void sound_shutdown() {
  unsigned int i;
  /** Shutdown the soundthread */
  if (1 == SoundInitOK) {
    if (0 != pthread_cancel(SoundThread)) {
      perror("Killing soundthread");
    } else {
      // printf("Waiting for thread to die... \n");
      if (0 != pthread_join(SoundThread, NULL)) {
	perror ("pthread_join"); 
      } else {
	// printf("SoundThread killed!!!!!!!\n");
      }
    } 
    // printf("All threads killed\n");
  } 

  if (SoundDevHandle != -1) {
    // printf("Closing sound device\n");
    if ( ioctl(SoundDevHandle, SNDCTL_DSP_SYNC) == -1 ) {
      perror("ioctl SNDCTL_DSP_SYNC");
    } 
    close(SoundDevHandle);
    // printf("Sound device closed\n");
    SoundDevHandle = -1;
  }                      
  // printf("freeing memory\n");
  if (NULL != LoadedSounds) {
    for (i = 0; i < NumSoundsAllocated; i++) {
      if (NULL != LoadedSounds[i].data) {
	free(LoadedSounds[i].data);
	LoadedSounds[i].data = NULL;
      }
    }
    free(LoadedSounds);
  }
 

  /** Free the soundlist and sfxlist */
  free_sfx_elem(sfxlist);
  sfxlist = NULL;
  free_sound_elem(soundlist);
  soundlist = NULL;

  SoundOn = 0;
  SoundInitOK = 0;
}

/** ********************************************************************** */
/** SoundThreadExit */
void SoundThreadExit(void *) {
  // printf("SoundThreadExit called - syncing sound data\n");
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SYNC) == -1 ) {
    perror("ioctl SNDCTL_DSP_SYNC");
  } 
} 

/** ********************************************************************** */
/** The Thread function - this thread grabs requests from the sfx_list, and
    add them to the sondlist */
/** Feed data to the sound device */
void* SoundThreadFunction(void * data) {
  /** This function loops forever 
      It sleep, then reads the data from the sfxlist,
      mix it into a buffer, then feed it to the audio device.
  */
  /** A buffer to mix data intofor data. 
      This is statically allocated to ensure that there is no alloc errors */
  unsigned char mixerbuffer[FRAGMENTSIZE];
  unsigned char * buf;
  /** This one is for mixing... */
  int mixerbuffer2[FRAGMENTSIZE];
  int i; /** Counter */
  int status;
  struct sfx_elem * tmp, * tmp_sfx_elem;
  struct sound_elem * head, * tail, * tmp_sound_elem, *tmps;

  /** Loop forever - the shutdown will shut us down */
  pthread_cleanup_push(SoundThreadExit, NULL);
  while(1) {
    /** ********************************************************************** */
    /**  Grab any data from the sfxlist, add it to the soundlist */
    /** pthread_cleanup_push(pthread_mutex_unlock, (void *) &SoundMutex);
     */
    pthread_mutex_lock(&SoundMutex); 
    for (tmp_sfx_elem = sfxlist; tmp_sfx_elem != NULL;) {
      /** Check for play, loop, stop */
      switch (tmp_sfx_elem->sfx.snd_request) {
      case play:
      case loop: {
	/** Play and loop is almost the same */
	/** Check that the data is in the array and not NULL */
	if (tmp_sfx_elem->sfx.snd_num > NumSoundsAllocated 
	    || NULL != LoadedSounds[tmp_sfx_elem->sfx.snd_num].data) {
	  tmp_sound_elem 
	    = (struct sound_elem *) malloc(sizeof(struct sound_elem));
	  if (NULL == tmp_sound_elem) {
	    /** Uh oh better stop the for loop */
	    fprintf(stderr, "SoundThreadFunction - error allocating memory\n");
	    break;
	  }
	  /** Copy the sfx_elem, and size from list of loaded elements */
	  tmp_sound_elem->sfx  = tmp_sfx_elem->sfx.snd_num;
	  tmp_sound_elem->id  = tmp_sfx_elem->sfx.snd_request_id;
	  if (loop == tmp_sfx_elem->sfx.snd_request) {
	    tmp_sound_elem->loop = 1;
	  } else {
	    tmp_sound_elem->loop = 0;
	  }
	  tmp_sound_elem->pos  = 0;
	  tmp_sound_elem->size = LoadedSounds[tmp_sound_elem->sfx].size;
	  /** Insert the sound_elem into the queue */
	  tmp_sound_elem->next = soundlist;
	  soundlist = tmp_sound_elem;
	} /** Check that data is != NULL */
	break;
      }
      case stop: {
	/** Stop is selective. May identify a sound, or just 
	    all sounds */
	head = NULL; tail = NULL;
	for(tmp_sound_elem = soundlist; tmp_sound_elem != NULL;) {
	  if (tmp_sfx_elem->sfx.snd_request_id != -1 && 
	      tmp_sfx_elem->sfx.snd_request_id != tmp_sound_elem->id) {
	    /** Keep this element */
	    /** fprintf(stderr, "stop, keeping sfx;  id : %i, num : %i\n", 
		tmp_sound_elem->id, 
		tmp_sound_elem->sfx); */
	    if (NULL == head) {
	      /** First element in list! */
	      head = tmp_sound_elem;
	      tail = head;
	    } else {
	      /** Add to tail */
	      tail->next = tmp_sound_elem;
	      tail = tail->next;
	    }
	    tmp_sound_elem = tmp_sound_elem->next;
	  } else {
	    /** Dropping this element */
	    /**	fprintf(stderr, "stop, dropping sfx;  id : %i, num : %i\n", 
		tmp_sound_elem->id,  
		tmp_sound_elem->sfx); */
	    /** Free this element */
	    tmps = tmp_sound_elem->next;
	    free(tmp_sound_elem);
	    tmp_sound_elem = tmps;
	    if (NULL != tail) {
	      tail->next = tmp_sound_elem;
	    }
	  }
	} /** All elements in list */
	/** Set soundlist to the new head */
	soundlist = head;
	break;
      } /** case stop */
      } /** Switch */


      /** Loop - Delete the sfx_elem */
      tmp = tmp_sfx_elem->next;
      free(tmp_sfx_elem);
      tmp_sfx_elem = tmp;
    }
    /** The sfxlist is now empty */
    sfxlist = NULL;
    pthread_mutex_unlock(&SoundMutex);
    /** pthread_cleanup_pop(0); */
    /** printf("Mutex unlocked\n"); */

    /** ********************************************************************** */
    /** Mix sounds into buffer. */
    
    /** There are 2 cases - the empty list, and non empty */

    if (NULL != soundlist) {
      //    if (0==1) {

      /** Reset the buffer ... */
      memset(&mixerbuffer2, 0, FRAGMENTSIZE*sizeof(int));
      
      /**      printf("Mixing sounds ------------ \n"); */

      /** For each sound in the sound list
	  Mix it into the buffer */
      for(tmp_sound_elem = soundlist; 
	  tmp_sound_elem != NULL; tmp_sound_elem = tmp_sound_elem->next) {
	/**	printf("Mixing signal %i from %i (out of %i)\n", 
	       tmp_sound_elem->sfx, 
	       tmp_sound_elem->pos,
	       tmp_sound_elem->size);
	       printf("Mixerbuffer2\n"); */

	for(i = 0; i < FRAGMENTSIZE; i++) {
	  /** Check that we have not passed end. In that case, break */
	  if (tmp_sound_elem->size > i + tmp_sound_elem->pos) {
	    mixerbuffer2[i] += 
	      (*((unsigned char *)
		 ((unsigned char *) LoadedSounds[tmp_sound_elem->sfx].data
		  +tmp_sound_elem->pos+i)
		 )
	       ) - 128;
	    /** printf("%x ", mixerbuffer2[i]); */
	  } else {
	    /** Check for looping sounds */
	    if (1 == tmp_sound_elem->loop) {
	      /** Reset the position, copy to buffer */
	      tmp_sound_elem->pos = -i; /** Will start the sound at pos 0*/
	      mixerbuffer2[i] += 
		(*((unsigned char *)
		   ((unsigned char *) LoadedSounds[tmp_sound_elem->sfx].data
		    +tmp_sound_elem->pos+i)
		   )
		 ) - 128;
	    }
	  }
	}
	/** Update the position - Is this always correct? */
	tmp_sound_elem->pos += FRAGMENTSIZE;
	/** printf("\n"); */
      }
      
      /** printf("Updating soundlist ---------\n"); */
      
      /** Remove the stuff we are done playing. */
      head = NULL;
      tail = NULL;
      for(tmp_sound_elem = soundlist; tmp_sound_elem != NULL;) {
	if ((tmp_sound_elem->size - tmp_sound_elem->pos) > 0) {
	  /** Need to keep this element */
	  /** printf("Keeping signal %i from %i (out of %i)\n", 
		 tmp_sound_elem->sfx, 
		 tmp_sound_elem->pos,
		 tmp_sound_elem->size); */
	  if (NULL == head) {
	    /** First element in list! */
	    head = tmp_sound_elem;
	    tail = head;
	  } else {
	    /** Add to tail */
	    tail->next = tmp_sound_elem;
	    tail = tail->next;
	  }
	  tmp_sound_elem = tmp_sound_elem->next;
	} else {
	  /** printf("Dropping signal %i from %i (out of %i)\n", 
		 tmp_sound_elem->sfx, 
		 tmp_sound_elem->pos,
		 tmp_sound_elem->size); */
	  /** Free this element */
	  tmps = tmp_sound_elem->next;
	  free(tmp_sound_elem);
	  tmp_sound_elem = tmps;
	  if (NULL != tail) {
	    tail->next = tmp_sound_elem;
	  }
	}
      } /** All elements in list */

      /** Set soundlist to the new head */
      soundlist = head;

      /** Typeconvert into the bytemixer array */
      /** printf("Mixerbuffer\n"); */
      for (i = 0; i < FRAGMENTSIZE; i++) {
	if (mixerbuffer2[i] > 127) {
	  mixerbuffer[i] = 255;
	} else {
	  if (mixerbuffer2[i] < -127) {
	    mixerbuffer[i] = 0;
	  } else {
	    mixerbuffer[i] = (unsigned char) (mixerbuffer2[i] + 128);
	  }
	}

	/** printf("%x ", mixerbuffer[i]); */

      } /** Move buffer */
      /** printf("\nDone mixing sounds -------- \n"); */

    } else { /** Soundlist empty */
      memset(mixerbuffer, 128, FRAGMENTSIZE);
    }
       
    /** ********************************************************************** */
    /** Feed buffer to sound device */
    
    i = FRAGMENTSIZE;
    buf = mixerbuffer;
    while(i > 0) {
      status = write(SoundDevHandle, buf, i);
      if (status < 0) {
	i = -1;
      }
      //      /** Set sync.. dunno what it does, actually */
    //      /**     if ( ioctl(SoundDevHandle, SNDCTL_DSP_SYNC) == -1 ) {
    //	      perror("ioctl SNDCTL_DSP_SYNC");
    //	      sound_shutdown();
    //	      return errno;
    //	      }   */                    
      /** printf("sound play wrote %i bytes to sound dev\n", status); */
      buf=buf+status;
      i=i-status;
      
    }

    /* For now, dump the stuff */
    /** 
       printf("Sounds in sound list ----------------------------------\n");
    for(tmp_sound_elem = soundlist; 
	tmp_sound_elem != NULL; tmp_sound_elem = tmp_sound_elem->next) {
      printf("Signal %i, at %i, size %i\n", tmp_sound_elem->sfx, 
	     tmp_sound_elem->pos, tmp_sound_elem->size);
    }
    */


    /** Check if we need to stop */
    /** Note, not mutex protected, as it is readonly */
    /**    if (1 == ExitThread) {
      printf("ExitThread found\n");
      break;
      } */
    /** Check if we have been cancelled. */
    pthread_testcancel();
    /** Buffer over-/underrun handling */
    usleep(THREADSLEEP); /** sleep a little */
    /** Check if we have been cancelled. */
    pthread_testcancel();
  }
  pthread_cleanup_pop(0);

  /** I do not know if this makes sense .. */
  printf("Exiting thread\n");
  pthread_exit(NULL);
}
