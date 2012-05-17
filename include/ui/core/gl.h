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

#ifndef KAREN__UI_CORE_GL_H
#define KAREN__UI_CORE_GL_H

#include "ui/bitmap.h"
#include "ui/draw.h"
#include "ui/types.h"
#include "utils/collection.h"
#include "utils/pointer.h"

namespace karen { namespace ui { namespace core {

/**
 * OpenGL Canvas class. This class implements the canvas interface by using
 * OpenGL API.
 */
class OpenGLCanvas : public Canvas
{
public:

   /**
    * Create a new canvas with given size.
    */
   OpenGLCanvas(const DrawingContext& parentContext, const Vector& size);

   /**
    * Get canvas size. Obtain canvas dimensions.
    */
   inline virtual Vector size() const
   { return _size; }

   /**
    * Clear the canvas.
    */
   virtual void clear();
   
   /**
    * Flush drawing pipeline and process pending drawing operations.
    */
   virtual void flush();
   
   /**
    * Draw a line on canvas.
    */
   virtual void drawLine(const LineParams& line);

   /**
    * Draw an arc on canvas.
    */
   virtual void drawArc(const ArcParams& arc);

   /**
    * Draw a bezier line on canvas.
    */
   virtual void drawBezier(const BezierParams& line);

   /**
    * Draw a triangle on canvas.
    */
   virtual void drawTriangle(const TriangleParams& triangle);
   
   /**
    * Draw a quad on canvas.
    */
   virtual void drawQuad(const QuadParams& quad);
   
   /**
    * Draw an image on canvas.
    */
   virtual void drawImage(const ImageParams& img);
   
   /**
    * Draw (or write) text on canvas.
    */
   virtual void drawText(const TextParams& txt);
   
private:

   Vector _size;

};

}}}; /* namespace karen::ui::core */

#endif
