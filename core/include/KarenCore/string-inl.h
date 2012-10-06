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

#ifndef KAREN_CORE_STRING_INL_H
#define KAREN_CORE_STRING_INL_H

#include "KarenCore/iterator.h"
#include "KarenCore/pointer.h"
#include "KarenCore/string.h"
#include "KarenCore/types.h"

#include <cstdarg>
#include <cstdio>
#include <sstream>

namespace karen {

/**
 * String iterator. This class provides an implementation for a non-const-
 * string iterator.
 */
template <typename CharType>
class StringIterator : public AbstractConstIterator<CharType>,
                       public AbstractNonConstIterator<CharType>
{
public:

	typedef std::basic_string<CharType> StringImpl;
	typedef typename StringImpl::iterator IteratorImpl;

   /**
    * Create a new iterator with given string base implementations for
    * current iterator and end iterator. 
    */
   inline StringIterator(IteratorImpl itImpl,
                         IteratorImpl endImpl)
    : _itImpl(itImpl), _endImpl(endImpl)
   {
   }

   inline virtual bool isNull() const
   { return _itImpl == _endImpl; }      
   
   inline virtual StringIterator* clone() const
   { return new StringIterator(_itImpl, _endImpl); }

   inline virtual Ptr<AbstractIterator<const char>> toConstIterator()
   { return new StringIterator(*this); }

private:

   mutable IteratorImpl _itImpl;
   mutable IteratorImpl _endImpl;

   inline virtual void nextAfterNullCheck()
   {
      _itImpl++;
   }
   
   inline virtual void prevAfterNullCheck()
   {
      _itImpl--;
   }

   virtual char& getNonConstAfterNullCheck()
   {
      return *_itImpl;
   }

   virtual const char& getConstAfterNullCheck()
   {
      return *_itImpl;
   }

};

#ifndef MAX_FORMAT_LENGTH
#define MAX_FORMAT_LENGTH 10240
#endif

template <typename CharType>
StringBase<CharType>::StringBase() : _base() {}

template <typename CharType>
StringBase<CharType>::StringBase(const StringBase& str) : _base(str._base) {}

template <typename CharType>
StringBase<CharType>::StringBase(const CharType* str) : _base(str) {}

template <typename CharType>
StringBase<CharType>::StringBase(const CharType* str, unsigned long strLen) 
	: _base(str, strLen) {}

template <typename CharType>
StringBase<CharType>::StringBase(const std::string& str) : _base(str) {}

template <typename CharType>
StringBase<CharType>::~StringBase() {}

template <typename CharType>
StringBase<CharType>::operator const CharType* () const
{ return _base.c_str(); }

template <typename CharType>
StringBase<CharType>::operator const std::basic_string<CharType>& () const
{ return _base; }

template <typename CharType>
typename StringBase<CharType>::Length
StringBase<CharType>::length() const { return _base.length(); }

template <typename CharType>
bool
StringBase<CharType>::isEmpty() const { return _base.empty(); }

template <typename CharType>
void
StringBase<CharType>::clear() { _base.clear(); }

template <typename CharType>
bool
StringBase<CharType>::startsWith(const StringBase& str) const
{ return head(str.length()) == str; }

template <typename CharType>
bool
StringBase<CharType>::endsWith(const StringBase& str) const
{ return tail(str.length()) == str; }

template <typename CharType>
typename StringBase<CharType>::Position
StringBase<CharType>::findChar(CharType elem, Position pos) const
{ 
   size_t p = pos.isNull() ? _base.find(elem) : _base.find(elem, pos); 
   return (p == std::string::npos) ? Position() : Position(p); 
}

template <typename CharType>
typename StringBase<CharType>::Position
StringBase<CharType>::reverseFindChar(CharType elem, Position pos) const
{ 
   size_t p = pos.isNull() ? _base.rfind(elem) : _base.rfind(elem, pos); 
   return (p == std::string::npos) ? Position() : Position(p); 
}

template <typename CharType>
StringBase<CharType>&
StringBase<CharType>::append(CharType character, Length count)
{ _base.append(count, character); return *this; }

template <typename CharType>
StringBase<CharType>&
StringBase<CharType>::append(const StringBase &str) 
{ _base.append(str._base); return *this; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::concat(CharType character, Length count) const
{ StringBase res(*this); res.append(character, count); return res; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::concat(const StringBase& str) const
{ StringBase res(*this); res.append(str); return res; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::slice(Position pos, Length len) const
{ return (pos < length()) ? StringBase(_base.substr(pos, len)) : ""; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::head(Length len) const
{ return slice(0, len); }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::tail(Length len) const
{
   Length l = length();
   return (len < l) ? slice(l - len, len) : *this;
}

template <typename CharType>
StringBase<CharType>&
StringBase<CharType>::removeSlice(Position pos, Length len)
{
   if (pos < length()) 
      _base.erase(pos, len); 
   return *this;
}

template <typename CharType>
StringBase<CharType>&
StringBase<CharType>::removeFromHead(Length len)
{ return removeSlice(0, len); }

template <typename CharType>
StringBase<CharType>& 
StringBase<CharType>::removeFromTail(Length len)
{
   Length l = length();
   return removeSlice((len < l) ? (l - len) : 0, len); 
}      

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::removeHead(const StringBase& str) const
{ return startsWith(str) ? tail(length() - str.length()) : *this; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::removeTail(const StringBase& str) const
{ return endsWith(str) ? head(length() - str.length()) : *this; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::capitalize() const
{ std::string r(_base); if (r.length() > 0) r[0] = toupper(r[0]); return r; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::toLowerCase() const
{ 
   std::string r(_base); 
   for (unsigned int i = 0; i < r.length(); i++)
      r[i] = tolower(r[i]);
   return r;
}

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::toUpperCase() const
{ 
   std::string r(_base); 
   for (unsigned int i = 0; i < r.length(); i++)
      r[i] = toupper(r[i]);
   return r;
}

template <typename CharType>
StringBase<CharType>&
StringBase<CharType>::operator = (const StringBase& str)
{ _base = str._base; return *this; }

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::operator + (const StringBase& str) const
{ return StringBase(*this).append(str); }

template <typename CharType>
bool
StringBase<CharType>::operator == (const StringBase& str) const
{ return _base == str._base; }

template <typename CharType>
bool
StringBase<CharType>::operator == (const CharType* str) const
{ return _base == str; }

template <typename CharType>
bool
StringBase<CharType>::operator != (const StringBase& str) const
{ return _base != str._base; }    

template <typename CharType>
bool
StringBase<CharType>::operator != (const CharType* str) const
{ return _base != str; }

template <typename CharType>
bool
StringBase<CharType>::operator < (const StringBase &str) const
{ return _base < str._base; }

template <typename CharType>
bool
StringBase<CharType>::operator > (const StringBase &str) const
{ return _base > str._base; }

template <typename CharType>
bool
StringBase<CharType>::operator <= (const StringBase &str) const
{ return _base <= str._base; }

template <typename CharType>
bool
StringBase<CharType>::operator >= (const StringBase &str) const
{ return _base >= str._base; }


template <typename CharType>
StringBase<CharType>
StringBase<CharType>::format(const CharType *str, ...)
{
   Ptr<char> buf = new char[MAX_FORMAT_LENGTH];
   va_list args;
   va_start(args, str);
#if KAREN_COMPILER == KAREN_COMPILER_MSVC
   vsnprintf_s(buf, MAX_FORMAT_LENGTH,
               MAX_FORMAT_LENGTH / sizeof(CharType), str, args);
#else
   vsnprintf(buf, MAX_FORMAT_LENGTH / sizeof(CharType), str, args);
#endif
   va_end(args);

   StringBase res(buf);
   return res;
}

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::fromDouble(double num, unsigned int decimalDigits)
{
   long ip = (long) num;
   double tmp = num - ip;
   for (unsigned int i = 0; i < decimalDigits; i++)
      tmp *= 10l;
   long fp = (long) tmp;
   
   StringBase istr = StringBase::fromLong(ip);
   StringBase fstr = StringBase::fromLong(fp);
   
   while (fstr.length() < decimalDigits)
      fstr = StringBase("0") + fstr;
   
   return istr + "." + fstr;
}

template <typename CharType>
StringBase<CharType>
StringBase<CharType>::fromLong(long num)
{
   std::stringstream s;
   s << num;
   return StringBase(s.str());
}

template <typename CharType>
CharType&
StringBase<CharType>::operator [] (const Position &pos)
{
   Length len = length();
   if (pos < len)
      return _base.at(pos);
   else
      KAREN_THROW(OutOfBoundsException, 
         "attempt of accessing string element at position %d; "
         "string length is %d", (unsigned long) pos, len);
}

template <typename CharType>
const CharType&
StringBase<CharType>::operator [] (const Position &pos) const
{ return ((StringBase<CharType>&) *this)[pos]; }

template <typename CharType>
Iterator<CharType>
StringBase<CharType>::begin()
{
   Ptr<AbstractIterator<CharType>> it = 
      new StringIterator<CharType>(_base.begin(), _base.end());
   return Iterator<CharType>(it);
}

template <typename CharType>
Iterator<CharType>
StringBase<CharType>::end()
{
   Ptr<AbstractIterator<CharType>> it = 
      new StringIterator<CharType>(_base.end(), _base.end());
   return Iterator<CharType>(it);
}

template <typename CharType>
Iterator<const CharType>
StringBase<CharType>::begin() const
{
   Ptr<AbstractIterator<const CharType> > it = 
      new StringIterator<CharType>(_base.begin(), _base.end());
   return Iterator<const CharType>(it);
}

template <typename CharType>
Iterator<const CharType>
StringBase<CharType>::end() const
{
   Ptr<AbstractIterator<const CharType>> it = 
      new StringIterator<CharType>(_base.end(), _base.end());
   return Iterator<const CharType>(it);
}

}; // namespace karen

#endif
