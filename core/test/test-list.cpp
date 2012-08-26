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


class ListTestSuite : public UnitTestSuite
{
public:
   ListTestSuite() : UnitTestSuite("Collections - List")
   {
      KAREN_UTEST_ADD(ListTestSuite::createEmptyList);
      KAREN_UTEST_ADD(ListTestSuite::insertOneElementinsertFront);
      KAREN_UTEST_ADD(ListTestSuite::insertOneElementinsertBack);
      KAREN_UTEST_ADD(ListTestSuite::insertSeveralElementsinsertFront);
      KAREN_UTEST_ADD(ListTestSuite::insertSeveralElementsinsertBack);
      KAREN_UTEST_ADD(ListTestSuite::iterateFromHead);
      KAREN_UTEST_ADD(ListTestSuite::contains);
      KAREN_UTEST_ADD(ListTestSuite::removeIterator);
      KAREN_UTEST_ADD(ListTestSuite::removeWithInvalidIterator);
      KAREN_UTEST_ADD(ListTestSuite::removeFirst);
      KAREN_UTEST_ADD(ListTestSuite::removeLast);
      KAREN_UTEST_ADD(ListTestSuite::clearEmpty);
      KAREN_UTEST_ADD(ListTestSuite::clearNotEmpty);
      KAREN_UTEST_ADD(ListTestSuite::insertBefore);
      KAREN_UTEST_ADD(ListTestSuite::insertAfter);
      KAREN_UTEST_ADD(ListTestSuite::iterateForRange);
   }
   
   void createEmptyList()
   {
      LinkedList<int> l;
      KAREN_UTEST_ASSERT(l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 0);      
   }
   
   void insertOneElementinsertFront()
   {
      LinkedList<int> l;
      l.insertFront(11);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 1);
      KAREN_UTEST_ASSERT(l.first() == 11);
      KAREN_UTEST_ASSERT(l.last() == 11);
   }

   void insertOneElementinsertBack()
   {
      LinkedList<int> l;
      l.insertBack(10);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 1);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 10);
   }

   void insertSeveralElementsinsertFront()
   {
      LinkedList<int> l;
      l.insertFront(11);
      l.insertFront(12);
      l.insertFront(13);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 3);
      KAREN_UTEST_ASSERT(l.first() == 13);
      KAREN_UTEST_ASSERT(l.last() == 11);
   }

   void insertSeveralElementsinsertBack()
   {
      LinkedList<int> l;
      l.insertBack(11);
      l.insertBack(12);
      l.insertBack(13);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 3);
      KAREN_UTEST_ASSERT(l.first() == 11);
      KAREN_UTEST_ASSERT(l.last() == 13);
   }

   void iterateFromHead()
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 19);
      Iterator<int> it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == j++);
   }

   void contains()
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 19);
      KAREN_UTEST_ASSERT(l.hasElement(15));
      KAREN_UTEST_ASSERT(!l.hasElement(25));
   }

   void removeIterator()
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 19);

      Iterator<int> it = l.begin();
      for (int i = 0; i < 5; i++)
         it++;
      l.remove(it);
      KAREN_UTEST_ASSERT(*it == 16);
      KAREN_UTEST_ASSERT(!l.hasElement(15));
   }

   void removeFirst()
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.removeFirst();
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 9);
      KAREN_UTEST_ASSERT(l.first() == 11);
      KAREN_UTEST_ASSERT(l.last() == 19);
   }
   
   void removeLast()
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.removeLast();
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 9);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 18);
   }
   
   void removeWithInvalidIterator()
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      
      try
      {
         Iterator<int> it;
         l.remove(it);
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (InvalidInputException&) {}
      try
      {
         LinkedList<int> ll;
         Iterator<int> it = ll.begin();
         l.remove(it);
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (InvalidInputException&) {}
   }
   
   void clearNotEmpty()
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.clear();
      KAREN_UTEST_ASSERT(l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 0);
   }
   
   void clearEmpty()
   {
      LinkedList<int> l;
      l.clear();
      KAREN_UTEST_ASSERT(l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 0);
   }
   
   void insertBefore()
   {
      LinkedList<int> l;
      l.insertBack(10);
      l.insertBack(11);
      l.insertBack(12);
      l.insertBack(14);
      Iterator<int> it = l.begin();
      ++it; ++it; ++it;
      l.insertBefore(13, it);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 5);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 14);
      KAREN_UTEST_ASSERT(*it == 14);
      int j = 10;
      it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == j++);
   }

   void insertAfter()
   {
      LinkedList<int> l;
      l.insertBack(10);
      l.insertBack(12);
      l.insertBack(13);
      l.insertBack(14);
      Iterator<int> it = l.begin();
      l.insertAfter(11, it);
      KAREN_UTEST_ASSERT(!l.isEmpty());
      KAREN_UTEST_ASSERT(l.size() == 5);
      KAREN_UTEST_ASSERT(l.first() == 10);
      KAREN_UTEST_ASSERT(l.last() == 14);
      KAREN_UTEST_ASSERT(*it == 10);
      int j = 10;
      it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; ++it)
         KAREN_UTEST_ASSERT(*it == j++);
   }
   
   void iterateForRange()
   {
      LinkedList<int> l;
      l.insertBack(10);
      l.insertBack(12);
      l.insertBack(13);
      l.insertBack(14);
      int i = 0;
      int values[] = { 10, 12, 13, 14 };
      for (int num : l)
      {
         KAREN_UTEST_ASSERT(num == values[i++]);
      }
      KAREN_UTEST_ASSERT(i == 4);
      
   }
   
};

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   ListTestSuite suite;
   suite.run(&rep, NULL, 0);
}
