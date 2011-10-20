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

#ifndef KAREN_UTILS_NUMERIC_H
#define KAREN_UTILS_NUMERIC_H

#include "string.h"
#include "exception.h"

namespace karen { namespace utils {

class Integer
{
public:

   Integer() = delete;
   
   /**
    * Convert a string object into a long value. The string object is
    * interpreted in the base specified as argument. If string value cannot
    * be converted into long, a InvalidConversionException is raised. 
    */
   static long toLong(const String &str, unsigned int base = 10) 
         throw (InvalidConversionException);

};

class Float
{
public:

   Float() = delete;

   /**
    * Convert a string object into a long value. The string object is
    * interpreted in the base specified as argument. If string value cannot
    * be converted into long, a InvalidConversionException is raised. 
    */
   static double toDouble(const String &str) throw (InvalidConversionException);

};

}}; // namespace karen::utils

#endif
