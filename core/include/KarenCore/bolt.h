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

#ifndef KAREN_CORE_BOLT_H
#define KAREN_CORE_BOLT_H

#include "KarenCore/exception.h"

namespace karen {

/**
 * Boolean bolt condition type. This struct provides a valid type to be used
 * as condition for Bolt template class. It wraps a boolean value, considering
 * the condition as met when the value evalutes to true.
 */
struct BooleanBoltCondition
{
   const bool* value;
   
   inline BooleanBoltCondition(const bool* value) : value(value) {}
   
   inline bool operator()() const { return *value; }

};

/**
 * Bolt template class. This template class provides a mechanism for
 * avoiding the write access to a wrapped object when an specific condition
 * is met. The condition is represented by the condition object passed to
 * the constructor. The templated Condition type must be callable with no
 * arguments. When such a condition is met, the wrapped object may be
 * obtained by invoking the dereference operator on the bolt. If that is
 * done while condition is not met, a InvalidStateException is thrown instead.
 */
template <class T, class Condition = BooleanBoltCondition>
class Bolt
{
public:

   /**
    * Create a new bolt object wrapping given object with given condition.
    */
   inline Bolt(T* t, const Condition& condition)
      : _t(t), _condition(condition) {}
   
   /**
    * Dereference operator. It returns the wrapped object when condition is
    * met. Otherwise, a InvalidStateException is thrown.
    */
   T* operator -> () throw (InvalidStateException)
   {
      if (_condition())
         KAREN_THROW(InvalidStateException,
            "cannot access object wrapped by bolt: the bolt is locked");
      return _t;
   }
   
   /**
    * Deference operator. This dereferences the bolt regardless the condition
    * is met or not.
    */
   const T* operator -> () const { return _t; }

private:

   T* _t;
   Condition _condition;

};

}; // namespace karen

#endif
