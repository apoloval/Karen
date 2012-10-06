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

#ifndef KAREN_CORE_POINTER_INL_H
#define KAREN_CORE_POINTER_INL_H

#include "KarenCore/pointer.h"

namespace karen {

template <typename T>
Ptr<T>::Ptr() : _refc(NULL), _obj(NULL) {}

template <typename T>
Ptr<T>::Ptr(T *value) : _refc(value ? new RefCounter(1) : NULL), _obj(value) {}

template <typename T>
Ptr<T>::Ptr(const Ptr &p) { copy(p); }

template <typename T>
Ptr<T>::Ptr(Ptr&& p) { move(p); }

template <typename T>
template <class Other>
Ptr<T>::Ptr(const Ptr<Other> &p) { copy(p); }

template <typename T>
Ptr<T>::~Ptr() { release(); }

template <typename T>
Ptr<T>&
Ptr<T>::operator = (const Ptr &p)
{ release(); copy(p); return *this; }

template <typename T>
template <class Other>
Ptr<T>&
Ptr<T>::operator = (const Ptr<Other> &p)
{ release(); copy(p); return *this; }

template <typename T>
template <class Other>
Ptr<T>&
Ptr<T>::operator = (Ptr<Other>&& p)
{ release(); move(p); return *this; }

template <typename T>
template <class Other>
bool
Ptr<T>::operator == (const Ptr<Other> &p) const
{ return this->_refc == p._refc; }

template <typename T>
Ptr<T>::operator T* () const { return _obj; }

template <typename T>
Ptr<T>::operator bool () const
{ return isNotNull(); }

template <typename T>
bool
Ptr<T>::isNull() const
{ return _refc == NULL; }

template <typename T>
bool
Ptr<T>::isNotNull() const
{ return _refc != NULL; }

template <typename T>
unsigned int
Ptr<T>::count() const
{ return _refc ? *_refc : 0; }

template <typename T>
T*
Ptr<T>::operator -> () throw (NullPointerException)
{ return safeObject(); }

template <typename T>
const T*
Ptr<T>::operator -> () const throw (NullPointerException)
{ return safeObject(); }

template <typename T>
T&
Ptr<T>::operator * () throw (NullPointerException)
{ return *safeObject(); }

template <typename T>
const T&
Ptr<T>::operator * () const throw (NullPointerException)
{ return *safeObject(); }

template <typename T>
template <class Other>
bool
Ptr<T>::isOfClass() const throw (NullPointerException)
{ return dynamic_cast<Other*>(safeObject()) != NULL; }

template <typename T>
template <class Other>
Ptr<Other>
Ptr<T>::dynCasting() throw (NullPointerException)
{
   Other *cast = dynamic_cast<Other*>(safeObject());
   Ptr<Other> result;
   if (cast)
      result.uncheckedCopy(*this, cast);
   return result;
}

template <typename T>
template <class Other>
Ptr<const Other>
Ptr<T>::dynCasting() const throw (NullPointerException)
{
   const Other *cast = dynamic_cast<const Other*>(safeObject());
   Ptr<const Other> result;
   if (cast)
      result.uncheckedCopy(*this, cast);
   return result;
}

template <typename T>
void
Ptr<T>::release()
{
   if (_refc)
   {
      (*_refc)--;
      if ((*_refc) == 0)
      {
         if (_obj)
            delete _obj;
         delete _refc;
      }
   }
   _refc = NULL;
   _obj = NULL;
}

template <typename T>
template <class Other>
void
Ptr<T>::copy(const Ptr<Other> &p)
{
   this->_refc = p._refc;
   this->_obj  = p._obj;
   if (this->_refc)
      (*this->_refc)++;
}

template <typename T>
template <class Other>
void
Ptr<T>::move(Ptr<Other>& p)
{
   this->_refc  = p._refc;
   this->_obj   = p._obj;
   p._refc      = NULL;
   p._obj       = NULL;
}

template <typename T>
template <class Other, class Obj>
void
Ptr<T>::uncheckedCopy(const Ptr<Other> &p, Obj *obj)
{
   this->_refc = p._refc;
   this->_obj  = obj;
   if (this->_refc)
      (*this->_refc)++;
}

template <typename T>
T*
Ptr<T>::safeObject() throw (NullPointerException)
{
   if (!_obj)
      KAREN_THROW(NullPointerException,
                  "attempt to dereference a null pointer");
   return _obj;
}

template <typename T>
const T*
Ptr<T>::safeObject() const throw (NullPointerException)
{ return ((Ptr*) this)->safeObject(); }

}

#endif
