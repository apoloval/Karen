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

#ifndef KAREN_UI_CORE_GLUT_H
#define KAREN_UI_CORE_GLUT_H

#include "ui/draw.h"
#include "ui/engine.h"
#include "ui/event.h"
#include "ui/timer.h"
#include "ui/core/gl.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui { namespace core {

class GlutDrawingContext : public DrawingContext
{
public:
   
   /**
    * Initialize the screen from given properties. If given properties
    * cannot be satisfied, a InvalidInputException is thrown.
    */
   virtual Canvas& initScreen(const ScreenProperties& screenProps)
         throw (utils::InvalidInputException);

   /**
    * Obtain the screen as a canvas object to execute drawing operations 
    * on it. If screen was not initialize, a InvalidStateException is thrown.
    */
   virtual Canvas& screen() throw (utils::InvalidStateException);
   
   /**
    * Set the object target of drawing operations.
    */
   inline virtual void setDrawingTarget(Drawable* target)
   { _target = target; }
   
   /**
    * Mark the drawing context to as needing to be redisplayed.
    */
   virtual void postRedisplay();

private:

   /*
    * Set GlutEngine as friend to allow the construction of GLUT drawing
    * context objects.
    */
   friend class GlutEngine;

   /**
    * GLUT display handler.
    */
   static void glutDisplayHandler();
   
   /**
    * Private constructor.
    */
   GlutDrawingContext();
   
   static GlutDrawingContext* _activeContext;
   utils::Ptr<OpenGLCanvas> _canvas;
   Drawable* _target;

};

/**
 * GLUT loop interrupted exception. This exception is thrown to indicate
 * the condition when GLUT loop is interrupted. As GLUT specs, the main
 * loop never ends, so any GLUT application ends by calling a exit() syscall.
 * This exception is used as a trick in C++ environment, which consist in
 * throwing it when application needs to be stopped. This is catched from 
 * GlutEngine::runLoop() to finalize the main loop cleanly. 
 */
KAREN_DECL_EXCEPTION(GlutLoopInterruptedException);

/**
 * Glut engine class. This class implements a UI engine supported by
 * GLUT library.
 */
class KAREN_EXPORT GlutEngine : public Engine, public InputEventConsumer
{
public:

   /**
    * Set Engine class as friend to allow the creation of new instances
    * of this class only for Engine.
    */
   friend class Engine;

   /**
    * The name of this engine.
    */
   static const String ENGINE_NAME;

   /**
    * Obtain the GLUT-based drawing context.
    */
   inline virtual DrawingContext& drawingContext()
   throw (utils::InvalidStateException)
   { return *_drawingContext; }
         
   /**
    * Obtain the GLUT-based timer.
    */
   virtual Timer& timer();

   /**
    * Run the engine main loop. The main loop iterates processing input
    * events and drawing into screen until the engine is requested to stop.    
    */
   virtual void runLoop();
   
   /**
    * Stop the engine main loop. This causes the main loop to end and return
    * the control to its caller. 
    */
   virtual void stopLoop();

   /**
    * Consume an input event.
    */
   virtual void consumeInputEvent(const InputEvent& ev);

private:

   /**
    * Create a new instance of GLUT engine.
    */
   GlutEngine();
   
   Ptr<GlutDrawingContext> _drawingContext;
   
};

}}}; /* Namespace karen::ui::core */

#endif
