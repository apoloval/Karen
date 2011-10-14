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

#include "ui/image.h"
#include "utils/exception.h"

namespace karen { namespace ui {

Image::~Image()
{
   for (auto obs : _observers)
      obs->onImageDestruction(*this);
}

void
Image::addLifecycleObserver(ImageLifecycleObserver* obs)
throw (utils::InvalidInputException)
{
   if (_observers.contains(obs))
      KAREN_THROW(utils::InvalidInputException, 
            "cannot add image lifecycle observer: already present");
   _observers.insert(obs);
}

void
Image::removeLifecycleObserver(ImageLifecycleObserver* obs)
throw (utils::NotFoundException)
{
   if (!_observers.contains(obs))
      KAREN_THROW(utils::NotFoundException, 
            "cannot remove image lifecycle observer: not present");
   _observers.remove(obs);
}

}}; /* namespace karen::ui */
