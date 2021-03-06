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

#include "KarenCore/buffer.h"

namespace karen {

Buffer::Buffer(unsigned long length)
 : _length(length), _data(new UInt8[length]), _dirty(false)
{
}

Buffer::Buffer(void* data, unsigned long length)
 : _length(length), _data((UInt8*) data), _dirty(false)
{
}

Buffer::Buffer(const Buffer& buf)
 : _length(buf._length), _data(new UInt8[buf._length]), _dirty(false)
{
   memcpy(_data, buf._data, _length);
}

Buffer::Buffer(Buffer&& buf)
 : _length(buf._length), _dirty(false), _data(buf._data)
{
   buf._data = NULL;
}

Buffer::~Buffer()
{ if (_data) delete _data; }

Buffer&
Buffer::operator = (const Buffer& buf)
{
   if (_data) delete _data;
   
   _length  = buf._length;
   _data    = buf._data;
   _dirty   = false;
   
   return *this;
}
   
Buffer&
Buffer::operator = (Buffer&& buf)
{
   if (_data) delete _data;
   
   _length  = buf._length;
   _data    = buf._data;
   _dirty   = false;
   
   buf._data = NULL;
   
   return *this;
}

void
Buffer::copyFromBuffer(
      const Buffer& src, 
      unsigned long len,
      unsigned long srcOffset, 
      unsigned long dstOffset)
throw (OutOfBoundsException)
{
   if (_length < dstOffset + len)
      KAREN_THROW(InvalidInputException,
         "cannot copy contents from buffer: destination range is not valid");
   if (src._length < srcOffset + len)
      KAREN_THROW(InvalidInputException,
         "cannot copy contents from buffer: source range is not valid");
   memcpy(&(_data[dstOffset]), &(src._data[srcOffset]), len);
   _dirty = true;
}

void
Buffer::read(UInt8* dest, unsigned long len, unsigned long offset) const
throw (OutOfBoundsException)
{
   if (!isValidRange(offset, len))
      KAREN_THROW(OutOfBoundsException, 
         "cannot read from buffer: invalid range %d+%d",
         offset, len);
   memcpy(dest, _data + offset, len);
}
   
void
Buffer::write(const UInt8* src, unsigned long len, unsigned long offset)
throw (OutOfBoundsException)
{
   if (!isValidRange(offset, len))
      KAREN_THROW(OutOfBoundsException, 
         "cannot write to buffer: invalid range %d+%d",
         offset, len);
   memcpy(_data + offset, src, len);
   _dirty = true;
}

unsigned long
BufferInputStream::readBytes(void* data, unsigned long len)
throw (IOException)
{
   unsigned long left = bytesLeftToRead();
   if (left < len)
      len = left;
   _buffer->read((UInt8*) data, len, _index);
   _index += len;
   return len;
}

unsigned long
BufferOutputStream::writeBytes(const void* data, unsigned long len)
throw (IOException)
{
   unsigned long left = bytesLeftToWrite();
   if (left < len)
      len = left;
   _buffer->write((UInt8*) data, len, _index);
   _index += len;
   return len;   
}

}; // namespace karen
