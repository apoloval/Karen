/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2012 Alvaro Polo
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

#include "KarenCore/numeric.h"

#include <cerrno>
#include <cstdlib>
#include <sstream>

namespace karen {

long
Integer::toLong(const String &str, unsigned int base)
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

}; // namespace karen
