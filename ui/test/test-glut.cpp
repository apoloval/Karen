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

#include <KarenUI/core/glut.h>
#include <KarenCore/exception.h>

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
   ui::IVector(SCREEN_W, SCREEN_H),     // dimensions
   ui::PixelFormat::FORMAT_32BPP_RGBA, // pixel format
   false,                              // fullscreen
   true,                               // double buffered
   "Karen GLUT Test"                   // caption
};

class SimpleDrawing : public ui::Drawable, public ui::TimerCallback
{
public:

   SimpleDrawing(ui::DrawingContext* ctx)
    : mov(SPEED_X, SPEED_Y), frameCount(0), context(ctx)
   {}

   inline virtual void draw(ui::Canvas& canvas)
   {
      ui::Canvas::QuadParams quad =
      {
         { ui::DVector(pos.x,              pos.y), 
           ui::DVector(pos.x + FIG_SIZE,   pos.y), 
           ui::DVector(pos.x + FIG_SIZE,   pos.y + FIG_SIZE), 
           ui::DVector(pos.x,              pos.y + FIG_SIZE), },
         { ui::Color(pos.x * 255.0f / SCREEN_W , 0xff, 0xff), 
           ui::Color(0xff, pos.y * 255.0f / SCREEN_H, 0xff), 
           ui::Color(0xff, 0xff, pos.x * 255.0f / SCREEN_W), 
           ui::Color(0xff, 0x00, pos.y * 255.0f / SCREEN_H) },
         1.0f,
         true,
      };
      canvas.drawQuad(quad);
   }
   
   virtual Nullable<double> onTimeElapsed(double ms)
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

      context->postRedisplay();
      
      return 1000.0f / FPS;
   }

private:

   ui::DVector pos;
   ui::DVector mov;
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
      ui::Timer& timer = engine.timer();
      timer.registerCallback(&sd, 1000.0f / FPS);
      
      engine.runLoop();
   }
   catch (Exception& e)
   {
      std::cerr << "Failure: " << e.cause() << std::endl;
   }
}
