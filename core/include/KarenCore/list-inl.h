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

#ifndef KAREN_CORE_LIST_INL_H
#define KAREN_CORE_LIST_INL_H

#include "KarenCore/collection.h"

namespace karen {

template <class T>
template <class Equals>
void
List<T>::removeAll(const T& t, Equals eq)
{
   Iterator<T> it = this->begin(), end = this->end();
   for (; it != end; it++)
      if (eq(*it, t))
         this->remove(it);
}

template <class T>
LinkedList<T>::LinkedList() : _impl(new std::list<T>()) {}

template <class T>
LinkedList<T>::~LinkedList()
{ delete _impl; }

template <class T>
unsigned long
LinkedList<T>::size() const
{ return _impl->size(); }
   
template <class T>
void
LinkedList<T>::clear()
{ _impl->clear(); }

template <class T>
Iterator<T>
LinkedList<T>::begin()
{
   Ptr<AbstractIterator<T> > it = new LinkedListIterator(
         *this, _impl->begin(), _impl->end());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<T>
LinkedList<T>::end()
{
   Ptr<AbstractIterator<T> > it = new LinkedListIterator(
         *this, _impl->end(), _impl->end());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::begin() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListIterator(
         *this, _impl->begin(), _impl->end());
   return Iterator<const T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::end() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListIterator(
         *this, _impl->end(), _impl->end());
   return Iterator<const T>(it);
}
   
template <class T>
Iterator<T>
LinkedList<T>::rbegin()
{
   Ptr<AbstractIterator<T> > it = new LinkedListReverseIterator(
         *this, _impl->rbegin(), _impl->rend());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<T>
LinkedList<T>::rend()
{
   Ptr<AbstractIterator<T> > it = new LinkedListReverseIterator(
         *this, _impl->rend(), _impl->rend());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::rbegin() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListReverseIterator(
         *this, _impl->rbegin(), _impl->rend());
   return Iterator<const T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::rend() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListReverseIterator(
         *this, _impl->rend(), _impl->rend());
   return Iterator<const T>(it);
}
   
template <class T>
void
LinkedList<T>::remove(Iterator<T>& it)
{
   LinkedListIterator *nit = it.template impl<LinkedListIterator>();
   LinkedListReverseIterator *nrit = it.template impl<LinkedListReverseIterator>();
   if (nit && (nit->collection() == this))
   {
      Ptr<AbstractIterator<T>> newIt = 
         new LinkedListIterator(*this, _impl->erase(nit->impl()), _impl->end());
      it = newIt;
   }
   else if (nrit && (nrit->collection() == this))
   {
      typename _Impl::reverse_iterator newItImpl =
         typename _Impl::reverse_iterator(
               _impl->erase(--(nrit->impl().base())));
      Ptr<AbstractIterator<T>> newIt = new LinkedListReverseIterator(
            *this, newItImpl, _impl->rend());
      it = newIt;
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from linked list from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T>
const T&
LinkedList<T>::first() const 
throw (NotFoundException)
{ 
   if (_impl->size() > 0)
      return _impl->front();
   else
      KAREN_THROW(NotFoundException, 
         "cannot fetch first element of linked list: list is empty");
}

template <class T>
const T&
LinkedList<T>::last() const
throw (NotFoundException)
{
   if (_impl->size() > 0)
      return _impl->back();
   else
      KAREN_THROW(NotFoundException, 
         "cannot fetch last element of linked list: list is empty");
}
   
template <class T>
void
LinkedList<T>::insertFront(const T& t)
{ _impl->push_front(t); }
   
template <class T>
void
LinkedList<T>::insertBack(const T& t)
{ _impl->push_back(t); }

template <class T>
void
LinkedList<T>::insertBefore(const T& t, Iterator<T>& it)
throw (InvalidInputException)
{
   LinkedListIterator *nit = it.template impl<LinkedListIterator>();
   if (nit && (nit->collection() == this))   
      _impl->insert(nit->impl(), t);
   else
      KAREN_THROW(InvalidInputException, 
            "cannot insert element to linked list from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T>
void
LinkedList<T>::insertAfter(const T& t, Iterator<T>& it)
throw (InvalidInputException)
{
   Iterator<T> itc(it);
   itc++;
   LinkedListIterator *nit = itc.template impl<LinkedListIterator>();
   if (nit && (nit->collection() == this))   
      _impl->insert(nit->impl(), t);
   else
      KAREN_THROW(InvalidInputException, 
            "cannot insert element to linked list from given iterator:"
            " the iterator does not belongs to this collection");
}

template <class T>
void
LinkedList<T>::removeFirst()
throw (NotFoundException)
{
   if (size() > 0)
      _impl->pop_front();
   else
      KAREN_THROW(NotFoundException, 
         "cannot remove first element of linked list: list is empty");
}

template <class T>
void
LinkedList<T>::removeLast()
throw (NotFoundException)
{
   if (size() > 0)
      _impl->pop_back();
   else
      KAREN_THROW(NotFoundException, 
         "cannot remove last element of linked list: list is empty");
}

}

#endif
