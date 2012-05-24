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

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

namespace karen { namespace ui { namespace core {


class GLTextureStore : public Bitmap::LockCoordinator
{
public:

   virtual bool isLocked(const Bitmap& bmp) const;
   
   virtual void lock(const Bitmap& bmp);
   
   virtual void unlock(const Bitmap& bmp);
   
   virtual void onBind(const Bitmap& bmp);
   
   virtual void onDispose(const Bitmap& bmp);

   /**
    * Obtain the texture name for given bitmap. 
    */
   GLint textureName(const Bitmap& bmp);

private:

   struct BitmapInfo
   {
      GLuint         textureName;
      const Bitmap*  bitmap;
      bool           locked;
      
      inline BitmapInfo(const Bitmap& bmp)
       : textureName(0), bitmap(&bmp), locked(false) {}
   };      

   utils::TreeMap<const Bitmap*, Ptr<BitmapInfo>> _bitmapInfo;
   
   void updateTextureName(BitmapInfo& info);
   
   void releaseTextureName(BitmapInfo& info);

};

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
   OpenGLCanvas(const DrawingContext& parentContext, const IVector& size);

   /**
    * Get canvas size. Obtain canvas dimensions.
    */
   inline virtual DVector size() const
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

   DVector        _size;
   GLTextureStore _textureStore;

};

}}}; /* namespace karen::ui::core */

#endif
