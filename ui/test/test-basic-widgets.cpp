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

#include <KarenUI/app.h>
#include <KarenCore/collection.h>
#include <KarenCore/exception.h>

#include <iostream>

using namespace karen;
using namespace karen::ui;

#define SCREEN_W     640.0f
#define SCREEN_H     480.0f
#define SPEED_X      2.0f
#define SPEED_Y      0.0f
#define FIG_SIZE     100.0f
#define FPS          60.0f

int main(int argc, char* argv[])
{
   try
   {
      TreeMap<String, String> props;
      props.put(Application::UI_ENGINE_PROPERTY,
                   "karen.core.cocoa-engine");
      props.put(Application::SCREEN_WIDTH_PROPERTY,
                   "800");
      props.put(Application::SCREEN_HEIGHT_PROPERTY,
                   "600");
      props.put(Application::FULLSCREEN_PROPERTY,
                   "no");
      props.put(Application::DOUBLE_BUFFER_PROPERTY,
                   "yes");
      Application& app = Application::init(props);
      app.run();
   }
   catch (Exception& e)
   {
      std::cerr << "Failure: " << e.cause() << std::endl;
   }
}
