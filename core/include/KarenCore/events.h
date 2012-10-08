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

#include "KarenCore/list.h"
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

class Subscriber
{
public:

   virtual ~Subscriber() {}

   virtual void invoke(const Event&) = 0;
};

KAREN_EXPORT_TEMPLATE(Ptr<Subscriber>);
KAREN_EXPORT_TEMPLATE(LinkedList<Ptr<Subscriber>>);

class KAREN_EXPORT LocalEventChannel
{
public:

   LocalEventChannel();

   virtual ~LocalEventChannel();
   
   template <typename T, typename E>
   void subscribe(T* target, void (T::* callback) (const E&));
   
#ifdef KAREN_CXX11_HAVE_LAMBDA_FUNCTIONS
   template <typename E>
   void subscribe(std::function<void(const E&)> lambda);
#endif

protected:

   virtual void sendEvent(const Event& ev);

private:

   typedef LinkedList<Ptr<Subscriber>> EventSubscriberList;
   
   EventSubscriberList _eventSubscribers;
};

#define KAREN_DECL_EVENT(name, ...) \
   class name : public ::karen::Event {\
   public:\
      __VA_ARGS__; \
   };   

}

#include "KarenCore/events-inl.h"

#endif
