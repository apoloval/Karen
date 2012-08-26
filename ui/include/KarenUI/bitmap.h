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

#ifndef KAREN_UI_BITMAP_H
#define KAREN_UI_BITMAP_H

#include "KarenUI/types.h"
#include <KarenCore/buffer.h>
#include <KarenCore/collection.h>
#include <KarenCore/exception.h>
#include <KarenCore/platform.h>

namespace karen { namespace ui {

class Bitmap;

/**
 * Bitmap class. This class holds a map of bits used to represent images
 * on a canvas. 
 */
class Bitmap
{
public:

   /**
    * Bitmap lock coordinator class. This abstract class provides the interface 
    * for an object able to coordinate bitmap locking. 
    */
   class LockCoordinator
   {
   public:

      /**
       * Virtual destructor.
       */
      inline virtual ~LockCoordinator() {}

      /**
       * Check whether a bitmap being coordinated is locked.
       */
      virtual bool isLocked(const Bitmap& bmp) const = 0;
      
      /**
       * Lock a coordinated bitmap.
       */
      virtual void lock(const Bitmap& bmp) = 0;
      
      /**
       * Unlock a coordinated bitmap.
       */
      virtual void unlock(const Bitmap& bmp) = 0;
      
      /**
       * Callback invoked when a new bitmap is bounded to this coordinator.
       */
      virtual void onBind(const Bitmap& bmp) = 0;
      
      /**
       * Callback invoked when a binding between this coordinator and given
       * bitmap is disposed.
       */
      virtual void onDispose(const Bitmap& bmp) = 0;
            
   };

   /**
    * Create an empty bitmap with given dimensions, pitch and format. If given
    * dimensions or pitch are not valid, a InvalidInputException is thrown.
    */
   Bitmap(const IVector& dims, const IVector& pitch, const PixelFormat& format) 
         throw (InvalidInputException);
         
   /**
    * Create an empty bitmap with given dimensions and format. If given
    * dimensions are not valid, a InvalidInputException is thrown.
    */
   Bitmap(const IVector& dims, const PixelFormat& format) 
         throw (InvalidInputException);
         
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
    * Set lock coordinator for this bitmap.
    */
   void setLockCoordinator(LockCoordinator* lockCoord);
   
   /**
    * Obtain the lock coordinator for this bitmap.
    */
   inline const LockCoordinator& lockCoordinator() const
   { return *_lockCoord; }
   
   /**
    * Fill this bitmap with given color. 
    */
   void fillWithColor(const Color& col);
   
   /**
    * Fill the given region of this bitmap with given color. This operation 
    * fills the region determined by reg with col color. If reg is out of
    * image boundaries, InvalidInputException is thrown.
    */
   void fillRegionWithColor(const IRect& reg, const Color& col) 
         throw (InvalidInputException);
   /**
    * Obtain the offset of the pixel at given position in the data buffer 
    * in bytes. 
    */
   unsigned long pixelOffset(const IVector& pos) const;
   
   /**
    * Check whether given vector is a valid pixel position for this bitmap.
    */
   inline bool isValidPixelPosition(const IVector& pos) const
   { return pos.isInside(IRect(_size)); }

   /**
    * Obtain the pitch of the bitmap, i.e. the exact number of columns and 
    * rows in the data buffer. 
    */
   inline const IVector& pitch() const
   { return _pitch; }
   
   /**
    * Obtain the color value of the pixel stored at given position.
    */
   Color pixelAt(const IVector& pos) const 
         throw (InvalidInputException);
   
   /** 
    * Obtain the data buffer that holds the pixels. 
    */
   inline Buffer& pixels()
   { return *_pixels; }

   /** 
    * Obtain the data buffer that holds the pixels. 
    */
   inline const Buffer& pixels() const
   { return *_pixels; }
   
   /**
    * Obtain the bitmap pixel format. 
    */
   inline const PixelFormat& pixelFormat() const
   { return _format; }
   
   /**
    * Set the color of the pixel stored at given position.
    */
   void setPixelAt(const IVector& pos, const Color& pix)
         throw (InvalidInputException);

   /**
    * Obtain the size of the bitmap in pixels.
    */
   inline const IVector& size() const
   { return _size; }
   
private:

   IVector              _size;
   IVector              _pitch;
   PixelFormat          _format;
   Ptr<Buffer>   _pixels;
   LockCoordinator*     _lockCoord;

};

}}; /* Namespace karen::ui */

#endif
