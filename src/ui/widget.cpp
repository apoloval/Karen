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

#include "ui/engine.h"
#include "ui/widget.h"

namespace karen { namespace ui {

Widget::Widget()
 : _parent(NULL)
{
}

Widget::~Widget()
{
}

Widget*
Widget::activateWidgetAtPos(const Vector& localPos)
{ return this; }

void
Widget::requestRedraw()
{
   Engine::instance().drawingContext().postRedisplay();
}

void
Container::setAsParentOfWidget(Widget& widget)
{
   widget._parent = this;
}

void
Container::setFrameForChild(Widget& widget, const Rect& frame)
{
   widget._frame = frame;
}

void
GridContainer::addWidget(
      Ptr<Widget> widget,
      const Rect& coordinates)
throw (utils::InvalidInputException)
{
   if (widget.isNull())
      KAREN_THROW(utils::InvalidInputException, 
        "cannot add widget to container: null widget as input");
   if (widget->parent())
      KAREN_THROW(utils::InvalidInputException, 
        "cannot add widget to container: widget already has a parent");
   
   for (auto child : _children)
      if (child.widget == widget)
         KAREN_THROW(utils::InvalidInputException, 
         "cannot add widget to container: widget already added");
   
   ChildInfo child = { widget, coordinates };
   _children.insertBack(child);
   setAsParentOfWidget(*widget);
   setFrameForChild(*widget, coordinates);
   requestRedraw();
}

Widget*
GridContainer::activateWidgetAtPos(const Vector& localPos)
{
   for (utils::ConstIterator<ChildInfo> it = _children.begin(); it; it++)
      if (localPos.isInside(it->coord))
      {
         ChildInfo ci = *it;
         Widget* result = ci.widget;
         Vector childPos(localPos - it->coord.position());
         
         /*
          * For this container, the activation consists in moving
          * this widget to the head of the children list.
          */
         _children.remove(it);
         _children.insertFront(ci);
                           
         return result->activateWidgetAtPos(childPos);
      }
   
   return this;
}

void
GridContainer::draw(Canvas& canvas)
{
   for (auto child : _children)
      child.widget->draw(canvas);
}

}}; /* Namespace karen::ui */
