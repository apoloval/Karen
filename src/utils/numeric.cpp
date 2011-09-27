/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#include "utils/numeric.h"

#include <cerrno>
#include <sstream>

namespace karen { namespace utils {

long
Integer::toLong(const String &str, unsigned long base)
throw (InvalidConversionException)
{
   char *end;
   long l = strtol(str, &end, base);
   if ((*end != '\0') || (!l && (errno == EINVAL)))
      KAREN_THROW(InvalidConversionException, 
      "cannot convert string %s to long", (const char*) str);
   return l;
}

double
Float::toDouble(const String &str)
throw (InvalidConversionException)
{
   char *end;
   double d = strtod(str, &end);
   if ((*end != '\0') || (!d && (errno == EINVAL)))
      KAREN_THROW(InvalidConversionException, 
      "cannot convert string %s to double", (const char*) str);
   return d;
}

}}; // namespace karen::utils
