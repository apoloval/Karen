cmake_minimum_required(VERSION 2.6)

project(KarenUI)

set(flag_std_cxx11 "-std=c++11")
set(test_output_dir "build/test")
set(lib_output_dir "build/lib")

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

find_library(KAREN_CORE_LIB KarenCore ../core/build/lib/)
find_library(COCOA_LIB Cocoa)
find_library(GL_LIB OpenGL)
find_library(GLUT_LIB GLUT)

include_directories(include)

set_target_properties(KarenUI PROPERTIES
   COMPILE_FLAGS "${flag_std_cxx11}"
   FRAMEWORK true
   PUBLIC_HEADER "${headers}"
   LIBRARY_OUTPUT_DIRECTORY "${lib_output_dir}"
)

target_link_libraries(KarenUI 
   ${KAREN_CORE_LIB} 
   ${COCOA_LIB} 
   ${GL_LIB} 
   ${GLUT_LIB}
)

add_executable(KarenUI-BasicWidgets test/test-basic-widgets.cpp)
add_executable(KarenUI-Cocoa test/test-cocoa.cpp)
add_executable(KarenUI-Glut test/test-glut.cpp)
add_executable(KarenUI-Image test/test-image.cpp)

set_target_properties(
      KarenUI-BasicWidgets
      KarenUI-Cocoa
      KarenUI-Glut
      KarenUI-Image
   PROPERTIES
      COMPILE_FLAGS "${flag_std_cxx11}"
      RUNTIME_OUTPUT_DIRECTORY "${test_output_dir}"
      MACOSX_BUNDLE true
)

target_link_libraries(KarenUI-BasicWidgets KarenUI ${KAREN_CORE_LIB})
target_link_libraries(KarenUI-Cocoa KarenUI ${KAREN_CORE_LIB})
target_link_libraries(KarenUI-Glut KarenUI ${KAREN_CORE_LIB})
target_link_libraries(KarenUI-Image KarenUI ${KAREN_CORE_LIB})
