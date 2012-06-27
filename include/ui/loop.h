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

#ifndef KAREN_UI_LOOP_H
#define KAREN_UI_LOOP_H

#include "ui/event.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui {

/**
 * Event loop class. This class represents a UI application loop. UI 
 * event loop is responsible of iterating in a loop that fetches events from
 * UI input and sends events to the UI output.
 */
class KAREN_EXPORT EventLoop
{
public:

   /**
    * Run the loop until it is requested to stop.
    */
   virtual void run() = 0;

   /**
    * Request to stop the loop. If the loop is not running, a
    * InvalidStateException is raised.
    */
   virtual void stop() throw (InvalidStateException) = 0;
   
   /**
    * Add a new loop observer. If observer already exists, an
    * InvalidInputException is raised.
    */
   virtual void addObserver(LoopObserver* obs) 
         throw (InvalidInputException) = 0;
         
   /**
    * Remove an existing loop observer. If observer is not present, a
    * InvalidInputException is raised.
    */
   virtual void removeObserver(LoopObserver* obs) 
         throw (InvalidInputException) = 0;
         
   /**
    * Add a new input event consumer to receive input events managed
    * by this loop. If consumer is already present, a InvalidInputException
    * is raised. 
    */
   virtual void addInputEventConsumer(InputEventConsumer* consumer)
         throw (InvalidInputException) = 0;
         
   /**
    * Remove an existing input event consumer. If consumer is not present, 
    * a InvalidInputException is raised. 
    */
   virtual void removeInputEventConsumer(InputEventConsumer* consumer)
         throw (InvalidInputException) = 0;
         
   /**
    * Obtain the input event producer for this loop.
    */
   virtual InputEventProducer& inputEventProducer() = 0;
         

};

}}; /* Namespace karen::ui */

#endif
