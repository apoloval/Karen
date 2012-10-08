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

#ifndef KAREN_CORE_SET_H
#define KAREN_CORE_SET_H

#include "KarenCore/collection.h"

#include <set>

namespace karen {

/**
 * Set template class. This template class provides the interface for a
 * collection that behaves as a set.
 */
template <class T>
class Set : public AssociativeCollection<const T>
{
public:

   /**
    * Inserts a new element in the set. It returns an iterator to the
    * newly inserted element. 
    */
   virtual Iterator<const T> insert(const T& t) = 0;
   
   /**
    * Removes all ocurrences of given element (if any). 
    */
   virtual void removeAll(const T& t) = 0;
   
};

template <class T, class Compare = DefaultLessThan<T> >
class TreeSet : public Set<T>
{
public:

   inline TreeSet(const Compare& cmp = Compare());

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

   inline Iterator<const T> insert(const T& t);
   
   inline void removeAll(const T& t);

private:

   typedef std::set<T, Compare> _Impl;

   typedef IteratorImpl<T, TreeSet, 
         typename _Impl::iterator> TreeSetIterator;
   
   typedef IteratorImpl<T, TreeSet,
         typename _Impl::reverse_iterator> TreeSetReverseIterator;
   
   mutable _Impl _impl;
   
};

template <class T, class Compare = DefaultLessThan<T> >
class TreeMultiset : public Set<T>
{
public:

   inline TreeMultiset(const Compare& cmp = Compare());

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

   inline Iterator<const T> insert(const T& t);
   
   inline void removeAll(const T& t);

private:

   typedef std::multiset<T, Compare> _Impl;

   typedef IteratorImpl<T, TreeMultiset, 
         typename _Impl::iterator> TreeMultisetIterator;
   
   typedef IteratorImpl<T, TreeMultiset,
         typename _Impl::reverse_iterator> TreeMultisetReverseIterator;
   
   mutable _Impl _impl;
   
};

}

#include "KarenCore/set-inl.h"

#endif
