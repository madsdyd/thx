/*
   Copyright (C) 2000 Nate Miller nkmiller@calpoly.edu

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   See gpl.txt for more information regarding the GNU General Public License.
*/
/*
   CHANGELOG
      - 04/27/00
         - code now deals with buffers rather than file streams   
         - added a header struct, things make more sense now
   TODO  
      - add ability to write tga from passed data and format
*/
#ifndef TGA_H
#define TGA_H

#include "utility.h"

enum
{
   notFound = 1000,
   badType,
   badBits,    
   badData,    
   bitsRGB = 24,
   bitsRGBA = 32,
   bitsGray = 8
};

typedef struct
{
   byte numIden;
   byte colorMapType;
   byte imageType;
   byte colorMapSpec[5]; // not used, just here to take up space
   byte origX[2];
   byte origY[2];
   byte width[2];
   byte height[2];
   byte bpp;
   byte imageDes; // don't use, space eater
} tgaHeader_t;

struct tga_t
{
   tga_t();
   tga_t(const char *name);
   virtual ~tga_t()           {}
   int Load(const char *name);
   int LoadBuffer(byte *buff) {return ParseBuffer(buff);}
   void Write(char *dest, byte *buff, int w, int h, int pbits);
   void Reset(void);
   void Release(void)         {delete [] data; data = 0;}
   int GetLastError(void)     {return lastError;}
   int GetWidth(void)         {return width;}
   void SetWidth(int w)       {width = w;}
   int GetHeight(void)        {return height;}
   void SetHeight(int h)      {height = h;}
   int GetBits(void)          {return bits;}
   void SetBits(int b)        {bits = b;}
   byte *GetData(void)        {return data;}
   void SetData(byte *src)    {data = src;}
   byte operator[](int ndx)   {return data[ndx];}
   
protected:
   int ParseBuffer(byte *buffer);
   int CheckSize(int x);
   byte *GetRGB(byte *buff, int size);
   byte *GetRGBA(byte *buff, int size);
   byte *GetGray(byte *buff, int size);
   byte *GrabData(byte *buff, int size);

   int lastError;
   int bits;
   int width;
   int height;
   int size;
   byte *data;
};
#endif
