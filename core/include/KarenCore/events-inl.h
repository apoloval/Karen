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

#ifndef KAREN_CORE_EVENTS_INL_H
#define KAREN_CORE_EVENTS_INL_H

namespace karen {

template <typename T, typename E>
struct FunctionMemberEventSubscriber : Subscriber
{
   T* _target;
   void (T::* _callback)(const E&);

   FunctionMemberEventSubscriber(T* target,
                                 void (T::* callback)(const E&)) :
      _target(target), _callback(callback)
   {}

   virtual void invoke(const Event& ev)
   {
      auto narrowed = dynamic_cast<const E*>(&ev);
      if (narrowed)
      {
         (_target->*_callback)(*narrowed);
      }
   }
};

#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS

template <typename E>
struct LambdaEventSubscriber : Subscriber
{
   std::function<void(const E&)> _lambda;

   LambdaEventSubscriber(std::function<void(const E&)>& lambda) :
         _lambda(lambda)
   {}

   virtual void invoke(const Event& ev)
   {
      auto narrowed = dynamic_cast<const E*>(&ev);
      if (narrowed)
      {
         _lambda(*narrowed);
      }
   }
};

#endif

template <typename T, typename E>
void
LocalEventChannel::subscribe(T* target, void (T::* callback) (const E&))
{
   auto subscriber = new FunctionMemberEventSubscriber<T, E>(
         target, callback);
   _eventSubscribers.insertBack(subscriber);
}

#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
template <typename E>
void
LocalEventChannel::subscribe(std::function<void(const E&)> lambda)
{
   auto subscriber = new LambdaEventSubscriber<E>(lambda);
   _eventSubscribers.insertBack(subscriber);
}
#endif


}

#endif
