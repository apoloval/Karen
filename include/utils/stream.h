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

#ifndef KAREN_UTILS_STREAM_H
#define KAREN_UTILS_STREAM_H

#include "utils/exception.h"

namespace karen { namespace utils {

/**
 * Input stream class. This class provides an abstraction for an input stream,
 * this is, an object which allows reading bytes from it.
 */
class InputStream
{
public:

   /**
    * Read one element of template class T from the stream. If the element
    * cannot be read, a InvalidStateException is thrown.
    */
   template <class T>
   T read() throw (InvalidStateException)
   {
      UInt8 dat[sizeof(T)];
      UInt8* ptr = dat;
      unsigned long left = sizeof(T), nread;
      
      while (left && (nread = readBytes(ptr, left)))
      {
         ptr += nread;
         left -= nread;
      }
      if (left)
         KAREN_THROW(InvalidStateException, 
            "cannot read element from input stream: no more bytes left");
      return *((T*) dat);
   }

   /**
    * Read len bytes from stream and store them in dst, returning the number
    * of bytes read or zero if there is no more data. If stream source
    * cannot be read, a InvalidStateException is thrown.
    */
   virtual unsigned long readBytes(void* dst, unsigned long len) 
         throw (InvalidStateException) = 0;

};

/**
 * Output stream class. This class provides an abstraction for an output stream,
 * this is, an object which allows writting bytes to it.
 */
class OutputStream
{
public:

   /**
    * Write one element of template class T to the stream. If there was a 
    * problem while writing, a InvalidStateException is thrown.
    */
   template <class T>
   void write(const T& data) throw (InvalidStateException)
   {
      UInt8* ptr = (UInt8*) &data;
      unsigned long left = sizeof(T), nwrite;
      while (left && (nwrite = writeBytes(ptr, left)))
      {
         ptr += nwrite;
         left -= nwrite;
      }
      if (left)
         KAREN_THROW(InvalidStateException,
            "cannot write element into stream: no more space left in stream");
   }

   /**
    * Write len bytes stored in data to this stream and return the number of
    * bytes actually written. If there was a problem while writing, a 
    * InvalidStateException is thrown.
    */
   virtual unsigned long writeBytes(const void* data, unsigned long len) 
      throw (InvalidStateException) = 0;

};

}}; /* namespace karen::utils */

#endif
