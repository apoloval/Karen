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

#ifndef KAREN_UI_ENGINE_H
#define KAREN_UI_ENGINE_H

#include "ui/draw.h"
#include "ui/event.h"
#include "ui/timer.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui {

/**
 * UI engine class. This abstract class defines the interface that any
 * UI engine shall implement. Its operations may be used to initialize
 * the engine and gain access to the components that comprise the engine.
 */
class KAREN_EXPORT Engine
{
public:

   /**
    * Initialize the UI engine from implementation name passed as argument.
    *
    * @param engineName The name of the engine to be initialized. 
    * @return An instance of Engine if initialization is successfull. 
    * @throw InvalidInputException if given engine name is unknown.
    * @throw InvalidStateException if UI engine is already initialized.
    */
   static Engine& init(const String& engineName)
         throw (utils::InvalidInputException, utils::InvalidStateException);
   
   /**
    * Obtain the initialized instance of UI engine. If engine is still
    * uninitialized, InvalidStateException is raised. 
    */
   static Engine& instance()
         throw (utils::InvalidStateException);

   /**
    * Virtual destructor.
    */
   inline virtual ~Engine() {}
         
   /**
    * Obtain engine name.
    */
   const String& name() const { return _name; }
   
   /**
    * Obtain the input event channel for this engine.
    */
   inline EventChannel& eventChannel()
   { return *_eventChannel; }

   /**
    * Obtain the drawing context for this engine. If the screen has not
    * been initialized yet, a InvalidStateException is raised. 
    */
   virtual DrawingContext& drawingContext()
         throw (utils::InvalidStateException) = 0;
         
   /**
    * Obtain the engine timer.
    */
   virtual Timer& timer() = 0;

   /**
    * Run the engine main loop. The main loop iterates processing input
    * events and drawing into screen until the engine is requested to stop.    
    */
   virtual void runLoop() = 0;
   
   /**
    * Stop the engine main loop. This causes the main loop to end and return
    * the control to its caller. 
    */
   virtual void stopLoop() = 0;

protected:

   /**
    * Protected constructor.
    */
   Engine(const String& name);
   
private:

   String _name;
   Ptr<EventChannel> _eventChannel;

};

}}; /* Namespace karen::ui */

#endif
