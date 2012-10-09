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

#ifndef KAREN_CORE_COLLECTIONS_H
#define KAREN_CORE_COLLECTIONS_H

#include "KarenCore/exception.h"
#include "KarenCore/first-class.h"
#include "KarenCore/iterator.h"
#include "KarenCore/pointer.h"
#include "KarenCore/types.h"

namespace karen {

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
   inline bool hasElement(const T& t,
         const BinaryPredicate<T, T>& eq = Equals<T, T>()) const;
   
   /**
    * Clear the collection by removing all its elements.
    */
   virtual void clear() = 0;
   
   /**
    * Remove the element pointed by given iterator. The iterator is updated
    * to point to the element that followed the removed one. 
    */
   virtual void remove(Iterator<T>& it) = 0;
   
   /**
    * Obtain a const iterator to the beginning of the collection.
    */
   virtual Iterator<const T> begin() const = 0;
   
   /**
    * Obtain a const iterator to the end of the collection (actually beyond it).
    */
   virtual Iterator<const T> end() const = 0;
   
   /**
    * Obtain an iterator to the beginning of the collection.
    */
   virtual Iterator<T> begin() = 0;
   
   /**
    * Obtain an iterator to the end of the collection (actually beyond it).
    */
   virtual Iterator<T> end() = 0;
   
   /**
    * Obtain a reverse const iterator to the beginning of the collection.
    */
   virtual Iterator<const T> rbegin() const = 0;
   
   /**
    * Obtain a reverse const iterator to the end of the collection 
    * (actually beyond it).
    */
   virtual Iterator<const T> rend() const = 0;
   
   /**
    * Obtain a reverse iterator to the beginning of the collection.
    */
   virtual Iterator<T> rbegin() = 0;
   
   /**
    * Obtain a reverse iterator to the end of the collection (actually 
    * beyond it).
    */
   virtual Iterator<T> rend() = 0;
   
};


template <class T>
class SequentialCollection : public Collection<T>
{
public:
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

template <class T, 
          class CollectionClass, 
          class IteratorClass>
class IteratorImpl : public AbstractConstIterator<T>,
                     public AbstractNonConstIterator<T>
{
public:

   inline IteratorImpl(const CollectionClass& col,
                       const IteratorClass& impl,
                       const IteratorClass& end);
   
   inline virtual bool isNull() const;
   
   inline virtual IteratorImpl* clone() const;

   inline virtual Ptr<AbstractIterator<const T>> toConstIterator();

   inline IteratorClass& impl();
   
   inline const CollectionClass* collection();
   
private:

   mutable const CollectionClass*   _collection;
   mutable IteratorClass            _impl;
   mutable IteratorClass            _end;

   inline virtual void nextAfterNullCheck();
   
   inline virtual void prevAfterNullCheck();
   
   inline virtual const T& getConstAfterNullCheck();
   
   inline virtual T& getNonConstAfterNullCheck();
   
};

template <typename T, typename CollectionType, typename StdIteratorType>
class StdIteratorAdaptorBase : public AbstractIterator<T>
{
public:

   inline StdIteratorAdaptorBase(const CollectionType& collection,
                                 const StdIteratorType& iterator,
                                 const StdIteratorType& end);

   inline virtual bool isNull() const;
   
   inline StdIteratorType& impl();
   
   inline const CollectionType* collection();

   inline virtual void nextAfterNullCheck();
   
   inline virtual void prevAfterNullCheck();

   inline virtual T& getAfterNullCheck();

protected:

   mutable const CollectionType* _collection;
   mutable StdIteratorType _stdIterator;
   mutable StdIteratorType _stdEnd;
};

template <typename T, typename CollectionType, typename StdIteratorType>
class ConstStdIteratorAdaptor : 
      public StdIteratorAdaptorBase<const T, CollectionType, StdIteratorType>
{
public:

   inline ConstStdIteratorAdaptor(const CollectionType& collection,
                                  const StdIteratorType& iterator,
                                  const StdIteratorType& end);

   inline virtual ConstStdIteratorAdaptor* clone() const;

   inline virtual Ptr<AbstractIterator<const T>> toConstIterator();
};

template <typename T, typename CollectionType, typename StdIteratorType>
class StdIteratorAdaptor : 
      public StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>
{
public:

   inline StdIteratorAdaptor(const CollectionType& collection,
                             const StdIteratorType& iterator,
                             const StdIteratorType& end);

   inline virtual StdIteratorAdaptor* clone() const;

   inline virtual Ptr<AbstractIterator<const T>> toConstIterator();
};

}; // namespace karen

#include "KarenCore/collection-inl.h"

#endif
