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

#include "KarenUI/engine.h"
#include "KarenUI/core/glut.h"
#include <KarenCore/exception.h>
#include <KarenCore/platform.h>
#include <KarenCore/string.h>
#include <KarenCore/types.h>

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
#include "KarenUI/core/cocoa.h"
#endif

namespace karen { namespace ui {

static Ptr<Engine> _engineInstance;

Engine&
Engine::init(const String& engineName)
throw (InvalidInputException, InvalidStateException)
{
   if (_engineInstance.isNotNull())
      KAREN_THROW(InvalidStateException, 
            "cannot initialize UI engine: already initialized");
   
   if (engineName == core::GlutEngine::ENGINE_NAME)
      _engineInstance = new core::GlutEngine();
#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
   if (engineName == core::CocoaEngine::ENGINE_NAME)
      _engineInstance = new core::CocoaEngine();
#endif   
   if (_engineInstance.isNull())
      KAREN_THROW(InvalidInputException, 
            "cannot initialize UI engine: unkown engine %s", 
            (const char*) engineName);
   return *_engineInstance;
}

Engine&
Engine::instance()
throw (InvalidStateException)
{
   if (_engineInstance.isNull())
      KAREN_THROW(InvalidStateException, 
            "cannot obtain instance of UI engine: not initialized yet");
   return *_engineInstance;
}

Engine::Engine(const String& name)
 : _name(name),
   _eventChannel(EventChannel::newInstance())
{
}

}}; /* Namespace karen::ui */
