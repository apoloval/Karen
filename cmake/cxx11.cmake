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

####
# Check for properties for specific compiler
####

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   set(karen_cxx_flags "-std=c++11 -stdlib=libc++")
   set(karen_ld_flags "-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   set(karen_cxx_flags "-std=c++11")
   set(karen_ld_flags "")
else()
   set(karen_cxx_flags "")
   set(karen_ld_flags "")
endif()

if (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   set(karen_ld_flags "${karen_ld_flags} -static-libgcc -static-libstdc++")
   set(karen_cxx_flags "${karen_cxx_flags} -Wno-deprecated")
endif()
