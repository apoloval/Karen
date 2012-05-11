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

const UInt32 KeyEvent::LEFT_CURSOR_KEY       = 0xf702;
const UInt32 KeyEvent::RIGHT_CURSOR_KEY      = 0xf703;
const UInt32 KeyEvent::UP_CURSOR_KEY         = 0xf700;
const UInt32 KeyEvent::DOWN_CURSOR_KEY       = 0xf701;
const UInt32 KeyEvent::BACKSPACE_KEY         = 0x0008;
const UInt32 KeyEvent::DELETE_KEY            = 0x007f;
const UInt32 KeyEvent::TAB_KEY               = 0x0009;
const UInt32 KeyEvent::RETURN_KEY            = 0x000d;
const UInt32 KeyEvent::SCAPE_KEY             = 0x001b;
const UInt32 KeyEvent::INSERT_KEY            = 0x0115;
const UInt32 KeyEvent::HOME_KEY              = 0x0116;
const UInt32 KeyEvent::END_KEY               = 0x0117;
const UInt32 KeyEvent::PAGEUP_KEY            = 0x0118;
const UInt32 KeyEvent::PAGEDOWN_KEY          = 0x0119;
const UInt32 KeyEvent::F1_KEY                = 0x011a;
const UInt32 KeyEvent::F2_KEY                = 0x011b;
const UInt32 KeyEvent::F3_KEY                = 0x011c;
const UInt32 KeyEvent::F4_KEY                = 0x011d;
const UInt32 KeyEvent::F5_KEY                = 0x011e;
const UInt32 KeyEvent::F6_KEY                = 0x011f;
const UInt32 KeyEvent::F7_KEY                = 0x0120;
const UInt32 KeyEvent::F8_KEY                = 0x0121;
const UInt32 KeyEvent::F9_KEY                = 0x0122;
const UInt32 KeyEvent::F10_KEY               = 0x0123;
const UInt32 KeyEvent::F11_KEY               = 0x0124;
const UInt32 KeyEvent::F12_KEY               = 0x0125;
const UInt32 KeyEvent::F13_KEY               = 0x0126;
const UInt32 KeyEvent::F14_KEY               = 0x0127;
const UInt32 KeyEvent::F15_KEY               = 0x0128;

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
      if (_consumers.hasElement(consumer))
         KAREN_THROW(utils::InvalidInputException, 
                     "cannot add input event consumer: already registered");
      _consumers.insertBack(consumer);
   }

   virtual void removeEventConsumer(EventConsumer* consumer)
   throw (utils::NotFoundException)
   {
      if (!_consumers.hasElement(consumer))
         KAREN_THROW(utils::NotFoundException, 
                     "cannot remove consumer: not found");
      _consumers.removeAll(consumer);
   }

private:

   utils::LinkedList<EventConsumer*> _consumers;

};

Ptr<EventChannel>
EventChannel::newInstance()
{ return new EventChannelImpl(); }

void
EventResponder::respondToMouseButtonPressed(
      const Vector& pressedAt,
      MouseButton btn)
{
   EventResponder* next = nextResponder();
   if (next)
      next->respondToMouseButtonPressed(pressedAt, btn);
}

void
EventResponder::respondToMouseButtonReleased(
      const Vector& pressedAt,
      MouseButton btn)
{
   EventResponder* next = nextResponder();
   if (next)
      next->respondToMouseButtonReleased(pressedAt, btn);
}

void
EventResponder::respondToMouseMoved(
      const Vector& fromPos,
      const Vector& toPos)
{
   EventResponder* next = nextResponder();
   if (next)
      next->respondToMouseMoved(fromPos, toPos);
}

void
EventResponder::respondToMouseDragged(
      const Vector& fromPos,
      const Vector& toPos)
{
   EventResponder* next = nextResponder();
   if (next)
      next->respondToMouseDragged(fromPos, toPos);
}

}}; /* Namespace karen::ui */
