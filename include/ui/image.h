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

#ifndef KAREN_UI_IMAGE_H
#define KAREN_UI_IMAGE_H

#include "ui/types.h"
#include "utils/collection.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui {

class AbstractImage;

/**
 * Image lifecycle observer class. This class provides an interface for
 * an object with the ability of observe the lifecycle of an Image instance.
 */
class ImageLifecycleObserver
{
public:

   /**
    * Callback method for image modification.
    */
   virtual void onImageModification(const AbstractImage& img) = 0;

   /**
    * Callback method for image destruction.
    */
   virtual void onImageDestruction(const AbstractImage& img) = 0;

};

class AbstractImage
{
public:

   /**
    * Pixel type. This type provides color information for a pixel.
    */
   struct Pixel
   {
      UInt8 r;
      UInt8 g;
      UInt8 b;
      UInt8 a;
   };

   /**
    * Virtual destructor.
    */
   virtual ~AbstractImage();
   
   /**
    * Add a new lifecycle observer. If observer is already present, a
    * InvalidInputException is thrown.
    */
   void addLifecycleObserver(ImageLifecycleObserver* obs)
         throw (utils::InvalidInputException);
         
   /**
    * Remove a lifecycle observer. If observer is not present, a
    * NotFoundException is raised.
    */
   void removeLifecycleObserver(ImageLifecycleObserver* obs)
         throw (utils::NotFoundException);

   /**
    * Obtain image size.
    */
   virtual const Vector& size() const = 0;
   
   /**
    * Obtain pixel format.
    */
   virtual const PixelFormat& pixelFormat() const = 0;
   
   /**
    * Obtain the pixel at given position. If position is not valid for
    * this image, a InvalidInputException is thrown.
    */
   virtual Pixel pixelAt(const Vector& pos) const 
         throw (utils::InvalidInputException) = 0;
         
   /**
    * Set pixel for given image position. If position is not valid,
    * a InvalidInputException is thrown.
    */
   virtual void setPixelAt(const Vector& pos, const Pixel& pix)
         throw (utils::InvalidInputException) = 0;
   
   /**
    * Obtain a pointer to raw pixels on memory.
    */
   virtual void* pixels() = 0;

   /**
    * Obtain a pointer to raw pixels on memory.
    */
   virtual const void* pixels() const = 0;
   
private:

   Set<ImageLifecycleObserver*> _observers;

};

/**
 * Image class. This abstract class represents an image that
 * may be processed by screen objects for drawing.
 */
class Image : public AbstractImage
{
public:

   /**
    * Create an empty image with given dimensions and format. If given
    * dimensions are not valid, a InvalidInputException is thrown.
    */
   Image(const Vector& dims, const PixelFormat& format) 
         throw (utils::InvalidInputException);
         
   /**
    * Create a new image object as a copy of that one passed as argument.
    */
   Image(const Image& img);
   
   /**
    * Virtual destructor.
    */
   virtual ~Image();
   
   /**
    * Obtain image size.
    */
   inline virtual const Vector& size() const
   { return _size; }
   
   /**
    * Obtain pixel format.
    */
   inline virtual const PixelFormat& pixelFormat() const
   { return _format; }
   
   /**
    * Obtain the pixel at given position. If position is not valid for
    * this image, a InvalidInputException is thrown.
    */
   virtual Pixel pixelAt(const Vector& pos) const 
         throw (utils::InvalidInputException);
   
   /**
    * Set pixel for given image position. If position is not valid,
    * a InvalidInputException is thrown.
    */
   virtual void setPixelAt(const Vector& pos, const Pixel& pix)
         throw (utils::InvalidInputException) = 0;

   /**
    * Obtain a pointer to raw pixels on memory.
    */
   inline virtual void* pixels()
   { return _pixels; }

   /**
    * Obtain a pointer to raw pixels on memory.
    */
   inline virtual const void* pixels() const
   { return _pixels; }

private:

   Vector      _size;
   PixelFormat _format;
   UInt8*      _pixels;

};

}}; /* Namespace karen::ui */

#endif
