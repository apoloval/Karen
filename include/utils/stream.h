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
    * Read len bytes from stream and write them in data. If there was a problem
    * while reading, a InvalidStateException is thrown.
    */
   virtual unsigned long read(void* data, unsigned long len) 
         throw (InvalidStateException) = 0;
   
   /**
    * Close the input stream and release any associated system resources.
    */
   virtual void close() = 0;

};

/**
 * Output stream class. This class provides an abstraction for an output stream,
 * this is, an object which allows writting bytes to it.
 */
class OutputStream
{
public:

   /**
    * Write len bytes stored in data to this stream and return the number of
    * bytes actually written. If there was a problem while writing, a 
    * InvalidStateException is thrown.
    */
   virtual unsigned long write(const void* data, unsigned long len) 
      throw (InvalidStateException) = 0;

   /**
    * Close the input stream and release any associated system resources.
    */
   virtual void close() = 0;

};

}}; /* namespace karen::utils */

#endif
