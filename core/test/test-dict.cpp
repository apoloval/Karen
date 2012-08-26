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

class DictTestSuite : public UnitTestSuite
{
public:

   DictTestSuite() : UnitTestSuite("Collections - TreeMap")
   {
      KAREN_UTEST_ADD(DictTestSuite::shouldCreateAnEmptyTreeMap);
      KAREN_UTEST_ADD(DictTestSuite::shouldInsertOneElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldInsertOneElementUsingIndexingOperator);
      KAREN_UTEST_ADD(DictTestSuite::shouldUpdateAnExistingElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldRemoveAnExistingElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldClear);
      KAREN_UTEST_ADD(DictTestSuite::shouldIterate);
   }
   
   void shouldCreateAnEmptyTreeMap()
   {
      TreeMap<String, int> d;
      KAREN_UTEST_ASSERT(d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 0);
   }
   
   void shouldInsertOneElement()
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      KAREN_UTEST_ASSERT(!d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.hasKey("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 45);
   }
   
   void shouldInsertOneElementUsingIndexingOperator()
   {
      TreeMap<String, int> d;
      d["Mark"] = 45;
      KAREN_UTEST_ASSERT(!d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.hasKey("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 45);
   }
      
   void shouldUpdateAnExistingElement()
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d["Mark"] = 40;
      KAREN_UTEST_ASSERT(!d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.hasKey("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 40);
   }
      
   void shouldRemoveAnExistingElement()
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d.put("John", 35);
      d.remove("Mark");
      KAREN_UTEST_ASSERT(!d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.hasKey("John"));
   }
      
   void shouldClear()
   {
      TreeMap<String, int> d;
      d.put("Mark", 45);
      d.put("John", 35);
      d.clear();
      KAREN_UTEST_ASSERT(d.isEmpty());
      KAREN_UTEST_ASSERT(d.size() == 0);
   }
   
   void shouldIterate()
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
         KAREN_UTEST_ASSERT(it->get<0>() == elems[i].get<0>());
         KAREN_UTEST_ASSERT(it->get<1>() == elems[i].get<1>());
         i++;
      }
   }
      
};

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   DictTestSuite suite;
   suite.run(&rep, NULL, 0);
}
