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

class Image;

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
   virtual void onImageModification(const Image& img) = 0;

   /**
    * Callback method for image destruction.
    */
   virtual void onImageDestruction(const Image& img) = 0;

};

/**
 * Image class. This abstract class represents an image that
 * may be processed by screen objects for drawing.
 */
class Image
{
public:

   /**
    * Obtain the pixel format mask for given pixel format.
    */
   static const PixelFormatMask& pixelFormatMask(PixelFormat format);
   
   /**
    * Virtual destructor.
    */
   virtual ~Image();
   
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
   virtual PixelFormat pixelFormat() const = 0;
   
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

}}; /* Namespace karen::ui */

#endif
