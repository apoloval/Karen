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

#ifndef KAREN_UTILS_COLLECTIONS_INL_H
#define KAREN_UTILS_COLLECTIONS_INL_H

#include "utils/collection.h"

namespace karen { namespace utils {

template <class T>
bool
DefaultLessThan<T>::operator()(const T &lhs, const T &rhs) const
{ return lhs < rhs; }

template <class T>
bool
DefaultEquals<T>::operator() (const T& lhs, const T& rhs) const
{ return lhs == rhs; }

template <class T>
template <class Equals>
bool
Collection<T>::hasElement(const T& t, Equals eq) const
{
   Iterator<const T> it = this->begin(), end = this->end();
   for (; it != end; it++)
      if (eq(*it, t))
         return true;
   return false;
}

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
template <class Equals>
void
List<T>::removeAll(const T& t, Equals eq)
{
   Iterator<T> it = this->begin(), end = this->end();
   for (; it != end; it++)
      if (eq(*it, t))
         this->remove(it);
}

template <class K, class T>
const T&
Map<K, T>::operator[] (const K& k) const
throw (NotFoundException)
{
}

template <class K, class T>
T&
Map<K, T>::operator[] (const K& k)
{
   if (!this->hasKey(k))
      this->put(k, T());
   return this->get(k);
}

template <class K, class T>
void
Map<K, T>::put(const Tuple<const K, T>& value)
{ this->put(value.first(), value.second()); }

template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::IteratorImpl(
      const CollectionClass& col,
      const typename ImplementationClass::iterator& impl,
      const typename ImplementationClass::iterator& end)
 : _collection(&col), _impl(impl), _end(end)
{
}
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
bool
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::isNull() const
{ return _impl == _end; }

template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>*
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::clone() const
{ return new IteratorImpl(*_collection, _impl, _end); }
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
Ptr<AbstractIterator<const T>>
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::toConstIterator()
{ return clone(); }
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
IteratorClass&
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::impl()
{ return _impl; }
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
const CollectionClass*
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::collection()
{ return _collection; }
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
void
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::nextAfterNullCheck()
{ _impl++; }   
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
void
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::prevAfterNullCheck()
{ _impl--; }   
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
const T&
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::getAfterNullCheck() const
{ return *_impl; }   
   
template <class T, class CollectionClass, 
          class ImplementationClass, class IteratorClass>
T&
IteratorImpl<T, CollectionClass, ImplementationClass, IteratorClass>::getAfterNullCheck()
{ return const_cast<T&>(*_impl); }   

template <class T>
DynArray<T>::DynArray() : _impl() {}

template <class T>
DynArray<T>::DynArray(unsigned long size) : _impl(size) {}

template <class T>
DynArray<T>::DynArray(const T* tv, unsigned long len) : _impl()
{ for (unsigned long i = 0; i < len; i++) append(tv[i]); }

template <class T>
unsigned long
DynArray<T>::size() const
{ return _impl.size(); }

template <class T>
void
DynArray<T>::clear()
{ return _impl.clear(); }

template <class T>
Iterator<T>
DynArray<T>::begin()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayIterator(*this, _impl.begin(), _impl.end());
   return Iterator<T>(it);
}

template <class T>
Iterator<T>
DynArray<T>::end()
{ 
   Ptr<AbstractIterator<T> > it = 
      new DynArrayIterator(*this, _impl.end(), _impl.end());
   return Iterator<T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::begin() const
{ 
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayIterator(*this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}

template <class T>
Iterator<const T>
DynArray<T>::end() const
{
   Ptr<AbstractIterator<const T> > it = 
      new DynArrayIterator(*this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}

template <class T>
void
DynArray<T>::remove(Iterator<T>& it)
{
   DynArrayIterator* nit = it.template impl<DynArrayIterator>();
   if (nit && (nit->collection() == this))
   {
      Ptr<AbstractIterator<T>> newIt = 
         new DynArrayIterator(*this, _impl.erase(nit->impl()), _impl.end());
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
   if (pos < _impl.size())
      return _impl.at(pos);
   else
      KAREN_THROW(OutOfBoundsException, 
         "cannot obtain element at position %d: "
         "no such position for target dynamic array", pos);
}

template <class T>
void
DynArray<T>::resize(unsigned long size)
{ _impl.resize(size); }

template <class T>
void
DynArray<T>::set(const T& t, unsigned long pos) 
throw (OutOfBoundsException)
{
   if (pos < _impl.size())
      _impl[pos] = t;
   else
      KAREN_THROW(OutOfBoundsException, 
         "cannot set element at position %d: "
         "no such position for target dynamic array", pos);
}

template <class T>
void
DynArray<T>::append(const T& t)
{ _impl.push_back(t); }

template <class T>
unsigned long
LinkedList<T>::size() const
{ return _impl.size(); }
   
template <class T>
void
LinkedList<T>::clear()
{ _impl.clear(); }

template <class T>
Iterator<T>
LinkedList<T>::begin()
{
   Ptr<AbstractIterator<T> > it = new LinkedListIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<T>
LinkedList<T>::end()
{
   Ptr<AbstractIterator<T> > it = new LinkedListIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::begin() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T>
Iterator<const T>
LinkedList<T>::end() const
{
   Ptr<AbstractIterator<const T> > it = new LinkedListIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T>
void
LinkedList<T>::remove(Iterator<T>& it)
{
   LinkedListIterator *nit = it.template impl<LinkedListIterator>();
   if (nit && (nit->collection() == this))
   {
      Ptr<AbstractIterator<T>> newIt = 
         new LinkedListIterator(*this, _impl.erase(nit->impl()), _impl.end());
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
   typename std::list<T>::const_iterator it = _impl.begin(), end = _impl.end();
   if (it == end)
      KAREN_THROW(NotFoundException, 
         "cannot fetch first element of linked list: list is empty");
   return *it;   
}

template <class T>
const T&
LinkedList<T>::last() const
throw (NotFoundException)
{
   typename std::list<T>::const_reverse_iterator it = _impl.rbegin(), end = _impl.rend();
   if (it == end)
      KAREN_THROW(NotFoundException, 
         "cannot fetch last element of linked list: list is empty");
   return *it;   
}
   
template <class T>
void
LinkedList<T>::insertFront(const T& t)
{ _impl.push_front(t); }
   
template <class T>
void
LinkedList<T>::insertBack(const T& t)
{ _impl.push_back(t); }

template <class T>
void
LinkedList<T>::insertBefore(const T& t, Iterator<T>& it)
throw (InvalidInputException)
{
   LinkedListIterator *nit = it.template impl<LinkedListIterator>();
   if (nit && (nit->collection() == this))   
      _impl.insert(nit->impl(), t);
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
      _impl.insert(nit->impl(), t);
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
      _impl.pop_front();
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
      _impl.pop_back();
   else
      KAREN_THROW(NotFoundException, 
         "cannot remove last element of linked list: list is empty");
}

template <class T, class Compare>
TreeSet<T, Compare>::TreeSet(const Compare& cmp)
 : _impl(cmp)
{
}

template <class T, class Compare>
unsigned long
TreeSet<T, Compare>::size() const
{ return _impl.size(); }
   
template <class T, class Compare>
void
TreeSet<T, Compare>::clear()
{ _impl.clear(); }

template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::begin() const
{
   Ptr<AbstractIterator<const T> > it = new TreeSetIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::end() const
{
   Ptr<AbstractIterator<const T> > it = new TreeSetIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::begin()
{
   Ptr<AbstractIterator<const T> > it = new TreeSetIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::end()
{
   Ptr<AbstractIterator<const T> > it = new TreeSetIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
void
TreeSet<T, Compare>::remove(Iterator<const T>& it)
{
   Iterator<const T> itCopy = it;
   TreeSetIterator *nit = itCopy.template impl<TreeSetIterator>();
   if (nit && (nit->collection() == this))   
   {
      it++;
      _impl.erase(nit->impl());
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from tree set from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T, class Compare>
void
TreeSet<T, Compare>::insert(const T& t)
{ _impl.insert(t); }
   
template <class T, class Compare>
void
TreeSet<T, Compare>::removeAll(const T& t)
{ _impl.erase(t); }

template <class K, class T, class Compare>
TreeMap<K, T, Compare>::TreeMap(const Compare& cmp)
 : _impl(KeyCompare(cmp))
{}

template <class K, class T, class Compare>
TreeMap<K, T, Compare>::TreeMap(
      const Tuple<const K, T>* elems, 
      unsigned long nelems, 
      const Compare& cmp)
 : _impl(KeyCompare(cmp))
{
   for (unsigned long i = 0; i < nelems; i++)
      Map<K, T>::put(elems[i]);
}

template <class K, class T, class Compare>
unsigned long
TreeMap<K, T, Compare>::size() const
{ return _impl.size(); }

template <class K, class T, class Compare>
void
TreeMap<K, T, Compare>::clear()
{ _impl.clear(); }

template <class K, class T, class Compare>
void
TreeMap<K, T, Compare>::remove(Iterator<Tuple<const K, T> >& it)
{
   Iterator<Tuple<const K, T>> itCopy = it;
   TreeMapIterator* nit = itCopy.template impl<TreeMapIterator>();
   if (nit && (nit->collection() == this))
   {
      it++;
      _impl.erase(nit->impl());
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from tree map from given iterator:"
            " the iterator does not belongs to this collection");
}

template <class K, class T, class Compare>
Iterator<Tuple<const K, T> >
TreeMap<K, T, Compare>::begin()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl.begin(), _impl.end());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T, class Compare>
Iterator<Tuple<const K, T> >
TreeMap<K, T, Compare>::end()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl.end(), _impl.end());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T, class Compare>
Iterator<const Tuple<const K, T> >
TreeMap<K, T, Compare>::begin() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl.begin(), _impl.end());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T, class Compare>
Iterator<const Tuple<const K, T> >
TreeMap<K, T, Compare>::end() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl.end(), _impl.end());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T, class Compare>
bool
TreeMap<K, T, Compare>::hasKey(const K& k) const
{}

template <class K, class T, class Compare>
void
TreeMap<K, T, Compare>::put(const K& k, const T& t)
{}

template <class K, class T, class Compare>
const T&
TreeMap<K, T, Compare>::get(const K& k) const
throw (NotFoundException)
{}

template <class K, class T, class Compare>
T&
TreeMap<K, T, Compare>::get(const K& k)
throw (NotFoundException)
{}

template <class K, class T, class Compare>
void
TreeMap<K, T, Compare>::remove(const K& k)
{}

template <class T, class Backend>
const T&
Queue<T, Backend>::head() const
throw (NotFoundException)
{ return this->_backend.first(); }

template <class T, class Backend>
void
Queue<T, Backend>::put(const T& t)
{ this->_backend.insertLast(t); }
   
template <class T, class Backend>
T
Queue<T, Backend>::poll()
throw (NotFoundException)
{
   T t = this->_backend.first();
   this->_backend.removeFirst();
   return t;
}

template <class T, class Backend>
template <class Equals>
void
Queue<T, Backend>::removeAll(const T& t, Equals eq)
{ this->_backend.removeAll<Backend>(t, eq); }

template <class T, class Compare, class Backend>
PriorityQueue<T, Compare, Backend>::PriorityQueue(const Compare& cmp)
 : _backend(cmp)
{}

template <class T, class Compare, class Backend>
unsigned long
PriorityQueue<T, Compare, Backend>::size() const
{ return _backend.size(); }
   
template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::clear()
{ _backend.clear(); }

template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::begin()
{ return _backend.begin(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::end()
{ return _backend.end(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::begin() const
{ return _backend.begin(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::end() const
{ return _backend.end(); }
   
template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::remove(Iterator<const T>& it)
{ return _backend.remove(it); }
   
template <class T, class Compare, class Backend>
const T&
PriorityQueue<T, Compare, Backend>::head() const
{ return *_backend.begin(); }

template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::put(const T& t)
{ _backend.insert(t); }
   
template <class T, class Compare, class Backend>
T
PriorityQueue<T, Compare, Backend>::poll()
{
   T t = head();
   Iterator<const T> it = _backend.begin();
   _backend.remove(it);
   return t;
}

template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::removeAll(const T& t)
{ this->_backend.removeAll(t); }

}}; // namespace karen::utils

#endif
