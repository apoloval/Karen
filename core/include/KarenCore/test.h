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

#ifndef KAREN_CORE_UTEST_H
#define KAREN_CORE_UTEST_H

#define KAREN_MAX_TESTS_PER_SUITE 256
#define KAREN_MAX_TESTS_PER_UNIT 256

#include "KarenCore/string.h"
#include "KarenCore/exception.h"
#include "KarenCore/first-class.h"
#include "KarenCore/types.h"

namespace karen {

// Pre-declarations
class UnitTestReporter;

/**
 * Invalid assertion exception. This exception is raised when an assertion
 * is not fit. 
 */
KAREN_DECL_EXCEPTION(InvalidAssertionException);

/**
 * Test failed exception. This exceptio is raised when a unit test fails. 
 */
KAREN_DECL_EXCEPTION(TestFailedException);

class KAREN_EXPORT Test
{
public:

   String name;
   
   inline Test(const String& name) : name(name) {}
   
   inline virtual ~Test() {}
   
   virtual void run() = 0;
   
   void assertionFailed(const String& cause);

   template <typename T>
   static void assert(const BinaryPredicate<T, T>& predicate,
                      const T& expected, const T& actual);

   template <typename T>
   inline static void assertEquals(const T& expected, const T& actual)
   { return Test::assert<T>(Equals<T, T>(), expected, actual); }
   
   template <typename T>
   inline static void assertNotEquals(const T& expected, const T& actual)
   { return Test::assert<T>(NotEquals<T, T>(), expected, actual); }
};

template <>
void
Test::assert<int>(const BinaryPredicate<int, int>& predicate,
                  const int& expected, const int& actual)
{
   if (!predicate(expected, actual))
      KAREN_THROW(::karen::InvalidAssertionException,
         "assertion failed: expected '%d' %s '%d'",
         expected, (const char*) predicate, actual);
}

template <>
void
Test::assert<float>(const BinaryPredicate<float, float>& predicate,
                    const float& expected, const float& actual)
{
   if (!predicate(expected, actual))
      KAREN_THROW(::karen::InvalidAssertionException,
         "assertion failed: expected '%f' %s '%f'",
         expected, (const char*) predicate, actual);
}

template <>
void
Test::assert<String>(const BinaryPredicate<String, String>& predicate,
                     const String& expected, const String& actual)
{
   if (!predicate(expected, actual))
      KAREN_THROW(::karen::InvalidAssertionException,
         "assertion failed: expected '%s' %s '%s'",
         (const char*) expected, (const char*) predicate, (const char*) actual);
}

enum TestResultStatus
{
   TEST_RESULT_PASSED,
   TEST_RESULT_ASSERTION_FAILED,
   TEST_RESULT_FAILED,
};

struct TestResult
{
   const Test*       test;
   TestResultStatus  status;
};

class KAREN_EXPORT UnitTest
{
public:

   UnitTest(const String& name);
   
   virtual ~UnitTest();
   
   unsigned int run(UnitTestReporter* reporter,
                    TestResult* testResults,
                    unsigned int testResultsCount);

protected:

   void addTest(Test* test);

private:
   String _name;
   Test* _tests[KAREN_MAX_TESTS_PER_UNIT];
   unsigned int _nextTest;
};

#define KAREN_BEGIN_UNIT_TEST(name) \
   class name : public UnitTest { \
   public:\
      inline name() : UnitTest(#name) {

#define KAREN_DECL_TEST(name, ...) \
         struct name : Test { \
            inline name() : Test(#name) {} \
            virtual void run() { __VA_ARGS__ }\
         }; \
         this->addTest(new name());
   
#define KAREN_END_UNIT_TEST(name) \
      }\
   }

#define assertTrue(expr) { \
   if (!(expr)) { \
      KAREN_THROW(::karen::InvalidAssertionException, \
      "assertion failed: expression '%s' is not true", #expr); \
   }\
}
#define assertFalse(expr) { \
   if (expr) { \
      KAREN_THROW(::karen::InvalidAssertionException, \
      "assertion failed: expression '%s' is not false", #expr); \
   }\
}

/*
 * Unit test reporter class. This abstract class provides the interface for
 * a unit test reporter, i.e. an object with the ability to report unit test
 * results.
 */
class KAREN_EXPORT UnitTestReporter
{
public:

   /**
    * Report begin of unit test suite
    */
   virtual void beginUnitTestSuite(const String& suiteName) = 0;

   /** 
    * Report end of unit test suite
    */
   virtual void endUnitTestSuite(unsigned int success, unsigned int tests) = 0;
   
   /** 
    * Report begin of unit test
    */
   virtual void beginUnitTest(const String& testName) = 0;

   /**
    * Report end of unit test
    */
   virtual void endUnitTest(
         TestResultStatus status, 
         const Nullable<String>& info = Nullable<String>()) = 0;

};

//! Standard output unit test reporter
class KAREN_EXPORT StdOutUnitTestReporter : public UnitTestReporter
{
public:

   /**
    * Report begin of unit test suite
    */
   virtual void beginUnitTestSuite(const String& suiteName);

   /** 
    * Report end of unit test suite
    */
   virtual void endUnitTestSuite(unsigned int success, unsigned int tests);
   
   /** 
    * Report begin of unit test
    */
   virtual void beginUnitTest(const String& testName);

   /**
    * Report end of unit test
    */
   virtual void endUnitTest(
         TestResultStatus status, 
         const Nullable<String>& info = Nullable<String>());

};

}; // namespace karen

#endif
