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

#include "ui/draw.h"

namespace karen { namespace ui {

utils::Bolt<Bitmap>&
BitmapBinding::bitmap()
throw (utils::InvalidStateException)
{
   if (_unlocked)
      KAREN_THROW(utils::InvalidStateException,
         "cannot fetch bitmap for bitmap binding: the binding is locked");
   return _bolt;
}

void
BitmapBinding::lock()
{
   if (_unlocked)
   {
      _unlocked = false;
      onLock();
   }
}

void
BitmapBinding::unlock()
{
   if (!_unlocked)
   {
      _unlocked = true;
      onUnlock();
   }
}

BitmapBinding::BitmapBinding(
      const Bitmap& bitmap, const DrawingContext& parentContext)
 : _unlocked(false), 
   _parentContext(&parentContext), 
   _bitmap(bitmap),
   _bolt(&_bitmap, utils::BooleanBoltCondition(&_unlocked))
{}

}}; // namespace karen::ui
