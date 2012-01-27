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

#ifndef KAREN_UTILS_BUFFER_H
#define KAREN_UTILS_BUFFER_H

#include "utils/platform.h"
#include "utils/stream.h"

namespace karen { namespace utils {

/**
 * Dynamic data buffer class. This class provides an implementation for a
 * dynamic data buffer. That's a data buffer which is automatically enlarged
 * when full. 
 */
class Buffer
{
public:

   /**
    * Create a new buffer with given properties. 
    */
   Buffer(unsigned long length);
   
   /**
    * Create a new buffer from given initial data.
    */
   Buffer(void* data, unsigned long length);

   /**
    * Destroy the buffer and deallocate all its memory. 
    */
   virtual ~Buffer();
   
   /**
    * Copy the contents of given buffer to this one. Copy len bytes from src
    * starting from srcOffset into this buffer starting at dstOffset. If
    * source or destination ranges are not valid, a InvalidInputException
    * is thrown. 
    */
   void copyFromBuffer(
         const Buffer& src, 
         unsigned long len,
         unsigned long srcOffset = 0, 
         unsigned long dstOffset = 0) throw (utils::OutOfBoundsException);
   
   /**
    * Obtain current buffer length.
    */
   inline unsigned long length() const
   { return _length; }
   
   /**
    * Check whether the range defined by given offset and length is valid.
    */
   inline bool isValidRange(unsigned long offset, unsigned long len) const
   { return offset + len < _length; }
   
   /**
    * Read bytes. Read len bytes from given offset and store them in dest. 
    * If requested byte range is out of buffer boundaries, a 
    * OutOfBoundsException is thrown. 
    */
   void read(UInt8* dest, unsigned long len, unsigned long offset = 0) const
         throw (utils::OutOfBoundsException);
   
   /**
    * Write bytes. Write len bytes from given offset and store them in dest. 
    * If requested byte range is out of buffer boundaries, a 
    * OutOfBoundsException is thrown. 
    */
   void write(const UInt8* src, unsigned long len, unsigned long offset = 0)
         throw (utils::OutOfBoundsException);
   
private:

   unsigned long  _length;
   UInt8*         _data;
   
};

class BufferInputStream : public InputStream<UInt8>
{
public:

   /**
    * Create a new input stream that reads bytes from a buffer.
    */
   inline BufferInputStream(const Buffer* buff) : _buffer(buff), _index(0) {}
   
   /**
    * Obtain the number of byte elements left to read from buffer.
    */
   inline unsigned long bytesLeftToRead() const
   { return _buffer->length() - _index; }

   /**
    * Read one element from the stream and return it. If there was a problem
    * while reading, a InvalidStateException is thrown.
    */
   virtual UInt8 read() throw (InvalidStateException);

   /**
    * Read len bytes from buffer and write them in data memory region. If 
    * there was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(UInt8* data, unsigned long len) 
         throw (InvalidStateException);

private:

   const Buffer*        _buffer;
   unsigned long  _index;
   
};

class BufferOutputStream : public OutputStream<UInt8>
{
public:

   /**
    * Create a new output stream that writes bytes to a buffer.
    */
   inline BufferOutputStream(Buffer* buff) : _buffer(buff), _index(0) {}

   /**
    * Obtain the number of byte elements left to write into buffer.
    */
   inline unsigned long bytesLeftToWrite() const
   { return _buffer->length() - _index; }

   /**
    * Write one element to the stream. If there was a problem
    * while reading, a InvalidStateException is thrown.
    */
   virtual void write(const UInt8& data) throw (InvalidStateException);

   /**
    * Write len bytes stored in data memory region to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a InvalidStateException is thrown.
    */
   virtual unsigned long write(const UInt8* data, unsigned long len)
         throw (InvalidStateException);
   
private:

   Buffer*        _buffer;
   unsigned long  _index;

};

}}; /* namespace karen::utils */

#endif
