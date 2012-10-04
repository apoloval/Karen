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

#ifndef KAREN_UI_PIXEL_H
#define KAREN_UI_PIXEL_H

#include "KarenCore/platform.h"

namespace karen { namespace ui {

/**
 * Pixel format class. This class encapsulates the pixel format data. 
 */
class PixelFormat
{
public:

   /**
    * Pixel format mask. This struct provides the binary mask used to
    * retrieve the RGBA values from each pixel in a 32-bit length binary 
    * representation.
    */
   struct Mask
   {
      UInt32 r;
      UInt32 g;
      UInt32 b;
      UInt32 a;
      
      /**
       * Create a mask object from given RGBA mask values. 
       */
      inline Mask(UInt32 theR, UInt32 theG, UInt32 theB, UInt32 theA)
       : r(theR), g(theG), b(theB), a(theA)
      {
      }
      
      /**
       * Equals to operator.
       */
      bool operator == (const Mask& m) const
      { return r == m.r && g == m.g && b == m.b && a == m.a; }
      
      /**
       * Not equals to operator.
       */
      bool operator != (const Mask& m) const
      { return !(*this == m); }

   };
   
   /**
    * Pixel format shift. This struct provides the left shift of the RGBA 
    * values of each pixel in a 32-bit length binary representation.  
    */
   struct Shift
   {
      unsigned int r;
      unsigned int g;
      unsigned int b;
      unsigned int a;

      /**
       * Create a mask object from given RGBA mask values. 
       */
      Shift(unsigned int theR, unsigned int theG, 
            unsigned int theB, unsigned int theA)
       : r(theR), g(theG), b(theB), a(theA)
      {
      }
      
      /**
       * Equals to operator.
       */
      bool operator == (const Shift& s) const
      { return r == s.r && g == s.g && b == s.b && a == s.a; }
      
      /**
       * Not equals to operator.
       */
      bool operator != (const Shift& s) const
      { return !(*this == s); }

   };
   
   /**
    * 8 bits per pixel, single greyscale byte.
    */
   static const PixelFormat FORMAT_8BPP_GREYSCALE;
   
   /**
    * 16 bits per pixel, double greyscale byte.
    */
   static const PixelFormat FORMAT_16BPP_GREYSCALE;
   
   /**
    * 24 bits per pixel, RGB channels.
    */
   static const PixelFormat FORMAT_24BPP_RGB;
   
   /**
    * 24 bits per pixel, BGR channels.
    */
   static const PixelFormat FORMAT_24BPP_BGR;
   
   /**
    * 32 bits per pixel, RGB channels, right padding.
    */
   static const PixelFormat FORMAT_32BPP_RGBX;
   
   /**
    * 32 bits per pixel, RGB channels, left padding.
    */
   static const PixelFormat FORMAT_32BPP_XRGB;
   
   /**
    * 32 bits per pixel, BGR channels, right padding.
    */
   static const PixelFormat FORMAT_32BPP_BGRX;
   
   /**
    * 32 bits per pixel, BGR channels, left padding.
    */
   static const PixelFormat FORMAT_32BPP_XBGR;
   
   /**
    * 32 bits per pixel, RGBA channels.
    */
   static const PixelFormat FORMAT_32BPP_RGBA;
   
   /**
    * 32 bits per pixel, ABGR channels.
    */
   static const PixelFormat FORMAT_32BPP_ABGR;
   
   /**
    * Copy constructor.
    */
   inline PixelFormat(const PixelFormat& fmt)
    : _mask(fmt._mask), _shift(fmt._shift), _bitsPerPixel(fmt._bitsPerPixel)
   {
   }
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const PixelFormat& fmt) const
   { return _mask == fmt._mask && _bitsPerPixel == fmt._bitsPerPixel; }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const PixelFormat& fmt) const
   { return !(*this == fmt); }
   
   /**
    * Obtain pixel format mask.
    */
   inline const Mask& mask() const
   { return _mask; }
   
   /**
    * Obtain pixel format shift.
    */
   inline const Shift& shift() const
   { return _shift; }
   
   /**
    * Obtain the bits per pixel for this format.
    */
   inline unsigned int bitsPerPixel() const
   { return _bitsPerPixel; }
   
   /**
    * Obtain the bytes per pixel for this format.
    */
   inline unsigned int bytesPerPixel() const
   { return _bitsPerPixel / 8; }
   
private:

   /**
    * Private constructor.
    */
   inline PixelFormat(const Mask& mask, const Shift& shift, unsigned int bpp)
    : _mask(mask), _shift(shift), _bitsPerPixel(bpp)
   {
   }

   Mask           _mask;
   Shift          _shift;
   unsigned int   _bitsPerPixel;
   
};

}}

#endif