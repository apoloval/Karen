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

#ifndef KAREN_UTILS_FILE_POSIX_H
#define KAREN_UTILS_FILE_POSIX_H

#include "utils/platform.h"

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX || \
    KAREN_PLATFORM == KAREN_PLATFORM_LINUX

#include "utils/file.h"

namespace karen { namespace utils {

/**
 * Posix file class. This class provides a POSIX-based implementation of File.
 * It should never been used directy. Use wrapper class File instead. 
 */
class PosixFile : public AbstractFile
{
public:

   /**
    * Create a new POSIX file instance from given location and file open mode.
    * If the file cannot be open, a IOException is thrown.
    */
   PosixFile(const String& location, 
             const FileOpenMode& mode) throw (IOException);
   
   ~PosixFile();

   virtual unsigned long readBytes(void* dest, unsigned long nbytes) 
         throw (IOException);

   virtual unsigned long writeBytes(const void* src, unsigned long nbytes) 
         throw (IOException);   

private:

   int _fd;

};

class PosixFileFactory : public FileFactory
{
public:

   virtual Ptr<AbstractFile> createFile(
         const String& location, 
         const FileOpenMode& mode) throw (IOException);

};

}}; // namespace karen::utils

#endif
#endif
