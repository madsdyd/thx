/***************************************************************************
                          sound_dump.c  -  description
 
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

#define DefSoundSetting 0x00030009 /** 512 bytes, 11025 / 512 ~= 21.5 ms */
#define DefSoundChannels 0
#define DefSoundFormat AFMT_U8
#define DefSoundRate 11025
int SoundDevHandle = -1;
int SoundSetting = DefSoundSetting;   
int SoundChannels = DefSoundChannels; /** 0=mono 1=stereo */
int SoundFormat = DefSoundFormat;     /** unsigned 8 bit...  */
int SoundRate = DefSoundRate;         /** Low sample rate */


int main(int argc, char * argv[]) {
  
  char * dev;

  dev = "/dev/dsp";
  if (1 < argc) {
    dev = argv[1];
  }
  
  printf("Opening sound device --%s--\n", dev);
  if ( (SoundDevHandle = open(dev,O_WRONLY)) == -1 ) {
    perror("open sounddev");  
    return -1;
  }                            
  
  /** Device opened, set some options. */
  
  /** Setup the buffer we will use */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SETFRAGMENT, &SoundSetting) == -1 ) {
    perror("SoundDevHandle; ioctl set fragment");
  }              
  printf("SoundSetting; wanting %x got %x\n", 
	 DefSoundSetting, SoundSetting);
  
  /** Set channels */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_STEREO, &SoundChannels) == -1 ) {
    perror("SoundChannels; ioctl mono");
  }              
  printf("SoundChannels; wanting %x got %x\n", 
	 DefSoundChannels, SoundChannels);
                      
  /** Set the format */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SETFMT, &SoundFormat) == -1 ) {
    perror("SoundFormat; ioctl format");
  }
  printf("SoundFormat; wanting %x got %x\n", 
	 DefSoundFormat, SoundFormat);

  /** Set the speed/rate */
  if ( ioctl(SoundDevHandle, SNDCTL_DSP_SPEED, &SoundRate) == -1 ) {
    perror("SoundRate; ioctl sample rate");
  }                        
  printf("SoundRate; wanting %x got %x\n", 
	 DefSoundRate, SoundRate);

  if (SoundDevHandle != -1) {
    if ( ioctl(SoundDevHandle, SNDCTL_DSP_SYNC) == -1 ) {
      perror("ioctl SNDCTL_DSP_SYNC");
    } 
    close(SoundDevHandle);
    printf("Sound device closed\n");
    SoundDevHandle = -1;
  }                      
  return 0;

}








