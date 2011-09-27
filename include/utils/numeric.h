/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
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
   static long toLong(const String &str, unsigned long base = 10) 
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
