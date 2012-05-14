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

#ifndef KAREN_UTILS_COLLECTIONS_H
#define KAREN_UTILS_COLLECTIONS_H

#include "utils/exception.h"
#include "utils/iterator.h"
#include "utils/pointer.h"
#include "utils/types.h"

#include <vector>
#include <list>
#include <set>
#include <map>

namespace karen { namespace utils {

/**
 * Default less than class. This class provides a default implementation
 * for comparing two elements and determine which is less than the other.
 */
template <class T>
class DefaultLessThan
{
public:

   inline bool operator() (const T& lhs, const T& rhs) const;

};

template <class T>
class DefaultEquals
{
public:

   inline bool operator() (const T& lhs, const T& rhs) const;

};

/**
 * Collection class. This abstract template class provides the interface
 * for any object that behaves as a collection. 
 */
template <class T>
class Collection
{
public:

   /**
    * Virtual destructor. 
    */
   inline virtual ~Collection() {}
   
   /**
    * Obtain the number of elements contained in this collection.
    */
   virtual unsigned long size() const = 0;

   /**
    * Check whether this collection is empty. The collection class provides
    * a default implementation for this method that considers that a 
    * collection with 0 elements is empty. This method may be overriden 
    * by child classes to provide a more specific behavior.
    */
   inline virtual bool isEmpty() const
   { return size() == 0; }

   /**
    * Check whether this collection has the given element. This performs
    * an iteration on the collection to find the element.
    */
   template <class Equals = DefaultEquals<T> >
   inline bool hasElement(const T& t, Equals eq = Equals()) const;
   
   /**
    * Clear the collection by removing all its elements.
    */
   virtual void clear() = 0;
   
   /**
    * Remove the element pointed by given iterator. The iterator is updated
    * to point to the element that followed the removed one. 
    */
   virtual void remove(ConstIterator<T>& it) = 0;
   
   /**
    * Obtain a const iterator to the beginning of the collection.
    */
   virtual ConstIterator<T> begin() const = 0;
   
   /**
    * Obtain a const iterator to the end of the collection (actually beyond it).
    */
   virtual ConstIterator<T> end() const = 0;
   
};


template <class T>
class SequentialCollection : public Collection<T>
{
public:

   /**
    * Obtain an iterator to the beginning of the collection.
    */
   virtual Iterator<T> begin() = 0;
   
   /**
    * Obtain an iterator to the end of the collection (actually beyond it).
    */
   virtual Iterator<T> end() = 0;
   
};

template <class T>
class AssociativeCollection : public Collection<T>
{
public:

};

template <class T, class Backend>
class CollectionAdaptor : public Collection<T>
{
public:

protected:

   Backend _backend;
};

/**
 * Array template class. This interface class provides an abstraction for
 * a collection that behaves as an array, i.e. an sequential collection of
 * elements with random access for getting and inserting. This is NOT an
 * instantiable class but an interface aimed to be implemented by concrete
 * implementations.
 */
template <class T>
class Array : public SequentialCollection<T>
{
public:

   /**
    * Subscript operator. Obtains the element located an pos, or throw a 
    * OutOfBoundsException if pos is not valid. 
    */
   inline const T& operator[] (unsigned long pos) const
         throw (OutOfBoundsException);

   /**
    * Subscript operator. Obtains the element located an pos, or throw a 
    * OutOfBoundsException if pos is not valid. 
    */
   inline T& operator[] (unsigned long pos)
         throw (OutOfBoundsException);

   /**
    * Obtain the element located in given position. If there is no such
    * position, a OutOfBoundsException is thrown.
    */
   virtual const T& get(unsigned long pos) const 
         throw (OutOfBoundsException) = 0;

   /**
    * Obtain the element located in given position. If there is no such
    * position, a OutOfBoundsException is thrown.
    */
   virtual T& get(unsigned long pos)
         throw (OutOfBoundsException) = 0;

   /**
    * Resize the array to allocate space for size elements. If the array
    * already have space for the given size, nothing is done.
    */
   virtual void resize(unsigned long size) = 0;
   
   /**
    * Set the element placed at given position. If there is no such position,
    * a OutOfBoundsException is thrown.
    */
   virtual void set(const T& t, unsigned long pos) 
         throw (OutOfBoundsException) = 0;

   /**
    * Append the given element at the end of the array.
    */
   virtual void append(const T& t) = 0;

};

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
   template <class Equals = DefaultEquals<T> >
   inline void removeAll(const T& t, Equals eq = Equals());

};

/**
 * Set template class. This template class provides the interface for a
 * collection that behaves as a set.
 */
template <class T>
class Set : public AssociativeCollection<T>
{
public:

   /**
    * Inserts a new element in the set.
    */
   virtual void insert(const T& t) = 0;
   
   /**
    * Removes all ocurrences of given element (if any). 
    */
   virtual void removeAll(const T& t) = 0;
   
};

/**
 * Map template class. This template class provides the interface for
 * a collection that behaves as a map or dictionary. 
 */
template <class K, class T>
class Map : public AssociativeCollection<Tuple<K, T> >
{
public:

   /**
    * Array subscript operator. This returns the value for given key or
    * throws a NotFoundException is such key is undefined. 
    */
   inline const T& operator[] (const K& k) const throw (NotFoundException);

   /**
    * Array subscript operator. This returns the value for given key if exists
    * or creates a new value and returns it otherwise.
    */
   inline T& operator[] (const K& k);

   /**
    * Check whether this map has any entry with given key.
    */
   virtual bool hasKey(const K& k) const = 0;

   /**
    * Put a new element in the map with given key. 
    */
   virtual void put(const K& k, const T& t) = 0;
   
   /**
    * Put a new element in the map with given key from its wrapping tuple.
    */
   inline void put(const Tuple<K, T>& value);

   /**
    * Retrieve an element by its key, or throw a NotFoundException if 
    * there is no such key defined in the map.
    */
   virtual const T& get(const K& k) const throw (NotFoundException) = 0;
   
   /**
    * Retrieve an element by its key, or throw a NotFoundException if 
    * there is no such key defined in the map.
    */
   virtual T& get(const K& k) throw (NotFoundException) = 0;
   
   /**
    * Remove an elemenet by its key. If there is no entry with such key,
    * it has no effect. 
    */
   virtual void remove(const K& k) = 0;
   
};




template <class T, 
          class CollectionClass, 
          class ImplementationClass,
          class IteratorClass = typename ImplementationClass::iterator>
class IteratorImpl : public AbstractIterator<T>,
                     public AbstractConstIterator<T>
{
public:

   inline IteratorImpl(const CollectionClass& col,
                       const typename ImplementationClass::iterator& impl,
                       const typename ImplementationClass::iterator& end);
   
   inline virtual bool isNull() const;
   
   inline virtual IteratorImpl* clone() const;

   inline IteratorClass& impl();
   
   inline const CollectionClass* collection();
   
private:

   mutable const CollectionClass*   _collection;
   mutable IteratorClass            _impl;
   mutable IteratorClass            _end;

   inline virtual void nextAfterNullCheck();
   
   inline virtual void prevAfterNullCheck();
   
   inline virtual const T& getAfterNullCheck() const;
   
   inline virtual T& getAfterNullCheck();
   
   inline Ptr<AbstractConstIterator<T> > toConstIterator();

};

template <class T>
class DynArray : public Array<T>
{
public:

   /**
    * Create a new dynamic array with an initial size of 0. 
    */
   inline DynArray();
   
   /**
    * Create a new dynamic array with a given initial size.
    */
   inline DynArray(unsigned long size);
   
   /**
    * Create a new dynamic array from a raw C-array.
    */
   inline DynArray(const T* tv, unsigned long len);
   
   inline virtual unsigned long size() const;

   inline void clear();

   inline virtual Iterator<T> begin();
   
   inline virtual Iterator<T> end();
   
   inline virtual ConstIterator<T> begin() const;
   
   inline virtual ConstIterator<T> end() const;

   inline void remove(ConstIterator<T>& it);

   inline virtual const T& get(unsigned long pos) const 
      throw (OutOfBoundsException);

   inline virtual T& get(unsigned long pos)
         throw (OutOfBoundsException);

   inline virtual void resize(unsigned long size);
   
   inline virtual void set(const T& t, unsigned long pos) 
         throw (OutOfBoundsException);

   virtual void append(const T& t);

private:

   typedef IteratorImpl<T, DynArray, std::vector<T> > DynArrayIterator;

   mutable std::vector<T> _impl;
   
};

template <class T>
class LinkedList : public List<T>
{
public:

   inline virtual unsigned long size() const;
   
   inline void clear();

   inline virtual Iterator<T> begin();
   
   inline virtual Iterator<T> end();
   
   inline virtual ConstIterator<T> begin() const;
   
   inline virtual ConstIterator<T> end() const;
   
   inline void remove(ConstIterator<T>& it);

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

   typedef IteratorImpl<T, LinkedList, std::list<T>> LinkedListIterator;
   
   mutable std::list<T> _impl;
};

template <class T, class Compare = DefaultLessThan<T> >
class TreeSet : public Set<T>
{
public:

   inline TreeSet(const Compare& cmp = Compare());

   inline virtual unsigned long size() const;
   
   inline void clear();

   inline virtual ConstIterator<T> begin() const;
   
   inline virtual ConstIterator<T> end() const;

   inline void remove(ConstIterator<T>& it);

   inline void insert(const T& t);
   
   inline void removeAll(const T& t);

private:

   typedef IteratorImpl<T, TreeSet, std::set<T, Compare> > TreeSetIterator;
   
   mutable std::set<T, Compare> _impl;
   
};

template <class K, class T, class Compare = DefaultLessThan<K> >
class TreeMap : public Map<K, T>
{
public:

   inline TreeMap(const Compare& cmp = Compare());
   
   inline TreeMap(const Tuple<K, T>* elems, 
                  unsigned long nelems,
                  const Compare& cmp = Compare());

   inline virtual unsigned long size() const;
   
   inline virtual void clear();
   
   inline virtual void remove(ConstIterator<Tuple<K, T> >& it);
   
   inline virtual ConstIterator<Tuple<K, T> > begin() const;
   
   inline virtual ConstIterator<Tuple<K, T> > end() const;

   inline virtual bool hasKey(const K& k) const;

   inline virtual void put(const K& k, const T& t);
   
   inline virtual const T& get(const K& k) const throw (NotFoundException);
   
   inline virtual T& get(const K& k) throw (NotFoundException);
   
   inline virtual void remove(const K& k);

private:

private:

   struct KeyCompare
   {
      Compare cmp;
      
      inline KeyCompare(const Compare& c = Compare()) : cmp(c) {}
   
      inline bool operator () (const Tuple<K, T>& lhs, const Tuple<K, T>& rhs) const
      { return cmp(lhs.first(), rhs.first()); }
   };

   typedef IteratorImpl<Tuple<K, T>, TreeMap, 
                        std::set<Tuple<K, T>, KeyCompare> > TreeMapIterator;
   
   mutable std::set<Tuple<K, T>, KeyCompare> _impl;

};

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
   template <class Equals = DefaultEquals<T> >
   inline void removeAll(const T& t, Equals eq = Equals());

};

template <class T, 
          class Compare = DefaultLessThan<T>, 
          class Backend = TreeSet<T, Compare> >
class PriorityQueue : public CollectionAdaptor<T, Backend>
{
public:

   inline PriorityQueue(const Compare& cmp = Compare());

   inline virtual unsigned long size() const;
   
   inline void clear();

   inline virtual ConstIterator<T> begin() const;
   
   inline virtual ConstIterator<T> end() const;

   inline void remove(ConstIterator<T>& it);

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

}} /* namespace karen::utils */

#include "utils/collection-inl.h"

#endif
