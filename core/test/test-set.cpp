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

#include <KarenCore/set.h>
#include <KarenCore/test.h>

using namespace karen;

KAREN_BEGIN_UNIT_TEST(SetTestSuite);

   KAREN_DECL_TEST(createEmptySet,
   {
      TreeSet<int> s;
      assertTrue(s.isEmpty());
      assertEquals<int>(0, s.size());
   });
   
   KAREN_DECL_TEST(insertOneElement,
   {
      TreeSet<int> s;
      s.insert(10);
      assertFalse(s.isEmpty());
      assertEquals<int>(1, s.size());
   });
   
   KAREN_DECL_TEST(insertSeveralElements,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      assertFalse(s.isEmpty());
      assertEquals<int>(3, s.size());
   });
   
   KAREN_DECL_TEST(insertDuplicatedElements,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      assertFalse(s.isEmpty());
      assertEquals<int>(3, s.size());
   });
   
   KAREN_DECL_TEST(containsElement,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      assertTrue(s.hasElement(7));
      assertFalse(s.hasElement(13));
   });
   
   KAREN_DECL_TEST(iterate,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);
      
      int seq[] = { 2, 7, 10, 15, 17, 20 };
      int i = 0;
      Iterator<const int> it = s.begin();
      assertTrue(it);
      for (; it; it++)
         assertEquals<int>(seq[i++], *it);
   });
   
   KAREN_DECL_TEST(removeElement,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);

      s.removeAll(10);
      
      int seq[] = { 2, 7, 15, 17, 20 };
      int i = 0;
      for (Iterator<const int> it = s.begin(); it; it++)
         assertEquals<int>(seq[i++], *it);
   });
   
   KAREN_DECL_TEST(clear,
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);
      
      s.clear();
      assertTrue(s.isEmpty());
      assertEquals<int>(0, s.size());
   });
   
KAREN_END_UNIT_TEST(SetTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   SetTestSuite suite;
   suite.run(&rep, NULL, 0);
}
