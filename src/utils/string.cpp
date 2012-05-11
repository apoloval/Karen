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

#include "utils/iterator.h"
#include "utils/pointer.h"
#include "utils/string.h"
#include "utils/types.h"

#include <sstream>

namespace karen { namespace utils {

/**
 * String iterator. This class provides an implementation for a non-const-
 * string iterator.
 */
class StringIterator : public AbstractIterator<char>,
                       public AbstractConstIterator<char>
{
public:

   /**
    * Create a new iterator with given string base implementations for
    * current iterator and end iterator. 
    */
   inline StringIterator(StringBase::iterator itImpl,
                         StringBase::iterator endImpl)
    : _itImpl(itImpl), _endImpl(endImpl)
   {
   }

   inline virtual bool isNull() const
   { return _itImpl == _endImpl; }      
   
   inline virtual Ptr<AbstractConstIterator> toConstIterator()
   { return new StringIterator(*this); }

private:

   mutable StringBase::iterator _itImpl;
   mutable StringBase::iterator _endImpl;

   inline virtual void nextAfterNullCheck()
   {
      _itImpl++;
   }
   
   inline virtual void prevAfterNullCheck()
   {
      _itImpl--;
   }

   virtual char& getAfterNullCheck()
   {
      return *_itImpl;
   }

   virtual const char& getAfterNullCheck() const
   {
      return *_itImpl;
   }

};

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

Iterator<char>
String::begin()
{
   Ptr<AbstractIterator<char> > it = 
      new StringIterator(_base.begin(), _base.end());
   return Iterator<char>(it);
}

Iterator<char>
String::end()
{
   Ptr<AbstractIterator<char> > it = 
      new StringIterator(_base.end(), _base.end());
   return Iterator<char>(it);
}

ConstIterator<char>
String::begin() const
{
   Ptr<AbstractConstIterator<char> > it = 
      new StringIterator(_base.begin(), _base.end());
   return ConstIterator<char>(it);
}

ConstIterator<char>
String::end() const
{
   Ptr<AbstractConstIterator<char> > it = 
      new StringIterator(_base.end(), _base.end());
   return ConstIterator<char>(it);
}


}}; // namespace karen::utils
