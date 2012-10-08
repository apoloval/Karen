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

#ifndef KAREN_CORE_ARRAY_INL_H
#define KAREN_CORE_ARRAY_INL_H

#include "KarenCore/array.h"

namespace karen {

template <class T>
const T&
Array<T>::operator[] (unsigned long pos) const
throw (OutOfBoundsException)
{ return this->get(pos); }

template <class T>
T&
Array<T>::operator[] (unsigned long pos)
throw (OutOfBoundsException)
{ return this->get(pos); }

template <class T>
DynArray<T>::DynArray() : _impl(new std::vector<T>()) {}

template <class T>
DynArray<T>::DynArray(unsigned long size) : _impl(new std::vector<T>(size)) {}

template <class T>
DynArray<T>::DynArray(const T* tv, unsigned long len)
   : _impl(new std::vector<T>())
{ for (unsigned long i = 0; i < len; i++) append(tv[i]); }

template <class T>
DynArray<T>::~DynArray()
{ delete _impl; }

template <class T>
unsigned long
DynArray<T>::size() const
{ return _impl->size(); }

template <class T>
void
DynArray<T>::clear()
{ return _impl->clear(); }

template <class T>
Iterator<T>
DynArray<T>::begin()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayIterator(*this, _impl->begin(), _impl->end());
   return Iterator<T>(it);
}

template <class T>
Iterator<T>
DynArray<T>::end()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayIterator(*this, _impl->end(), _impl->end());
   return Iterator<T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::begin() const
{ 
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayIterator(*this, _impl->begin(), _impl->end());
   return Iterator<const T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::end() const
{
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayIterator(*this, _impl->end(), _impl->end());
   return Iterator<const T>(it);
}

template <class T>
Iterator<T>
DynArray<T>::rbegin()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayReverseIterator(*this, _impl->rbegin(), _impl->rend());
   return Iterator<T>(it);
}

template <class T>
Iterator<T>
DynArray<T>::rend()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayReverseIterator(*this, _impl->rend(), _impl->rend());
   return Iterator<T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::rbegin() const
{ 
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayReverseIterator(*this, _impl->rbegin(), _impl->rend());
   return Iterator<const T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::rend() const
{
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayReverseIterator(*this, _impl->rend(), _impl->rend());
   return Iterator<const T>(it);
}

template <class T>
void
DynArray<T>::remove(Iterator<T>& it)
{
   DynArrayIterator* nit = it.template impl<DynArrayIterator>();
   DynArrayReverseIterator* nrit = it.template impl<DynArrayReverseIterator>();
   if (nit && (nit->collection() == this))
   {
      Ptr<AbstractIterator<T>> newIt = 
         new DynArrayIterator(*this, _impl->erase(nit->impl()), _impl->end());
      it = newIt;
   }
   else if (nrit && (nrit->collection() == this))
   {
      typename _Impl::reverse_iterator newItImpl =
         typename _Impl::reverse_iterator(_impl->erase(--(nrit->impl().base())));
      Ptr<AbstractIterator<T>> newIt = new DynArrayReverseIterator(
            *this, newItImpl, _impl->rend());
      it = newIt;
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from dynamic array from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T>
const T&
DynArray<T>::get(unsigned long pos) const 
throw (OutOfBoundsException)
{
   return const_cast<DynArray<T>*>(this)->get(pos);
}

template <class T>
T&
DynArray<T>::get(unsigned long pos)
throw (OutOfBoundsException)
{
   if (pos < _impl->size())
      return _impl->at(pos);
   else
      KAREN_THROW(OutOfBoundsException, 
         "cannot obtain element at position %d: "
         "no such position for target dynamic array", pos);
}

template <class T>
void
DynArray<T>::resize(unsigned long size)
{ _impl->resize(size); }

template <class T>
void
DynArray<T>::set(const T& t, unsigned long pos) 
throw (OutOfBoundsException)
{
   if (pos < _impl->size())
      (*_impl)[pos] = t;
   else
      KAREN_THROW(OutOfBoundsException, 
         "cannot set element at position %d: "
         "no such position for target dynamic array", pos);
}

template <class T>
void
DynArray<T>::append(const T& t)
{ _impl->push_back(t); }

}

#endif   
