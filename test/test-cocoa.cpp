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
#include "utils/exception.h"

#include <iostream>

using namespace karen;

#define SCREEN_W     640.0f
#define SCREEN_H     480.0f
#define SPEED_X      2.0f
#define SPEED_Y      0.0f
#define FIG_SIZE     100.0f
#define FPS          60.0f

ui::ScreenProperties screenProps =
{
   ui::Vector(SCREEN_W, SCREEN_H),     // dimensions
   ui::PixelFormat::FORMAT_32BPP_RGBA, // pixel format
   false,                              // fullscreen
   true,                               // double buffered
   "Karen Cocoa Test"                  // caption
};

class SimpleDrawing : public ui::Drawable, 
                      public ui::TimerCallback,
                      public ui::EventConsumer
{
public:

   SimpleDrawing(ui::Engine* en)
    : mov(SPEED_X, SPEED_Y), frameCount(0), engine(en)
   {}

   inline virtual void draw(ui::Canvas& canvas)
   {
      ui::Canvas::QuadParams quad =
      {
         { ui::Vector(pos.x,              pos.y), 
           ui::Vector(pos.x + FIG_SIZE,   pos.y), 
           ui::Vector(pos.x + FIG_SIZE,   pos.y + FIG_SIZE), 
           ui::Vector(pos.x,              pos.y + FIG_SIZE), },
         { ui::Color(pos.x * 255.0f / SCREEN_W , 0xff, 0xff), 
           ui::Color(0xff, pos.y * 255.0f / SCREEN_H, 0xff), 
           ui::Color(0xff, 0xff, pos.x * 255.0f / SCREEN_W), 
           ui::Color(0xff, 0x00, pos.y * 255.0f / SCREEN_H) },
         1.0f,
         true,
      };
      canvas.drawQuad(quad);
   }
   
   virtual utils::Nullable<double> onTimeElapsed(double ms)
   {
      float ts = ms * FPS / 1000.0f;
      if (pos.x < 0)
      {
         pos.x = 0;
         mov.x = -mov.x;
      }
      else if (SCREEN_W < pos.x + FIG_SIZE)
      {
         pos.x = SCREEN_W - FIG_SIZE;
         mov.x = -mov.x;
      }
      if (pos.y < 0)
      {
         pos.y = 0;
         mov.y = -mov.y;
      }
      else if (SCREEN_H < pos.y + FIG_SIZE)
      {
         pos.y = SCREEN_H - FIG_SIZE;
         mov.y = -mov.y * 0.9f;
      }
            
      pos.x += mov.x * ts;
      pos.y += mov.y * ts;
      
      mov.y += 0.5f * ms * ms * 0.001f;

      engine->drawingContext().postRedisplay();
      
      return 1000.0f / FPS;
   }
   
   inline virtual void consumeEvent(const ui::Event& ev)
   {
      switch (ev.type)
      {
         case ui::MOUSE_PRESSED_EVENT:
            std::cerr << "Mouse pressed at " <<
                         ev.mouseButtonPos().toString() << std::endl;
            break;            
         case ui::MOUSE_RELEASED_EVENT:
            std::cerr << "Mouse relesed at " << 
                         ev.mouseButtonPos().toString() << std::endl;
            break;
         case ui::MOUSE_MOTION_EVENT:
            std::cerr << "Mouse moved from " << 
                         ev.mouseMotionFromPos().toString() << 
                         " to " << 
                         ev.mouseMotionToPos().toString() << std::endl;
            break;
         case ui::KEY_PRESSED_EVENT:
            std::cerr << "Key pressed: " << ev.key.unicode << std::endl;
         case ui::KEY_RELEASED_EVENT:
            std::cerr << "Key released: " << ev.key.unicode << std::endl;
            if (ev.key.unicode == ui::KeyEvent::SCAPE_KEY)
               engine->stopLoop();
         default:
            break;
      }
   }

private:

   ui::Vector pos;
   ui::Vector mov;
   long frameCount;
   ui::Engine* engine;

};

int main(int argc, char* argv[])
{
   try
   {
      std::cerr << "Initializing Cocoa engine... ";
      ui::Engine& engine = ui::Engine::init(ui::core::CocoaEngine::ENGINE_NAME);
      ui::DrawingContext& dc = engine.drawingContext();
      std::cerr << "Done" << std::endl;

      std::cerr << "Initializing screen... ";
      dc.initScreen(screenProps);
      std::cerr << "Done" << std::endl;
      
      SimpleDrawing sd(&engine);   
      dc.setDrawingTarget(&sd);
      ui::Timer& timer = engine.timer();
      timer.registerCallback(&sd, 1000.0f / FPS);
      
      engine.eventChannel().addEventConsumer(&sd);
      
      engine.runLoop();
   }
   catch (utils::Exception& e)
   {
      std::cerr << "Failure: " << e.cause() << std::endl;
   }
}
