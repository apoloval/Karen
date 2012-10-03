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

#ifndef KAREN_CORE_PLATFORM_H
#define KAREN_CORE_PLATFORM_H

/* Constant definitions for platform properties. */
#define KAREN_ARCH_32            1
#define KAREN_ARCH_64            2

#define KAREN_PLATFORM_WINDOWS   1
#define KAREN_PLATFORM_LINUX     2
#define KAREN_PLATFORM_OSX       3

#define KAREN_COMPILER_GCC       1
#define KAREN_COMPILER_CLANG     2
#define KAREN_COMPILER_MSVC      3
#define KAREN_COMPILER_CYGWIN    4
#define KAREN_COMPILER_MINGW     5

#define KAREN_BIG_ENDIAN         1
#define KAREN_LITTLE_ENDIAN      2

/* Check the platform architecture. */
#if defined(__x86_64__) || defined(_M_X64)   || defined(__powerpc64__) || \
    defined(__alpha__)  || defined(__ia64__) || defined(__s390__)      || \
    defined(__s390x__)
   #define KAREN_ARCH_TYPE KAREN_ARCH_64
#else
   #define KAREN_ARCH_TYPE KAREN_ARCH_32
#endif

/* Check the platform OS. */
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( _WIN64 )
   #define KAREN_PLATFORM KAREN_PLATFORM_WINDOWS
   #define KAREN_PLATFORM_IS_WIN32
#elif defined( __APPLE_CC__) || (defined(__APPLE__) && defined(__MACH__))
   #define KAREN_PLATFORM KAREN_PLATFORM_OSX
   #define KAREN_PLATFORM_IS_POSIX
#else
   #define KAREN_PLATFORM KAREN_PLATFORM_LINUX
   #define KAREN_PLATFORM_IS_POSIX
#endif

/* Check the compiler. */
#if defined(__llvm__) || defined(__clang__)
   #define KAREN_COMPILER KAREN_COMPILER_CLANG
   #define KAREN_COMPILER_MAJOR_VERSION __clang_major__
   #define KAREN_COMPILER_MINOR_VERSION __clang_minor__
#elif defined(__GNUC__)
   #define KAREN_COMPILER KAREN_COMPILER_GCC
   #define KAREN_COMPILER_MAJOR_VERSION __GNUC__
   #define KAREN_COMPILER_MINOR_VERSION __GNUC_MINOR__
#elif defined(_MSC_VER)
   #define KAREN_COMPILER KAREN_COMPILER_MSVC
   #define KAREN_COMPILER_MAJOR_VERSION ((_MSC_VER / 100) - 6)
   #define KAREN_COMPILER_MINOR_VERSION ((_MSC_VER % 100) / 10)
#elif defined(__CYGWIN__)
   #define KAREN_COMPILER KAREN_COMPILER_CYGWIN
   #define KAREN_COMPILER_MAJOR_VERSION __GNUC__
   #define KAREN_COMPILER_MINOR_VERSION __GNUC_MINOR__
#elif defined(__MINGW32__)
   #define KAREN_COMPILER KAREN_COMPILER_MINGW
   #define KAREN_COMPILER_MAJOR_VERSION __GNUC__
   #define KAREN_COMPILER_MINOR_VERSION __GNUC_MINOR__
#else
   #error cannot determine C++ compiler vendor
#endif

#define KAREN_COMPILER_AT_LEAST(major, minor) \
   (KAREN_COMPILER_MAJOR_VERSION > major || \
         (major == KAREN_COMPILER_MAJOR_VERSION && \
          KAREN_COMPILER_MINOR_VERSION >= minor ))

/* Check the endianness. */
#if defined(__i386__) || defined(_M_IX86)    || defined(_X86_)     || \
    defined(__ia64__) || defined(_M_IA64)    || defined(__amd64__) || \
    defined(__amd64)  || defined(__x86_64__) || defined(__x86_64)  || \
    defined(_M_X64)
   #define KAREN_ENDIANNESS KAREN_LITTLE_ENDIAN
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || \
      defined(__ppc__) || defined(_M_PPC)
   #define KAREN_ENDIANNESS KAREN_BIG_ENDIAN
#endif

/* Define the symbol export macros. */
#if KAREN_PLATFORM == KAREN_PLATFORM_WINDOWS
   #ifdef KAREN_LIB_BUILD
      #if KAREN_COMPILER == KAREN_COMPILER_CYGWIN || \
          KAREN_COMPILER == KAREN_COMPILER_MINGW || \
          KAREN_COMPILER == KAREN_COMPILER_CLANG
         #define KAREN_EXPORT __attribute__((dllexport))
         #define KAREN_LOCAL
      #else
         #define KAREN_EXPORT __declspec(dllexport)
         #define KAREN_LOCAL
      #endif
   #else
      #if KAREN_COMPILER == KAREN_COMPILER_CYGWIN || \
          KAREN_COMPILER == KAREN_COMPILER_MINGW || \
          KAREN_COMPILER == KAREN_COMPILER_CLANG
      #define KAREN_EXPORT __attribute__((dllimport))
      #define KAREN_LOCAL
   #else
      #define KAREN_EXPORT __declspec(dllimport)
      #define KAREN_LOCAL
   #endif
#endif
#else // non-windows
   #define KAREN_EXPORT __attribute__ ((visibility ("default")))
   #define KAREN_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

// Integer formats of fixed bit width
#if KAREN_ARCH_TYPE == KAREN_ARCH_32
typedef unsigned long      UInt32;
typedef long               Int32;
#else
typedef unsigned int       UInt32;
typedef int                Int32;
#endif
typedef unsigned short     UInt16;
typedef short              Int16;
typedef unsigned char      UInt8;
typedef char               Int8;
typedef UInt8              Byte;


// Special case: 64-bit types
#if KAREN_ARCH_TYPE == KAREN_ARCH_64
   #if KAREN_COMPILER == KAREN_COMPILER_MSVC
typedef unsigned __int64   UInt64;
typedef __int64            Int64;
   #else
typedef unsigned long long UInt64;
typedef long long          Int64;
   #endif
#endif

/* Check for C++11 features. */
#if KAREN_COMPILER == KAREN_COMPILER_CLANG
   #if KAREN_COMPILER_AT_LEAST(2, 9)
      #define KAREN_CXX11_HAVE_AUTO_TYPE
      #define KAREN_CXX11_HAVE_RVALUE_REFS
      #define KAREN_CXX11_HAVE_STATIC_ASSERTIONS
      #define KAREN_CXX11_HAVE_VARIADIC_TEMPLATES
   #endif
   #if KAREN_COMPILER_AT_LEAST(3, 0)
      #define KAREN_CXX11_HAVE_DELEGATING_CONSTRUCTORS
      #define KAREN_CXX11_HAVE_NULL_PTR_CONSTANT
      #define KAREN_CXX11_HAVE_RANGE_FOR
   #endif
   #if KAREN_COMPILER_AT_LEAST(3, 1)
      #define KAREN_CXX11_HAVE_INITIALIZER_LISTS
   #endif
   #if KAREN_COMPILER_AT_LEAST(4, 0)
      #define KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
   #endif
#elif KAREN_COMPILER == KAREN_COMPILER_GCC
   #if KAREN_COMPILER_AT_LEAST(4, 3)
      #define KAREN_CXX11_HAVE_RVALUE_REFS
      #define KAREN_CXX11_HAVE_STATIC_ASSERTIONS
   #endif
   #if KAREN_COMPILER_AT_LEAST(4, 4)
      #define KAREN_CXX11_HAVE_AUTO_TYPE
      #define KAREN_CXX11_HAVE_INITIALIZER_LISTS
      #define KAREN_CXX11_HAVE_VARIADIC_TEMPLATES
   #endif
   #if KAREN_COMPILER_AT_LEAST(4, 5)
      #define KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
   #endif
   #if KAREN_COMPILER_AT_LEAST(4, 6)
      #define KAREN_CXX11_HAVE_NULL_PTR_CONSTANT
      #define KAREN_CXX11_HAVE_RANGE_FOR
   #endif
   #if KAREN_COMPILER_AT_LEAST(4, 7)
      #define KAREN_CXX11_HAVE_DELEGATING_CONSTRUCTORS
   #endif
#elif KAREN_COMPILER == KAREN_COMPILER_MSVC
   #if KAREN_COMPILER_AT_LEAST(10, 0)
      #define KAREN_CXX11_HAVE_RVALUE_REFS
      #define KAREN_CXX11_HAVE_STATIC_ASSERTIONS
      #define KAREN_CXX11_HAVE_AUTO_TYPE
      #define KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
      #define KAREN_CXX11_HAVE_NULL_PTR_CONSTANT
   #endif
#endif

#endif
