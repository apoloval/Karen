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

#ifndef KAREN_CORE_MAP_INL_H
#define KAREN_CORE_MAP_INL_H

#include "KarenCore/map.h"

namespace karen {

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
{ this->put(value.template get<0>(), value.template get<1>()); }

template <class K, class T>
TreeMap<K, T>::TreeMap(const Ptr<KeyComparator>& cmp)
   : _impl(new _Impl(KeyCmp(cmp)))
{}

template <class K, class T>
TreeMap<K, T>::TreeMap(
      const Tuple<const K, T>* elems, 
      unsigned long nelems, 
      const Ptr<KeyComparator>& cmp)
   : _impl(new _Impl(KeyCmp(cmp)))
{
   for (unsigned long i = 0; i < nelems; i++)
      Map<K, T>::put(elems[i]);
}

template <class K, class T>
TreeMap<K, T>::~TreeMap()
{ delete _impl; }

template <class K, class T>
unsigned long
TreeMap<K, T>::size() const
{ return _impl->size(); }

template <class K, class T>
void
TreeMap<K, T>::clear()
{ _impl->clear(); }

template <class K, class T>
void
TreeMap<K, T>::remove(Iterator<Tuple<const K, T> >& it)
{
   Iterator<Tuple<const K, T>> itCopy = it;
   TreeMapIterator* nit = itCopy.template impl<TreeMapIterator>();
   TreeMapReverseIterator* nrit = itCopy.template impl<TreeMapReverseIterator>();
   if (nit && (nit->collection() == this))
   {
      it++;
      _impl->erase(nit->impl());
   }
   else if (nrit && (nrit->collection() == this))
   {
      it++;
      _impl->erase(--nrit->impl().base());
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from tree map from given iterator:"
            " the iterator does not belongs to this collection");
}

template <class K, class T>
Iterator<Tuple<const K, T> >
TreeMap<K, T>::begin()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl->begin(), _impl->end());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<Tuple<const K, T> >
TreeMap<K, T>::end()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl->end(), _impl->end());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<const Tuple<const K, T> >
TreeMap<K, T>::begin() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl->begin(), _impl->end());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<const Tuple<const K, T> >
TreeMap<K, T>::end() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapIterator(*this, _impl->end(), _impl->end());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<Tuple<const K, T> >
TreeMap<K, T>::rbegin()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapReverseIterator(*this, _impl->rbegin(), _impl->rend());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<Tuple<const K, T> >
TreeMap<K, T>::rend()
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it =
         new TreeMapReverseIterator(*this, _impl->rend(), _impl->rend());
   return Iterator<Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<const Tuple<const K, T> >
TreeMap<K, T>::rbegin() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapReverseIterator(*this, _impl->rbegin(), _impl->rend());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T>
Iterator<const Tuple<const K, T> >
TreeMap<K, T>::rend() const
{
   Ptr<AbstractIterator<const Tuple<const K, T> > > it =
         new TreeMapReverseIterator(*this, _impl->rend(), _impl->rend());
   return Iterator<const Tuple<const K, T> >(it);
}

template <class K, class T>
bool
TreeMap<K, T>::hasKey(const K& k) const
{ return _impl->find(Tuple<const K, T>(k, T())) != _impl->end(); }

template <class K, class T>
Iterator<Tuple<const K, T>>
TreeMap<K, T>::put(const K& k, const T& t)
{
   Ptr<AbstractIterator<Tuple<const K, T> > > it = new TreeMapIterator(
         *this, _impl->insert(Tuple<const K, T>(k, t)).first, _impl->end());
   return it;
}

template <class K, class T>
const T&
TreeMap<K, T>::get(const K& k) const
throw (NotFoundException)
{
   typename std::set<Tuple<const K, T>, KeyCmp>::const_iterator it = 
         _impl->find(Tuple<const K, T>(k, T()));
   if (it != _impl->end())
      return it->template get<1>();
   else
      KAREN_THROW(NotFoundException,
         "cannot find element in tree map with such a key");
}

template <class K, class T>
T&
TreeMap<K, T>::get(const K& k)
throw (NotFoundException)
{
   typename std::set<Tuple<const K, T>, KeyCmp>::iterator it = 
         _impl->find(Tuple<const K, T>(k, T()));
   if (it != _impl->end())
      return const_cast<T&>(it->template get<1>());
   else
      return put(k, T())->template get<1>();
}

template <class K, class T>
void
TreeMap<K, T>::remove(const K& k)
{ _impl->erase(Tuple<const K, T>(k, T())); }

}

#endif
