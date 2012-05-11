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

#ifndef KAREN_UI_DRAW_H
#define KAREN_UI_DRAW_H

#include "../utils/collection.h"
#include "../utils/platform.h"

#include "bitmap.h"
#include "types.h"

using karen::utils::DynArray;

namespace karen { namespace ui {

/**
 * Canvas class. This abstract class provides an interface for executing
 * drawing primitives.
 */
class KAREN_EXPORT Canvas
{
public:

   /**
    * Line params type. This struct encapsulates the parameters needed 
    * for drawing a line. 
    */
   struct LineParams
   {
      Vector   beginPos;         //!< Position of begin point
      Vector   endPos;           //!< Position of end point
      Color    beginColor;       //!< Color of begin point
      Color    endColor;         //!< Color of end point
      float    lineWidth;        //!< Line width
   };
   
   /**
    * Arc params type. This struct encapsulates the parameters needed 
    * for drawing an arc.
    */
   struct ArcParams
   {
      Vector   beginPos;         //!< Position of begin point
      Vector   endPos;           //!< Position of end point
      Color    beginColor;       //!< Color of begin point
      Color    endColor;         //!< Color of end point
      float    lineWidth;        //!< Arc width
      float    radious;          //!< Arc radious
      unsigned precision;        //!< Number of segments that comprise the arc
   };
   
   /** 
    * Bezier params type. This struct encapsulates the parameters 
    * needed for drawing a bezier line.
    */
   struct BezierParams
   {
      DynArray<Vector>  points;  //!< Line points
      float          lineWidth;  //!< Bezier line width
      float          curvature;  //!< Bezier curvature
      float          precision;  //!< Number of segments that comprise the line
      Color          color;      //!< Bezier line color
   };
   
   /**
    * Triangle params type. This struct encapsulates the parameters 
    * needed for drawing a triangle
    */
   struct TriangleParams
   {
      Vector   vertex[3];        //!< Position of triangle vertices
      Color    vertexColor[3];   //!< Color of triangle vertices
      float    lineWidth;        //!< Line width
      bool     fill;             //!< Indicator of whether triangle must be filled
   };
   
   /**
    * Quad params type. This struct encapsulates the parameters 
    * needed for drawing a quad.
    */
   struct QuadParams
   {
      Vector   vertex[4];        //!< Position of quad vertices
      Color    vertexColor[4];   //!< Color of quad vertices
      float    lineWidth;        //!< Line width
      bool     fill;             //!< Indicator of whether quad must be filled
      
      /**
       * Create a new instance of QuadParams from the parameters of a
       * rectangle.
       */
      inline static QuadParams createRectangle(
            const Vector& topleft,
            const Vector& bottomright,
            const Color& color,
            float linewidth = 1,
            bool fill = true)
      {
         QuadParams params =
         {
            { topleft, Vector(bottomright.x, topleft.y),
              bottomright, Vector(topleft.x, bottomright.y) },
            { color, color, color, color },
            linewidth,
            fill,
         };          
         return params;
      }
      
      /**
       * Create a new instance of QuadParams from the parameters of
       * a rectangle.
       */
      inline static QuadParams createRectangle(
            const Rect& rect,
            const Color& color,
            float linewidth = 1,
            bool fill = true)
      { 
         return createRectangle(
               rect.position(), rect.position() + rect.size(), 
               color, linewidth, fill);      
      }
   };

   /**
    * Image display mode. This enumeration defines the different display
    * modes applicable to a image drawing.
    */
   enum ImageDisplayMode
   {
      /**
       * Display image aligned at top-left corner.
       */
      DISPLAY_MODE_TOPLEFT,
      
      /**
       * Display image centered.
       */
      DISPLAY_MODE_CENTER,
      
      /**
       * Display image scaled, taking up the whole space available
       * and adjusting its size to fullfill such space.
       */
      DISPLAY_MODE_SCALE,
   };
   
   /**
    * Image params type. This struct encapsulates the parameters 
    * needed for drawing an image.
    */
   struct ImageParams
   {
      Bitmap*           image;
      Rect              imageRect;
      Rect              canvasRect;
      ImageDisplayMode  displayMode;
      double            light;
   };
   
   /**
    * Text params type. This struct encapsulates the parameters
    * needed for writing text.
    */
   struct TextParams
   {
      String      text;
      Font        font;
      Rect        canvasRect;
   };
   
   /**
    * Get canvas size. Obtain canvas dimensions.
    */
   virtual Vector size() const = 0;

   /**
    * Clear the canvas.
    */
   virtual void clear() = 0;
   
   /**
    * Flush drawing pipeline and process pending drawing operations.
    */
   virtual void flush() = 0;
   
   /**
    * Draw a line on canvas.
    */
   virtual void drawLine(const LineParams& line) = 0;

   /**
    * Draw an arc on canvas.
    */
   virtual void drawArc(const ArcParams& arc) = 0;

   /**
    * Draw a bezier line on canvas.
    */
   virtual void drawBezier(const BezierParams& line) = 0;

   /**
    * Draw a triangle on canvas.
    */
   virtual void drawTriangle(const TriangleParams& triangle) = 0;
   
   /**
    * Draw a quad on canvas.
    */
   virtual void drawQuad(const QuadParams& quad) = 0;
   
   /**
    * Draw an image on canvas.
    */
   virtual void drawImage(const ImageParams& img) = 0;
   
   /**
    * Draw (or write) text on canvas.
    */
   virtual void drawText(const TextParams& txt) = 0;
   
};

/**
 * Drawable class. This class provides the interface for an object
 * able to be drawn.
 */
class KAREN_EXPORT Drawable
{
public:

   /**
    * Draw this object on given canvas. 
    */
   virtual void draw(Canvas& canvas) = 0;

};

/**
 * Screen properties type. This type encapsulates the screen properties.
 */
struct ScreenProperties
{
   /**
    * Screen dimensions. 
    */
   Vector         dimensions;
   
   /**
    * Pixel format. Not all pixel formats defined by PixelFormat
    * type may be used for screen. Some of its values may produce a
    * InvalidInputException while initializing the screen.
    */
   PixelFormat    pixelFormat;
   
   /**
    * Fullscreen flag. When enabled, the screen is initialized at
    * full screen. When disabled, it is initialize in windowed mode. Under
    * fullscreen mode, the screen dimensions should be consistent respect
    * screen resolutions admitted by the hardware. If not, an
    * InvalidInputException would be raised.
    */
   bool           fullscreen;
   
   /**
    * Double-buffer flag. This flag indicates whether screen is using
    * duble buffer capabilities.
    */
   bool           doubleBuffer;
   
   /**
    * Screen caption. In non fullscreen mode, this value is used to 
    * fill the window caption. 
    */
   String         caption;   
};

/**
 * Drawing context class. This class provides an abstract for a drawing
 * context. It provides an interface for managing drawing actions for UI.
 */
class KAREN_EXPORT DrawingContext
{
public:

   /**
    * Initialize the screen from given properties. If given properties
    * cannot be satisfied, a InvalidInputException is thrown.
    */
   virtual Canvas& initScreen(const ScreenProperties& screenProps)
         throw (utils::InvalidInputException) = 0;

   /**
    * Obtain the screen as a canvas object to execute drawing operations 
    * on it. If screen was not initialize, a InvalidStateException is thrown.
    */
   virtual Canvas& screen() throw (utils::InvalidStateException) = 0;
   
   /**
    * Set the object target of drawing operations.
    */
   virtual void setDrawingTarget(Drawable* target) = 0;
   
   /**
    * Mark the drawing context to as needing to be redisplayed.
    */
   virtual void postRedisplay() = 0;

};

}}; /* Namespace karen::ui */

#endif
