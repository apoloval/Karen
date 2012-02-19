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

#ifndef KAREN_UTILS_EXCEPTION_H
#define KAREN_UTILS_EXCEPTION_H

#include <exception>

#include "utils/platform.h"

namespace karen { namespace utils {

class String;

/**
 * Abstract exception class. This class provides an abstraction for a
 * exception in Karen. Any exception class extends from this, indicating
 * more specific exception conditions. 
 */
class KAREN_EXPORT Exception : public std::exception
{
public:

   /**
    * Obtain exception cause.
    */
   inline const String& cause() const
   { return *_cause; }
   
   /**
    * Obtain nested exception. If there is no nested exception, it returns
    * null.
    */
   inline const Exception* nestedException() const
   { return _nestedException; }

protected:

   /**
    * Create a new exception from given cause, source filename, source 
    * line and nested exception.
    */
   Exception(const String& cause, 
             const String& sourceFile,
             long sourceLine,
             Exception* nestedException);
   
   /**
    * Create a new exception from given cause, source filename and source
    * line.
    */
   Exception(const String& cause,
                    const String& sourceFile,
                    long sourceLine);

   /**
    * Create a new exception from given cause and nested exception
    */
   Exception(const String& cause,
             Exception* nestedException);
   
   /**
    * Virtual destructor. 
    */
   virtual ~Exception() throw ();
   
private:

   String*        _cause;
   String*        _sourceFile;
   long           _sourceLine;
   Exception*     _nestedException;

};

#define KAREN_DECL_EXCEPTION(classname) \
   class KAREN_EXPORT classname : public karen::utils::Exception { \
   public:\
      classname(const String& cause, const String& file, long line) \
        : karen::utils::Exception(cause, file, line) {}\
      \
      classname(const String& cause, const String& file, \
                long line, karen::utils::Exception* nestedException) \
        : karen::utils::Exception(cause, file, line, nestedException) {}\
      \
   }

#define KAREN_DECL_CHILD_EXCEPTION(classname, parent) \
   class KAREN_EXPORT classname : public parent { \
   public:\
      classname(const String &cause, const String &file, int line) \
        : parent(cause, file, line) {}\
      \
      classname(const String& cause, const String& file, \
                long line, Exception* nestedException) \
        : Exception(cause, file, line, nestedException) {}\
      \
   }

#define KAREN_THROW(classname, msg, ...) throw classname(\
   String::format(msg, ## __VA_ARGS__), __FILE__, __LINE__);

#define KAREN_THROW_NESTED(classname, nested, msg, ...) throw classname(\
   String::format(msg, ## __VA_ARGS__), __FILE__, __LINE__, &nested);

/**
 * Internal error exception. This exception is raised when an unexpected
 * internal error is produced.
 */   
KAREN_DECL_EXCEPTION(InternalErrorException);

/**
 * Index out of bounds exception. This exception is raised when an indexable
 * resource is indexed out of its bounds. 
 */
KAREN_DECL_EXCEPTION(OutOfBoundsException);

/**
 * Null pointer exception. This exception is raised when a null pointer
 * is deferrenced. 
 */
KAREN_DECL_EXCEPTION(NullPointerException);

/**
 * Invalid input exception. This exception is raised when a invalid input
 * is received by a subprogram. 
 */
KAREN_DECL_EXCEPTION(InvalidInputException);

/**
 * Invalid conversion exception. This exception is raised when one object
 * cannot be converted from one class to another. 
 */
KAREN_DECL_EXCEPTION(InvalidConversionException);

/**
 * Invalid state exception. This exception is raised when one object
 * have an state that prevents the execution of certain action. 
 */
KAREN_DECL_EXCEPTION(InvalidStateException);

/**
 * Not found exception. This exception is raised when some action
 * is requested that implies a resource that cannot be found.
 */
KAREN_DECL_EXCEPTION(NotFoundException);

/**
 * I/O exception. This exception is raised when an I/O error takes place.
 */
KAREN_DECL_EXCEPTION(IOException);

}}; // namespace karen::utils

#include "utils/string.h"

#endif
