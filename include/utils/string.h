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

#ifndef KAREN_UTILS_STRING_H
#define KAREN_UTILS_STRING_H

#include "utils/types.h"

#include <string>
#include <cctype>

namespace karen { namespace utils {

template <class T>
class Iterator;

template <class T>
class ConstIterator;

/**
 * String base class. This typedef redeclares a STL-based string class
 * as the base class used in Karen to implement strings. 
 */
typedef std::string StringBase;

/**
 * String class. This class provides an abstraction for a string of
 * characters or bytes. It is based on STL string class as base, including
 * a member renaming (to fit Karen coding standards) and additional
 * members not included in STL strings. 
 */
class String
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
    * String element data type. This data type provides an abstraction for
    * the character elements of a string. 
    */
   typedef char Element;

   /**
    * Format a string following by substituting a variable-length set of
    * parameters. The substitution follows the printf() rules.
    */
   static String format(const char *str, ...);

   /**
    * Double to string converter. This static function creates a new String
    * object representing the value of given double with the given number
    * of decimal digits.
    */
   static String fromDouble(double num, unsigned int decimalDigits = 2);
   
   /**
    * Long to string converter. This static function creates a new String
    * object representing the value of given long.
    */
   static String fromLong(long num);

   /**
    * Default constructor. This creates a new zero-length string. 
    */
   inline String() : _base() {}
   
   /**
    * Value constructor. This creates a new string by copying the one
    * passed as argument.
    */
   inline String(const String& str) : _base(str._base) {}
   
   /**
    * Value constructor. This creates a new string from given null-terminated
    * char pointer. 
    */
   inline String(const char* str) : _base(str) {}
   
   /**
    * Value constructor. This creates a new string from given char array and
    * given array length. 
    */
   inline String(const char* str, unsigned long strLen) : _base(str, strLen) {}
   
   /**
    * Value constructor. This creates a new string from given STL string
    * object. 
    */
   inline String(const std::string& str) : _base(str) {}
   
   /**
    * Virtual destructor.     
    */
   inline virtual ~String() {}
   
   /**
    * Cast operator to null-terminated C-like string.
    */
   inline operator const char* () const
   { return _base.c_str(); }
   
   /**
    * Return the length of the string. 
    */
   inline Length length() const { return _base.length(); }
   
   /**
    * Indicate whether string is empty (zero-length).
    */
   inline bool empty() const { return _base.empty(); }
   
   /**
    * Clear the string, removing all characters and setting it to
    * zero-length. 
    */
   inline void clear() { _base.clear(); }
   
   /**
    * Check whether this string starts with given one passed as argument.
    */
   inline bool startsWith(const String& str) const
   { return head(str.length()) == str; }
   
   /**
    * Check whether this string ends with given one passed as argument.
    */
   inline bool endsWith(const String& str) const
   { return tail(str.length()) == str; }
   
   /**
    * Find given character element in this string. If not found, the
    * nullable element has a null value. 
    */
   inline Position findChar(Element elem, Position pos = Position()) const
   { 
      size_t p = pos.isNull() ? _base.find(elem) : _base.find(elem, pos); 
      return (p == std::string::npos) ? Position() : Position(p); 
   }
   
   /**
    * Find given character element in this string in reverse way (from tail
    * to head). If not found, the nullable element has a null value. 
    */
   inline Position reverseFindChar(
         Element elem, Position pos = Position()) const
   { 
      size_t p = pos.isNull() ? _base.rfind(elem) : _base.rfind(elem, pos); 
      return (p == std::string::npos) ? Position() : Position(p); 
   }
   
   /**
    * Append a determined count of a character to the end of the string.
    */
   inline String& append(Element character, Length count)
   { _base.append(count, character); return *this; }
   
   /**
    * Append string passed as argument to this string. 
    */
   inline String& append(const String &str) 
   { _base.append(str._base); return *this; }
   
   /**
    * Return the concatenation of this string with a determined count of
    * character passed as arguments. 
    */
   inline String concat(Element character, Length count) const
   { String res(*this); res.append(character, count); return res; }
   
   /**
    * Return the concatenation of this string and given one passed as
    * argument.
    */
   inline String concat(const String& str) const
   { String res(*this); res.append(str); return res; }
   
   /**
    * Return the string slice by given position and length.
    */
   inline String slice(Position pos, Length len) const
   { return (pos < length()) ? String(_base.substr(pos, len)) : ""; }
   
   /**
    * Return the string head by given length.
    */
   inline String head(Length len) const
   { return slice(0, len); }
   
   /**
    * Return the string tail by given length.
    */
   inline String tail(Length len) const
   {
      Length l = length();
      return (len < l) ? slice(l - len, len) : *this;
   }
   
   /**
    * Remove given slice from this string.
    */
   inline String& removeSlice(Position pos, Length len)
   {
      if (pos < length()) 
         _base.erase(pos, len); 
      return *this;
   }
   
   /**
    * Remove given count of elements from head.
    */
   inline String& removeFromHead(Length len)
   { return removeSlice(0, len); }
   
   /**
    * Remove given count of elements from tail.
    */
   inline String& removeFromTail(Length len)
   {
      Length l = length();
      return removeSlice((len < l) ? (l - len) : 0, len); 
   }      
   
   /**
    * Return the string resulting from removing the head passed as argument
    * if it matches with this string head. If it doesn't match, returns a
    * this string unaltered. 
    */
   inline String removeHead(const String& str) const
   { return startsWith(str) ? tail(length() - str.length()) : *this; }
   
   /**
    * Return the string resulting from removing the tail passed as argument
    * if it matches with this string tail. If it doesn't match, returns a
    * this string unaltered. 
    */
   inline String removeTail(const String& str) const
   { return endsWith(str) ? head(length() - str.length()) : *this; }
   
   /**
    * Capitalize the string, converting the first symbol to upper case if
    * possible.
    */
   inline String capitalize() const
   { std::string r(_base); if (r.length() > 0) r[0] = toupper(r[0]); return r; }
   
   /**
    * Return a lower case copy of this string. 
    */
   inline String toLowerCase() const
   { 
      std::string r(_base); 
      for (int i = 0; i < r.length(); i++) 
         r[i] = tolower(r[i]);
      return r;
   }
   
   /**
    * Return a upper case copy of this string. 
    */
   inline String toUpperCase() const
   { 
      std::string r(_base); 
      for (int i = 0; i < r.length(); i++) 
         r[i] = toupper(r[i]);
      return r;
   }
   
   /**
    * Assign operator.
    */
   inline String& operator = (const String& str)
   { _base = str._base; return *this; }
   
   /**
    * Add operator. This add operator return a string resulting of
    * concatenating this string with given one passed as argument. 
    */
   inline String operator + (const String& str) const
   { return String(*this).append(str); }
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const String& str) const
   { return _base == str._base; }
   
   /**
    * Equals to operator.
    */
   bool operator == (const char* str) const
   { return _base == str; }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const String& str) const
   { return _base != str._base; }    

   /**
    * Not equals to operator.
    */
   inline bool operator != (const char* str) const
   { return _base != str; }
   
   /**
    * Less than operator.
    */
   inline bool operator < (const String &str) const
   { return _base < str._base; }
   
   /**
    * Greater than operator.
    */
   inline bool operator > (const String &str) const
   { return _base > str._base; }
   
   /**
    * Less than or equal operator.
    */
   inline bool operator <= (const String &str) const
   { return _base <= str._base; }
   
   /**
    * Greater than or equal operator.
    */
   inline bool operator >= (const String &str) const
   { return _base >= str._base; }
   
   /**
    * Index operator. If given position is not valid for this string, a
    * OutOfBoundsException is raised. 
    */
   Element& operator [] (const Position &pos);

   /**
    * Index operator. If given position is not valid for this string, a
    * OutOfBoundsException is raised. 
    */
   const Element& operator [] (const Position &pos) const;

   /**
    * Obtain an iterator pointing to the first element of the string.
    */
   Iterator<char> begin();

   /**
    * Obtain an iterator pointing to (beyond) the end of the string.
    */
   Iterator<char> end();

   /**
    * Obtain a const-iterator pointing to the first element of the string.
    */
   ConstIterator<char> begin() const;

   /**
    * Obtain a const-iterator pointing to (beyond) the end of the string.
    */
   ConstIterator<char> end() const;

private:

   StringBase _base;

};

}}; // namespace karen::utils

using ::karen::utils::String;

#endif
