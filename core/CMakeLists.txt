cmake_minimum_required(VERSION 2.6)

project(KarenCore)

set(flag_std_cxx11 "-std=c++11")
set(test_output_dir "build/test")
set(lib_output_dir "build/lib")

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
   include/KarenCore/exception.h
   include/KarenCore/file-posix.h
   include/KarenCore/file.h
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
   COMPILE_FLAGS "${flag_std_cxx11}"
   FRAMEWORK true
   PUBLIC_HEADER "${headers}"
   LIBRARY_OUTPUT_DIRECTORY "${lib_output_dir}"
)

# Unit test executables
add_executable(KarenCore-UnitTest-Array test/test-array.cpp)
add_executable(KarenCore-UnitTest-Buffer test/test-buffer.cpp)
add_executable(KarenCore-UnitTest-Dict test/test-dict.cpp)
add_executable(KarenCore-UnitTest-File test/test-file.cpp)
add_executable(KarenCore-UnitTest-List test/test-list.cpp)
add_executable(KarenCore-UnitTest-Queue test/test-queue.cpp)
add_executable(KarenCore-UnitTest-Set test/test-set.cpp)
add_executable(KarenCore-UnitTest-String test/test-string.cpp)

# Unit test target config
set_target_properties(
      KarenCore-UnitTest-Array
      KarenCore-UnitTest-Buffer
      KarenCore-UnitTest-Dict
      KarenCore-UnitTest-File
      KarenCore-UnitTest-List
      KarenCore-UnitTest-Queue
      KarenCore-UnitTest-Set
      KarenCore-UnitTest-String
   PROPERTIES
      COMPILE_FLAGS "${flag_std_cxx11}"
      RUNTIME_OUTPUT_DIRECTORY "${test_output_dir}"
)

target_link_libraries(KarenCore-UnitTest-Array KarenCore)
target_link_libraries(KarenCore-UnitTest-Buffer KarenCore)
target_link_libraries(KarenCore-UnitTest-Dict KarenCore)
target_link_libraries(KarenCore-UnitTest-File KarenCore)
target_link_libraries(KarenCore-UnitTest-List KarenCore)
target_link_libraries(KarenCore-UnitTest-Queue KarenCore)
target_link_libraries(KarenCore-UnitTest-Set KarenCore)
target_link_libraries(KarenCore-UnitTest-String KarenCore)

# Unit test listing
enable_testing()
add_test("KarenCore-UnitTest-Array"    
         "${test_output_dir}/KarenCore-UnitTest-Array")
add_test("KarenCore-UnitTest-Buffer"
         "${test_output_dir}/KarenCore-UnitTest-Buffer")
add_test("KarenCore-UnitTest-Dict"
         "${test_output_dir}/KarenCore-UnitTest-Dict")
add_test("KarenCore-UnitTest-File"
         "${test_output_dir}/KarenCore-UnitTest-File")
add_test("KarenCore-UnitTest-List"
         "${test_output_dir}/KarenCore-UnitTest-List")
add_test("KarenCore-UnitTest-Queue"
         "${test_output_dir}/KarenCore-UnitTest-Queue")
add_test("KarenCore-UnitTest-Set"
         "${test_output_dir}/KarenCore-UnitTest-Set")
add_test("KarenCore-UnitTest-Strings"
         "${test_output_dir}/KarenCore-UnitTest-String")
