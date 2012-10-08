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

#ifndef KAREN_CORE_ARRAY_H
#define KAREN_CORE_ARRAY_H

#include <vector>
 
#include "KarenCore/collection.h"

namespace karen {

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

   inline ~DynArray();
   
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

   inline virtual const T& get(unsigned long pos) const 
      throw (OutOfBoundsException);

   inline virtual T& get(unsigned long pos)
         throw (OutOfBoundsException);

   inline virtual void resize(unsigned long size);
   
   inline virtual void set(const T& t, unsigned long pos) 
         throw (OutOfBoundsException);

   virtual void append(const T& t);

private:

   typedef std::vector<T> _Impl;

   typedef IteratorImpl<T, DynArray, 
         typename _Impl::iterator> DynArrayIterator;

   typedef IteratorImpl<T, DynArray, 
         typename _Impl::reverse_iterator> DynArrayReverseIterator;

   mutable _Impl* _impl;
   
};

}

#include "KarenCore/array-inl.h"

#endif
