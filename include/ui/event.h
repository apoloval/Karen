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

#ifndef KAREN_UI_EVENT_H
#define KAREN_UI_EVENT_H

#include "ui/types.h"
#include "utils/exception.h"
#include "utils/platform.h"
#include "utils/pointer.h"

namespace karen { namespace ui {

/**
 * Input event type. This type enumerates the different types of input
 * events available in Karen.
 */
enum InputEventType
{
   MOUSE_MOTION_INPUT_EVENT,
   MOUSE_BUTTON_INPUT_EVENT,
   KEY_PRESSED_INPUT_EVENT,
   KEY_RELEASED_INPUT_EVENT,
   CUSTOM_INPUT_EVENT,
};

/**
 * Mouse motion input event. This struct encapsulates the properties
 * of a mouse motion input event.
 */
struct MouseMotionInputEvent
{
   int   fromX;
   int   fromY;
   int   toX;
   int   toY;
   int   relX;
   int   relY;
};

/**
 * Mouse button input event. This struct encapsulates the properties
 * of a mouse button input event.
 */
struct MouseButtonInputEvent
{
   int   posX;
   int   posY;
   UInt8 button;
};

/**
 * Custom event type. This struct encapsulates the properties of a custom
 * input event. It attempts to encapsulate some generic values that may
 * be useful for representing any custom event.
 */
struct CustomInputEvent
{
   int         type;
   long        longVal1;
   long        longVal2;
   double      doubleVal1;
   double      doubleVal2;
   void*       ptr1;
   void*       ptr2;
   UInt8       data1[32];
   UInt8       data2[32];
};

/**
 * Input event type. This type encapsulates the properties of an input
 * event. It is based on union objects to easily discriminate the event
 * type and obtain access to its data.
 */
struct InputEvent
{
   InputEventType type;
   union
   {
      MouseMotionInputEvent mouseMotion;
      MouseButtonInputEvent mouseButton;
   };
};

/**
 * Input event producer class. This class provides an interface for an
 * object capable of producing input events.
 */
class KAREN_EXPORT InputEventProducer
{
public:

   /**
    * Produce an input event. If no event is available, blocking indicates 
    * whether it should block until a new event is available. Otherwise, the
    * invocation returns immediately. The returned value indicates if an
    * event has been produced or not. 
    * 
    * @param ev An InputEvent object to be filled with the properties
    *        of produced event.
    * @param blocking indicates whether caller should block until a new
    *        event is available. 
    * @return a boolean value that indicates whether an event has been
    *         produced. For blocking mode, it is allways true.
    */
   virtual bool produceInputEvent(InputEvent *ev, bool blocking = true) = 0;

};

/**
 * Input event consumer class. This class provides an interface for an
 * object capable of consuming input events.
 */
class KAREN_EXPORT InputEventConsumer
{
public:

   /**
    * Consume an input event.
    */
   virtual void consumeInputEvent(const InputEvent& ev) = 0;

};

/**
 * Input event channel. This class provides a channel to communicate
 * input events. It implements the InputEventConsumer interface and allow
 * the registration of InputEventConsumer objects. Any event pushed into
 * this event channel is transmited to all registered consumers. 
 */
class KAREN_EXPORT InputEventChannel : public InputEventConsumer
{
public:

   /**
    * Create a new instance of this class. 
    */
   static Ptr<InputEventChannel> newInstance();

   /**
    * Consume an input event, sending it to all registered consumers. 
    */
   virtual void consumeInputEvent(const InputEvent& ev) = 0;
   
   /**
    * Add a new consumer to this channel. It throws a InvalidInputException 
    * if consumer was already registered.
    */
   virtual void addInputEventConsumer(InputEventConsumer* consumer)
         throw (utils::InvalidInputException) = 0;

   /**
    * Remove a previously registered consumer. If consumer was not registerd
    * it throws a NotFoundException.
    */
   virtual void removeInputEventConsumer(InputEventConsumer* consumer)
         throw (utils::NotFoundException) = 0;


};


}}; /* Namespace karen::ui */

#endif
