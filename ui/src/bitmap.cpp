/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2012 Alvaro Polo
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

#include "KarenUI/bitmap.h"
#include <KarenCore/exception.h>

namespace karen { namespace ui {

#define SET_PIXEL8(buf, offset, value8) buf.set<UInt8>(value8, offset)
#define SET_PIXEL16(buf, offset, value16) buf.set<UInt16>(value16, offset)

#if KAREN_ENDIANNESS == KAREN_BIG_ENDIAN
   #define SET_PIXEL24(buf, offset, value32) \
      buf.set<UInt8>(value32      , offset + 2); \
      buf.set<UInt8>(value32 >> 8 , offset + 1); \
      buf.set<UInt8>(value32 >> 16, offset);
#else
   #define SET_PIXEL24(buf, offset, value32) \
      buf.set<UInt8>(value32      , offset); \
      buf.set<UInt8>(value32 >> 8 , offset + 1); \
      buf.set<UInt8>(value32 >> 16, offset + 2);
#endif   

#define SET_PIXEL32(buf, offset, value32) buf.set<UInt32>(value32, offset)

#define SET_PIXEL(bpp, buf, offset, value) \
{ \
   switch (bpp) \
   { \
      case 1: SET_PIXEL8(buf,  offset,  UInt8(value)); break; \
      case 2: SET_PIXEL16(buf, offset, UInt16(value)); break; \
      case 3: SET_PIXEL24(buf, offset, UInt32(value)); break; \
      case 4: SET_PIXEL32(buf, offset, UInt32(value)); break; \
   } \
}

#define GET_PIXEL8(buf, offset, value8) value8 = buf.get<UInt8>(offset);

#define GET_PIXEL16(buf, offset, value16) value16 = buf.get<UInt16>(offset);

#if KAREN_ENDIANNESS == KAREN_BIG_ENDIAN
   #define GET_PIXEL24(buf, offset, value32) { \
      UInt32 __byte1 = UInt32(buf.get<UInt8>(offset    )) << 16; \
      UInt32 __byte2 = UInt32(buf.get<UInt8>(offset + 1)) << 8; \
      UInt32 __byte3 = UInt32(buf.get<UInt8>(offset + 2)); \
      value32 = 0xffffffff & __byte1 & __byte2 & __byte3; \
   }
#else
   #define GET_PIXEL24(buf, offset, value32) { \
      UInt32 __byte1 = UInt32(buf.get<UInt8>(offset + 2)) << 16; \
      UInt32 __byte2 = UInt32(buf.get<UInt8>(offset + 1)) << 8; \
      UInt32 __byte3 = UInt32(buf.get<UInt8>(offset    )); \
      value32 = 0xffffffff & __byte1 & __byte2 & __byte3; \
   }
#endif   

#define GET_PIXEL32(buf, offset, value32) value32 = buf.get<UInt32>(offset);

#define GET_PIXEL(bpp, buf, offset, value) \
{ \
   switch (bpp) \
   { \
      case 1: GET_PIXEL8(buf,  offset, value); break; \
      case 2: GET_PIXEL16(buf, offset, value); break; \
      case 3: GET_PIXEL24(buf, offset, value); break; \
      case 4: GET_PIXEL32(buf, offset, value); break; \
   } \
}

#define CHECK_LOCKED(msg) \
   if (_lockCoord->isLocked(*this)) \
      KAREN_THROW(InvalidStateException, "%s: bitmap is locked", msg);

class DefaultLockCoordinator : public Bitmap::LockCoordinator
{
public:

   inline static DefaultLockCoordinator& instance()
   {
      DefaultLockCoordinator* inst = NULL;
      if (!inst)
         inst = new DefaultLockCoordinator();
      return *inst;
   }

   inline virtual ~DefaultLockCoordinator() 
   {}

   inline virtual bool isLocked(const Bitmap& bmp) const
   {
      try { return _locks.get(&bmp); }
      catch (NotFoundException&) { return false; }
   }
   
   inline virtual void lock(const Bitmap& bmp)
   { _locks[&bmp] = true; }
   
   inline virtual void unlock(const Bitmap& bmp)
   { _locks[&bmp] = false; }
   
   inline virtual void onBind(const Bitmap& bmp)
   { unlock(bmp); }
   
   inline virtual void onDispose(const Bitmap& bmp)
   { _locks.remove(&bmp); }

private:

   TreeMap<const Bitmap*, bool> _locks;
   
   inline DefaultLockCoordinator() {}

};

template <class PixelType>
void
fillGreyscaleRegionWithColor(
      Bitmap& img, 
      const IRect& reg,
      const Color& col)
throw (InvalidInputException)
{
   const PixelFormat& pf = img.pixelFormat();
   if ((pf != PixelFormat::FORMAT_8BPP_GREYSCALE) ||
       (pf != PixelFormat::FORMAT_16BPP_GREYSCALE))
      KAREN_THROW(InvalidInputException, 
         "cannot fill greyscale image: pixel format is not greyscale");
   
   if (pf.bytesPerPixel() != sizeof(PixelType))
      KAREN_THROW(InvalidInputException, 
         "cannot fill greyscale image: %d-bits pixel type for %d-bits image",
         pf.bitsPerPixel(), sizeof(PixelType) * 8);
   
   if (!reg.isInside(img.size()))
      KAREN_THROW(InvalidInputException, 
         "cannot fill image region: outside image boundaries");

   Buffer& pix = img.pixels();
   const IVector& pitch = img.pitch();

   // Luminance combination of RGB channels. 
   PixelType mpix = (col.r * 0.30 + col.g * 0.59 + col.b * 0.11) * 
                    pow(2.0, (sizeof(PixelType) - 8) * 8);
   for (int j = reg.y; j < reg.y + reg.h; j++)
      for (int i = reg.x; i < reg.x + reg.w; i++)
      {
         unsigned long offset = (j * pitch.x + i) * sizeof(PixelType);
         SET_PIXEL(sizeof(PixelType), pix, offset, mpix);
      }
}

static void
fillTruecolorRegionWithColor(
      Bitmap& img, 
      const IRect& reg,
      const Color& col)
throw (InvalidInputException)
{
   Buffer& pix = img.pixels();
   const PixelFormat& fmt = img.pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned int bpp = fmt.bytesPerPixel();
   const IVector& pitch = img.pitch();

   if (bpp != 3 && bpp != 4)
      KAREN_THROW(InvalidInputException, 
         "cannot fill true color image: invalid pixel format");

   if (!reg.isInside(img.size()))
      KAREN_THROW(InvalidInputException, 
         "cannot fill image region: outside image boundaries");

   // Prepare a master pixel with given color properties using source
   // image pixel format. This master will be used as pattern to be copied
   // over image pixels.
   UInt32 mpix = 0;
   mpix |= (UInt32(col.r) << shift.r) & mask.r;
   mpix |= (UInt32(col.g) << shift.g) & mask.g;
   mpix |= (UInt32(col.b) << shift.b) & mask.b;
   mpix |= (UInt32(col.a) << shift.a) & mask.a;

   for (int j = reg.y; j < reg.y + reg.h; j++)
      for (int i = reg.x; i < reg.x + reg.w; i++)
      {
         unsigned long poff = (j * pitch.x + i) * bpp;
         SET_PIXEL(bpp, pix, poff, mpix);
      }
}

Bitmap::Bitmap(const IVector& dims, 
               const IVector& pitch, 
               const PixelFormat& format)
throw (InvalidInputException)
 : _size(dims), _pitch(pitch), _format(format), _pixels(NULL), _lockCoord(NULL)
{
   if (pitch.x < dims.x || pitch.y < dims.y)
      KAREN_THROW(InvalidInputException, 
         "cannot initialize image object: invalid pitch as input");
   unsigned long npixels = pitch.x * pitch.y;
   if (npixels <= 0)
      KAREN_THROW(InvalidInputException, 
         "cannot initialize image object: invalid dimensions as input");
   unsigned long buflen = npixels * format.bytesPerPixel();
   _pixels = new Buffer(buflen);
   
   setLockCoordinator(&DefaultLockCoordinator::instance());
}

Bitmap::Bitmap(const IVector& dims, const PixelFormat& format)
throw (InvalidInputException)
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
   setLockCoordinator(&DefaultLockCoordinator::instance());
   
   bmp._pixels = NULL;
}
   
Bitmap::~Bitmap()
{ _lockCoord->onDispose(*this); }

Bitmap&
Bitmap::operator = (const Bitmap& bmp)
{
   _size       = bmp._size;
   _pitch      = bmp._pitch;
   _format     = bmp._format;
   _pixels     = bmp._pixels;
   _lockCoord  = NULL;
   
   setLockCoordinator(bmp._lockCoord);

   return *this;
}
   
Bitmap&
Bitmap::operator = (Bitmap&& bmp)
{
   _size    = bmp._size;
   _pitch   = bmp._pitch;
   _format  = bmp._format;
   _pixels  = bmp._pixels;
   _lockCoord  = NULL;
   
   setLockCoordinator(bmp._lockCoord);
   
   bmp._pixels = NULL;

   return *this;
}

void
Bitmap::setLockCoordinator(LockCoordinator* lockCoord)
{
   if (_lockCoord)
      _lockCoord->onDispose(*this);
   _lockCoord = lockCoord;
   _lockCoord->onBind(*this);
}

void
Bitmap::fillWithColor(const Color& col)
{ fillRegionWithColor(size(), col); }

void
Bitmap::fillRegionWithColor(const IRect& reg, const Color& col) 
throw (InvalidInputException)
{
   CHECK_LOCKED("cannot fill bitmap");
   const PixelFormat& fmt = pixelFormat();

   if (fmt == PixelFormat::FORMAT_8BPP_GREYSCALE)
      fillGreyscaleRegionWithColor<UInt8>(*this, reg, col);
   else if (fmt == PixelFormat::FORMAT_16BPP_GREYSCALE)
      fillGreyscaleRegionWithColor<UInt16>(*this, reg, col);
   else
      fillTruecolorRegionWithColor(*this, reg, col);
}
   
Color
Bitmap::pixelAt(const IVector& pos) const 
throw (InvalidInputException)
{
   if (!isValidPixelPosition(pos))
      KAREN_THROW(InvalidInputException, 
         "cannot obtain pixel data from bitmap: invalid position (%d, %d)",
         pos.x, pos.y);

   Color pix(0x00, 0x00, 0x00, 0x00);
   const PixelFormat& fmt = pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned long poff = pixelOffset(pos);
   
   UInt32 data = 0;
   GET_PIXEL(fmt.bytesPerPixel(), (*_pixels), poff, data);
      
   pix.r = (data & mask.r) >> shift.r;
   pix.g = (data & mask.g) >> shift.g;
   pix.b = (data & mask.b) >> shift.b;
   // No alpha channel means opaque pixel. If so, it needs to be fixed.
   pix.a = (mask.a == 0) ? 0xff : (data & mask.a) >> shift.a;
   
   return pix;
}

void
Bitmap::setPixelAt(const IVector& pos, const Color& color)
throw (InvalidInputException)
{
   CHECK_LOCKED("cannot set pixel data of image");
   if (!isValidPixelPosition(pos))
      KAREN_THROW(InvalidInputException, 
         "cannot set pixel data of image: invalid position");

   const PixelFormat& fmt = pixelFormat();   
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned long poff = pixelOffset(pos);
   unsigned int bpp = fmt.bytesPerPixel();
   
   UInt32 data = 0;   
   data |= (UInt32(color.r) << shift.r) & mask.r;
   data |= (UInt32(color.g) << shift.g) & mask.g;
   data |= (UInt32(color.b) << shift.b) & mask.b;
   data |= (UInt32(color.a) << shift.a) & mask.a;
   
   SET_PIXEL(bpp, (*_pixels), poff, data);
}

unsigned long
Bitmap::pixelOffset(const IVector& pos) const
{ return (pos.y * _pitch.x + pos.x) * _format.bytesPerPixel(); }

}}; /* namespace karen::ui */
