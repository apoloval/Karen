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
 * Redeclaration of update method for redraw a new frame when required.
 */
-(void) update;

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

-(void) update
{
   [super update];
   if (drawingTarget != nil && screenCanvas != nil)
   {
      drawingTarget->draw(*screenCanvas);
      [[self openGLContext] flushBuffer];
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
   switch (screenProps.pixelFormat)
   {
      case FORMAT_24BPP_RGB:
         fmtAttrs[attrIndex++] = NSOpenGLPFAColorSize;
         fmtAttrs[attrIndex++] = 8;
         fmtAttrs[attrIndex++] = NSOpenGLPFAAlphaSize;
         fmtAttrs[attrIndex++] = 0;
         break;
      case FORMAT_32BPP_RGBA:
         fmtAttrs[attrIndex++] = NSOpenGLPFAColorSize;
         fmtAttrs[attrIndex++] = 8;
         fmtAttrs[attrIndex++] = NSOpenGLPFAAlphaSize;
         fmtAttrs[attrIndex++] = 8;
         break;
      default:
         KAREN_THROW(utils::InvalidInputException, 
               "cannot init Cocoa screen: unsupported pixel format");            
   }
   if (screenProps.fullscreen)
      fmtAttrs[attrIndex++] = NSOpenGLPFAFullScreen;
   else
      fmtAttrs[attrIndex++] = NSOpenGLPFAWindow;
   if (screenProps.doubleBuffer)
      fmtAttrs[attrIndex++] = NSOpenGLPFADoubleBuffer;
      
   NSOpenGLPixelFormat* fmt = 
         [[NSOpenGLPixelFormat alloc] initWithAttributes:fmtAttrs];
   
   if (fmt == nil)
      KAREN_THROW(utils::InvalidInputException, 
               "cannot init Cocoa screen: "
               "no suitable pixel format for given screen properties");

   NSRect viewRect = NSMakeRect(
         0, 0, screenProps.dimensions.x, screenProps.dimensions.y);
   _glView = [[KarenOpenGLView alloc] initWithFrame:viewRect pixelFormat:fmt];
   [fmt release];
   
   if (_glView == nil)
      KAREN_THROW(utils::InternalErrorException,
            "cannot init Cocoa screen: "
            "NSOpenGLView object cannot be initialized");

   _window = [[NSWindow alloc] initWithContentRect: viewRect 
                               styleMask: NSTitledWindowMask 
                               backing: NSBackingStoreBuffered
                               defer: NO];
   
   if (_window == nil)
      KAREN_THROW(utils::InternalErrorException,
            "cannot init Cocoa screen: "
            "NSWindow object cannot be initialized");
            
   [_window setTitle: [NSString stringWithUTF8String: screenProps.caption]];
   [[_window contentView] addSubview: _glView];
   
   _glCanvas = new OpenGLCanvas(screenProps.dimensions);
   
   [_glView setScreenCanvas: _glCanvas];
   
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
}

void
CocoaDrawingContext::postRedisplay()
{
   [_glView setNeedsDisplay: YES];
}



const String CocoaEngine::ENGINE_NAME("karen.core.cocoa-engine");


CocoaEngine::~CocoaEngine()
{
   [_memPool release];
};

DrawingContext&
CocoaEngine::drawingContext()
throw (utils::InvalidStateException)
{
}

Timer&
CocoaEngine::timer()
{
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
   _drawingContext = new CocoaDrawingContext();
}

}}}; /* namespace karen::ui::core */
