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

#include "KarenCore/file.h"

namespace karen {

FileOpenMode FileOpenMode::READ_WRITE_MODE = 
      { true, true, false, false, false };
FileOpenMode FileOpenMode::READ_ONLY_MODE = 
      { true, false, false, false, false };
FileOpenMode FileOpenMode::WRITE_ONLY_MODE = 
      { false, true, false, false, false };
FileOpenMode FileOpenMode::CREATE_AND_WRITE_MODE = 
      { false, true, true, false, false };
FileOpenMode FileOpenMode::APPEND_AND_WRITE_MODE = 
      { false, true, true, true, false };
FileOpenMode FileOpenMode::TRUNCATE_AND_WRITE_MODE = 
      { false, true, true, false, true };


File::File(const String& location, const FileOpenMode& mode)
throw (IOException, InvalidStateException)
 : _impl(NULL)
{
   Ptr<FileFactory> factory = FileFactory::getActiveFileFactory();
   if (factory.isNull())
      KAREN_THROW(InvalidStateException, 
         "cannot instantiate File class: no active file factory");
   _impl = factory->createFile(location, mode);
}

Ptr<FileFactory> FileFactory::_activeFactory;

}; // namespace karen
