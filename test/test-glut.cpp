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

#define XMIN   0.0f
#define XMAX   540.0f
#define SIZE   100.0f
#define SPEED  0.1f

class SimpleDrawing : public ui::Drawable
{
public:

   SimpleDrawing(ui::DrawingContext* ctx)
    : xpos(XMIN), inc(SPEED), frameCount(0), context(ctx)
   {}

   inline virtual void draw(ui::Canvas& canvas)
   {
      ui::Canvas::QuadParams quad =
      {
         { ui::Vector(xpos, 100), ui::Vector(xpos + SIZE, 100), 
           ui::Vector(xpos + SIZE, 100 + SIZE), ui::Vector(xpos, 100 + SIZE), },
         { ui::Color::RED, ui::Color::GREEN, 
           ui::Color::BLUE, ui::Color::MAGENTA },
         1.0f,
         true,
      };
      canvas.drawQuad(quad);
      animate();
      context->postRedisplay();
   }
   
   void animate()
   {
      if (xpos < XMIN)
      {
         xpos = XMIN;
         inc = -inc;
      }
      else if (XMAX < xpos)
      {
         xpos = XMAX;
         inc = -inc;
      }
      xpos += inc;
   }

private:

   float xpos;
   float inc;
   long frameCount;
   ui::DrawingContext* context;

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
      dc.initScreen(screenProps);
      std::cerr << "Done" << std::endl;
      
      SimpleDrawing sd(&dc);   
      dc.setDrawingTarget(&sd);
      
      engine.runLoop();
   }
   catch (utils::Exception& e)
   {
      std::cerr << "Failure: " << e.cause() << std::endl;
   }
}
