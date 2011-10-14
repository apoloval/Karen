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

#include "ui/core/glut.h"
#include "utils/exception.h"

#include <iostream>

using namespace karen;

ui::ScreenProperties screenProps =
{
   ui::Vector(640, 480),      // dimensions
   ui::FORMAT_32BPP_RGBA,     // pixel format
   false,                     // fullscreen
   true,                      // double buffered
   "Karen GLUT Test"          // caption
};

int main(int argc, char* argv[])
{
   try
   {
      std::cerr << "Initializing GLUT engine... ";
      ui::Engine& engine = ui::Engine::init(ui::core::GlutEngine::ENGINE_NAME);
      ui::DrawingContext& dc = engine.drawingContext();
      std::cerr << "Done" << std::endl;

      std::cerr << "Initializing screen... ";
      ui::Canvas& cvs = dc.initScreen(screenProps);
      std::cerr << "Done" << std::endl;
      
      engine.runLoop();
   }
   catch (utils::Exception& e)
   {
      std::cerr << "Failure: " << e.cause() << std::endl;
   }
}