/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#include "utils/test.h"

#include <iostream>

namespace karen { namespace utils {

UnitTestSuite::UnitTestSuite(const String& suiteName)
 : _name(suiteName),
   _testCount(0)
{
}

void
UnitTestSuite::addTest(Test t)
{ _test[_testCount++] = t; }

unsigned int
UnitTestSuite::run(
      UnitTestReporter* rep,
      TestResult* tstv, 
      unsigned int tstc)
{
   if (rep) rep->beginUnitTestSuite(_name);
   int i = 0, passed = 0;
   while (i < _testCount)
   {
      if (rep) rep->beginUnitTest(_test[i].name);
      if (tstv) tstv[i].test = _test[i];
      try
      {
         (this->*_test[i].code)();
         if (rep) rep->endUnitTest(TEST_RESULT_PASSED);
         if (tstv) tstv[i].status = TEST_RESULT_PASSED;
         passed++;
      }
      catch (InvalidAssertionException &e)
      {
         if (rep) rep->endUnitTest(TEST_RESULT_ASSERTION_FAILED, e.cause());
         if (tstv) tstv[i].status = TEST_RESULT_ASSERTION_FAILED;
      }
      catch (TestFailedException &e)
      {
         if (rep) rep->endUnitTest(TEST_RESULT_FAILED, e.cause());
         if (tstv) tstv[i].status = TEST_RESULT_FAILED;
      }
      catch (Exception& e)
      {
         if (rep) rep->endUnitTest(TEST_RESULT_FAILED, String::format(
               "unexpected exception: %s", (const char*) e.cause()));
         if (tstv) tstv[i].status = TEST_RESULT_FAILED;
      }
      catch (std::exception& e)
      {
         if (rep) rep->endUnitTest(TEST_RESULT_FAILED, String::format(
               "unexpected exception: %s", e.what()));
         if (tstv) tstv[i].status = TEST_RESULT_FAILED;
      }
      catch (...)
      {
         if (rep) rep->endUnitTest(
               TEST_RESULT_FAILED, String("unknown exception raised"));
         if (tstv) tstv[i].status = TEST_RESULT_FAILED;
      }
      i++;
   }
   if (rep) rep->endUnitTestSuite(passed, i);
   return tstv ? i : 0;
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
      UnitTestSuite::TestResultStatus status,
      const Nullable<String>& info)
{
   switch (status)
   {
      case UnitTestSuite::TEST_RESULT_PASSED: 
         std::cerr << "success"; break;
      case UnitTestSuite::TEST_RESULT_ASSERTION_FAILED:
         std::cerr << "ASSERTION FAILED"; break;
      case UnitTestSuite::TEST_RESULT_FAILED:
         std::cerr << "FAILED"; break;
   }
   if (!info.isNull())
      std::cerr << String::format(": %s", (const char*) (const String&) info);
   std::cerr << std::endl;
}


}}; // namespace karen::utils
