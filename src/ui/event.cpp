/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
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

#include "ui/event.h"

#include "utils/collection.h"

namespace karen { namespace ui {

class EventChannelImpl : public EventChannel
{
public:

   virtual void consumeEvent(const Event& ev)
   {
      for (auto c : _consumers)
         c->consumeEvent(ev);
   }

   virtual void addEventConsumer(EventConsumer* consumer)
   throw (utils::InvalidInputException)
   {
      if (_consumers.contains(consumer))
         KAREN_THROW(utils::InvalidInputException, 
                     "cannot add input event consumer: already registered");
      _consumers.toTail(consumer);
   }

   virtual void removeEventConsumer(EventConsumer* consumer)
   throw (utils::NotFoundException)
   {
      if (!_consumers.contains(consumer))
         KAREN_THROW(utils::NotFoundException, 
                     "cannot remove consumer: not found");
      _consumers.remove(consumer);
   }

private:

   List<EventConsumer*> _consumers;

};

Ptr<EventChannel>
EventChannel::newInstance()
{ return new EventChannelImpl(); }

}}; /* Namespace karen::ui */
