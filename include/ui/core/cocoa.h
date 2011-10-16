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

#ifndef KAREN_UI_CORE_COCOA_H
#define KAREN_UI_CORE_COCOA_H

#include "utils/platform.h"

/*
 * This header file provides UI engine classes for Cocoa framework. It
 * makes no sense unless we are compiling for OSX.
 */
#if KAREN_PLATFORM == KAREN_PLATFORM_OSX

#include "ui/draw.h"
#include "ui/engine.h"
#include "ui/event.h"
#include "ui/timer.h"
#include "ui/core/gl.h"

/*
 * The C++ classes declared here have pointers to Cocoa objects as
 * attributes. This would cause a compilation error when the header
 * is imported from pure C++ code. To avoid this, the Cocoa classes are
 * masked as they were C++ classes when this header is included from C++
 * code. When included from Objective-C++ code, they must be pre-declared.
 * The following macros do this.
 */
#if defined(__cplusplus) && !defined(__OBJC__)
namespace karen { namespace ui { namespace core {
class NSWindow;
class KarenOpenGLView;
class NSAutoreleasePool;
}}};
#elif defined(__OBJC__)
@class NSWindow;
@class KarenOpenGLView;
@class NSAutoreleasePool;
#endif

namespace karen { namespace ui { namespace core {

/**
 * Cocoa drawing context. This class provides a Cocoa-based implementation
 * of a drawing context.
 */
class KAREN_EXPORT CocoaDrawingContext
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
   virtual void setDrawingTarget(Drawable* target);
   
   /**
    * Mark the drawing context to as needing to be redisplayed.
    */
   virtual void postRedisplay();

private:

   NSWindow*         _window;
   KarenOpenGLView*  _glView;
   Ptr<OpenGLCanvas> _glCanvas;

};

/**
 * Cocoa engine class. This class provides a Cocoa-based implementation of
 * UI engine.
 */
class KAREN_EXPORT CocoaEngine : public Engine
{
public:

   /**
    * The name of Cocoa engine.
    */
   static const String ENGINE_NAME;

   /**
    * Virtual destructor.
    */
   virtual ~CocoaEngine();

   /**
    * Obtain the drawing context for this engine. If the screen has not
    * been initialized yet, a InvalidStateException is raised. 
    */
   virtual DrawingContext& drawingContext()
         throw (utils::InvalidStateException);
         
   /**
    * Obtain the engine timer.
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
   
private:

   friend class Engine;
   
   /**
    * Create a new instance of Cocoa engine.
    */
   CocoaEngine();

   NSAutoreleasePool*         _memPool;
   Ptr<CocoaDrawingContext>   _drawingContext;

};

}}}; /* namespace karen::ui::core */

#endif /* OSX platform */

#endif
