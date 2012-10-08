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

#include <KarenCore/list.h>
#include <KarenCore/test.h>

using namespace karen;


KAREN_BEGIN_UNIT_TEST(ListTestSuite);

   KAREN_DECL_TEST(createEmptyList,
   {
      LinkedList<int> l;
      assertTrue(l.isEmpty());
      assertEquals<int>(0, l.size());
   });
   
   KAREN_DECL_TEST(insertOneElementinsertFront,
   {
      LinkedList<int> l;
      l.insertFront(11);
      assertFalse(l.isEmpty());
      assertEquals<int>(1, l.size());
      assertEquals<int>(11, l.first());
      assertEquals<int>(11, l.last());
   });

   KAREN_DECL_TEST(insertOneElementinsertBack,
   {
      LinkedList<int> l;
      l.insertBack(10);
      assertFalse(l.isEmpty());
      assertEquals<int>(1, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(10, l.last());
   });

   KAREN_DECL_TEST(insertSeveralElementsinsertFront,
   {
      LinkedList<int> l;
      l.insertFront(11);
      l.insertFront(12);
      l.insertFront(13);
      assertFalse(l.isEmpty());
      assertEquals<int>(3, l.size());
      assertEquals<int>(13, l.first());
      assertEquals<int>(11, l.last());
   });

   KAREN_DECL_TEST(insertSeveralElementsinsertBack,
   {
      LinkedList<int> l;
      l.insertBack(11);
      l.insertBack(12);
      l.insertBack(13);
      assertFalse(l.isEmpty());
      assertEquals<int>(3, l.size());
      assertEquals<int>(11, l.first());
      assertEquals<int>(13, l.last());
   });

   KAREN_DECL_TEST(iterateFromHead,
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      assertFalse(l.isEmpty());
      assertEquals<int>(10, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(19, l.last());
      Iterator<int> it = l.begin();
      assertTrue(it);
      for (; it; it++)
         assertEquals<int>(j++, *it);
   });

   KAREN_DECL_TEST(contains,
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      assertFalse(l.isEmpty());
      assertEquals<int>(10, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(19, l.last());
      assertTrue(l.hasElement(15));
      assertFalse(l.hasElement(25));
   });

   KAREN_DECL_TEST(removeIterator,
   {
      LinkedList<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.insertBack(i);
            
      assertFalse(l.isEmpty());
      assertEquals<int>(10, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(19, l.last());

      Iterator<int> it = l.begin();
      for (int i = 0; i < 5; i++)
         it++;
      l.remove(it);
      assertEquals(16, *it);
      assertFalse(l.hasElement(15));
   });

   KAREN_DECL_TEST(removeFirst,
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.removeFirst();
      assertFalse(l.isEmpty());
      assertEquals<int>(9, l.size());
      assertEquals<int>(11, l.first());
      assertEquals<int>(19, l.last());
   });
   
   KAREN_DECL_TEST(removeLast,
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.removeLast();
      assertFalse(l.isEmpty());
      assertEquals<int>(9, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(18, l.last());
   });
   
   KAREN_DECL_TEST(removeWithInvalidIterator,
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      
      try
      {
         Iterator<int> it;
         l.remove(it);
         assertionFailed("expected exception not raised");
      }
      catch (InvalidInputException&) {}
      try
      {
         LinkedList<int> ll;
         Iterator<int> it = ll.begin();
         l.remove(it);
         assertionFailed("expected exception not raised");
      }
      catch (InvalidInputException&) {}
   });
   
   KAREN_DECL_TEST(clearNotEmpty,
   {
      LinkedList<int> l;
      for (int i = 10; i < 20; i++)
         l.insertBack(i);
      l.clear();
      assertTrue(l.isEmpty());
      assertEquals<int>(0, l.size());
   });
   
   KAREN_DECL_TEST(clearEmpty,
   {
      LinkedList<int> l;
      l.clear();
      assertTrue(l.isEmpty());
      assertEquals<int>(0, l.size());
   });
   
   KAREN_DECL_TEST(insertBefore,
   {
      LinkedList<int> l;
      l.insertBack(10);
      l.insertBack(11);
      l.insertBack(12);
      l.insertBack(14);
      Iterator<int> it = l.begin();
      ++it; ++it; ++it;
      l.insertBefore(13, it);
      assertFalse(l.isEmpty());
      assertEquals<int>(5, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(14, l.last());
      assertEquals<int>(14, *it);
      int j = 10;
      it = l.begin();
      assertTrue(it);
      for (; it; it++)
         assertEquals<int>(j++, *it);
   });

   KAREN_DECL_TEST(insertAfter,
   {
      LinkedList<int> l;
      l.insertBack(10);
      l.insertBack(12);
      l.insertBack(13);
      l.insertBack(14);
      Iterator<int> it = l.begin();
      l.insertAfter(11, it);
      assertFalse(l.isEmpty());
      assertEquals<int>(5, l.size());
      assertEquals<int>(10, l.first());
      assertEquals<int>(14, l.last());
      assertEquals(10, *it);
      int j = 10;
      it = l.begin();
      assertTrue(it);
      for (; it; ++it)
         assertEquals<int>(j++, *it);
   });
   
   KAREN_DECL_TEST(iterateForRange,
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
         assertEquals<int>(values[i++], num);
      }
      assertEquals<int>(4, i);
      
   });
   
KAREN_END_UNIT_TEST(ListTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   ListTestSuite suite;
   suite.run(&rep, NULL, 0);
}
