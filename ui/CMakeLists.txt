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

project(KarenUI)

set(sources)
list(APPEND sources
   src/app.cpp
   src/bitmap.cpp
   src/button.cpp
   src/core/cocoa.mm
   # src/core/ft.cpp
   src/core/gl.cpp
   src/core/glut.cpp
   src/draw.cpp
   src/engine.cpp
   src/event.cpp
   src/types.cpp
   src/widget.cpp
)

set(headers)
list(APPEND headers
   include/KarenUI/app.h
   include/KarenUI/bitmap.h
   include/KarenUI/button.h
   include/KarenUI/core/cocoa.h
   include/KarenUI/core/ft.h
   include/KarenUI/core/gl.h
   include/KarenUI/core/glut.h
   include/KarenUI/draw.h
   include/KarenUI/engine.h
   include/KarenUI/event.h
   include/KarenUI/loop.h
   include/KarenUI/timer.h
   include/KarenUI/types.h
   include/KarenUI/widget.h
   include/KarenUI/window.h
)

add_library(KarenUI SHARED ${sources} ${headers})

find_library(KAREN_CORE_LIB KarenCore .)
find_library(COCOA_LIB Cocoa)
find_library(GL_LIB OpenGL)
find_library(GLUT_LIB GLUT)

include_directories(include)

if (CMAKE_HOST_APPLE)
   # TODO: fix this to provide a more convenient directory name
   set(karen_cxx_flags "${karen_cxx_flags} -F${CMAKE_BINARY_DIR}/core/")
endif(CMAKE_HOST_APPLE)

set_target_properties(KarenUI PROPERTIES
   COMPILE_FLAGS "${karen_cxx_flags}"
   LINK_FLAGS "${karen_ld_flags}"
   FRAMEWORK true
   PUBLIC_HEADER "${headers}"
)

target_link_libraries(KarenUI 
   KarenCore
   ${COCOA_LIB} 
   ${GL_LIB} 
   ${GLUT_LIB}
)

set(test_libs "KarenUI" "KarenCore")
karen_add_test(KarenUI-BasicWidgets
               test/test-basic-widgets.cpp
               "${test_libs}")
karen_add_test(KarenUI-Cocoa
               test/test-cocoa.cpp
               "${test_libs}")
karen_add_test(KarenUI-Glut 
               test/test-glut.cpp
               "${test_libs}")
karen_add_test(KarenUI-Image
               test/test-image.cpp
               "${test_libs}")
