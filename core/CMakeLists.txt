#
# ---------------------------------------------------------------------
#  This file is part of Karen
#
#  Copyright (c) 2007-2012 Alvaro Polo
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
#  02110-1301 USA
#
#  ---------------------------------------------------------------------
#

cmake_minimum_required(VERSION 2.6)

project(KarenCore)

set(sources)
list(APPEND sources
   src/buffer.cpp
   src/exception.cpp
   src/file-posix.cpp
   src/file.cpp
   src/numeric.cpp
   src/parsing.cpp
   src/string.cpp
   src/test.cpp
   src/timing.cpp
)

set(headers)
list(APPEND headers
   include/KarenCore.h
   include/KarenCore/bolt.h
   include/KarenCore/buffer.h
   include/KarenCore/collection-inl.h
   include/KarenCore/collection.h
   include/KarenCore/events.h
   include/KarenCore/exception.h
   include/KarenCore/file-posix.h
   include/KarenCore/file.h
   include/KarenCore/first-class.h
   include/KarenCore/iterator.h
   include/KarenCore/numeric.h
   include/KarenCore/parsing.h
   include/KarenCore/platform.h
   include/KarenCore/pointer.h
   include/KarenCore/stream.h
   include/KarenCore/string.h
   include/KarenCore/test.h
   include/KarenCore/timing.h
   include/KarenCore/types.h
)
   
add_library(KarenCore SHARED ${sources} ${headers})

include_directories(include)

set_target_properties(KarenCore PROPERTIES
   COMPILE_FLAGS "${karen_cxx_flags}"
   LINK_FLAGS "${karen_ld_flags}"
   DEFINE_SYMBOL "KAREN_LIB_BUILD=1"
   FRAMEWORK true
   GNUtoMS true
   PUBLIC_HEADER "${headers}"
   PREFIX ""
)

# Unit test executables
karen_add_test(KarenCore-UnitTest-Array test/test-array.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-Buffer test/test-buffer.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-Dict test/test-dict.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-Events test/test-events.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-File test/test-file.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-List test/test-list.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-Queue test/test-queue.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-Set test/test-set.cpp KarenCore)
karen_add_test(KarenCore-UnitTest-String test/test-string.cpp KarenCore)


