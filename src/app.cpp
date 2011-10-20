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

#include "app.h"
#include "ui/engine.h"
#include "utils/numeric.h"

namespace karen {

static utils::Nullable<String>
getStringProperty(
   const Application::Properties& props,
   const String& propKey,
   const String& propName,
   bool mandatory)
throw (utils::InvalidInputException)
{
   utils::Nullable<String> res;
   try
   {
      res = props[propKey];
   }
   catch (utils::NotFoundException&)
   {
      if (mandatory)
         KAREN_THROW(utils::InvalidInputException, 
            "cannot initialize Karen application: missing %s property", 
            (const char*) propName);
   }
   return res;
}

static utils::Nullable<long>
getLongProperty(
   const Application::Properties& props,
   const String& propKey,
   const String& propName,
   bool mandatory)
throw (utils::InvalidInputException)
{
   try
   {
      utils::Nullable<String> strVal(
         getStringProperty(props, propKey, propName, mandatory));
      utils::Nullable<long> res;
      if (!strVal.isNull())         
         res = utils::Integer::toLong(strVal);
      return res;
   }
   catch (utils::InvalidConversionException& ex)
   {
      KAREN_THROW_NESTED(utils::InvalidInputException, ex,
            "cannot initialize Karen application: "
            "not numeric value for %s property", (const char*) propName);
   }
}

static utils::Nullable<bool>
getBoolProperty(
   const Application::Properties& props,
   const String& propKey,
   const String& propName,
   bool mandatory)
throw (utils::InvalidInputException)
{
   utils::Nullable<String> strVal(
      getStringProperty(props, propKey, propName, mandatory));
   utils::Nullable<bool> res;
   if (!strVal.isNull())
   {
      if (strVal->toLowerCase() == "yes" || 
          strVal->toLowerCase() == "true" || 
          strVal->toLowerCase() == "1")
         res = true;
      else if (strVal->toLowerCase() == "no" || 
               strVal->toLowerCase() == "false" || 
               strVal->toLowerCase() == "0")
         res = false;
      else
         KAREN_THROW(utils::InvalidInputException,
            "cannot initialize Karen application: "
            "not boolean value for %s property", (const char*) propName);
   }
   return res;
}

class ApplicationImpl : public Application
{
public:

   inline ApplicationImpl(const Properties& props)
   throw (utils::InvalidInputException)
   {
      try
      {
         _engine = &ui::Engine::init(
            getStringProperty(props, UI_ENGINE_PROPERTY, "UI engine", true));
      }
      catch (utils::Exception& ex)
      {
         KAREN_THROW_NESTED(utils::InvalidInputException, ex, 
               "cannot initialize Karen application: "
               "UI engine initialization error");
      }
      ui::ScreenProperties screenProps =
      {
         ui::Vector(640, 480),
         ui::FORMAT_32BPP_RGBA,
         false,
         true,
         "Karen Application",
      };
      
      screenProps.dimensions.x = getLongProperty(
         props, SCREEN_WIDTH_PROPERTY, "screen width", false);
      screenProps.dimensions.y = getLongProperty(
         props, SCREEN_HEIGHT_PROPERTY, "screen height", false);
      screenProps.fullscreen = getBoolProperty(
         props, FULLSCREEN_PROPERTY, "fullscreen", false);
      screenProps.doubleBuffer = getBoolProperty(
         props, DOUBLE_BUFFER_PROPERTY, "double buffer", false);
         
      try
      {
         _screenCanvas = &_engine->drawingContext().initScreen(screenProps);
      }
      catch (utils::InvalidInputException& ex)
      {
         KAREN_THROW_NESTED(utils::InvalidInputException, ex,
               "cannot initialize Karen application: "
               "screen initialization error");
      }
   }
   
   inline void run()
   {
      _engine->runLoop();
   }

private:

   ui::Engine* _engine;
   ui::Canvas* _screenCanvas;
};

static Application* __app_instance = NULL;

const String Application::UI_ENGINE_PROPERTY       ("ui-engine");
const String Application::SCREEN_WIDTH_PROPERTY    ("screen-width");
const String Application::SCREEN_HEIGHT_PROPERTY   ("screen-height");
const String Application::FULLSCREEN_PROPERTY      ("fullscreen");
const String Application::DOUBLE_BUFFER_PROPERTY   ("double-buffer");

Application&
Application::init(const Properties& props)
throw (utils::InvalidInputException, utils::InvalidStateException)
{
   if (__app_instance)
      KAREN_THROW(utils::InvalidStateException, 
         "cannot initialize Karen application: already initialized");
   __app_instance = new ApplicationImpl(props);
   return *__app_instance;
}

}; /* Namespace karen. */