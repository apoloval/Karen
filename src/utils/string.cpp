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



}}; // namespace karen::utils