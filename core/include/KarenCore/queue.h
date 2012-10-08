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

#ifndef KAREN_CORE_QUEUE_H
#define KAREN_CORE_QUEUE_H

#include "KarenCore/collection.h"
#include "KarenCore/list.h"

namespace karen {

/**
 * Queue template class. This template class provides the interface for
 * a collection that behaves as a queue.
 */
template <class T, class Backend = LinkedList<T> >
class Queue : public CollectionAdaptor<T, Backend>
{
public:

   /**
    * Obtain the head element of the queue, or throw a NotFoundException
    * if the queue is empty. 
    */
   inline virtual const T& head() const throw (NotFoundException);

   /**
    * Puts a new element in the queue.
    */
   inline virtual void put(const T& t);
   
   /**
    * Retrieves the next element of the queue and removes it, or throw a 
    * NotFoundException if the queue is empty. 
    */
   inline virtual T poll() throw (NotFoundException);

   /**
    * Remove all ocurrences of given element from the queue.
    */
   template <class Equals>
   inline void removeAll(const T& t, Equals eq = Equals());

};

template <class T, 
          class Compare = DefaultLessThan<T>, 
          class Backend = TreeMultiset<T, Compare> >
class PriorityQueue : public CollectionAdaptor<const T, Backend>
{
public:

   inline PriorityQueue(const Compare& cmp = Compare());

   inline virtual unsigned long size() const;
   
   inline void clear();

   inline virtual Iterator<const T> begin();
   
   inline virtual Iterator<const T> end();

   inline virtual Iterator<const T> begin() const;
   
   inline virtual Iterator<const T> end() const;

   inline virtual Iterator<const T> rbegin();
   
   inline virtual Iterator<const T> rend();

   inline virtual Iterator<const T> rbegin() const;
   
   inline virtual Iterator<const T> rend() const;

   inline void remove(Iterator<const T>& it);

   inline virtual const T& head() const;

   inline virtual void put(const T& t);
   
   inline virtual T poll();

   /**
    * Remove all ocurrences of given element from the queue.
    */
   inline void removeAll(const T& t);

private:

   Backend _backend;

};

}

#include "KarenCore/queue-inl.h"

#endif
