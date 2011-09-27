/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#ifndef KAREN_UTILS_PLATFORM_H
#define KAREN_UTILS_PLATFORM_H

/* Constant definitions for platform properties. */
#define KAREN_ARCH_32            1
#define KAREN_ARCH_64            2

#define KAREN_PLATFORM_WINDOWS   1
#define KAREN_PLATFORM_LINUX     2
#define KAREN_PLATFORM_OSX       3

#define KAREN_COMPILER_GCC       1
#define KAREN_COMPILER_LLVM      2
#define KAREN_COMPILER_MSVC      3
#define KAREN_COMPILER_CYGWIN    4
#define KAREN_COMPILER_MINGW     5

#define KAREN_BIG_ENDIAN         1
#define KAREN_LITTLE_ENDIAN      2

/* Check the platform architecture. */
#if defined(__x86_64__) || defined(_M_X64)   || defined(__powerpc64__) || \
    defined(__alpha__)  || defined(__ia64__) || defined(__s390__)      || \
    defined(__s390x__)
#  define KAREN_ARCH_TYPE KAREN_ARCH_64
#else
#  define KAREN_ARCH_TYPE KAREN_ARCH_32
#endif

/* Check the platform OS. */
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( _WIN64 )
#  define KAREN_PLATFORM KAREN_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__) || (defined(__APPLE__) && defined(__MACH__))
#  define KAREN_PLATFORM KAREN_PLATFORM_OSX
#else
#  define KAREN_PLATFORM KAREN_PLATFORM_LINUX
#endif

/* Check the compiler. */
#if defined(__GNUC__)
#  define KAREN_COMPILER KAREN_COMPILER_GCC
#elif defined(__llvm__) || defined(__clang__)
#  #define KAREN_COMPILER KAREN_COMPILER_LLVM
#elif defined(_MSC_VER)
#  #define KAREN_COMPILER KAREN_COMPILER_MSVC
#elif defined(__CYGWIN__)
#  #define KAREN_COMPILER KAREN_COMPILER_CYGWIN
#elif defined(__MINGW32__)
#  #define KAREN_COMPILER KAREN_COMPILER_MINGW
#else
#  #error cannot determine C++ compiler vendor
#endif

/* Check the endianness. */
#if defined(__i386__) || defined(_M_IX86)    || defined(_X86_)     || \
    defined(__ia64__) || defined(_M_IA64)    || defined(__amd64__) || \
    defined(__amd64)  || defined(__x86_64__) || defined(__x86_64)  || \
    defined(_M_X64)
#  define KAREN_ENDIANNESS KAREN_LITTLE_ENDIAN
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || \
      defined(__ppc__) || defined(_M_PPC)
#  define KAREN_ENDIANNESS KAREN_BIG_ENDIAN
#endif

/* Define the symbol export macros. */
#if KAREN_PLATFORM == KAREN_PLATFORM_WINDOWS
#  ifdef KAREN_LIB_BUILD
#     if KAREN_COMPILER == KAREN_COMPILER_CYGWIN || \
#        KAREN_COMPILER == KAREN_COMPILER_MINGW
#        define KAREN_EXPORT __attribute__((dllexport))
#        define KAREN_LOCAL
#     else
#        define KAREN_EXPORT __declspec(dllexport)
#        define KAREN_LOCAL
#     endif
#  else
#     if KAREN_COMPILER == KAREN_COMPILER_CYGWIN || \
#        KAREN_COMPILER == KAREN_COMPILER_MINGW
#        define KAREN_EXPORT __attribute__((dllimport))
#        define KAREN_LOCAL
#     else
#        define KAREN_EXPORT __declspec(dllimport)
#        define KAREN_LOCAL
#     endif
#  endif
#else // non-windows
#  define KAREN_EXPORT __attribute__ ((visibility ("default")))
#  define KAREN_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#endif