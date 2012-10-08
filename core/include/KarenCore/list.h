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

#ifndef KAREN_CORE_LIST_H
#define KAREN_CORE_LIST_H

#include <list>
 
#include "KarenCore/collection.h"

namespace karen {

/**
 * List template class. This template class provides the interface for a
 * collection that heaves as a list. 
 */
template <class T>
class List : public SequentialCollection<T>
{
public:

   /**
    * Get the first element of the list, or thow a NotFoundException if
    * the list is empty.
    */
   virtual const T& first() const throw (NotFoundException) = 0;

   /**
    * Get the last element of the list, or thow a NotFoundException if
    * the list is empty.
    */
   virtual const T& last() const throw (NotFoundException) = 0;
   
   /**
    * Insert a new element at list front.
    */
   virtual void insertFront(const T& t) = 0;
   
   /**
    * Insert a new element at list back. 
    */
   virtual void insertBack(const T& t) = 0;
   
   /**
    * Insert a new element before that one pointed by given iterator.
    * It throws a InvalidInputException if given iterator is not valid.
    */
   virtual void insertBefore(const T& t, Iterator<T>& it)
      throw (InvalidInputException) = 0;
   
   /**
    * Insert a new element after that one pointed by given iterator.
    * It throws a InvalidInputException if given iterator is not valid.
    */
   virtual void insertAfter(const T& t, Iterator<T>& it)
      throw (InvalidInputException) = 0;
   
   /**
    * Remove the first element of the list, or throw a NotFoundException
    * if the list is empty.
    */
   virtual void removeFirst() throw (NotFoundException) = 0;
   
   /**
    * Remove the last element of the list, or throw a NotFoundException
    * if the list is empty.
    */
   virtual void removeLast() throw (NotFoundException) = 0;
   
   /**
    * Remove all ocurrences of given element from the list.
    */
   template <class Equals>
   inline void removeAll(const T& t, Equals eq = Equals());

};

template <class T>
class LinkedList : public List<T>
{
public:

   inline LinkedList();

   inline virtual ~LinkedList();

   inline virtual unsigned long size() const;
   
   inline void clear();

   inline virtual Iterator<T> begin();
   
   inline virtual Iterator<T> end();
   
   inline virtual Iterator<const T> begin() const;
   
   inline virtual Iterator<const T> end() const;
   
   inline virtual Iterator<T> rbegin();
   
   inline virtual Iterator<T> rend();
   
   inline virtual Iterator<const T> rbegin() const;
   
   inline virtual Iterator<const T> rend() const;
   
   inline virtual void remove(Iterator<T>& it);

   inline virtual const T& first() const throw (NotFoundException);

   inline virtual const T& last() const throw (NotFoundException);
   
   inline virtual void insertFront(const T& t);
   
   inline virtual void insertBack(const T& t);

   inline virtual void insertBefore(const T& t, Iterator<T>& it)
      throw (InvalidInputException);
   
   inline virtual void insertAfter(const T& t, Iterator<T>& it)
      throw (InvalidInputException);

   inline virtual void removeFirst() throw (NotFoundException);
   
   inline virtual void removeLast() throw (NotFoundException);

private:

   typedef std::list<T> _Impl;

   typedef IteratorImpl<T, LinkedList, 
         typename _Impl::iterator> LinkedListIterator;
   
   typedef IteratorImpl<T, LinkedList, 
         typename _Impl::reverse_iterator> LinkedListReverseIterator;
   
   mutable _Impl* _impl;
};

}

#include "KarenCore/list-inl.h"

#endif
