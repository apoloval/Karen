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

KAREN_BEGIN_UNIT_TEST(ArrayTestSuite);
   
   KAREN_DECL_TEST(shouldCreateEmptyArray,
   {
      DynArray<int> a;
      assertTrue(a.isEmpty());
      assertEquals<int>(0, a.size());
   });
   
   KAREN_DECL_TEST(shouldNotIndexWhenEmpty,
   {
      DynArray<int> a;
      try
      {
         a[0];
         assertionFailed("expected exception not raised");
      }
      catch (OutOfBoundsException&) {}
   });
   
   KAREN_DECL_TEST(shouldCreateFromRawArray,
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      assertFalse(a.isEmpty());
      assertEquals<int>(6, a.size());
   });
   
   KAREN_DECL_TEST(shouldPushBackItems,
   {
      DynArray<int> a;
      a.append(10);
      assertFalse(a.isEmpty());
      assertEquals<int>(1, a.size());
      assertEquals(10, a[0]);
      a.append(11);
      assertFalse(a.isEmpty());
      assertEquals<int>(2, a.size());
      assertEquals(11, a[1]);
      a.append(12);
      assertFalse(a.isEmpty());
      assertEquals<int>(3, a.size());
      assertEquals(12, a[2]);
   });

   KAREN_DECL_TEST(shouldIndexWhenNotEmpty,
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      assertFalse(a.isEmpty());
      assertEquals<int>(6, a.size());
      for (unsigned int i = 0; i < a.size(); i++)
         assertEquals(a[i], raw[i]);
   });
   
   KAREN_DECL_TEST(shouldIterateArray,
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      assertFalse(a.isEmpty());
      assertEquals<int>(6, a.size());
      int i = 0;
      for (Iterator<int> it = a.begin(), end = a.end(); it != end; it++)
         assertEquals(*it, raw[i++]);
   });

#ifdef KAREN_CXX11_HAVE_RANGE_FOR
   KAREN_DECL_TEST(shouldIterateArrayUsingForRange,
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      DynArray<int> a(raw, 6);
      assertFalse(a.isEmpty());
      assertEquals<int>(6, a.size());
      int i = 0;
      for (int n : a)
         assertEquals(n, raw[i++]);
   });
#endif

KAREN_END_UNIT_TEST(ArrayTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   ArrayTestSuite suite;
   suite.run(&rep, NULL, 0);
}
