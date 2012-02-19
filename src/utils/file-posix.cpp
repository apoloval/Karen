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

#include "utils/file-posix.h"

#include "utils/platform.h"

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX || \
    KAREN_PLATFORM == KAREN_PLATFORM_LINUX
    
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>    

namespace karen { namespace utils {

PosixFile::PosixFile(const String& location, 
                     const FileOpenMode& mode) 
throw (IOException)
{
   int flags = 0;

   if (mode.forReading && !mode.forWriting)  flags |= O_RDONLY;
   if (!mode.forReading && mode.forWriting)  flags |= O_WRONLY;
   if (mode.forReading && mode.forWriting)   flags |= O_RDWR;
   if (mode.createIfNotExists)               flags |= O_CREAT;
   if (mode.append)                          flags |= O_APPEND;
   if (mode.truncate)                        flags |= O_TRUNC;

   _fd = ::open(location, flags, S_IRUSR | S_IWUSR);
   if (_fd < 0)
      KAREN_THROW(IOException, "cannot open file %s; %s",
            (const char *) location, strerror(errno));
}

PosixFile::~PosixFile()
{
   if (_fd)
      ::close(_fd);
}

unsigned long
PosixFile::readBytes(void* dest, unsigned long nbytes) 
throw (IOException)
{
   ssize_t nread = ::read(_fd, dest, nbytes);
   if (nread < 0)
   {
      KAREN_THROW(IOException, 
         "cannot read bytes from file: %s", strerror(errno));
   }
   else
      return (unsigned long) nread;
}

unsigned long
PosixFile::writeBytes(const void* src, unsigned long nbytes) 
throw (IOException)
{
   ssize_t nwrite = ::write(_fd, src, nbytes);
   if (nwrite < 0)
   {
      KAREN_THROW(IOException, "cannot write to file: %s", strerror(errno));
   }
   else
      return (unsigned long) nwrite;
}

Ptr<AbstractFile> 
PosixFileFactory::createFile(
         const String& location, 
         const FileOpenMode& mode) 
throw (IOException)
{
   return new PosixFile(location, mode);
}

}}; // namespace karen::utils

#endif
