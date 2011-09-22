/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#ifndef KAREN_UTILS_STRING_H
#define KAREN_UTILS_STRING_H

#include <string>

namespace karen { namespace utils {

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
    * Length data type. This data type provides an abstraction for the
    * length of a string. 
    */
   typedef unsigned long Length;

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
   inline String(const String &str) : _base(str._base) {}
   
   /**
    * Value constructor. This creates a new string from given null-terminated
    * char pointer. 
    */
   inline String(const char *str) : _base(str) {}
   
   /**
    * Value constructor. This creates a new string from given STL string
    * object. 
    */
   inline String(const std::string &str) : _base(str) {}
   
   /**
    * Virtual destructor.     
    */
   inline virtual ~String() {}
   
   /**
    * Return the length of the string. 
    */
   inline Length length() const { return _base.length(); }
   
   /**
    * Clear the string, removing all characters and setting it to
    * zero-length. 
    */
   inline void clear() { _base.clear(); }
   
   /**
    * Append string passed as argument to this string. 
    */
   inline String& append(const String &str) 
   { _base.append(str._base); return *this; }
   
   /**
    * Add operator. This add operator return a string resulting of
    * concatenating this string with given one passed as argument. 
    */
   inline String operator + (const String& str) const
   { return String(*this).append(str); }
    

private:

   std::string _base;

};

}}; // namespace karen::utils

using ::karen::utils::String;

#endif
