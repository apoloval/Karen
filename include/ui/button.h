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

#ifndef KAREN_UI_BUTTON_H
#define KAREN_UI_BUTTON_H

#include "ui/types.h"
#include "ui/widget.h"
#include "utils/string.h"

namespace karen { namespace ui {

class Button : public Widget
{
public:

   /**
    * Create a new instance of button with given caption and 
    * background theme color.
    */
   Button(const String& caption, const Color& bgColor);

   /**
    * Create a new instance of button with given caption.
    */
   Button(const String& caption);

   /**
    * Default theme color.
    */
   static const Color DEFAULT_THEME_COLOR;

   /**
    * Draw this widget on given canvas. 
    */
   virtual void draw(Canvas& canvas);   

private:

   Color    _bgColor;
   String   _caption;

};

}}; /* Namespace karen::ui */

#endif
