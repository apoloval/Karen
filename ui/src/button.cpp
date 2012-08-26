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

#include "KarenUI/button.h"

namespace karen { namespace ui {

const Color Button::DEFAULT_THEME_COLOR(0xc0, 0xc0, 0x90);

Button::Button(const String& caption, const Color& bgColor)
 : _caption(caption), _bgColor(bgColor)
{
}

Button::Button(const String& caption)
 : Button(caption, DEFAULT_THEME_COLOR) 
{
}

void
Button::draw(Canvas& canvas)
{
   IRect f = frame();
   IVector pos = f.position();
   IVector size = f.size();
   Color col1(((_bgColor + 40) * 0.5) + 128);
   Color col2((_bgColor * 0.75) + 64);
   Color col3(_bgColor);
   Color col4(_bgColor + 50);
   
   Canvas::QuadParams frame =
   {
      { pos, 
        pos + DVector(f.w, 0.0), 
        pos + DVector(f.w, f.h), 
        pos + DVector(0.0, f.h),
      },
      { _bgColor, _bgColor, _bgColor, _bgColor },
      1.0f,
      false
   };
   Canvas::QuadParams topGradient =
   {
      { pos, 
        pos + DVector(f.w, 0.0), 
        pos + DVector(f.w, f.h * 0.5), 
        pos + DVector(0.0, f.h * 0.5),
      },
      { col1, col1, col2, col2, },
      1.0f,
      true
   };
   Canvas::QuadParams bottomGradient =
   {
      { pos + DVector(0.0, f.h * 0.5), 
        pos + DVector(f.w, f.h * 0.5), 
        pos + DVector(f.w, f.h), 
        pos + DVector(0.0, f.h)
      },
      { col3, col3, col4, col4, },
      1.0f,
      true
   };
   canvas.drawQuad(topGradient);
   canvas.drawQuad(bottomGradient);   
   canvas.drawQuad(frame);
}

}}; /* Namespace karen::ui */

