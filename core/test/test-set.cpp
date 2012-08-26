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

class SetTestSuite : public UnitTestSuite
{
public:

   SetTestSuite() : UnitTestSuite("Collections - Set")
   {
      KAREN_UTEST_ADD(SetTestSuite::createEmptySet);
      KAREN_UTEST_ADD(SetTestSuite::insertOneElement);
      KAREN_UTEST_ADD(SetTestSuite::insertSeveralElements);
      KAREN_UTEST_ADD(SetTestSuite::insertDuplicatedElements);
      KAREN_UTEST_ADD(SetTestSuite::containsElement);
      KAREN_UTEST_ADD(SetTestSuite::iterate);
      KAREN_UTEST_ADD(SetTestSuite::removeElement);
      KAREN_UTEST_ADD(SetTestSuite::clear);
   }
   
   void createEmptySet()
   {
      TreeSet<int> s;
      KAREN_UTEST_ASSERT(s.isEmpty());
      KAREN_UTEST_ASSERT(s.size() == 0);
   }
   
   void insertOneElement()
   {
      TreeSet<int> s;
      s.insert(10);
      KAREN_UTEST_ASSERT(!s.isEmpty());
      KAREN_UTEST_ASSERT(s.size() == 1);
   }
   
   void insertSeveralElements()
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      KAREN_UTEST_ASSERT(!s.isEmpty());
      KAREN_UTEST_ASSERT(s.size() == 3);
   }
   
   void insertDuplicatedElements()
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      KAREN_UTEST_ASSERT(!s.isEmpty());
      KAREN_UTEST_ASSERT(s.size() == 3);
   }
   
   void containsElement()
   {
      TreeSet<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      KAREN_UTEST_ASSERT(s.hasElement(7));
      KAREN_UTEST_ASSERT(!s.hasElement(13));
   }
   
   void iterate()
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
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == seq[i++]);
   }
   
   void removeElement()
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
         KAREN_UTEST_ASSERT(*it == seq[i++]);
   }
   
   void clear()
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
      KAREN_UTEST_ASSERT(s.isEmpty());
      KAREN_UTEST_ASSERT(s.size() == 0);
   }
   
};

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   SetTestSuite suite;
   suite.run(&rep, NULL, 0);
}
