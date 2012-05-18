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
#define BITMAP_SIZE  128
#define FPS          60.0f

ui::ScreenProperties screenProps =
{
   ui::IVector(SCREEN_W, SCREEN_H),    // dimensions
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
   {
   }

   inline virtual void draw(ui::Canvas& canvas)
   {
      if (_bitmap.isNull())
         initBitmap();
   
      ui::Canvas::ImageParams imgParams =
      {
         _bitmap,
         ui::IRect(0.0, 0.0, 1.0, 1.0),
         ui::DRect(pos.x, pos.y, FIG_SIZE, FIG_SIZE),
         ui::Canvas::DISPLAY_MODE_SCALE,
         1.0
      };
      canvas.drawImage(imgParams);
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

   ui::DVector pos;
   ui::DVector mov;
   long frameCount;
   ui::Engine* engine;
   Ptr<ui::BitmapBinding> _bitmap;
   
   void initBitmap()
   {
      ui::Bitmap bmp(ui::IVector(BITMAP_SIZE, BITMAP_SIZE), 
                     ui::PixelFormat::FORMAT_32BPP_RGBA);
      for (int i = 0; i < BITMAP_SIZE; i++)
         for (int j = 0; j < BITMAP_SIZE; j++)
         {
            bool evenRow = (i / (BITMAP_SIZE / 8)) % 2;
            bool evenCol = (j / (BITMAP_SIZE / 8)) % 2;
            ui::Color col(
                  evenRow == evenCol ? ui::Color::RED : ui::Color::BLUE);
            bmp.setPixelAt(ui::IVector(i, j), col);
         }
      _bitmap = engine->drawingContext().bindBitmap(bmp);
   }

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
