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
template <class T>
class InputStream
{
public:

   /**
    * Read one element from the stream and return it. If there was a problem
    * while reading, a InvalidStateException is thrown.
    */
   virtual T read() throw (InvalidStateException) = 0;

   /**
    * Read len elements from stream and write them in data array. If there 
    * was a problem while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(T* data, unsigned long len) 
         throw (InvalidStateException) = 0;
   
};

/**
 * Output stream class. This class provides an abstraction for an output stream,
 * this is, an object which allows writting bytes to it.
 */
template <class T>
class OutputStream
{
public:

   /**
    * Write one element to the stream. If there was a problem
    * while reading, a InvalidStateException is thrown.
    */
   virtual void write(const T& data) throw (InvalidStateException) = 0;

   /**
    * Write len elements stored in data to this stream and return the number of
    * bytes actually written. If there was a problem while writing, a 
    * InvalidStateException is thrown.
    */
   virtual unsigned long write(const T* data, unsigned long len) 
      throw (InvalidStateException) = 0;

};

}}; /* namespace karen::utils */

#endif
