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

#ifndef KAREN_CORE_STRING_H
#define KAREN_CORE_STRING_H

#include "KarenCore/platform.h"
#include "KarenCore/types.h"

#include <string>
#include <cctype>

namespace karen {

template <class T>
class Iterator;

template <class T>
class ConstIterator;

/**
 * String class. This class provides an abstraction for a string of
 * characters or bytes. It is based on STL string class as base, including
 * a member renaming (to fit Karen coding standards) and additional
 * members not included in STL strings. 
 */
template <typename CharType>
class StringBase
{
public:

   /**
    * Position data type. This data type provides an abstraction for the
    * position of a string. 
    */
   typedef Nullable<unsigned long> Position;

   /**
    * Length data type. This data type provides an abstraction for the
    * length of a string. 
    */
   typedef unsigned long Length;
   
   /**
    * Format a string following by substituting a variable-length set of
    * parameters. The substitution follows the printf() rules.
    */
   static StringBase format(const CharType *str, ...);

   /**
    * Double to string converter. This static function creates a new String
    * object representing the value of given double with the given number
    * of decimal digits.
    */
   static StringBase fromDouble(double num, unsigned int decimalDigits = 2);
   
   /**
    * Long to string converter. This static function creates a new String
    * object representing the value of given long.
    */
   static StringBase fromLong(long num);

   /**
    * Default constructor. This creates a new zero-length string. 
    */
   StringBase();
   
   /**
    * Value constructor. This creates a new string by copying the one
    * passed as argument.
    */
   StringBase(const StringBase& str);
   
   /**
    * Value constructor. This creates a new string from given null-terminated
    * char pointer. 
    */
   StringBase(const CharType* str);
   
   /**
    * Value constructor. This creates a new string from given char array and
    * given array length. 
    */
   StringBase(const CharType* str, unsigned long strLen);
   
   /**
    * Value constructor. This creates a new string from given STL string
    * object. 
    */
   StringBase(const std::string& str);
   
   /**
    * Virtual destructor.     
    */
   virtual ~StringBase();
   
   /**
    * Cast operator to null-terminated C-like string.
    */
   operator const CharType* () const;

   /**
    * Cast operator to STL string.
    */
   operator const std::basic_string<CharType>& () const;
   
   /**
    * Return the length of the string. 
    */
   Length length() const;
   
   /**
    * Indicate whether string is empty (zero-length).
    */
   bool isEmpty() const;
   
   /**
    * Clear the string, removing all characters and setting it to
    * zero-length. 
    */
   void clear();
   
   /**
    * Check whether this string starts with given one passed as argument.
    */
   bool startsWith(const StringBase& str) const;
   
   /**
    * Check whether this string ends with given one passed as argument.
    */
   bool endsWith(const StringBase& str) const;
   
   /**
    * Find given character CharType in this string. If not found, the
    * nullable CharType has a null value. 
    */
   Position findChar(CharType elem, Position pos = Position()) const;
   
   /**
    * Find given character CharType in this string in reverse way (from tail
    * to head). If not found, the nullable CharType has a null value. 
    */
   Position reverseFindChar(CharType elem, Position pos = Position()) const;
   
   /**
    * Append a determined count of a character to the end of the string.
    */
   StringBase& append(CharType character, Length count);
   
   /**
    * Append string passed as argument to this string. 
    */
   StringBase& append(const StringBase &str);
   
   /**
    * Return the concatenation of this string with a determined count of
    * character passed as arguments. 
    */
   StringBase concat(CharType character, Length count) const;
   
   /**
    * Return the concatenation of this string and given one passed as
    * argument.
    */
   StringBase concat(const StringBase& str) const;
   
   /**
    * Return the string slice by given position and length.
    */
   StringBase slice(Position pos, Length len) const;
   
   /**
    * Return the string head by given length.
    */
   StringBase head(Length len) const;
   
   /**
    * Return the string tail by given length.
    */
   StringBase tail(Length len) const;
   
   /**
    * Remove given slice from this string.
    */
   StringBase& removeSlice(Position pos, Length len);
   
   /**
    * Remove given count of CharTypes from head.
    */
   StringBase& removeFromHead(Length len);
   
   /**
    * Remove given count of CharTypes from tail.
    */
   StringBase& removeFromTail(Length len);
   
   /**
    * Return the string resulting from removing the head passed as argument
    * if it matches with this string head. If it doesn't match, returns a
    * this string unaltered. 
    */
   StringBase removeHead(const StringBase& str) const;
   
   /**
    * Return the string resulting from removing the tail passed as argument
    * if it matches with this string tail. If it doesn't match, returns a
    * this string unaltered. 
    */
   StringBase removeTail(const StringBase& str) const;
   
   /**
    * Capitalize the string, converting the first symbol to upper case if
    * possible.
    */
   StringBase capitalize() const;
   
   /**
    * Return a lower case copy of this string. 
    */
   StringBase toLowerCase() const;
   
   /**
    * Return a upper case copy of this string. 
    */
   StringBase toUpperCase() const;
   
   /**
    * Assign operator.
    */
   StringBase& operator = (const StringBase& str);
   
   /**
    * Add operator. This add operator return a string resulting of
    * concatenating this string with given one passed as argument. 
    */
   StringBase operator + (const StringBase& str) const;
   
   /**
    * Equals to operator.
    */
   bool operator == (const StringBase& str) const;
   
   /**
    * Equals to operator.
    */
   bool operator == (const CharType* str) const;
   
   /**
    * Not equals to operator.
    */
   bool operator != (const StringBase& str) const;

   /**
    * Not equals to operator.
    */
   bool operator != (const CharType* str) const;
   
   /**
    * Less than operator.
    */
   bool operator < (const StringBase &str) const;
   
   /**
    * Greater than operator.
    */
   bool operator > (const StringBase &str) const;
   
   /**
    * Less than or equal operator.
    */
   bool operator <= (const StringBase &str) const;
   
   /**
    * Greater than or equal operator.
    */
   bool operator >= (const StringBase &str) const;
   
   /**
    * Index operator. If given position is not valid for this string, a
    * OutOfBoundsException is raised. 
    */
   CharType& operator [] (const Position &pos);

   /**
    * Index operator. If given position is not valid for this string, a
    * OutOfBoundsException is raised. 
    */
   const CharType& operator [] (const Position &pos) const;

   /**
    * Obtain an iterator pointing to the first CharType of the string.
    */
   Iterator<CharType> begin();

   /**
    * Obtain an iterator pointing to (beyond) the end of the string.
    */
   Iterator<CharType> end();

   /**
    * Obtain a const-iterator pointing to the first CharType of the string.
    */
   Iterator<const CharType> begin() const;

   /**
    * Obtain a const-iterator pointing to (beyond) the end of the string.
    */
   Iterator<const CharType> end() const;

private:

   mutable std::basic_string<CharType> _base;

};

template class KAREN_EXPORT std::allocator<char>;
template class KAREN_EXPORT std::basic_string<char>;
template class KAREN_EXPORT StringBase<char>;
typedef StringBase<char> String;

}; // namespace karen

using ::karen::String;

#include "KarenCore/string-inl.h"

#endif
