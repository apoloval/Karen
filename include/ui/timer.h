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

#ifndef KAREN_UI_TIMER_H
#define KAREN_UI_TIMER_H

#include "utils/exception.h"
#include "utils/types.h"

namespace karen { namespace ui {

/**
 * Timer callback class. This class provides the interface for any object
 * with the ability of receiving timer events.
 */
class TimerCallback
{
public:

   /**
    * Virtual destructor.
    */
   virtual ~TimerCallback() {}
   
   /**
    * Callback member function. This member function is invoked when the
    * callback object is registered into a timer object and elapsed time
    * indicated in registration elapses. This operation returns the new
    * elapse time required for the next invokation. If no more callback
    * calls are needed, a null value shall be returned.
    */
   virtual utils::Nullable<unsigned long> onTimeElapsed(unsigned long ms) = 0;

};

/**
 * Timer class. This class provides the interface of a timer, i.e. an
 * UI utility class able to communicate timed events. 
 */
class Timer
{
public:

   /**
    * Register a new timer callback. This callback would be invoked when
    * indicated time elapses. Once registered, the callback cannot be removed.
    * Instead, the callback may be ignored and it may return a null value
    * to indicate no further invocations.
    */
   virtual void registerCallback(TimerCallback* callback, unsigned long ms)
      throw (utils::InvalidInputException) = 0;

};

}}; // namespace karen::ui

#endif
