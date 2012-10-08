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

#ifndef KAREN_CORE_MAP_H
#define KAREN_CORE_MAP_H

#include <map>
#include <set>

#include "KarenCore/collection.h"
#include "KarenCore/pointer.h"

namespace karen {

/**
 * Map template class. This template class provides the interface for
 * a collection that behaves as a map or dictionary. 
 */
template <class K, class T>
class Map : public AssociativeCollection<Tuple<const K, T> >
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
   virtual Iterator<Tuple<const K, T>> put(const K& k, const T& t) = 0;
   
   /**
    * Put a new element in the map with given key from its wrapping tuple.
    */
   inline void put(const Tuple<const K, T>& value);

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

template <class K, class T>
class TreeMap : public Map<K, T>
{
public:

   typedef BinaryPredicate<K, K> KeyComparator;

   inline TreeMap(const Ptr<KeyComparator>& cmp = new LessThan<K, K>());
   
   inline TreeMap(const Tuple<const K, T>* elems, 
                  unsigned long nelems,
                  const Ptr<KeyComparator>& cmp = new LessThan<K, K>());

   inline ~TreeMap();

   inline virtual unsigned long size() const;
   
   inline virtual void clear();
   
   inline virtual void remove(Iterator<Tuple<const K, T> >& it);
   
   inline virtual Iterator<Tuple<const K, T> > begin();
   
   inline virtual Iterator<Tuple<const K, T> > end();

   inline virtual Iterator<const Tuple<const K, T> > begin() const;
   
   inline virtual Iterator<const Tuple<const K, T> > end() const;

   inline virtual Iterator<Tuple<const K, T> > rbegin();
   
   inline virtual Iterator<Tuple<const K, T> > rend();

   inline virtual Iterator<const Tuple<const K, T> > rbegin() const;
   
   inline virtual Iterator<const Tuple<const K, T> > rend() const;

   inline virtual bool hasKey(const K& k) const;

   inline virtual Iterator<Tuple<const K, T>> put(const K& k, const T& t);
   
   inline virtual const T& get(const K& k) const throw (NotFoundException);
   
   inline virtual T& get(const K& k) throw (NotFoundException);
   
   inline virtual void remove(const K& k);

private:

   struct KeyCmp
   {
      Ptr<KeyComparator> cmp;
      
      inline KeyCmp(const Ptr<KeyComparator>& c = new LessThan<K, K>()) 
         : cmp(c) {}
   
      inline bool operator () (const Tuple<const K, T>& lhs, 
                               const Tuple<const K, T>& rhs) const
      { 
         return (*cmp)(lhs.template get<0>(), rhs.template get<0>()); 
      }
   };
   
   typedef std::set<Tuple<const K, T>, KeyCmp> _Impl;

   typedef IteratorImpl<Tuple<const K, T>, TreeMap,
         typename _Impl::iterator> TreeMapIterator;
   
   typedef IteratorImpl<Tuple<const K, T>, TreeMap,
         typename _Impl::reverse_iterator> TreeMapReverseIterator;
   
   mutable _Impl* _impl;

};

}

#include "KarenCore/map-inl.h"

#endif
