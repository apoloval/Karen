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

#ifndef KAREN_CORE_FILE_H
#define KAREN_CORE_FILE_H

#include "KarenCore/exception.h"
#include "KarenCore/pointer.h"
#include "KarenCore/stream.h"
#include "KarenCore/string.h"

namespace karen {

/**
 * File open mode. This struct indicates in which mode a file is opened. 
 */
struct KAREN_EXPORT FileOpenMode
{
   bool forReading;           //!< The file is opened for reading. 
   bool forWriting;           //!< The file is opened for writing.
   bool createIfNotExists;    //!< The file is created if it doesn't exist
   bool append;               //!< Append to the end of the file
   bool truncate;             //!< Truncate the file
   
   /**
    * Read and write mode. Opened for reading and writing, but not create it
    * if it doesn't exist.
    */
   static FileOpenMode READ_WRITE_MODE;
   
   /**
    * Read only mode. Opened for reading, but not for writing or creating it
    * if doesn't exist.
    */
   static FileOpenMode READ_ONLY_MODE;
   
   /**
    * Write only mode. Opened for writing, but not for reading or creating it
    * if it doesn't exist. No truncate or append. 
    */
   static FileOpenMode WRITE_ONLY_MODE;
   
   /**
    * Create and write mode. Create it if doesn't exist and open it for
    * writing but not for reading. No truncate or append. 
    */
   static FileOpenMode CREATE_AND_WRITE_MODE;

   /**
    * Append and write mode. Open it for writing and append data at the end 
    * of the file, but not for reading.
    */
   static FileOpenMode APPEND_AND_WRITE_MODE;

   /**
    * Truncate and write mode. Open it for writing and truncate its size to 
    * zero, but not for reading.
    */
   static FileOpenMode TRUNCATE_AND_WRITE_MODE;
   
};

/**
 * Abstract file class. This class provides the interface for an abstract
 * file. This interface provides only read and write methods, but not
 * open or close. It follows RAII principles so the file is opened in 
 * AbstractFile object creation and it is closed when it is destroyed.
 * AbstractFile inherits from InputStream and OutputStream, allowing its 
 * instances to behave as streams for reading and writing. 
 */
class KAREN_EXPORT AbstractFile : public InputStream, public OutputStream
{
public:

   /**
    * Virtual destructor. 
    */
   inline virtual ~AbstractFile() {}

   /**
    * Read nbytes from this file and store them in dest. It returns
    * the number of read bytes, or zero if there are no further data 
    * to be read. If file cannot be read or it was opened in a mode
    * with forReading disabled, a IOException is thrown.
    */
   virtual unsigned long readBytes(void* dest, unsigned long nbytes) 
         throw (IOException) = 0;

   /**
    * Write nbytes from src to this file. It returns the number of bytes 
    * written. If file cannot be write or it was opened in a mode
    * with forWriting disabled, a IOException is thrown.
    */
   virtual unsigned long writeBytes(const void* src, unsigned long nbytes) 
         throw (IOException) = 0;

};

template class KAREN_EXPORT Ptr<AbstractFile>;

/**
 * File class. This class provides a concrete representation of a file
 * not bounded to the concrete backend that supports the filesystem access.
 * File class is designed in a way that it wraps an actual implementation
 * of AbstractFile that maintains the real business logic for the file. The
 * actual implementation is obtained from the active abstract file factory 
 * represtened by AbstractFileFactory class. 
 */
class KAREN_EXPORT File : public AbstractFile
{
public:

   /**
    * Open the file placed at given location. If there is no file in given
    * location or it cannot be opened,  a IOException is thrown. If no file 
    * factory has been activated, a InvalidStateException is thrown.
    */
   File(const String& location, const FileOpenMode& mode) 
      throw (IOException, InvalidStateException);

   inline virtual unsigned long readBytes(
         void* dest, 
         unsigned long nbytes) throw (IOException)
   { return _impl->readBytes(dest, nbytes); }
   
   inline virtual unsigned long writeBytes(
         const void* src, 
         unsigned long nbytes) throw (IOException)
   { return _impl->writeBytes(src, nbytes); }

private:

   Ptr<AbstractFile> _impl;

};

class FileFactory;

template class KAREN_EXPORT Ptr<FileFactory>;

/**
 * File factory class. This abstract class provides the interface
 * for a factory able to create instances of AbstractFile class. The purpose
 * of this factory is not being used directly by client code, but to configure
 * a concrete implementation for FileFactory that will be used by File class
 * to instantiate a concrete implementation. 
 */
class KAREN_EXPORT FileFactory
{
public:

   /**
    * Obtain the active file factory. A null pointer is returned if no
    * file factory has been actived yet. 
    */
   inline static const Ptr<FileFactory>& getActiveFileFactory()
   { return _activeFactory; }

   /**
    * Set the active file factory to be used by File class to instantiate
    * a concrete implementation of AbstractFile.
    */
   inline static void setActiveFileFactory(const Ptr<FileFactory>& factory)
   { _activeFactory = factory; }
   
   /**
    * Virtual destructor.
    */
   inline virtual ~FileFactory() {}
   
   /**
    * Create a concrete implementation of AbstractFile that may be used by
    * File class as delegate.
    */
   virtual Ptr<AbstractFile> createFile(
         const String& location, 
         const FileOpenMode& mode) throw (IOException) = 0;

private:

   static Ptr<FileFactory> _activeFactory;

};

}; // namespace karen

#endif
