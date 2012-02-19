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
    * Create a new buffer from given initial data. The ownership of data is
    * passed to the buffer object after its creation. This memory area should
    * not be allocated in stack, and its deallocation is managed by the buffer.
    */
   Buffer(void* data, unsigned long length);
   
   /**
    * Create a new buffer as a copy of the one passed as argument.
    */
   Buffer(const Buffer& buf);
   
   /**
    * Move the contents of buffer passed as argument to the new instance of
    * buffer.
    */
   Buffer(Buffer&& buf);

   /**
    * Destroy the buffer and deallocate all its memory. 
    */
   virtual ~Buffer();

   /**
    * Copy assigment operator.
    */
   Buffer& operator = (const Buffer& buf);
   
   /**
    * Move assignment operator.
    */
   Buffer& operator = (Buffer&& buf);
   
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
    * Get a T-type object from buffer. Obtain the object of type T
    * located at buffer position determined by offset. This method performs a
    * unchecked cast to the object located at offset. If offset is outside 
    * buffer boundaries, a OutOfBoundsException is thrown.     
    */
   template <class T> 
   const T& get(unsigned long offset) const throw (utils::OutOfBoundsException)
   {
      if (!isValidRange(offset, sizeof(T)))
         KAREN_THROW(utils::OutOfBoundsException,
            "cannot get data from buffer: invalid range %d+%d",
            offset, sizeof(T));
      return *(const T*) &_data[offset];
   }
   
   /**
    * Obtain current buffer length.
    */
   inline unsigned long length() const
   { return _length; }
   
   /**
    * Check whether this buffer is dirty. A buffer is marked as dirty on each
    * write or set operation. This method may be used in combination with
    * markAsCleaned() as a basic mechanism to track updates on the buffer. 
    */
   inline bool isDirty() const
   { return _dirty; }
   
   /**
    * Check whether the range defined by given offset and length is valid.
    */
   inline bool isValidRange(unsigned long offset, unsigned long len) const
   { return offset <= _length && offset + len <= _length; }
   
   /**
    * Mark this buffer as cleaned. A buffer is marked as dirty on each
    * write or set operation. This method may be used in combination with
    * isDirty() as a basic mechanism to track updates on the buffer. 
    */
   inline void markAsClean()
   { _dirty = false; }
   
   /**
    * Read bytes. Read len bytes from given offset and store them in dest. 
    * If requested byte range is out of buffer boundaries, a 
    * OutOfBoundsException is thrown. 
    */
   void read(UInt8* dest, unsigned long len, unsigned long offset = 0) const
         throw (utils::OutOfBoundsException);
   
   /**
    * Set a T-type object into buffer. Set the value of object of type T
    * located at buffer position determined by offset. This method performs a
    * unchecked cast to the object located at offset. If offset is outside 
    * buffer boundaries, a OutOfBoundsException is thrown.
    */
   template <class T>
   void set(const T& obj, unsigned long offset)
   throw (utils::OutOfBoundsException)
   {
      if (!isValidRange(offset, sizeof(T)))
         KAREN_THROW(utils::OutOfBoundsException,
            "cannot set data into buffer: invalid range %d+%d",
            offset, sizeof(T));
      T* dst = (T*) &_data[offset];
      *dst = obj;      
      _dirty = true;
   }
   
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
   bool           _dirty;
   
};

class BufferInputStream : public InputStream
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
    * Read len bytes from buffer and write them in data memory region. If 
    * there was a problem while reading, a IOException is thrown.
    */
   virtual unsigned long readBytes(void* data, unsigned long len) 
         throw (IOException);

private:

   const Buffer*        _buffer;
   unsigned long  _index;
   
};

class BufferOutputStream : public OutputStream
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
    * Write len bytes stored in data memory region to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a IOException is thrown.
    */
   virtual unsigned long writeBytes(const void* data, unsigned long len)
         throw (IOException);
   
private:

   Buffer*        _buffer;
   unsigned long  _index;

};

}}; /* namespace karen::utils */

#endif
