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

#ifndef KAREN_UI_WIDGET_H
#define KAREN_UI_WIDGET_H

#include "ui/draw.h"
#include "ui/event.h"
#include "utils/platform.h"

namespace karen { namespace ui {

class Container;

/**
 * Widget class. This class provides an abstraction of an object able to
 * be deployed as a UI element to respond to UI events and draw itself
 * on screen.
 */
class KAREN_EXPORT Widget : public EventResponder,
                            public ui::Drawable
{
public:

   friend class Container;

   Widget();
   
   /**
    * Virtual destructor.
    */
   virtual ~Widget();
   
   /**
    * Obtain the size this widget requires to parent container. A value of
    * zero on any coordinate means that there is no requirement for that
    * component.
    */
   inline const Vector& requiredSize() const
   { return _requiredSize; }
   
   /**
    * Obtain the frame assigned to this widget by parent at its deployment.
    */
   inline const Nullable<Rect>& frame() const
   { return _frame; }
   
   /**
    * Obtain the parent for this widget, or null if no parent has been
    * assigned yet. 
    */
   inline const Container* parent() const
   { return _parent; }
   
   /**
    * Obtain the parent for this widget, or null if no parent has been
    * assigned yet. 
    */
   inline Container* parent()
   { return _parent; }

   /**
    * Activate and return the widget at given position respect local 
    * coordinates. This operation is used to determine what is the widget
    * under given mouse cursor position. The associated action is to consider
    * such a widget as activated and return it in order to other widgets
    * up in the chain to know it. This default implementation returns this 
    * widget itself. The method is aimed to be overriden by children classes     
    * to provide their own implementation.
    */
   virtual Widget* activateWidgetAtPos(const Vector& localPos);

protected:

   /**
    * Request a redraw to the UI engine.
    */
   void requestRedraw();

private:

   Container*     _parent;
   Vector         _requiredSize;
   Nullable<Rect> _frame;

};

/**
 * Container class. This class provides an abstraction for a widget able
 * to contain other widgets.
 */
class KAREN_EXPORT Container : public Widget
{
public:

protected:

   /**
    * Sets this container as parent of given widget. This method is meant
    * to be invoked from Container extensions in order to link a widget
    * object with the container it has just been inserted into.    
    */
   virtual void setAsParentOfWidget(Widget& widget);
   
   /**
    * Set the frame for a child widget.
    */
   virtual void setFrameForChild(Widget& widget, const Rect& frame);

};

/**
 * Grid container class. This class provides a container that disposes its
 * children widget in a grid layout.
 */
class KAREN_EXPORT GridContainer : public Container
{
public:

   /**
    * Add a new widget at given coordinates and size. If given widget 
    * alreary has a parent or the widget was already added, a 
    * InvalidInputException is thrown.
    */
   void addWidget(Ptr<Widget> widget, const Rect& coordinates)
         throw (utils::InvalidInputException);
   
   /**
    * Activate and return the widget at given position respect local 
    * coordinates. This operation is used to determine what is the widget
    * under given mouse cursor position. The associated action is to consider
    * such a widget as activated and return it in order to other widgets
    * up in the chain to know it. This implementation locates the widget
    * at given position and marks it as the object-in-front.
    */
   virtual Widget* activateWidgetAtPos(const Vector& localPos);

   /**
    * Draw this widget on given canvas. 
    */
   virtual void draw(Canvas& canvas);

private:

   struct ChildInfo
   {
      Ptr<Widget> widget;
      Rect        coord;      
   };

   utils::LinkedList<ChildInfo> _children;

};

}}; /* Namespace karen::ui */

#endif
