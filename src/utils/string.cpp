/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#include "utils/string.h"
#include "utils/types.h"

#include <sstream>

namespace karen { namespace utils {

#ifndef MAX_FORMAT_LENGTH
#define MAX_FORMAT_LENGTH 10240
#endif

String
String::format(const char *str, ...)
{
   Ptr<char> buf = new char[MAX_FORMAT_LENGTH];
   va_list args;
   va_start(args, str);
   vsnprintf(buf, MAX_FORMAT_LENGTH, str, args);
   va_end(args);

   String res(buf);
   return res;
}

String
String::fromDouble(double num, unsigned int decimalDigits)
{
   long ip = (long) num;
   float tmp = num - ip;
   for (int i = 0; i < decimalDigits; i++)
      tmp *= 10l;
   long fp = (long) tmp;
   
   String istr = String::fromLong(ip);
   String fstr = String::fromLong(fp);
   
   while (fstr.length() < decimalDigits)
      fstr = String("0") + fstr;
   
   return istr + "." + fstr;
}

String
String::fromLong(long num)
{
   std::stringstream s;
   s << num;
   return String(s.str());
}

String::Element&
String::operator [] (const Position &pos)
{
   Length len = length();
   if (pos < len)
      return _base.at(pos);
   else
      KAREN_THROW(OutOfBoundsException, 
         "attempt of accessing string element at position %d; "
         "string length is %d", (unsigned long) pos, len);
}

const String::Element&
String::operator [] (const Position &pos) const
{ return ((String&) *this)[pos]; }

Array<String>
StringTokenizer::tokenize(const String& str, String::Element separator)
{
   Array<String> res;
   if (!str.empty())
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

         res.pushBack(String(b, c));
      }
   }
   
   return res;
}

}}; // namespace karen::utils