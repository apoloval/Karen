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

#ifndef KAREN_UI_COLOR_H
#define KAREN_UI_COLOR_H

#include "KarenCore/platform.h"

namespace karen { namespace ui {

/** 
 * Color type. This struct provides an abstraction for a color
 * using RGBA representations.
 */
struct KAREN_EXPORT Color
{

   /**
    * Predefined opaque black color.
    */
   static const Color BLACK;

   /**
    * Predefined opaque white color.
    */
   static const Color WHITE;

   /**
    * Predefined opaque red color.
    */
   static const Color RED;

   /**
    * Predefined opaque green color.
    */
   static const Color GREEN;

   /**
    * Predefined opaque blue color.
    */
   static const Color BLUE;

   /**
    * Predefined opaque yellow color.
    */
   static const Color YELLOW;

   /**
    * Predefined opaque cyan color.
    */
   static const Color CYAN;

   /**
    * Predefined opaque magenta color.
    */
   static const Color MAGENTA;

   /**
    * Predefined opaque grey color.
    */
   static const Color GREY;
   
   /**
    * Red channel value. 
    */
   UInt8 r;

   /**
    * Green channel value.
    */
   UInt8 g;

   /**
    * Blue channel value.
    */
   UInt8 b;

   /**
    * Alpha(transparency or opacity) channel value.
    */
   UInt8 a;

   /**
    * Create a new color instance with all channels set to maximum value
    * (white color).
    */
   inline Color() : Color(0xff, 0xff, 0xff, 0xff) {}
   
   /**
    * Create a new color by setting all channels to given value but alpha,
    * which is set to completely opaque.
    */
   inline Color(UInt8 v) : Color(v, v, v, 0xff) {}

   /**
    * Create a new color by copying given one.
    */
   inline Color(const Color& col) : Color(col.r, col.g, col.b, col.a) {}

   /**
    * Create a new color by given channel values.
    */
   inline Color(UInt8 _r, UInt8 _g, UInt8 _b, UInt8 _a)
    : r(_r), g(_g), b(_b), a(_a)
   {}

   /**
    * Create a new color by given RGB parameters, setting alpha channel to
    * completely opaque.
    */
   inline Color(UInt8 _r, UInt8 _g, UInt8 _b) : Color(_r, _g, _b, 0xff) {}

   /**
    * Create a new color by given values for all channels with independent
    * value for alpha channel.
    */
   inline Color(UInt8 val, UInt8 alpha) : Color(val, val, val, alpha) {}
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const Color& col) const
   { return r == col.r && g == col.g && b == col.b && a == col.a; }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const Color& col) const
   { return !(*this == col); }
   
   /**
    * Add operator. Perform an addition of two colors using the
    * alpha channel as weight of each channel. 
    * @param col Color to be added to this.
    * @return New color object with the addition of two colors.
    */
   inline Color operator + (const Color& col) const
   {
      double a1 = this->a / 255.0;
      double a2 = col.a / 255.0;
      double r = this->r * a1 + col.r * a2;
      double g = this->g * a1 + col.g * a2;
      double b = this->b * a1 + col.b * a2;
      
      if (r > 255.0) r = 255.0;
      if (g > 255.0) g = 255.0;
      if (b > 255.0) b = 255.0;
   
      return Color(
         (unsigned int) r,
         (unsigned int) g,
         (unsigned int) b,
         (unsigned int) (((a1 + a2) / 2.0)) * 255.0);
   }
      
   /**
    * Subtract operator. Perform an subtraction of two colors.
    * @param col Color to be subtracted to this.
    * @return New color object with the subtract of two colors.
    */
   inline Color operator - (const Color& col) const
   {
      double a1 = this->a / 255.0;
      double a2 = col.a / 255.0;
      double r = this->r * a1 - col.r * a2;
      double g = this->g * a1 - col.g * a2;
      double b = this->b * a1 - col.b * a2;
   
      if (r < 0.0) r = 0.0;
      if (g < 0.0) g = 0.0;
      if (b < 0.0) b = 0.0;

      return Color(
         (unsigned int) r,
         (unsigned int) g,
         (unsigned int) b,
         (unsigned int) (a1 - a2) * 255.0);
   }
   
   /**
    * Add a constant value for each RGB channel on this color.
    * @param val Constant value to add.
    * @return New color object with RGB channels incremented.
    */
   inline Color operator + (unsigned int val) const
   { return Color(r + val, g + val, b + val, a); }

   /**
    * Subtract a constant value for each RGB channel on this color.
    * @param val Constant value to subtract.
    * @return New color object with RGB channels decremented.
    */
   inline Color operator - (unsigned int val) const
   {
      return Color(
         r < val ? 0 : r - val, 
         g < val ? 0 : g - val, 
         b < val ? 0 : b - val, 
         a);
   }
   
   /**
    * Multiply a constant value for each RGB channel in this color.
    * @param val Constant value to multiply.
    * @return New color object with RGB channels multiplied.
    */
   inline Color operator * (double val) const
   { return Color(r * val, g * val, b * val, a); }

   /** 
    * Copy color and set a new alpha value for that copy. 
    * @param alpha Desired alpha value for the new color.
    * @return New color object with RGB values of this one and
    *         new alpha value passed as argument.
    */
   inline Color withAlpha(unsigned int alpha) const
   { return Color(this->r, this->g, this->b, alpha); }
   
};

}}

#endif