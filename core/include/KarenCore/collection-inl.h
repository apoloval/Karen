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

#ifndef KAREN_CORE_COLLECTIONS_INL_H
#define KAREN_CORE_COLLECTIONS_INL_H

#include "KarenCore/collection.h"

namespace karen {

template <class T>
bool
Collection<T>::hasElement(const T& t, const BinaryPredicate<T, T>& eq) const
{
   Iterator<const T> it = this->begin(), end = this->end();
   for (; it != end; it++)
      if (eq(*it, t))
         return true;
   return false;
}

template <class T, class CollectionClass, class IteratorClass>
IteratorImpl<T, CollectionClass, IteratorClass>::IteratorImpl(
      const CollectionClass& col, 
      const IteratorClass& impl,
      const IteratorClass& end)
   : _collection(&col), _impl(impl), _end(end)
{}

template <class T, class CollectionClass, class IteratorClass>
bool
IteratorImpl<T, CollectionClass, IteratorClass>::isNull() const
{ return _impl == _end; }

template <class T, class CollectionClass, class IteratorClass>
IteratorImpl<T, CollectionClass, IteratorClass>*
IteratorImpl<T, CollectionClass, IteratorClass>::clone() const
{ return new IteratorImpl(*_collection, _impl, _end); }
   
template <class T, class CollectionClass, class IteratorClass>
Ptr<AbstractIterator<const T>>
IteratorImpl<T, CollectionClass, IteratorClass>::toConstIterator()
{ return clone(); }
   
template <class T, class CollectionClass, class IteratorClass>
IteratorClass&
IteratorImpl<T, CollectionClass, IteratorClass>::impl()
{ return _impl; }
   
template <class T, class CollectionClass, class IteratorClass>
const CollectionClass*
IteratorImpl<T, CollectionClass, IteratorClass>::collection()
{ return _collection; }
   
template <class T, class CollectionClass, class IteratorClass>
void
IteratorImpl<T, CollectionClass, IteratorClass>::nextAfterNullCheck()
{ _impl++; }   
   
template <class T, class CollectionClass, class IteratorClass>
void
IteratorImpl<T, CollectionClass, IteratorClass>::prevAfterNullCheck()
{ _impl--; }   
   
template <class T, class CollectionClass, class IteratorClass>
const T&
IteratorImpl<T, CollectionClass, IteratorClass>::getConstAfterNullCheck()
{ return *_impl; }   
   
template <class T, class CollectionClass, class IteratorClass>
T&
IteratorImpl<T, CollectionClass, IteratorClass>::getNonConstAfterNullCheck()
{ return const_cast<T&>(*_impl); }   

template <typename T, typename CollectionType, typename StdIteratorType>
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::StdIteratorAdaptorBase(
      const CollectionType& collection,
      const StdIteratorType& iterator,
      const StdIteratorType& end)
   : _collection(&collection), _stdIterator(iterator), _stdEnd(end)
{}

template <typename T, typename CollectionType, typename StdIteratorType>
bool
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::isNull() const
{ return _stdIterator == _stdEnd; }

template <typename T, typename CollectionType, typename StdIteratorType>
StdIteratorType&
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::impl()
{ return _stdIterator; }
   
template <typename T, typename CollectionType, typename StdIteratorType>
const CollectionType*
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::collection()
{ return _collection; }
   
template <typename T, typename CollectionType, typename StdIteratorType>
void
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::nextAfterNullCheck()
{ _stdIterator++; }   
   
template <typename T, typename CollectionType, typename StdIteratorType>
void
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::prevAfterNullCheck()
{ _stdIterator--; }   

template <typename T, typename CollectionType, typename StdIteratorType>
T&
StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>::getAfterNullCheck()
{ return *_stdIterator; }   

template <typename T, typename CollectionType, typename StdIteratorType>
ConstStdIteratorAdaptor<T, CollectionType, StdIteratorType>::ConstStdIteratorAdaptor(
      const CollectionType& collection,
      const StdIteratorType& iterator,
      const StdIteratorType& end)
   : StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>(
         collection, iterator, end)
{}

template <typename T, typename CollectionType, typename StdIteratorType>
ConstStdIteratorAdaptor<T, CollectionType, StdIteratorType>*
ConstStdIteratorAdaptor<T, CollectionType, StdIteratorType>::clone() const
{ return new ConstStdIteratorAdaptor(*_collection, _stdIterator, _stdEnd); }
   
template <typename T, typename CollectionType, typename StdIteratorType>
Ptr<AbstractIterator<const T>>
ConstStdIteratorAdaptor<T, CollectionType, StdIteratorType>::toConstIterator()
{ return clone(); }
   
template <typename T, typename CollectionType, typename StdIteratorType>
StdIteratorAdaptor<T, CollectionType, StdIteratorType>::StdIteratorAdaptor(
      const CollectionType& collection,
      const StdIteratorType& iterator,
      const StdIteratorType& end)
   : StdIteratorAdaptorBase<T, CollectionType, StdIteratorType>(
         collection, iterator, end)
{}

template <typename T, typename CollectionType, typename StdIteratorType>
StdIteratorAdaptor<T, CollectionType, StdIteratorType>*
StdIteratorAdaptor<T, CollectionType, StdIteratorType>::clone() const
{ return new StdIteratorAdaptor(*_collection, _stdIterator, _stdEnd); }
   
template <typename T, typename CollectionType, typename StdIteratorType>
Ptr<AbstractIterator<const T>>
StdIteratorAdaptor<T, CollectionType, StdIteratorType>::toConstIterator()
{
   return new ConstStdIteratorAdaptor<T, CollectionType, StdIteratorType>(
         _collection, _stdIterator, _stdEnd);
}
   
}; // namespace karen

#endif
