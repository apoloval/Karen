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

#include "ui/core/cocoa.h"

#include <iostream>

/*
 * There is a name clashing between Ptr type as defined by Cocoa framework
 * and Ptr template class defined by Karen. To avoid this conflict, Cocoa
 * is imported by renaming Ptr to CocoaPtr.
 */
#define Ptr CocoaPtr
#import <Cocoa/Cocoa.h>
#undef Ptr

/**
 * Karen OpenGL view. This view extends the Cocoa default NSOpenGLView to
 * include additional members for integrating it with Karen.
 */
@interface KarenOpenGLView : NSOpenGLView
{
   karen::ui::Drawable* drawingTarget;
   karen::ui::Canvas*   screenCanvas;
}

/**
 * Create a new OpenGL view with given dimensions and pixel format.
 */
-(id) initWithFrame: (NSRect) frameRect 
      pixelFormat: (NSOpenGLPixelFormat*) format;
      
/**
 * Set a new drawing target.
 */
-(void) setDrawingTarget: (karen::ui::Drawable*) target;

/**
 * Set the screen canvas.
 */
-(void) setScreenCanvas: (karen::ui::Canvas*) canvas;

/**
 * Redeclaration of draw rect method for redraw when required.
 */
-(void) drawRect: (NSRect) dirtyRect;

@end

@implementation KarenOpenGLView

-(id) initWithFrame: (NSRect) frameRect
      pixelFormat: (NSOpenGLPixelFormat*) format
{
	self = [super initWithFrame: frameRect pixelFormat: format];
   drawingTarget = nil;
   return self;
}

-(void) setDrawingTarget: (karen::ui::Drawable*) target
{
   drawingTarget = target;
}

-(void) setScreenCanvas: (karen::ui::Canvas*) canvas
{
   screenCanvas = canvas;
}

-(void) drawRect: (NSRect) dirtyRect
{
   [super drawRect:dirtyRect];
   if (screenCanvas != nil)
   {
      screenCanvas->clear();
      if (drawingTarget != nil)
         drawingTarget->draw(*screenCanvas);
      [[self openGLContext] flushBuffer];
   }
}

@end

/**
 * Karen window class. This class provides a more concrete specialization
 * of a Cocoa window. The additional behavior consists in capturing the
 * UI events to redirect them to Karen rather than Cocoa. Also, it simplifies
 * the initialization of the window requiring just its frame and bar title
 * content.
 */
@interface KarenWindow: NSWindow
{
   karen::ui::EventConsumer* _eventConsumer;
}

/**
 * Initialize the window at given frame.
 */
-(id) initWithContentRect: (NSRect) contentRect
                    title: (NSString*) winTitle;

/**
 * Set a new Karen event consumer for this window. 
 */
-(void) setEventConsumer: (karen::ui::EventConsumer*) consumer;

/**
 * UI event handler.
 */
-(void) sendEvent :(NSEvent*) ev;

@end

@implementation KarenWindow

-(id) initWithContentRect: (NSRect) contentRect 
                    title: (NSString*) winTitle
{
   self = [super initWithContentRect: contentRect 
                           styleMask: NSTitledWindowMask
                             backing: NSBackingStoreBuffered
                               defer: YES];
   _eventConsumer = nil;
   if (self != nil)
   {
      [self setTitle: winTitle];
      [self setAcceptsMouseMovedEvents: YES];
   }
   return self;
}

-(void) setEventConsumer: (karen::ui::EventConsumer*) consumer
{
   _eventConsumer = consumer;
}

-(void) sendEvent :(NSEvent*) ev
{
   if (_eventConsumer)
   {
      karen::ui::Event kev;
      static NSPoint prevMousePos = NSMakePoint(0.0f, 0.0f);
      NSPoint mousePos = [ev locationInWindow];
      NSRect wframe = [(NSView*)[self contentView] frame];
      NSString* chars = nil;
      switch ([ev type])
      {
         case NSLeftMouseDown:
            kev.type = karen::ui::MOUSE_PRESSED_EVENT;
            kev.mouseButton.button = karen::ui::LEFT_MOUSE_BUTTON;
            kev.mouseButton.posX = mousePos.x;
            kev.mouseButton.posY = wframe.size.height - mousePos.y;
            prevMousePos = mousePos;
            break;
         case NSLeftMouseUp:
            kev.type = karen::ui::MOUSE_RELEASED_EVENT;
            kev.mouseButton.button = karen::ui::LEFT_MOUSE_BUTTON;
            kev.mouseButton.posX = mousePos.x;
            kev.mouseButton.posY = wframe.size.height - mousePos.y;
            prevMousePos = mousePos;
            break;
         case NSRightMouseDown:
            kev.type = karen::ui::MOUSE_PRESSED_EVENT;
            kev.mouseButton.button = karen::ui::RIGHT_MOUSE_BUTTON;
            kev.mouseButton.posX = mousePos.x;
            kev.mouseButton.posY = wframe.size.height - mousePos.y;
            prevMousePos = mousePos;
            break;
         case NSRightMouseUp:
            kev.type = karen::ui::MOUSE_RELEASED_EVENT;
            kev.mouseButton.button = karen::ui::RIGHT_MOUSE_BUTTON;
            kev.mouseButton.posX = mousePos.x;
            kev.mouseButton.posY = wframe.size.height - mousePos.y;
            prevMousePos = mousePos;
            break;
         case NSMouseMoved:
         case NSLeftMouseDragged:
         case NSRightMouseDragged:
            kev.type = karen::ui::MOUSE_MOTION_EVENT;
            kev.mouseMotion.fromX = prevMousePos.x;
            kev.mouseMotion.fromY = wframe.size.height - prevMousePos.y;
            kev.mouseMotion.toX = [ev locationInWindow].x;
            kev.mouseMotion.toY = wframe.size.height - mousePos.y;
            prevMousePos = mousePos;
            break;
         case NSKeyUp:
            kev.type =karen::ui::KEY_RELEASED_EVENT;
            chars = [ev characters];
            if ([chars length])               
               kev.key.unicode = [chars characterAtIndex: 0];
            break;
         case NSKeyDown:
            kev.type =karen::ui::KEY_PRESSED_EVENT;
            chars = [ev characters];
            if ([chars length])               
               kev.key.unicode = [chars characterAtIndex: 0];
         default:
            break;
      }
      
      /*
       * Look for determined conditions to send the event to the consumer.
       */
      karen::ui::Rect area(0, 0, wframe.size.width, wframe.size.height);
      switch (kev.type)
      {
         case karen::ui::MOUSE_PRESSED_EVENT:
         case karen::ui::MOUSE_RELEASED_EVENT:
            if (kev.mouseButtonPos().isInside(area))
               _eventConsumer->consumeEvent(kev);
            break;
         case karen::ui::MOUSE_MOTION_EVENT:
            if (kev.mouseMotionToPos().isInside(area))
               _eventConsumer->consumeEvent(kev);
            break;
         case karen::ui::KEY_PRESSED_EVENT:
         case karen::ui::KEY_RELEASED_EVENT:
            _eventConsumer->consumeEvent(kev);
            break;
         default:
            break;            
      }
           
   }
}

@end

/**
 * Karen timer delegate. This class is used as target of Cocoa timer
 * firing handlers.
 */
@interface KarenTimerTarget: NSObject
{
   karen::ui::TimerCallback*  cb;
   CFAbsoluteTime             launchedOn;
   double                     interval;
}

/**
 * Initialize the timer target with given Karen timer callback and creation
 * date.
 */
-(id) initWithCallback: (karen::ui::TimerCallback*) callback 
            launchedOn: (CFAbsoluteTime) timestamp
              interval: (double) ms;

/**
 * Callback method for fired timer.
 */
-(void) onTimerFired:(NSTimer*)theTimer;

@end

@implementation KarenTimerTarget

-(id) initWithCallback: (karen::ui::TimerCallback*) callback 
            launchedOn: (CFAbsoluteTime) timestamp
              interval: (double) ms
{
   self = [super init];
   cb = callback;
   launchedOn = timestamp;
   interval = ms;
   return self;
}

-(void) onTimerFired:(NSTimer*) timer
{
   CFTimeInterval elapsed = CFAbsoluteTimeGetCurrent() - launchedOn;
   karen::utils::Nullable<double> newInterval =
         cb->onTimeElapsed(elapsed * 1000.0);
   if (newInterval.isNull())
   {
      [timer invalidate];
      [timer release];
   }
   else if (newInterval != interval)
   {
      [timer invalidate];
      [timer release];

      if (!newInterval.isNull())
      {
         NSTimer* tm = [NSTimer timerWithTimeInterval: newInterval / 1000.0
                                               target: self
                                             selector: @selector(onTimerFired:) 
                                             userInfo: nil 
                                              repeats: YES];
         [[NSRunLoop currentRunLoop] addTimer: tm 
                                      forMode: NSDefaultRunLoopMode];
         launchedOn = CFAbsoluteTimeGetCurrent();
         interval = newInterval;
      }
   }
   else
   {
      launchedOn = CFAbsoluteTimeGetCurrent();
   }
}

@end

namespace karen { namespace ui { namespace core {

Canvas&
CocoaDrawingContext::initScreen(const ScreenProperties& screenProps)
throw (utils::InvalidInputException)
{
   NSOpenGLPixelFormatAttribute fmtAttrs[32];
   for (int i = 0; i < 32; i++)
      fmtAttrs[i] = 0;
      
   int attrIndex = 0;
   if (screenProps.pixelFormat == PixelFormat::FORMAT_24BPP_RGB)
   {
      fmtAttrs[attrIndex++] = NSOpenGLPFAColorSize;
      fmtAttrs[attrIndex++] = 8;
      fmtAttrs[attrIndex++] = NSOpenGLPFAAlphaSize;
      fmtAttrs[attrIndex++] = 0;
   }
   else if (screenProps.pixelFormat == PixelFormat::FORMAT_32BPP_RGBA)
   {
      fmtAttrs[attrIndex++] = NSOpenGLPFAColorSize;
      fmtAttrs[attrIndex++] = 8;
      fmtAttrs[attrIndex++] = NSOpenGLPFAAlphaSize;
      fmtAttrs[attrIndex++] = 8;
   }
   else
         KAREN_THROW(utils::InvalidInputException, 
               "cannot init Cocoa screen: unsupported pixel format");            
   if (screenProps.fullscreen)
      fmtAttrs[attrIndex++] = NSOpenGLPFAFullScreen;
   else
      fmtAttrs[attrIndex++] = NSOpenGLPFAWindow;
   if (screenProps.doubleBuffer)
      fmtAttrs[attrIndex++] = NSOpenGLPFADoubleBuffer;
   fmtAttrs[attrIndex++] = NSOpenGLPFADepthSize;
   fmtAttrs[attrIndex++] = (NSOpenGLPixelFormatAttribute) 16;

      
   NSOpenGLPixelFormat* fmt = 
         [[NSOpenGLPixelFormat alloc] initWithAttributes:fmtAttrs];
   
   if (fmt == nil)
      KAREN_THROW(utils::InvalidInputException, 
               "cannot init Cocoa screen: "
               "no suitable properties for current screen configuration");

   NSRect viewRect = NSMakeRect(
         0, 0, screenProps.dimensions.x, screenProps.dimensions.y);

   [NSApplication sharedApplication];

   _glView = [[KarenOpenGLView alloc] initWithFrame:viewRect pixelFormat:fmt];
   [fmt release];
   
   if (_glView == nil)
      KAREN_THROW(utils::InternalErrorException,
            "cannot init Cocoa screen: "
            "NSOpenGLView object cannot be initialized");

   /* 
    * Force the creation of OpenGL context. This is necessary in order to
    * create a GL canvas for it to execute OpenGL calls.
    */
   [_glView openGLContext];
   
   GLint swapInt = 1;
   [[_glView openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

   NSString* winTitle = [NSString stringWithUTF8String: screenProps.caption];
   _window = [[KarenWindow alloc] initWithContentRect: viewRect
                                                title: winTitle];
   
   if (_window == nil)
      KAREN_THROW(utils::InternalErrorException,
            "cannot init Cocoa screen: "
            "NSWindow object cannot be initialized");
            
   [_window setEventConsumer: &_engine->eventChannel()];
            
   [[_window contentView] addSubview: _glView];
   
   _glCanvas = new OpenGLCanvas(*this, screenProps.dimensions);
   
   [_glView setScreenCanvas: _glCanvas];
   
   [_window makeKeyAndOrderFront: nil];
   
   return *_glCanvas;
}

Canvas&
CocoaDrawingContext::screen()
throw (utils::InvalidStateException)
{
   if (_glCanvas.isNull())
      KAREN_THROW(utils::InvalidStateException, 
            "cannot obtain Cocoa screen canvas: not initialized yet");
   return *_glCanvas;
}

void
CocoaDrawingContext::setDrawingTarget(Drawable* target)
{
   [_glView setDrawingTarget: target];
   postRedisplay();
}

void
CocoaDrawingContext::postRedisplay()
{
   [_glView setNeedsDisplay: YES];
}



void
CocoaTimer::registerCallback(TimerCallback* callback, double ms)
throw (utils::InvalidInputException)
{
   KarenTimerTarget* tgt = 
      [[KarenTimerTarget alloc] initWithCallback: callback 
                                      launchedOn: CFAbsoluteTimeGetCurrent()
                                        interval: ms];
   NSTimer* tm = [NSTimer timerWithTimeInterval: ms / 1000.0
                                         target: tgt 
                                       selector: @selector(onTimerFired:) 
                                       userInfo: nil 
                                        repeats: YES];
   [[NSRunLoop currentRunLoop] addTimer: tm forMode: NSDefaultRunLoopMode];
}



const String CocoaEngine::ENGINE_NAME("karen.core.cocoa-engine");


CocoaEngine::~CocoaEngine()
{
   [_memPool release];
};

CocoaDrawingContext&
CocoaEngine::drawingContext()
throw (utils::InvalidStateException)
{
   return *_drawingContext;
}

Timer&
CocoaEngine::timer()
{
   return *_timer;
}

void
CocoaEngine::runLoop()
{
   [NSApp run];
}

void
CocoaEngine::stopLoop()
{
   [NSApp terminate: nil];
}

CocoaEngine::CocoaEngine() : Engine(ENGINE_NAME)
{
   _memPool = [[NSAutoreleasePool alloc] init];
   _drawingContext = new CocoaDrawingContext(this);
   _timer = new CocoaTimer();   
}

}}}; /* namespace karen::ui::core */
