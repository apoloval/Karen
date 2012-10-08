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

#ifndef KAREN_CORE_QUEUE_INL_H
#define KAREN_CORE_QUEUE_INL_H

#include "KarenCore/queue.h"

namespace karen {

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
{ this->_backend.template removeAll<Backend>(t, eq); }

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
Iterator<const T>
PriorityQueue<T, Compare, Backend>::rbegin()
{ return _backend.rbegin(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::rend()
{ return _backend.rend(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::rbegin() const
{ return _backend.rbegin(); }
   
template <class T, class Compare, class Backend>
Iterator<const T>
PriorityQueue<T, Compare, Backend>::rend() const
{ return _backend.rend(); }
   
template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::remove(Iterator<const T>& it)
{ return _backend.remove(it); }
   
template <class T, class Compare, class Backend>
const T&
PriorityQueue<T, Compare, Backend>::head() const
{ return *_backend.rbegin(); }

template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::put(const T& t)
{ _backend.insert(t); }
   
template <class T, class Compare, class Backend>
T
PriorityQueue<T, Compare, Backend>::poll()
{
   T t = head();
   Iterator<const T> it = _backend.rbegin();
   _backend.remove(it);
   return t;
}

template <class T, class Compare, class Backend>
void
PriorityQueue<T, Compare, Backend>::removeAll(const T& t)
{ this->_backend.removeAll(t); }

}

#endif
