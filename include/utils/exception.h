/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#ifndef KAREN_UTILS_EXCEPTION_H
#define KAREN_UTILS_EXCEPTION_H

#include <exception>

#include "platform.h"
#include "string.h"

namespace karen { namespace utils {

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
   { return _cause; }
   
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
   inline Exception(const String& cause, 
                    const String& sourceFile,
                    long sourceLine,
                    Exception* nestedException)
    : _cause(cause), 
      _sourceFile(sourceFile),
      _sourceLine(sourceLine),
      _nestedException(nestedException)
   {
   }
   
   /**
    * Create a new exception from given cause, source filename and source
    * line.
    */
   inline Exception(const String& cause,
                    const String& sourceFile,
                    long sourceLine) 
    : Exception(cause, sourceFile, sourceLine, NULL) {}

   /**
    * Create a new exception from given cause and nested exception
    */
   inline Exception(const String& cause,
                    Exception* nestedException) 
    : Exception(cause, "unknown file", -1, nestedException) {}
   
   /**
    * Virtual destructor. 
    */
   inline virtual ~Exception() throw () {}
   
private:

   String         _cause;
   String         _sourceFile;
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
   String::format(msg, ## __VA_ARGS__), __FILE__, __LINE__, nested);

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

}}; // namespace karen::utils

#endif
