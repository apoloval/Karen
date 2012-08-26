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

class PriorityQueueTestSuite : public UnitTestSuite
{
public:

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

   PriorityQueueTestSuite()
      : UnitTestSuite("Collections - Priority Queue")
   {
      KAREN_UTEST_ADD(PriorityQueueTestSuite::createEmptyQueue);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertOneElement);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertSeveralElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::removeElement);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertDuplicatedElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::iterateOnDuplicatedElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::pullDuplicatedElements);
   }
   
   void createEmptyQueue()
   {
      PriorityQueue<int> q;
      KAREN_UTEST_ASSERT(q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 0);
   }
   
   void insertOneElement()
   {
      PriorityQueue<int> q;
      q.put(10);
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 1);
      KAREN_UTEST_ASSERT(q.hasElement(10));
   }
   
   void insertSeveralElements()
   {
      PriorityQueue<int> q;
      q.put(10);
      q.put(4);
      q.put(15);
      q.put(1);
      q.put(3);
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 5);
      KAREN_UTEST_ASSERT(q.hasElement(1));
      KAREN_UTEST_ASSERT(q.hasElement(3));
      KAREN_UTEST_ASSERT(q.hasElement(4));
      KAREN_UTEST_ASSERT(q.hasElement(10));
      KAREN_UTEST_ASSERT(q.hasElement(15));
      KAREN_UTEST_ASSERT(q.head() == 15);
   }
   
   void removeElement()
   {
      PriorityQueue<int> q;
      q.put(10);
      q.put(4);
      q.put(15);
      q.put(1);
      q.put(3);
      q.removeAll(15);
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      KAREN_UTEST_ASSERT(q.hasElement(1));
      KAREN_UTEST_ASSERT(q.hasElement(3));
      KAREN_UTEST_ASSERT(q.hasElement(4));
      KAREN_UTEST_ASSERT(q.hasElement(10));
      KAREN_UTEST_ASSERT(!q.hasElement(15));
      KAREN_UTEST_ASSERT(q.head() == 10);
   }
   
   void insertDuplicatedElements()
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
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 4);
   }
   
   void iterateOnDuplicatedElements()
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
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      Iterator<const Entry> it = q.begin();
      for (int i = 0; i < 4; i++)
      {
         const Entry &e = *it;
         KAREN_UTEST_ASSERT(e.name == ord[i].name);
         it++;
      }
   }
   
   void pullDuplicatedElements()
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
      KAREN_UTEST_ASSERT(!q.isEmpty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      for (int i = 0; i < 4; i++)
      {
         Entry e = q.poll();
         KAREN_UTEST_ASSERT(e.name == ord[i].name);
      }
   }
   
};

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   PriorityQueueTestSuite suite;
   suite.run(&rep, NULL, 0);
}
