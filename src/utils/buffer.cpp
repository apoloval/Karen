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

const unsigned long DataBuffer::DEFAULT_LENGTH = 4096l;

DataBuffer::DataBuffer(unsigned long length, bool autosize)
 : _length(length),
   _autosize(autosize),
   _data(new UInt8[length]),
   _readCursor(_data),
   _writeCursor(_data)
{
}

DataBuffer::DataBuffer(void* data, unsigned long length, bool autosize)
 : _length(length),
   _autosize(autosize),
   _data((UInt8*) data),
   _readCursor(_data),
   _writeCursor(_data)
{
}

DataBuffer::~DataBuffer()
{
   close();
}

void
DataBuffer::resetForReading()
{
   _readCursor = _data;
}

void
DataBuffer::resetForWritting()
{
   _writeCursor = _data;
}

void
DataBuffer::reset()
{
   _readCursor = _writeCursor = _data;
}

unsigned long
DataBuffer::read(void* data, unsigned long len)
throw (InvalidStateException)
{
   unsigned long count = (unsigned int) (_writeCursor - _readCursor);
   if (count > len)
      count = len;
   memcpy(data, _readCursor, count);
   _readCursor += count;
   return count;
}

unsigned long
DataBuffer::read(OutputStream& data, unsigned long len)
throw (InvalidStateException)
{
   unsigned long count = (unsigned int) (_writeCursor - _readCursor);
   if (count > len)
      count = len;
   data.write(_readCursor, count);
   _readCursor += count;
   return count;   
}

unsigned long
DataBuffer::write(const void* data, unsigned long len)
throw (InvalidStateException)
{
   unsigned int left = (unsigned int) (_length - (_writeCursor - _data));
   if (left < len)
   {
      if (_autosize)
         enlarge(len + (unsigned int) (_writeCursor - _data));
      else
         len = left;
   }   
   memcpy(_writeCursor, data, len);
   _writeCursor += len;
   return len;   
}

unsigned long
DataBuffer::write(InputStream& data, unsigned long len)
throw (InvalidStateException)
{
   unsigned int left = (unsigned int) (_length - (_writeCursor - _data));
   if (left < len)
   {
      if (_autosize)
         enlarge(len + (unsigned int) (_writeCursor - _data));
      else
         len = left;
   }
   data.read(_writeCursor, len);
   _writeCursor += len;
   return len;   
}

void
DataBuffer::close()
{
   if (_data) delete _data;
   _readCursor = _writeCursor = _data = NULL;
   _length = 0;
}

void
DataBuffer::enlarge(unsigned long minBytes)
{
   if (_length < minBytes)
   {
      while (_length  < minBytes)
         _length *= 2;

      UInt8* newData = new UInt8[_length];
      memcpy(newData, _data, _length);
      _readCursor = &(newData[_readCursor - _data]);
      _writeCursor = &(newData[_writeCursor - _data]);
      delete _data;
      _data = newData;
   }
}


}}; /* namespace karen::utils */
