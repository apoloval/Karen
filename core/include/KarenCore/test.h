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

#define GEO_MAX_TESTS_PER_SUITE 256

#include "KarenCore/string.h"
#include "KarenCore/exception.h"
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

/**
 * Unit test suite class. This class represent unit test suites. This is
 * an abstract class aimed to be extended by concrete unit test suites.
 */
class UnitTestSuite
{
public:

   //! Test code type
   typedef void (UnitTestSuite::*TestCode)(void);
   
   //! Test type
   struct Test
   {
      String   name;
      TestCode code;
   };
   
   //! Test result type.
   enum TestResultStatus
   {
      TEST_RESULT_PASSED,
      TEST_RESULT_ASSERTION_FAILED,
      TEST_RESULT_FAILED,
   };
   
   /** 
    * Rest result type. This type provides the result of a single test,
    * including the test ifself and a flag indicating whether it passed.
    */
   struct TestResult
   {
      Test              test;
      TestResultStatus  status;
   };
   
   /**
    * Create a new unit test suite with given name. 
    */
   UnitTestSuite(const String& suiteName);
   
   /**
    * Get test suite name.
    */
   inline const String& name() const { return _name; }
   
   /** 
    * Run the test. This function member executes the test, reports on rep
    * and stores its results in the tstv vector. 
    * @param rep A unit test reporter object to report the results. If
    * null, no reporting is done.
    * @param tstv A vector of test result objects to be filled after suite 
    * execution. If null, no result is returned.
    * @param tstc The number of elements in tstv vector. If tstv is null, this
    * value has no meaning.
    * @return The number of filled elements in tstv if not null. Otherwise,
    * it returns 0.
    */
   unsigned int run(UnitTestReporter*  rep,
                    TestResult*        tstv, 
                    unsigned int       tstc);

protected:

   /**
    * Adds a new test to the suite.
    */
   void addTest(Test t);

private:

   String         _name;
   Test           _test[GEO_MAX_TESTS_PER_SUITE];
   unsigned int   _testCount;

};

#define KAREN_UTEST_ASSERT(expr) { \
   if (!(expr)) { \
      KAREN_THROW(::karen::InvalidAssertionException, \
      "assertion failed: %s", #expr); \
   }\
}
#define KAREN_UTEST_FAILED(reason) { \
   KAREN_THROW(::karen::TestFailedException, reason); \
}
#define KAREN_UTEST_ADD(code) { \
   Test tst = { #code, static_cast<TestCode>(&code) }; \
   addTest(tst); \
}

/*
 * Unit test reporter class. This abstract class provides the interface for
 * a unit test reporter, i.e. an object with the ability to report unit test
 * results.
 */
class UnitTestReporter
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
         UnitTestSuite::TestResultStatus status, 
         const Nullable<String>& info = Nullable<String>()) = 0;

};

//! Standard output unit test reporter
class StdOutUnitTestReporter : public UnitTestReporter
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
         UnitTestSuite::TestResultStatus status, 
         const Nullable<String>& info = Nullable<String>());

};

}; // namespace karen

#endif
