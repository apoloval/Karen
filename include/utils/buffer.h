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
 * Abstract data buffer class. This class provides an abstraction for
 * a data buffer, i.e. a region of memory to store temporary data. 
 */
class AbstractDataBuffer : public InputStream, public OutputStream
{
public:

   /**
    * Obtain current buffer length.
    */
   virtual unsigned long length() const = 0;
   
   /**
    * Obtain the amount of bytes readed.
    */
   virtual unsigned long bytesRead() const = 0;

   /**
    * Obtain the amount of bytes written.
    */
   virtual unsigned long bytesWritten() const = 0;
   
   /**
    * Obtain the amount of bytes left to read.
    */
   virtual unsigned long bytesLeftToRead() const = 0;
   
   /**
    * Reset the buffer for reading.
    */
   virtual void resetForReading() = 0;
   
   /**
    * Reset the buffer for writing.
    */
   virtual void resetForWritting() = 0;
   
   /**
    * Reset the buffer for both reading and writing.
    */
   inline virtual void reset()
   { resetForWritting(); resetForReading(); }
   
   /**
    * Read len bytes from buffer and write them in data memory region. If 
    * there was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(void* data, unsigned long len) 
         throw (InvalidStateException) = 0;
   
   /**
    * Read len bytes from buffer and write them in data output stream. If 
    * there was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(OutputStream& data, unsigned long len)
         throw (InvalidStateException) = 0;
   
   /**
    * Write len bytes stored in data memory region to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a InvalidStateException is thrown.
    */
   virtual unsigned long write(const void* data, unsigned long len)
         throw (InvalidStateException) = 0;
   
   /**
    * Write len bytes stored in data input stream to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a InvalidStateException is thrown.
    */
   virtual unsigned long write(InputStream& data, unsigned long len)
         throw (InvalidStateException) = 0;

};

/**
 * Data buffer class. This abstract class is used to read and write bytes or 
 * raw data as an intermediate storage. 
 */
class DataBuffer : public AbstractDataBuffer
{
public:

   /**
    * Default buffer length.
    */
   static const unsigned long DEFAULT_LENGTH;
   
   /**
    * Create a new buffer with given properties. 
    */
   DataBuffer(unsigned long length = DEFAULT_LENGTH, bool autosize = false);
   
   /**
    * Create a new buffer from given initial data.
    */
   DataBuffer(void* data, unsigned long length, bool autosize = false);

   /**
    * Destroy the buffer and deallocate all its memory. 
    */
   virtual ~DataBuffer();
   
   /**
    * Obtain current buffer length.
    */
   inline unsigned long length() const
   { return _length; }
   
   /**
    * Obtain the amount of bytes readed.
    */
   inline unsigned long bytesRead() const
   { return (unsigned long) (_readCursor - _data); }

   /**
    * Obtain the amount of bytes written.
    */
   inline unsigned long bytesWritten() const
   { return (unsigned long) (_writeCursor - _data); }
   
   //! Obtain the amount of bytes left to read
   inline unsigned long bytesLeftToRead() const
   { return (unsigned long) (_writeCursor - _readCursor); }
   
   //! Obtain the amount of bytes left to write
   inline unsigned long bytesLeftToWrite() const
   { return (unsigned long) ((_data + _length) - _writeCursor); }
   
   //! Obtain a pointer to internal data
   inline operator void* ()
   { return _data; }

   //! Reset the buffer for reading
   void resetForReading();
   
   //! Reset the buffer for writing
   void resetForWritting();
   
   //! Reset the buffer for both reading and writing
   void reset();
   
   /**
    * Read len bytes from buffer and write them in data memory region. If 
    * there was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(void* data, unsigned long len) 
         throw (InvalidStateException);
   
   /**
    * Read len bytes from buffer and write them in data output stream. If 
    * there was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(OutputStream& data, unsigned long len)
         throw (InvalidStateException);
   
   /**
    * Write len bytes stored in data memory region to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a InvalidStateException is thrown.
    */
   virtual unsigned long write(const void* data, unsigned long len)
         throw (InvalidStateException);
   
   /**
    * Write len bytes stored in data input stream to this buffer and return 
    * the number of bytes actually written. If there was a problem while 
    * writing, a InvalidStateException is thrown.
    */
   virtual unsigned long write(InputStream& data, unsigned long len)
         throw (InvalidStateException);
   
   /**
    * Close the buffer and release any associated system resources.
    */
   virtual void close();

private:

   unsigned long  _length;
   bool           _autosize;
   UInt8*         _data;
   UInt8*         _readCursor;
   UInt8*         _writeCursor;
   
   void enlarge(unsigned long minBytes);
   
};

}}; /* namespace karen::utils */

#endif
