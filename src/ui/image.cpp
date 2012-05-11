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

#include "ui/image.h"
#include "utils/exception.h"

namespace karen { namespace ui {

AbstractImage::~AbstractImage()
{
   for (auto obs : _observers)
      obs->onImageDestruction(*this);
}

void
AbstractImage::addLifecycleObserver(ImageLifecycleObserver* obs)
throw (utils::InvalidInputException)
{
   if (_observers.hasElement(obs))
      KAREN_THROW(utils::InvalidInputException, 
            "cannot add image lifecycle observer: already present");
   _observers.insert(obs);
}

void
AbstractImage::removeLifecycleObserver(ImageLifecycleObserver* obs)
throw (utils::NotFoundException)
{
   if (!_observers.hasElement(obs))
      KAREN_THROW(utils::NotFoundException, 
            "cannot remove image lifecycle observer: not present");
   _observers.removeAll(obs);
}

Image::Image(const Vector& dims, const PixelFormat& format)
throw (utils::InvalidInputException)
 : _size(dims), _format(format)
{
   unsigned long npixels = dims.x * dims.y;
   if (npixels <= 0)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot initialize image object: invalid dimensions as input");
   _pixels = new UInt8[npixels * format.bytesPerPixel()];
}

Image::Image(const Image& img)
 : Image(img._size, img._format)
{
   memcpy(_pixels, img._pixels, _size.x * _size.y * _format.bytesPerPixel());
}
   
Image::~Image()
{
   delete []_pixels;
}
   
Image::Pixel
Image::pixelAt(const Vector& pos) const 
throw (utils::InvalidInputException)
{
   if (pos.x < 0.0 || pos.y < 0.0 || pos.x >= _size.x || pos.y >= _size.y)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot obtain pixel data from image: invalid position");

   Pixel pix = { 0x00, 0x00, 0x00, 0x00 };
   const PixelFormat& fmt = pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned int bpp = fmt.bytesPerPixel();
   long x = pos.x, y = pos.y, w = _size.x;
   
   UInt32 data = *(UInt32*)&_pixels[(y * w + x) * bpp];
      
   pix.r = (data & mask.r) >> shift.r;
   pix.g = (data & mask.g) >> shift.g;
   pix.b = (data & mask.b) >> shift.b;
   pix.a = (data & mask.a) >> shift.a;
   
   return pix;
}

void
Image::setPixelAt(const Vector& pos, const Pixel& pix)
throw (utils::InvalidInputException)
{
   if (pos.x < 0.0 || pos.y < 0.0 || pos.x >= _size.x || pos.y >= _size.y)
      KAREN_THROW(utils::InvalidInputException, 
         "cannot obtain pixel data from image: invalid position");

   const PixelFormat& fmt = pixelFormat();
   const PixelFormat::Mask& mask = fmt.mask();
   const PixelFormat::Shift& shift = fmt.shift();
   unsigned int bpp = fmt.bytesPerPixel();
   long x = pos.x, y = pos.y, w = _size.x;
   
   UInt32 data = *(UInt32*)&_pixels[(y * w + x) * bpp];
   
   // Fill with zeroes everything but the padding byte
   data &= ~(mask.r | mask.g | mask.b | mask.a);
   
   data |= ((UInt32) pix.r) << shift.r;
   data |= ((UInt32) pix.g) << shift.g;
   data |= ((UInt32) pix.b) << shift.b;
   data |= ((UInt32) pix.a) << shift.a;
   
   *(UInt32*)&_pixels[(y * w + x) * bpp] = data;
}

}}; /* namespace karen::ui */
