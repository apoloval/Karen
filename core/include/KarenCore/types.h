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

#ifndef KAREN_CORE_TYPES_H
#define KAREN_CORE_TYPES_H

#include <utility>

namespace karen {

/**
 * Nullable class. This class allows a generic type to be wrapped in a way
 * that it accepts the null state. The wrapped object may be used as it
 * wasn't wrapped by either using the pointer-accessor operator or using
 * the cast operator. 
 */
template <class T>
class Nullable
{
public:

   /**
    * Create a new nullable object without any wrapped object. This
    * nullable is set to null. 
    */
   Nullable() : _null(true) {}
   
   /**
    * Create a new nullable object with a wrapped object. This nullable
    * object is not null. 
    */
   Nullable(const T &val) : _value(val), _null(false) {}
   
   /**
    * Cast to T operator. 
    */
   operator const T &() const { return _value; }
   
   /**
    * Cast to T operator. 
    */
   operator T &() { return _value; }
   
   /**    
    * Check whether nullable object is null. 
    */
   bool isNull() const { return _null; }
   
   /**
    * Pointer-accessor operator. 
    */
   T *operator ->() { return &_value; }

private:

   T     _value;
   bool  _null;

};

/**
 * Tuple template class. This variadic-template class provides a convenient 
 * wrapper for a n-tuple of elements. 
 */
template <class ... T>
class Tuple;

/**
 * Template specialization for base case. This template-specialization is
 * aimed to provide the base class for any tuple hierarchy made recursive
 * by the variadic template. This is not aimed to be instantiated directly
 * (what makes no sense, who wants a tuple of one element? ).
 */
template <class T1>
class Tuple<T1>
{
public:
   
   inline Tuple<T1>(const T1& elem) : _elem(elem) {}

private:
   
   template <int id, class ... _T>
   friend class Tuple<_T...>::_ElementAccessor;
   
   T1 _elem;
   
};

/*
 * Template specialization for recursive cases. This is the implementation 
 * for tuples with more than one element.
 */
template <class T1, class ... T>
class Tuple<T1, T...> : public Tuple<T...>
{
private:
   
   /*
    * Element accessor template class. This class provides an accessor for 
    * tuple elements. The id parameter indicates the position of the element 
    * to be accessed. 
    */
   template <int id, class ... _T> struct _ElementAccessor;
   
   /*
    * Template specialization for base case of accessing the first element.
    */
   template <class _T1, class ... _T>
   struct _ElementAccessor<0, _T1, _T...>
   {
      /* 
       * The type of the element accessed by this accessor.
       */      
      typedef _T1 type;
      
      /**
       * A pointer to the tuple used to obtain the element value. 
       */
      mutable const Tuple<_T1, _T...>* _tuple;

      /**
       * Creates a new accessor by a reference to the tuple to be accessed.
       */
      inline _ElementAccessor(const Tuple<_T1, _T...>& t) : _tuple(&t) {}
      
      /**
       * Cast operator. It returns the accessed element of the tuple.
       */
      inline operator const _T1&() const { return _tuple->_elem; }

      /**
       * Cast operator. It returns the accessed element of the tuple.
       */
      inline operator _T1&() { return const_cast<_T1&>(_tuple->_elem); }
   };
   
   /*
    * Template specialization for recursive cases.
    */
   template <int id, class _T1, class ... _T>
   struct _ElementAccessor<id, _T1, _T...> : _ElementAccessor<id - 1, _T...>
   {
      /**
       * The type of the element for the n-th (id) recursive case.
       */
      typedef typename _ElementAccessor<id - 1, _T...>::type type;

      /**
       * Creates a new accessor by a reference to the tuple to be accessed.
       */
      inline _ElementAccessor(const Tuple<_T1, _T...>& t) : 
         _ElementAccessor<id - 1, _T...>(t) {}
   };
   
public:
   
   /**
    * Public accessor type. This is a special case of struct. It is used like 
    * a static function of tuple class. It extends the _ElementAccessor 
    * template class to serve as element accessor.
    */
   template <int id> struct getElement : _ElementAccessor<id, T1, T...>
   {
      /**
       * The type of the n-th tuple element determined by id. 
       */
      typedef typename _ElementAccessor<id, T1, T...>::type type;
      
      /**
       * Creates a new accessor for given tuple instance to n-th element 
       * determined by id.
       */
      inline getElement(const Tuple<T1, T...>& t) : 
         _ElementAccessor<id, T1, T...>(t) {}
   };
   
   /**
    * Obtain the n-th element of the tuple detemined by template parameter
    * id. 
    */
   template <int id>
   const typename getElement<id>::type& get() const
   {
      return getElement<id>(*this);
   }
   
   /**
    * Obtain the n-th element of the tuple detemined by template parameter
    * id. 
    */
   template <int id>
   typename getElement<id>::type& get()
   {
      return getElement<id>(*this);
   }
   
   /**
    * Create a new tuple with given elements.
    */
   inline Tuple(const T1 &elem, const T& ... t) : 
      Tuple<T...>(t...), 
      _elem(elem)
   {} 
   
   /**
    * Create a new tuple as a copy of that one passed as argument.
    */
   inline Tuple(const Tuple<T1, T...>& t) : Tuple<T...>(t), _elem(t._elem) {}

private:
      
   template <int id, class ... _T>
   friend class Tuple<_T...>::_ElementAccessor;

   T1 _elem;

};

}; // namespace karen

using karen::Nullable;
using karen::Tuple;

#endif
