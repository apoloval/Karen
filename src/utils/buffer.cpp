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

#include "utils/buffer.h"

namespace karen { namespace utils {

Buffer::Buffer(unsigned long length) : Buffer(new UInt8[length], length)
{
}

Buffer::Buffer(void* data, unsigned long length)
 : _length(length), _data((UInt8*) data), _dirty(false)
{
}

Buffer::Buffer(const Buffer& buf)
 : Buffer(new UInt8[buf._length], buf._length)
{
   memcpy(_data, buf._data, _length);
}

Buffer::Buffer(Buffer&& buf)
 : _length(buf._length), _dirty(false), _data(buf._data)
{
}

Buffer::~Buffer()
{ if (_data) delete _data; }

void
Buffer::copyFromBuffer(
      const Buffer& src, 
      unsigned long len,
      unsigned long srcOffset, 
      unsigned long dstOffset)
throw (utils::OutOfBoundsException)
{
   if (_length < dstOffset + len)
      KAREN_THROW(utils::InvalidInputException,
         "cannot copy contents from buffer: destination range is not valid");
   if (src._length < srcOffset + len)
      KAREN_THROW(utils::InvalidInputException,
         "cannot copy contents from buffer: source range is not valid");
   memcpy(&(_data[dstOffset]), &(src._data[srcOffset]), len);
   _dirty = true;
}

void
Buffer::read(UInt8* dest, unsigned long len, unsigned long offset) const
throw (utils::OutOfBoundsException)
{
   if (!isValidRange(offset, len))
      KAREN_THROW(utils::OutOfBoundsException, 
         "cannot read from buffer: invalid range %d+%d",
         offset, len);
   memcpy(dest, _data + offset, len);
}
   
void
Buffer::write(const UInt8* src, unsigned long len, unsigned long offset)
throw (utils::OutOfBoundsException)
{
   if (!isValidRange(offset, len))
      KAREN_THROW(utils::OutOfBoundsException, 
         "cannot write to buffer: invalid range %d+%d",
         offset, len);
   memcpy(_data + offset, src, len);
   _dirty = true;
}

UInt8
BufferInputStream::read() 
throw (InvalidStateException)
{
   if (bytesLeftToRead())
   {
      UInt8 data;
      _buffer->read(&data, 1, _index++);
      return data;
   }
   else
      KAREN_THROW(InvalidStateException, 
         "cannot read from data buffer: no more bytes to read");
}

unsigned long
BufferInputStream::read(UInt8* data, unsigned long len)
throw (InvalidStateException)
{
   unsigned long left = bytesLeftToRead();
   if (left < len)
      len = left;
   _buffer->read(data, len, _index);
   _index += len;
   return len;
}

void
BufferOutputStream::write(const UInt8& data) 
throw (InvalidStateException)
{
   if (!bytesLeftToWrite())
      KAREN_THROW(InvalidStateException, 
         "cannot read from data buffer: no more bytes to read");
   _buffer->write(&data, sizeof(UInt8), _index++);
}

unsigned long
BufferOutputStream::write(const UInt8* data, unsigned long len)
throw (InvalidStateException)
{
   unsigned long left = bytesLeftToWrite();
   if (left < len)
      len = left;
   _buffer->write(data, len, _index);
   _index += len;
   return len;   
}

}}; /* namespace karen::utils */
