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
};

struct tga_t
{
   tga_t();
   tga_t(char *name);
   virtual ~tga_t()         {}
   int Load(char *name);
   void Reset(void);
   void Free(void)          {delete [] data;}
   int LastError(void)      {return lastError;}
   int Width(void)          {return width;}
   void Width(int w)        {width = w;}
   int Height(void)         {return height;}
   void Height(int h)       {height = h;}
   int Bits(void)           {return bits;}
   void Bits(int b)         {bits = b;}
   byte *Data(void)         {return data;}
   void Data(byte *src)     {data = src;}
   byte operator[](int ndx) {return data[ndx];}

protected:
   int Error(int errNum, FILE *strm);
   int CheckSize(int x);
   byte *GetRGB(FILE *strm, int size);
   byte *GetRGBA(FILE *strm, int size);
   byte *GetGray(FILE *strm, int size);
   byte *GrabData(FILE *strm, int size);

   int lastError;
   int bits;
   int width;
   int height;
   int size;
   byte *data;
};
#endif