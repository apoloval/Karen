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

#include "ui/timer.h"
#include "ui/core/gl.h"
#include "ui/core/glut.h"
#include "utils/collection.h"
#include "utils/timing.h"

#include <GLUT/GLUT.h>

namespace karen { namespace ui { namespace core {

class GlutTimer : public Timer
{
public:

   inline virtual void registerCallback(
         TimerCallback* callback, 
         double ms)
   throw (utils::InvalidInputException) 
   {
      for (auto entry : _callbacks)
      {
         if (entry.cb == callback)
            KAREN_THROW(utils::InvalidInputException, 
                        "cannot register timer callback: already registered");
      }
      TimerInfo info = { callback, ms, utils::getTimeSinceLaunched() };
      _callbacks.push(info);
      glutTimerFunc(ms, glutHandler, 0);
   }
   
   inline static GlutTimer& instance()
   {
      if (!_instance)
         _instance = new GlutTimer();
      return *_instance;
   }

private:

   inline GlutTimer()
   {
   }

   struct TimerInfo
   {
      TimerCallback* cb;
      unsigned long  ms;
      double         timestamp;      
   };
   
   struct TimerInfoLessThan
   {
      inline bool operator () (const TimerInfo& lhs, const TimerInfo& rhs)
      { return (lhs.timestamp + lhs.ms) < (rhs.timestamp + rhs.ms); }
   };
   
   static void glutHandler(int val)
   {
      GlutTimer& inst = instance();
      TimerInfo next = inst._callbacks.pull();
      double now = utils::getTimeSinceLaunched();
      double elapsed = now - next.timestamp;
      if (elapsed >= 0.0)
      {
         utils::Nullable<double> newMillis = 
               next.cb->onTimeElapsed(elapsed);
         if (!newMillis.isNull())
            inst.registerCallback(next.cb, next.ms);
      }
      else
         /* Theoretically, this should never happen. */
         inst.registerCallback(next.cb, -elapsed);
   }
   
   static GlutTimer* _instance;
   PriorityQueue<TimerInfo, TimerInfoLessThan> _callbacks;

};

GlutTimer* GlutTimer::_instance = NULL;




Canvas&
GlutDrawingContext::initScreen(const ScreenProperties& screenProps)
throw (utils::InvalidInputException)
{
   if (_canvas.isNotNull())
      KAREN_THROW(utils::InvalidInputException, 
            "cannot initialize screen: already initialized");

   /*
    * Process the screen properties before the rest to detect invalid
    * input ASAP.
    */   
   unsigned int mode = GLUT_DEPTH;
   switch(screenProps.pixelFormat)
   {
      case FORMAT_32BPP_RGBA:
         mode |= GLUT_RGBA;
         break;
      case FORMAT_24BPP_RGB:
         mode |= GLUT_RGB;
         break;
      default:
         KAREN_THROW(utils::InvalidInputException, 
               "cannot initialize screen: incompatible pixel format");
   }
   mode |= screenProps.doubleBuffer ? GLUT_DOUBLE : GLUT_SINGLE;

   glutInitWindowSize(screenProps.dimensions.x, screenProps.dimensions.y);   
   glutInitDisplayMode(mode);   
   glutCreateWindow(screenProps.caption);
   
   glutDisplayFunc(GlutDrawingContext::glutDisplayHandler);
   
   _canvas = new OpenGLCanvas(screenProps.dimensions);
   
   return *_canvas;
}

Canvas&
GlutDrawingContext::screen()
throw (utils::InvalidStateException)
{
   if (_canvas.isNull())
      KAREN_THROW(utils::InvalidStateException, 
            "cannot obtain screen canvas: not initialized yet");
   return *_canvas;
}

void
GlutDrawingContext::postRedisplay()
{
   glutPostRedisplay();
}

void
GlutDrawingContext::glutDisplayHandler()
{
   _activeContext->_canvas->clear();
   if (_activeContext->_target)
      _activeContext->_target->draw(*_activeContext->_canvas);
   _activeContext->_canvas->flush();
   glutSwapBuffers();
}

GlutDrawingContext::GlutDrawingContext()
 : _target(NULL)
{
   _activeContext = this;
}

GlutDrawingContext* GlutDrawingContext::_activeContext = NULL;




const String GlutEngine::ENGINE_NAME("karen.core.glut-engine");

Timer&
GlutEngine::timer()
{ return GlutTimer::instance(); }

void
GlutEngine::runLoop()
{
   try
   {
      glutMainLoop();
   }
   catch (GlutLoopInterruptedException&)
   {
   }
}

void
GlutEngine::stopLoop()
{
}

void
GlutEngine::consumeInputEvent(const InputEvent& ev)
{
   if (ev.type == APPLICATION_QUIT_EVENT)
   {
   }
}

GlutEngine::GlutEngine() : Engine(ENGINE_NAME)
{
   int argc = 0;
   char* argv[] = {};
   glutInit(&argc, argv);
   _drawingContext = new GlutDrawingContext();
   
   inputEventChannel().addInputEventConsumer(this);
}

}}}; /* Namespace karen::ui::core */
