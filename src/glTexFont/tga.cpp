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
#include "tga.h"

tga_t::tga_t()
{
   lastError = 0;
   bits = width = height = size = 0; 
   data = 0;
}

tga_t::tga_t(char *name)
{
   lastError = 0;
   bits = width = height = size = 0; 
   data = 0;

   Load(name);
}

int tga_t::Load(char *name)
{
   byte type[4];
   byte info[7];
   FILE *iFile;

   if (!(iFile = fopen(name, "r+bt")))
      return Error(notFound, 0);

   fread(&type, sizeof(byte), 3, iFile); // read in colormap info and image type, byte 0 ignored
   fseek(iFile, 12, SEEK_SET); // seek past the header and useless info
   fread(&info, sizeof(byte), 6, iFile);
   fseek(iFile, type[0], SEEK_CUR); // skip past image identification

   if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
      return Error (badType, iFile);

   width = info[0] + info[1] * 256; 
   height = info[2] + info[3] * 256;
   bits =	info[4]; 

   size = width * height; 

   // make sure we are loading a supported type 
   if (bits != 32 && bits != 24 && bits != 8)
      return Error(badBits, iFile);

   data = GrabData(iFile, size);

   // no image data 
   if (data == 0)
      return Error(badData, iFile);

   fclose(iFile);

   return 1;
}

void tga_t::Reset(void)
{
   delete [] data;
   lastError = 0;
   bits = width = height = size = 0; 
   data = 0;
}

int tga_t::Error(int errNum, FILE *strm)
{
   lastError = errNum;

   if (strm)
      fclose(strm);

   return 0;
}  

byte *tga_t::GetRGBA(FILE *strm, int size)
{
   byte *rgba;
   byte temp;
   int bread;
   int i;

   rgba = new byte[size * 4]; 

   if (rgba == 0)
      return 0;

   bread = fread(rgba, sizeof(byte), size * 4, strm); 

   if (bread != size * 4)
   {
      delete [] rgba;
      return 0;
   }

   for (i = 0; i < size * 4; i += 4)
   {
      temp = rgba[i];
      rgba[i] = rgba[i + 2];
      rgba[i + 2] = temp;
   }

   return rgba;
}

byte *tga_t::GetRGB(FILE *strm, int size)
{
   byte *rgb;
   byte temp;
   int bread;
   int i;

   rgb = new byte[size * 3]; 

   if (rgb == 0)
      return 0;

   bread = fread (rgb, sizeof(byte), size * 3, strm);

   if (bread != size * 3)
   {
      delete [] rgb;
      return 0;
   }

   for (i = 0; i < size * 3; i += 3)
   {
      temp = rgb[i];
      rgb[i] = rgb[i + 2];
      rgb[i + 2] = temp;
   }

   return rgb;
}

byte *tga_t::GetGray(FILE *strm, int size)
{
   byte *grayData;
   int bread;

   grayData = new byte[size];

   if (grayData == 0)
      return 0;

   bread = fread(grayData, sizeof(byte), size, strm);

   if (bread != size)
   {
      delete [] grayData;
      return 0;
   }

   return grayData;
}

byte *tga_t::GrabData(FILE *strm, int size)
{
   if (bits == 32)
      return GetRGBA (strm, size);
   else if (bits == 24)
      return GetRGB (strm, size);	
   else if (bits == 8)
      return GetGray (strm, size);
   
   return 0;
}

