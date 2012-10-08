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

#ifndef KAREN_CORE_SET_INL_H
#define KAREN_CORE_SET_INL_H

#include "KarenCore/set.h"

namespace karen {


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
Iterator<const T>
TreeSet<T, Compare>::rbegin() const
{
   Ptr<AbstractIterator<const T> > it = new TreeSetReverseIterator(
         *this, _impl.rbegin(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::rend() const
{
   Ptr<AbstractIterator<const T> > it = new TreeSetReverseIterator(
         *this, _impl.rend(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::rbegin()
{
   Ptr<AbstractIterator<const T> > it = new TreeSetReverseIterator(
         *this, _impl.rbegin(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::rend()
{
   Ptr<AbstractIterator<const T> > it = new TreeSetReverseIterator(
         *this, _impl.rend(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
void
TreeSet<T, Compare>::remove(Iterator<const T>& it)
{
   Iterator<const T> itCopy = it;
   TreeSetIterator *nit = itCopy.template impl<TreeSetIterator>();
   TreeSetReverseIterator *nrit = itCopy.template impl<TreeSetReverseIterator>();
   if (nit && (nit->collection() == this))   
   {
      it++;
      _impl.erase(nit->impl());
   }
   else if (nrit && (nrit->collection() == this))
   {
      it++;
      _impl.erase(--nit->impl());
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from tree set from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T, class Compare>
Iterator<const T>
TreeSet<T, Compare>::insert(const T& t)
{
   Ptr<AbstractIterator<const T> > it = new TreeSetIterator(
         *this, _impl.insert(t).first, _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
void
TreeSet<T, Compare>::removeAll(const T& t)
{ _impl.erase(t); }

template <class T, class Compare>
TreeMultiset<T, Compare>::TreeMultiset(const Compare& cmp)
 : _impl(cmp)
{
}

template <class T, class Compare>
unsigned long
TreeMultiset<T, Compare>::size() const
{ return _impl.size(); }
   
template <class T, class Compare>
void
TreeMultiset<T, Compare>::clear()
{ _impl.clear(); }

template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::begin() const
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::end() const
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::begin()
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetIterator(
         *this, _impl.begin(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::end()
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetIterator(
         *this, _impl.end(), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::rbegin() const
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetReverseIterator(
         *this, _impl.rbegin(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::rend() const
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetReverseIterator(
         *this, _impl.rend(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::rbegin()
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetReverseIterator(
         *this, _impl.rbegin(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::rend()
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetReverseIterator(
         *this, _impl.rend(), _impl.rend());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
void
TreeMultiset<T, Compare>::remove(Iterator<const T>& it)
{
   Iterator<const T> itCopy = it;
   TreeMultisetIterator *nit = 
         itCopy.template impl<TreeMultisetIterator>();
   TreeMultisetReverseIterator *nrit = 
         itCopy.template impl<TreeMultisetReverseIterator>();
   if (nit && (nit->collection() == this))   
   {
      it++;
      _impl.erase(nit->impl());
   }
   else if (nrit && (nrit->collection() == this))   
   {
      it++;
      _impl.erase(--nrit->impl().base());
   }
   else
      KAREN_THROW(InvalidInputException, 
            "cannot remove element from tree set from given iterator:"
            " the iterator does not belongs to this collection");
}
   
template <class T, class Compare>
Iterator<const T>
TreeMultiset<T, Compare>::insert(const T& t)
{
   Ptr<AbstractIterator<const T> > it = new TreeMultisetIterator(
         *this, _impl.insert(t), _impl.end());
   return Iterator<const T>(it);
}
   
template <class T, class Compare>
void
TreeMultiset<T, Compare>::removeAll(const T& t)
{ _impl.erase(t); }


}

#endif
