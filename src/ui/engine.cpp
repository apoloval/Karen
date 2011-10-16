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

#include "ui/engine.h"
#include "ui/core/glut.h"
#include "utils/exception.h"
#include "utils/platform.h"
#include "utils/string.h"
#include "utils/types.h"

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
#include "ui/core/cocoa.h"
#endif

namespace karen { namespace ui {

static utils::Ptr<Engine> _engineInstance;

Engine&
Engine::init(const String& engineName)
throw (utils::InvalidInputException, utils::InvalidStateException)
{
   if (_engineInstance.isNotNull())
      KAREN_THROW(utils::InvalidStateException, 
            "cannot initialize UI engine: already initialized");
   
   if (engineName == core::GlutEngine::ENGINE_NAME)
      _engineInstance = new core::GlutEngine();
#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
   if (engineName == core::CocoaEngine::ENGINE_NAME)
      _engineInstance = new core::CocoaEngine();
#endif   
   if (_engineInstance.isNull())
      KAREN_THROW(utils::InvalidInputException, 
            "cannot initialize UI engine: unkown engine %s", 
            (const char*) engineName);
   return *_engineInstance;
}

Engine&
Engine::instance()
throw (utils::InvalidStateException)
{
   if (_engineInstance.isNull())
      KAREN_THROW(utils::InvalidStateException, 
            "cannot obtain instance of UI engine: not initialized yet");
   return *_engineInstance;
}

Engine::Engine(const String& name)
 : _name(name),
   _inputEventChannel(InputEventChannel::newInstance())
{
}

}}; /* Namespace karen::ui */
