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

#ifndef KAREN_UI_BITMAP_H
#define KAREN_UI_BITMAP_H

#include "ui/types.h"
#include "utils/buffer.h"
#include "utils/collection.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui {

/**
 * Bitmap class. This class holds a map of bits used to represent images
 * on a canvas. 
 */
class Bitmap
{
public:

   /**
    * Create an empty bitmap with given dimensions, pitch and format. If given
    * dimensions or pitch are not valid, a InvalidInputException is thrown.
    */
   Bitmap(const Vector& dims, const Vector& pitch, const PixelFormat& format) 
         throw (utils::InvalidInputException);
         
   /**
    * Create an empty bitmap with given dimensions and format. If given
    * dimensions are not valid, a InvalidInputException is thrown.
    */
   Bitmap(const Vector& dims, const PixelFormat& format) 
         throw (utils::InvalidInputException);
         
   /**
    * Create a new bitmap object as a copy of that one passed as argument.
    */
   Bitmap(const Bitmap& bmp);
   
   /**
    * Move the contents of bitmap object into a new bitmap instance.
    */
   Bitmap(Bitmap&& bmp);
   
   /**
    * Virtual destructor.
    */
   virtual ~Bitmap();
   
   /**
    * Assign operator.
    */
   Bitmap& operator = (const Bitmap& bmp);
   
   /**
    * Move operator.
    */
   Bitmap& operator = (Bitmap&& bmp);
   
   /**
    * Fill this bitmap with given color. 
    */
   void fillWithColor(const Color& col);
   
   /**
    * Fill the given region of this bitmap with given color. This operation 
    * fills the region determined by reg with col color. If reg is out of
    * image boundaries, InvalidInputException is thrown.
    */
   void fillRegionWithColor(const Rect& reg, const Color& col) 
         throw (utils::InvalidInputException);
   /**
    * Obtain the offset of the pixel at given position in the data buffer 
    * in bytes. 
    */
   unsigned long pixelOffset(const Vector& pos) const;
   
   /**
    * Check whether given vector is a valid pixel position for this bitmap.
    */
   inline bool isValidPixelPosition(const Vector& pos) const
   { return pos.isInside(Rect(_size)); }

   /**
    * Obtain the pitch of the bitmap, i.e. the exact number of columns and 
    * rows in the data buffer. 
    */
   inline const Vector& pitch() const
   { return _pitch; }
   
   /**
    * Obtain the color value of the pixel stored at given position.
    */
   Color pixelAt(const Vector& pos) const 
         throw (utils::InvalidInputException);
   
   /** 
    * Obtain the data buffer that holds the pixels. 
    */
   inline utils::Buffer& pixels()
   { return *_pixels; }

   /** 
    * Obtain the data buffer that holds the pixels. 
    */
   inline const utils::Buffer& pixels() const
   { return *_pixels; }
   
   /**
    * Obtain the bitmap pixel format. 
    */
   inline const PixelFormat& pixelFormat() const
   { return _format; }
   
   /**
    * Set the color of the pixel stored at given position.
    */
   void setPixelAt(const Vector& pos, const Color& pix)
         throw (utils::InvalidInputException);

   /**
    * Obtain the size of the bitmap in pixels.
    */
   inline const Vector& size() const
   { return _size; }
   
private:

   Vector               _size;
   Vector               _pitch;
   PixelFormat          _format;
   Ptr<utils::Buffer>   _pixels;

};

}}; /* Namespace karen::ui */

#endif
