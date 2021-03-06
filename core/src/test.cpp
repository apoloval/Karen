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

#include "KarenCore/test.h"

#include <iostream>

namespace karen {

void
Test::assertionFailed(const String& cause)
{
   KAREN_THROW(::karen::InvalidAssertionException,
         (const char*) cause);
}

UnitTest::UnitTest(const String& name) : _name(name), _nextTest(0) {}

UnitTest::~UnitTest()
{
   for (unsigned int i = 0; i < _nextTest; i++)
      delete _tests[i];
}

unsigned int
UnitTest::run(
      UnitTestReporter* reporter,
      TestResult* testResults, 
      unsigned int testResultsCount)
{
   if (reporter)
      reporter->beginUnitTestSuite(_name);
   unsigned int i = 0, passed = 0;
   while (i < _nextTest)
   {
      if (reporter)
         reporter->beginUnitTest(_tests[i]->name);
      if (testResults)
         testResults[i].test = _tests[i];
      try
      {
         _tests[i]->run();
         if (reporter)
            reporter->endUnitTest(TEST_RESULT_PASSED);
         if (testResults)
            testResults[i].status = TEST_RESULT_PASSED;
         passed++;
      }
      catch (InvalidAssertionException &e)
      {
         if (reporter)
            reporter->endUnitTest(TEST_RESULT_ASSERTION_FAILED, e.cause());
         if (testResults)
            testResults[i].status = TEST_RESULT_ASSERTION_FAILED;
      }
      catch (TestFailedException &e)
      {
         if (reporter)
            reporter->endUnitTest(TEST_RESULT_FAILED, e.cause());
         if (testResults)
            testResults[i].status = TEST_RESULT_FAILED;
      }
      catch (Exception& e)
      {
         if (reporter)
            reporter->endUnitTest(TEST_RESULT_FAILED, String::format(
                  "unexpected exception: %s", (const char*) e.cause()));
         if (testResults)
            testResults[i].status = TEST_RESULT_FAILED;
      }
      catch (std::exception& e)
      {
         if (reporter)
            reporter->endUnitTest(TEST_RESULT_FAILED, String::format(
                  "unexpected exception: %s", e.what()));
         if (testResults)
            testResults[i].status = TEST_RESULT_FAILED;
      }
      catch (...)
      {
         if (reporter)
            reporter->endUnitTest(
                  TEST_RESULT_FAILED, String("unknown exception raised"));
         if (testResults)
            testResults[i].status = TEST_RESULT_FAILED;
      }
      i++;
   }
   if (reporter)
      reporter->endUnitTestSuite(passed, i);
   return testResults ? i : 0;
}

void
UnitTest::addTest(Test* test)
{
   _tests[_nextTest++] = test;
}

void
StdOutUnitTestReporter::beginUnitTestSuite(
      const String& suiteName)
{ 
   std::cerr << 
      String::format("Running test suite %s", (const char*) suiteName) << 
      std::endl;
}

void
StdOutUnitTestReporter::endUnitTestSuite(
      unsigned int success, 
      unsigned int tests)
{ 
   std::cerr << 
      String::format("Test suite done (passed %i of %i)", success, tests) <<
      std::endl;
}
   
void
StdOutUnitTestReporter::beginUnitTest(
      const String& testName)
{ std::cerr << String::format("   Testing %s... ", (const char*) testName); }

void
StdOutUnitTestReporter::endUnitTest(
      TestResultStatus status,
      const Nullable<String>& info)
{
   switch (status)
   {
      case TEST_RESULT_PASSED: 
         std::cerr << "success"; break;
      case TEST_RESULT_ASSERTION_FAILED:
         std::cerr << "ASSERTION FAILED"; break;
      case TEST_RESULT_FAILED:
         std::cerr << "FAILED"; break;
   }
   if (!info.isNull())
      std::cerr << String::format(": %s", (const char*) (const String&) info);
   std::cerr << std::endl;
}


}; // namespace karen
