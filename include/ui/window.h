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

#ifndef KAREN_UI_WINDOW_H
#define KAREN_UI_WINDOW_H

#include "ui/event.h"
#include "ui/widget.h"
#include "utils/platform.h"

namespace karen { namespace ui {

/**
 * Window class. This class provides an abstraction of a Karen window. The
 * concept of window in Karen is slightly different respect the classical
 * concept of window. In Karen, a window is a container that encapsulates
 * all the widgets that are deployed in the full screeen at some point
 * in time. Think about Karen windows as pages of a book. Everything you see
 * is in that page, which you may turn to continue reading.
 */
class KAREN_EXPORT Window : public EventResponder
{
public:

   void addChildWidget(Ptr<Widget>& childWidget, const Rect& location);

   /**
    * Draw the window and its contents on given canvas. 
    */
   virtual void draw(const Canvas& canvas);

   /**
    * Obtain the widget at given position respect local coordinates. 
    * This implementation locates the child widget at given position
    * and invokes the same operation on it. The result is that the final
    * widget at the pos is obtained recursively.
    */
   virtual Widget* widgetAtLocalPosition(const Vector& localPos);

};

}}; /* Namespace karen::ui */

#endif
