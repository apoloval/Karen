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

#include <KarenCore/collection.h>
#include <KarenCore/parsing.h>
#include <KarenCore/test.h>

using namespace karen;

class ArrayTestSuite : public UnitTestSuite
{
public:

   ArrayTestSuite() : UnitTestSuite("Collections - Array")
   {
      KAREN_UTEST_ADD(ArrayTestSuite::shouldCreateEmptyArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldCreateFromRawArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldNotIndexWhenEmpty);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldPushBackItems);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIndexWhenNotEmpty);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIterateArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIterateArrayUsingForRange);
   }
   
   void shouldCreateEmptyArray()
   {
      DynArray<int> a;
      KAREN_UTEST_ASSERT(a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 0);
   }
   
   void shouldNotIndexWhenEmpty()
   {
      DynArray<int> a;
      try
      {
         a[0];
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (OutOfBoundsException&) {}
   }
   
   void shouldCreateFromRawArray()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 6);
   }
   
   void shouldPushBackItems()
   {
      DynArray<int> a;
      a.append(10);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 1);
      KAREN_UTEST_ASSERT(a[0] == 10);
      a.append(11);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 2);
      KAREN_UTEST_ASSERT(a[1] == 11);
      a.append(12);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 3);
      KAREN_UTEST_ASSERT(a[2] == 12);   
   }

   void shouldIndexWhenNotEmpty()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      for (int i = 0; i < a.size(); i++)
         KAREN_UTEST_ASSERT(a[i] == raw[i]);
   }
   
   void shouldIterateArray()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      int i = 0;
      for (Iterator<int> it = a.begin(), end = a.end(); it != end; it++)
         KAREN_UTEST_ASSERT(*it == raw[i++]);
   }

   void shouldIterateArrayUsingForRange()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.isEmpty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      int i = 0;
      for (int n : a)
         KAREN_UTEST_ASSERT(n == raw[i++]);
   }

};

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   ArrayTestSuite suite;
   suite.run(&rep, NULL, 0);
}
