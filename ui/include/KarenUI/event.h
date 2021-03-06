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

#ifndef KAREN_UI_EVENT_H
#define KAREN_UI_EVENT_H

#include "KarenUI/euclidean.h"
#include <KarenCore/exception.h>
#include <KarenCore/platform.h>
#include <KarenCore/pointer.h>

namespace karen { namespace ui {

/**
 * Input event type. This type enumerates the different types of input
 * events available in Karen.
 */
enum EventType
{
   MOUSE_MOTION_EVENT,
   MOUSE_PRESSED_EVENT,
   MOUSE_RELEASED_EVENT,
   KEY_PRESSED_EVENT,
   KEY_RELEASED_EVENT,
   APPLICATION_QUIT_EVENT,
   CUSTOM_EVENT,
};

/**
 * Mouse motion input event. This struct encapsulates the properties
 * of a mouse motion input event.
 */
struct MouseMotionEvent
{
   int   fromX;
   int   fromY;
   int   toX;
   int   toY;
   int   relX;
   int   relY;
};

/**
 * Mouse button type. This enumeration represents a mouse button.
 */
enum MouseButton
{
   LEFT_MOUSE_BUTTON,
   RIGHT_MOUSE_BUTTON,
   MIDDLE_MOUSE_BUTTON,
   OTHER_MOUSE_BUTTON,
};

/**
 * Mouse button input event. This struct encapsulates the properties
 * of a mouse button input event.
 */
struct MouseButtonEvent
{
   int         posX;
   int         posY;
   MouseButton button;
};

/**
 * Key event. This struct encapsulates the properties of a key event.
 */
struct KeyEvent
{
   UInt32 unicode;

   static const UInt32 LEFT_CURSOR_KEY;
   static const UInt32 RIGHT_CURSOR_KEY;
   static const UInt32 UP_CURSOR_KEY;
   static const UInt32 DOWN_CURSOR_KEY;
   static const UInt32 BACKSPACE_KEY;
   static const UInt32 DELETE_KEY;
   static const UInt32 TAB_KEY;
   static const UInt32 RETURN_KEY;
   static const UInt32 SCAPE_KEY;
   static const UInt32 INSERT_KEY;
   static const UInt32 HOME_KEY;
   static const UInt32 END_KEY;
   static const UInt32 PAGEUP_KEY;
   static const UInt32 PAGEDOWN_KEY;
   static const UInt32 F1_KEY;
   static const UInt32 F2_KEY;
   static const UInt32 F3_KEY;
   static const UInt32 F4_KEY;
   static const UInt32 F5_KEY;
   static const UInt32 F6_KEY;
   static const UInt32 F7_KEY;
   static const UInt32 F8_KEY;
   static const UInt32 F9_KEY;
   static const UInt32 F10_KEY;
   static const UInt32 F11_KEY;
   static const UInt32 F12_KEY;
   static const UInt32 F13_KEY;
   static const UInt32 F14_KEY;
   static const UInt32 F15_KEY;

};

/**
 * Custom event type. This struct encapsulates the properties of a custom
 * input event. It attempts to encapsulate some generic values that may
 * be useful for representing any custom event.
 */
struct CustomEvent
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
struct Event
{
   EventType type;
   union
   {
      MouseMotionEvent mouseMotion;
      MouseButtonEvent mouseButton;
      KeyEvent         key;
   };
   
   /**
    * Obtain the mouse button position as a Karen vector.
    */
   inline IVector mouseButtonPos() const
   { return IVector(mouseButton.posX, mouseButton.posY); }
   
   /**
    * Obtain the mouse motion-from position as a Karen vector.
    */
   inline IVector mouseMotionFromPos() const
   { return IVector(mouseMotion.fromX, mouseMotion.fromY); }
   
   /**
    * Obtain the mouse motion-to position as a Karen vector.
    */
   inline IVector mouseMotionToPos() const
   { return IVector(mouseMotion.toX, mouseMotion.toY); }
   
};

/**
 * Input event consumer class. This class provides an interface for an
 * object capable of consuming input events.
 */
class KAREN_EXPORT EventConsumer
{
public:

   /**
    * Consume an input event.
    */
   virtual void consumeEvent(const Event& ev) = 0;

};

/**
 * Input event channel. This class provides a channel to communicate
 * input events. It implements the EventConsumer interface and allow
 * the registration of EventConsumer objects. Any event pushed into
 * this event channel is transmited to all registered consumers. 
 */
class KAREN_EXPORT EventChannel : public EventConsumer
{
public:

   /**
    * Create a new instance of this class. 
    */
   static Ptr<EventChannel> newInstance();
   
   /**
    * Virtual destructor.
    */
   inline virtual ~EventChannel() {}

   /**
    * Consume an input event, sending it to all registered consumers. 
    */
   virtual void consumeEvent(const Event& ev) = 0;
   
   /**
    * Add a new consumer to this channel. It throws a InvalidInputException 
    * if consumer was already registered.
    */
   virtual void addEventConsumer(EventConsumer* consumer)
         throw (InvalidInputException) = 0;

   /**
    * Remove a previously registered consumer. If consumer was not registerd
    * it throws a NotFoundException.
    */
   virtual void removeEventConsumer(EventConsumer* consumer)
         throw (NotFoundException) = 0;


};

/**
 * Event responder class. This class provides an abstraction of an object
 * able to respond to UI events. The response interface is structured as
 * a chain of responsibility. Each responder should determine whether
 * request a response from the next responder in the chain after it handles
 * it. Typically, one responder corresponds to one widget, and its next
 * responder corresponds to its parent container. 
 */
class EventResponder
{
public:

   /**
    * Create a new event responder and set its next responder the one
    * passed as argument.
    */
   inline EventResponder(EventResponder* next = NULL)
    : _nextResponder(next)
   {
   }   

   /**
    * Respond to a mouse button pressed event. The default implementation
    * considers in ignoring the event and pass it up in the chain.
    */
   virtual void respondToMouseButtonPressed(
         const IVector& pressedAt,
         MouseButton btn);
   
   /**
    * Respond to a mouse button released event. The default implementation
    * considers in ignoring the event and pass it up in the chain.
    */
   virtual void respondToMouseButtonReleased(
         const IVector& pressedAt,
         MouseButton btn);
   
   /**
    * Responde to a mouse moved event. The default implementation
    * considers in ignoring the event and pass it up in the chain.
    */
   virtual void respondToMouseMoved(
         const IVector& fromPos,
         const IVector& toPos);
   
   /**
    * Responde to a mouse dragged event. The default implementation
    * considers in ignoring the event and pass it up in the chain.
    */
   virtual void respondToMouseDragged(
         const IVector& fromPos,
         const IVector& toPos);
   
   /**
    * Obtain the next responder in the chain, or null if there is none.
    */
   inline EventResponder* nextResponder()
   { return _nextResponder; }
   
   /**
    * Set the next responder of this one.
    */
   inline void setNextResponder(EventResponder* next)
   { _nextResponder = next; }
   
private:

   EventResponder* _nextResponder;
};

}}; /* Namespace karen::ui */

#endif
