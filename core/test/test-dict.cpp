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
#include <KarenCore/test.h>

using namespace karen;


KAREN_BEGIN_UNIT_TEST(DictTestSuite);

   KAREN_DECL_TEST(shouldCreateAnEmptyTreeMap,
   {
      TreeMap<String, int> d;
      assertTrue(d.isEmpty());
      assertEquals<int>(0, d.size());
   });
   
   KAREN_DECL_TEST(shouldInsertOneElement,
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      assertFalse(d.isEmpty());
      assertEquals<int>(1, d.size());
      assertTrue(d.hasKey("Mark"));
      assertEquals<int>(45, d["Mark"]);
   });
   
   KAREN_DECL_TEST(shouldInsertOneElementUsingIndexingOperator,
   {
      TreeMap<String, int> d;
      d["Mark"] = 45;
      assertFalse(d.isEmpty());
      assertEquals<int>(1, d.size());
      assertTrue(d.hasKey("Mark"));
      assertEquals<int>(45, d["Mark"]);
   });
      
   KAREN_DECL_TEST(shouldUpdateAnExistingElement,
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d["Mark"] = 40;
      assertFalse(d.isEmpty());
      assertEquals<int>(1, d.size());
      assertTrue(d.hasKey("Mark"));
      assertEquals<int>(40, d["Mark"]);
   });
      
   KAREN_DECL_TEST(shouldRemoveAnExistingElement,
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d.put("John", 35);
      d.remove("Mark");
      assertFalse(d.isEmpty());
      assertEquals<int>(1, d.size());
      assertTrue(d.hasKey("John"));
   });
      
   KAREN_DECL_TEST(shouldClear,
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d.put("John", 35);
      d.clear();
      assertTrue(d.isEmpty());
      assertEquals<int>(0, d.size());
   });
   
   KAREN_DECL_TEST(shouldIterate,
   {
      Tuple<const String, int> elems[] =
      {
         Tuple<const String, int>("John",   12 ),
         Tuple<const String, int>("Laura",  33 ),
         Tuple<const String, int>("Mark",   45 ),
         Tuple<const String, int>("Patty",  18 ),
      };
      TreeMap<String, int> d(elems, 4);
      Iterator<Tuple<const String, int> > it;
      int i = 0;
      for (it = d.begin(); it; it++)
      {
         assertEquals(elems[i].get<0>(), it->get<0>());
         assertEquals(elems[i].get<1>(), it->get<1>());
         i++;
      }
   });
      
KAREN_END_UNIT_TEST(DictTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   DictTestSuite suite;
   suite.run(&rep, NULL, 0);
}
