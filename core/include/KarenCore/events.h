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

#ifndef KAREN_CORE_EVENTS_H
#define KAREN_CORE_EVENTS_H

#include "KarenCore/collection.h"
#include "KarenCore/exception.h"
#include "KarenCore/pointer.h"

#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
#include <functional>
#endif

namespace karen {

class KAREN_EXPORT Event
{
public:

	virtual ~Event() {}
};

class KAREN_EXPORT EventChannel
{
public:

	virtual ~EventChannel() {}

   virtual void sendEvent(const Event& event) const throw (IOException) = 0;
};

class KAREN_EXPORT LocalEventChannel
{
public:

   inline virtual ~LocalEventChannel()
   {
      _eventSubscribers.clear();
   }
   
   template <typename T, typename E>
   void subscribe(T* target, void (T::* callback) (const E&))
   {
      auto subscriber = new FunctionMemberEventSubscriber<T, E>(
            target, callback);
      _eventSubscribers.insertBack(subscriber);
   }
   
#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
   template <typename E>
   void subscribe(std::function<void(const E&)> lambda)
   {
      auto subscriber = new LambdaEventSubscriber<E>(lambda);
      _eventSubscribers.insertBack(subscriber);
   }
#endif

protected:

   virtual void sendEvent(const Event& ev)
   {
      for (auto it = _eventSubscribers.begin(),
           end = _eventSubscribers.end(); it != end; it++)
      {
         (*it)->invoke(ev);
      }
   }

private:

   struct Subscriber
   {
      virtual ~Subscriber() {}
      
      virtual void invoke(const Event&) = 0;
   };

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
   
   typedef LinkedList<Ptr<Subscriber>> EventSubscriberList;
   
   EventSubscriberList _eventSubscribers;
};

#define KAREN_DECL_EVENT(name, ...) \
   class name : public ::karen::Event {\
   public:\
      __VA_ARGS__; \
   };   

};

#endif
