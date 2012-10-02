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

KAREN_BEGIN_UNIT_TEST(PriorityQueueTestSuite);

   struct Entry
   {
      int priority;
      String name;
   };
   
   struct EntryLessThan
   {
      bool operator()(const Entry &e1, const Entry &e2) const
      { return e1.priority < e2.priority; }
   };

   KAREN_DECL_TEST(createEmptyQueue,
   {
      PriorityQueue<int> q;
      assertTrue(q.isEmpty());
      assertEquals<int>(0, q.size());
   });
   
   KAREN_DECL_TEST(insertOneElement,
   {
      PriorityQueue<int> q;
      q.put(10);
      assertFalse(q.isEmpty());
      assertEquals<int>(1, q.size());
      assertTrue(q.hasElement(10));
   });
   
   KAREN_DECL_TEST(insertSeveralElements,
   {
      PriorityQueue<int> q;
      q.put(10);
      q.put(4);
      q.put(15);
      q.put(1);
      q.put(3);
      assertFalse(q.isEmpty());
      assertEquals<int>(5, q.size());
      assertTrue(q.hasElement(1));
      assertTrue(q.hasElement(3));
      assertTrue(q.hasElement(4));
      assertTrue(q.hasElement(10));
      assertTrue(q.hasElement(15));
      assertEquals(15, q.head());
   });
   
   KAREN_DECL_TEST(removeElement,
   {
      PriorityQueue<int> q;
      q.put(10);
      q.put(4);
      q.put(15);
      q.put(1);
      q.put(3);
      q.removeAll(15);
      assertFalse(q.isEmpty());
      assertEquals<int>(4, q.size());
      assertTrue(q.hasElement(1));
      assertTrue(q.hasElement(3));
      assertTrue(q.hasElement(4));
      assertTrue(q.hasElement(10));
      assertFalse(q.hasElement(15));
      assertEquals<int>(10, q.head());
   });
   
   KAREN_DECL_TEST(insertDuplicatedElements,
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      for (int i = 0; i < 4; i++)
         q.put(ent[i]);
      assertFalse(q.isEmpty());
      assertEquals<int>(4, q.size());
   });
   
   KAREN_DECL_TEST(iterateOnDuplicatedElements,
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      Entry ord[] = {
         ent[0],
         ent[2],
         ent[1],
         ent[3],
      };
      for (int i = 0; i < 4; i++)
         q.put(ent[i]);
      assertFalse(q.isEmpty());
      assertEquals<int>(4, q.size());
      Iterator<const Entry> it = q.begin();
      for (int i = 0; i < 4; i++)
      {
         const Entry &e = *it;
         assertEquals<String>(ord[i].name, e.name);
         it++;
      }
   });
   
   KAREN_DECL_TEST(pullDuplicatedElements,
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      Entry ord[] = {
         ent[3],
         ent[1],
         ent[2],
         ent[0],
      };
      for (int i = 0; i < 4; i++)
         q.put(ent[i]);
      assertFalse(q.isEmpty());
      assertEquals<int>(4, q.size());
      for (int i = 0; i < 4; i++)
      {
         Entry e = q.poll();
         assertEquals<String>(ord[i].name, e.name);
      }
   });
   
KAREN_END_UNIT_TEST(PriorityQueueTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   PriorityQueueTestSuite suite;
   suite.run(&rep, NULL, 0);
}
