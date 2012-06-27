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

#include "utils/parsing.h"

namespace karen { namespace utils {

void
tokenizeString(
      const String& str, 
      Array<String>& tokens,
      String::Element separator)
{
   tokens.clear();
   if (!str.isEmpty())
   {
      const char *p = str, *b;
      int c;
      while (p && (*p != '\0'))
      {      
         // Ignore leading separators.
         while (*p == separator)
            p++;
         
         // Read characters until end of word
         b = p;
         c = 0;
         while (*p != separator && *p != '\0')
         {
            c++;
            p++;
         }

         // Ignore rest of spaces.
         while (p && (*p == separator))
            p++;

         tokens.append(String(b, c));
      }
   }
}

}}; // namespace karen::utils
