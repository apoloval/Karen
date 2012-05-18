/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301 USA
 * 
 * ---------------------------------------------------------------------
 */

#include <cstring>

#include "ui/bitmap.h"
#include "utils/exception.h"

namespace karen { namespace ui {

static void
fillGreyscale8RegionWithColor(
      Bitmap& img, 
      const IRect& reg,
      const Color& col)
throw (utils::InvalidInputException)
{
   if (img.pixelFormat() != PixelFormat::FORMAT_8BPP_GREYSCALE)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill 8-bit greyscale image: invalid pixel format");
   
   if (!reg.isInside(img.size()))
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill image region: outside image boundaries");

   utils::Buffer& pix = img.pixels();
   const IVector& pitch = img.pitch();

   // Luminance combination of RGB channels. 
   UInt8 mpix = col.r * 0.30 + col.g * 0.59 + col.b * 0.11;
   for (int j = reg.y; j < reg.y + reg.h; j++)
      for (int i = reg.x; i < reg.x + reg.w; i++)
      {
         unsigned long offset = (j * pitch.x + i) * sizeof(UInt8);
         pix.set<UInt8>(mpix, offset);
      }
}

static void
fillGreyscale16RegionWithColor(
      Bitmap& img, 
      const IRect& reg,
      const Color& col)
throw (utils::InvalidInputException)
{
   if (img.pixelFormat() != PixelFormat::FORMAT_16BPP_GREYSCALE)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill 16-bit greyscale image: invalid pixel format");

   if (!reg.isInside(img.size()))
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill image region: outside image boundaries");

   utils::Buffer& pix = img.pixels();
   const IVector& pitch = img.pitch();

   // Luminance combination of RGB channels. 
   UInt16 mpix = (col.r * 0.30 + col.g * 0.59 + col.b * 0.11) * 255;
   for (int j = reg.y; j < reg.y + reg.h; j++)
      for (int i = reg.x; i < reg.x + reg.w; i++)
      {
         unsigned long offset = (j * pitch.x + i) * sizeof(UInt16);
         pix.set<UInt16>(mpix, offset);
      }
}

static void
fillTruecolorRegionWithColor(
      Bitmap& img, 
      const IRect& reg,
      const Color& col)
throw (utils::InvalidInputException)
{
   utils::Buffer& pix = img.pixels();
   const PixelFormat& fmt = img.pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned int bpp = fmt.bytesPerPixel();
   const IVector& pitch = img.pitch();

   if (bpp != 3 && bpp != 4)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill true color image: invalid pixel format");

   if (!reg.isInside(img.size()))
      KAREN_THROW(utils::InvalidInputException, 
         "cannot fill image region: outside image boundaries");

   // Prepare a master pixel with given color properties using source
   // image pixel format. This master will be used as pattern to be copied
   // over image pixels.
   UInt32 mpix = 0;
   mpix |= (((UInt32) col.r) << shift.r) & mask.r;
   mpix |= (((UInt32) col.g) << shift.g) & mask.g;
   mpix |= (((UInt32) col.b) << shift.b) & mask.b;
   mpix |= (((UInt32) col.a) << shift.a) & mask.a;

   for (int j = reg.y; j < reg.y + reg.h; j++)
      for (int i = reg.x; i < reg.x + reg.w; i++)
      {
         unsigned long poff = (j * pitch.x + i) * bpp;
         pix.set<UInt32>(mpix, poff);
      }
}

Bitmap::Bitmap(const IVector& dims, 
               const IVector& pitch, 
               const PixelFormat& format)
throw (utils::InvalidInputException)
 : _size(dims), _pitch(pitch), _format(format), _pixels(NULL)
{
   if (pitch.x < dims.x || pitch.y < dims.y)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot initialize image object: invalid pitch as input");
   unsigned long npixels = pitch.x * pitch.y;
   if (npixels <= 0)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot initialize image object: invalid dimensions as input");
   unsigned long buflen = npixels * format.bytesPerPixel();
   if (buflen % 4)
      buflen += 4 - (buflen % 4);
   _pixels = new utils::Buffer(buflen);
}

Bitmap::Bitmap(const IVector& dims, const PixelFormat& format)
throw (utils::InvalidInputException)
 : Bitmap(dims, dims, format)
{
}

Bitmap::Bitmap(const Bitmap& bmp)
 : Bitmap(bmp._size, bmp._pitch, bmp._format)
{
   _pixels->copyFromBuffer(*bmp._pixels, bmp._pixels->length());
}

Bitmap::Bitmap(Bitmap&& bmp)
 : _size(bmp._size), _pitch(bmp._pitch), 
   _format(bmp._format), _pixels(bmp._pixels)
{
   bmp._pixels = NULL;
}
   
Bitmap::~Bitmap()
{
}

Bitmap&
Bitmap::operator = (const Bitmap& bmp)
{
   _size    = bmp._size;
   _pitch   = bmp._pitch;
   _format  = bmp._format;
   _pixels  = bmp._pixels;      

   return *this;
}
   
Bitmap&
Bitmap::operator = (Bitmap&& bmp)
{
   _size    = bmp._size;
   _pitch   = bmp._pitch;
   _format  = bmp._format;
   _pixels  = bmp._pixels;      
   
   bmp._pixels = NULL;

   return *this;
}

void
Bitmap::fillWithColor(const Color& col)
{ fillRegionWithColor(size(), col); }

void
Bitmap::fillRegionWithColor(const IRect& reg, const Color& col) 
throw (utils::InvalidInputException)
{
   const PixelFormat& fmt = pixelFormat();

   if (fmt == PixelFormat::FORMAT_8BPP_GREYSCALE)
      fillGreyscale8RegionWithColor(*this, reg, col);
   else if (fmt == PixelFormat::FORMAT_16BPP_GREYSCALE)
      fillGreyscale16RegionWithColor(*this, reg, col);
   else
      fillTruecolorRegionWithColor(*this, reg, col);
}
   
Color
Bitmap::pixelAt(const IVector& pos) const 
throw (utils::InvalidInputException)
{
   if (!isValidPixelPosition(pos))
      KAREN_THROW(utils::InvalidInputException, 
         "cannot obtain pixel data from bitmap: invalid position (%d, %d)",
         pos.x, pos.y);

   Color pix(0x00, 0x00, 0x00, 0x00);
   const PixelFormat& fmt = pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned long poff = pixelOffset(pos);
   
   UInt32 data = _pixels->get<UInt32>(poff);
      
   pix.r = (data & mask.r) >> shift.r;
   pix.g = (data & mask.g) >> shift.g;
   pix.b = (data & mask.b) >> shift.b;
   // No alpha channel means opaque pixel. If so, it needs to be fixed.
   pix.a = (mask.a == 0) ? 0xff : (data & mask.a) >> shift.a;
   
   return pix;
}

void
Bitmap::setPixelAt(const IVector& pos, const Color& color)
throw (utils::InvalidInputException)
{
   if (!isValidPixelPosition(pos))
      KAREN_THROW(utils::InvalidInputException, 
         "cannot obtain pixel data from image: invalid position");

   const PixelFormat& fmt = pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned long poff = pixelOffset(pos);
   
   UInt32 data = _pixels->get<UInt32>(poff);
   
   // Fill with zeroes everything but the padding bytes
   data &= ~(mask.r | mask.g | mask.b | mask.a);
   
   data |= ((UInt32) color.r) << shift.r;
   data |= ((UInt32) color.g) << shift.g;
   data |= ((UInt32) color.b) << shift.b;
   data |= ((UInt32) color.a) << shift.a;
   
   _pixels->set<UInt32>(data, poff);
}

unsigned long
Bitmap::pixelOffset(const IVector& pos) const
{ return (pos.y * _pitch.x + pos.x) * _format.bytesPerPixel(); }

}}; /* namespace karen::ui */
