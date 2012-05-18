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

#ifndef KAREN_UI_TYPES_H
#define KAREN_UI_TYPES_H

#include "utils/exception.h"
#include "utils/platform.h"
#include "utils/string.h"
#include "utils/types.h"

#include <cmath>

#define __max__(n1, n2) ((n1) > (n2) ? (n1) : (n2))
#define __min__(n1, n2) ((n1) < (n2) ? (n1) : (n2))

namespace karen { namespace ui {

/* Required pre-declarations. */
template <class T>
class Rect;

/**
 * Vector type. This struct provides an abstraction for a 2D vector.
 */
template <class T>
struct KAREN_EXPORT Vector
{
   /** X position. */
   T x;
   
   /** Y position. */
   T y;
   
   /**
    * Create a new vector with coordinates [0, 0]
    */
   inline Vector() : x(T()), y(T()) {}

   /**
    * Create a new vector with given coordinates.
    */
   Vector(const T& _x, const T& _y) : x(_x), y(_y) {}
   
   /**
    * Templated copy constructor. 
    */
   template <class Other>
   Vector(const Vector<Other>& v) : x(v.x), y(v.y) {}

   /**
    * Equals to operator.
    */
   inline bool operator == (const Vector& p) const
   { return x == p.x && y == p.y; }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const Vector& p) const
   { return x != p.x || y != p.y; }
   
   /**
    * Add operator.
    */
   inline Vector operator + (const Vector& p) const
   { return Vector(this->x + p.x, this->y + p.y); }
   
   /**
    * Subtract operator.
    */
   inline Vector operator - (const Vector& p) const
   { return Vector(this->x - p.x, this->y - p.y); }
   
   /**
    * Add operator.
    */
   inline Vector operator + (double value) const
   { return Vector(this->x + value, this->y + value); }
   
   /**
    * Subtract operator.
    */
   inline Vector operator - (double value) const
   { return Vector(this->x - value, this->y - value); }
   
   /**
    * Multiply operator.
    */
   inline Vector operator * (double value) const
   { return Vector(this->x * value, this->y * value); }
   
   /**
    * Scalar product operator.
    */
   inline double operator * (const Vector& v) const
   { return this->x * v.x + this->y * v.y; }
   
   /**
    * Divide by scalar operator.
    */
   inline Vector operator / (const T& value) const
   { return Vector(this->x / value, this->y / value); }
   
   /**
    * Add and assign operator.
    */
   inline Vector& operator += (const Vector& p)
   { x += p.x; y += p.y; return *this; }
   
   /**
    * Subtract and assign operator.
    */
   inline Vector& operator -= (const Vector& p)
   { x -= p.x; y -= p.y; return *this; }
   
   /**
    * Check if this vector is inside the rectangle defined by position p
    * and size s.
    */
   inline bool isInside(const Vector& p, const Vector& s) const
   { return x >= p.x && y >= p.y && x <= p.x + s.x && y <= p.y + s.y; }
      
   /**
    * Check if this vector is inside the rectangle defined by v rectangle.
    */
   inline bool isInside(const Rect<T>& v) const
   { return isInside(Vector(v.x, v.y), Vector(v.w, v.h)); }
   
   /**
    * Calculate distance between this vector and the one passed as
    * argument.
    */
   inline double distance(const Vector& p) const
   { return sqrt(pow((double) p.x - x, 2.0) + pow((double) p.y - y, 2.0)); } 
   
   /**
    * Obtain the length of this vector.
    */
   inline double length() const
   { return sqrt(pow((double) x, 2.0) + pow((double) y, 2.0)); }
   
   /**
    * Normalise this vector.
    */
   inline Vector& normalise()
   {
      double l = length();
      x /= l;
      y /= l;
      return *this;
   }
   
   /**
    * Obtain a normalised copy of this vector.
    */
   inline Vector normalisedCopy() const
   {
      Vector ret = *this;
      return ret.normalise();
   }
   
   /**
    * Obtain a perpendicular vector and optionally normalise it.
    */
   inline Vector perpendicular(bool normalise = true) const
   {
      Vector ret(this->y, -this->x);
      return normalise ? ret.normalise() : ret;
   }
   
   /**
    * Obtain a string representation of this vector.
    */
   inline String toString() const
   { return String::format("[%.2f, %.2f]", x, y); };
         
};

typedef Vector<double>  DVector;
typedef Vector<float>   FVector;
typedef Vector<int>     IVector;
typedef Vector<long>    LVector;

/**
 * Three-dimensional vector class. This class represents a three dimensional
 * vector.
 */
template <class T>
struct KAREN_EXPORT Vector3
{

   /** 
    * X coordinate.
    */
   T x;
   
   /** 
    * Y coordinate.
    */
   T y;

   /** 
    * Z coordinate.
    */
   T z;
   
   /**
    * Create a new vector with default coordinates [0, 0, 0]
    */
   inline Vector3() : x(T()), y(T()), z(T()) {}
   
   /**
    * Create a new vector with given value in all coordinates.
    */
   inline Vector3(const T& v) : x(v), y(v), z(v) {}
   
   /**
    * Create a new vector from given coordinates.
    */
   inline Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const Vector3& v) const
   { return (this->x == v.x) && (this->y == v.y) && (this->z == v.z); }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const Vector3& v) const
   { return !(*this == v); }
   
   /**
    * Add operator.
    */
   inline Vector3 operator + (const Vector3& v) const
   { return Vector3(this->x + v.x, this->y + v.y, this->z + v.z); }
   
   /**
    * Add operator.
    */
   inline Vector3 operator + (const T& val) const
   { return Vector3(this->x + val, this->y + val, this->z + val); }
   
   /**
    * Subtract operator.
    */
   inline Vector3 operator - (const Vector3& v) const
   { return Vector3(this->x - v.x, this->y - v.y, this->z - v.z); }

   /**
    * Subtract operator.
    */
   inline Vector3 operator - (const T& val) const
   { return Vector3(this->x - val, this->y - val, this->z - val); }
   
   /**
    * Cross-product operator.
    */
   inline Vector3 operator * (const Vector3& v) const
   { return Vector3((this->y * v.z - this->z * v.y),
                    -(this->x * v.z - this->z * v.x),
                    (this->x * v.y - this->x * v.y)); }
   
   /**
    * Multiply operator.
    */
   inline Vector3 operator * (const T& val) const
   { return Vector3(this->x * val, this->y * val, this->z * val); }
   
   /**
    * Divide operator.
    */
   inline Vector3 operator / (const T& val) const
   { return Vector3(this->x / val, this->y / val, this->z / val); }
   
   /**
    * Add and assign operator.
    */
   inline Vector3& operator += (const Vector3& v)
   { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }

   /**
    * Add and assign operator.
    */
   inline Vector3& operator += (const T& val)
   { this->x += val; this->y += val; this->z += val; return *this; }

   /**
    *  Subtract and assign operator.
    */
   inline Vector3& operator -= (const Vector3& v)
   { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
   
   /**
    * Subtract and assign operator.
    */
   inline Vector3& operator -= (const T& val)
   { this->x -= val; this->y -= val; this->z -= val; return *this; }

   /**
    * Multiply and assign operator.
    */
   inline Vector3& operator *= (const T& val)
   { this->x *= val; this->y *= val; this->z *= val; return *this; }

   /**
    * Calculate the scalar product.
    */
   inline double dotProduct(const Vector3& v) const
   { return (this->x * v.x) + (this->y * v.y) + (this->z * v.z); }
   
   /**
    * Calculate distance between two vectors.
    */
   double distance(const Vector3& p) const;
   
   /**
    * Obtain the length of this vector.
    */
   double length() const;
   
   /**
    * Normalise this vector.
    */
   inline Vector3& normalise()
   {
      double l = length();
      x /= l;
      y /= l;
      z /= l;
      return *this;
   }
   
   /**
    * Obtain a normalised copy of this vector.
    */
   inline Vector3 normalisedCopy() const
   {
      Vector3 ret = *this;
      return ret.normalise();
   }
   
};

typedef Vector3<double> DVector3;
typedef Vector3<float>  FVector3;
typedef Vector3<int>    IVector3;
typedef Vector3<long>   LVector3;

/**
 * Rect class. This class provides an abstraction for a rectangle in a 2D
 * euclidean space.
 */
template <class T>
struct KAREN_EXPORT Rect
{

   /**
    * X position of top-left corner of rectangle.
    */
   T x;

   /**
    * Y position of top-left corner of rectangle.
    */
   T y;
   
   /**
    * Rectangle width.
    */
   T w;

   /**
    * Rectangle height.
    */
   T h;

   /**
    * Create a new rectangle with top-left corner [0, 0] and size [0, 0].
    */
   inline Rect() : Rect(T(), T(), T(), T()) {}

   /**
    * Create a new rectangle with given top-left corner coordinates and size.
    */
   inline Rect(const T& valX, const T& valY, const T& valW, const T& valH)
    : x(valX), y(valY), w(valW), h(valH)
   {
   }

   /**
    * Create a new rectangle with given top-left position and given size.
    */
   inline Rect(const Vector<T>& p, const Vector<T>& s)
    : Rect(p.x, p.y, s.x, s.y) {}
    
   /**
    * Create a new rectangle with top-left corner at [0, 0] and given size. 
    */
   inline Rect(const Vector<T>& s) : Rect(Vector<T>(T(), T()), s) {}

   /**
    * Obtain (top-left) rectangle position.
    */
   inline Vector<T> position() const
   { return topLeft(); }
   
   /**
    * Obtain top-left rectangle position.
    */
   inline Vector<T> topLeft() const
   { return Vector<T>(x, y); }
   
   /**
    * Obtain top-right rectangle position.
    */
   inline Vector<T> topRight() const
   { return Vector<T>(x + w, y); }
   
   /**
    * Obtain bottom-left rectangle position.
    */
   inline Vector<T> bottomLeft() const
   { return Vector<T>(x, y + h); }
   
   /**
    * Obtain bottom-right rectangle position.
    */
   inline Vector<T> bottomRight() const
   { return Vector<T>(x + w, y + h); }
   
   /**
    * Obtain rectangle size.
    */
   inline Vector<T> size() const
   { return Vector<T>(w, h); }
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const Rect& v) const
   { return x == v.x && y == v.y && w == v.w && h == v.h; }

   /**
    * Not equals to operator.
    */
   inline bool operator != (const Rect& v) const
   { return x != v.x || y != v.y || w != v.w || h != v.h; }
   
   /**
    * Add operator.
    */
   inline Rect operator + (const Vector<T>& p) const
   { return Rect(x + p.x, y + p.y, w, h); }
   
   /**
    * Subtract operator.
    */
   inline Rect operator - (const Vector<T>& p) const
   { return Rect(x - p.x, y - p.y, w, h); }
   
   /**
    * Add and assign operator.
    */
   inline Rect& operator += (const Vector<T>& p)
   { x += p.x; y += p.y; return *this; }
   
   /**
    * Add operator. This add operator creates the minimum rectangle
    * that contains these ones. 
    */
   inline Rect operator + (const Rect& v) const
   {
      // rectangle 1 origin vector
      T v1x = x;
      T v1y = y;

      // rectangle 2 origin vector
      T v2x = v.x;
      T v2y = v.y;
      
      // rectangle 1 end vector
      T v3x = v1x + w;
      T v3y = v1y + h;
       
      // rectangle 2 end vector
      T v4x = v2x + v.w;
      T v4y = v2y + v.h;
      
      // Calculate resulting vector and size
      T rposx = __min__(v1x, v2x);
      T rposy = __min__(v1y, v2y);
      
      T rsizew = __max__(v3x, v4x) - rposx;
      T rsizeh = __max__(v3y, v4y) - rposy;
   
      return Rect(rposx, rposy, rsizew, rsizeh);
   }
   
   /**
    * Subtract operator. This operator creates a rectangle result of an 
    * intersection of given ones. 
    */
   inline utils::Nullable<Rect> operator - (const Rect& v) const
   {
      Rect result;
      
      // Origin vector of intersection
      T v1x = __max__(x, v.x);
      T v1y = __max__(y, v.y);
      
      // Ending vector of intersection
      T v2x = __min__(x + w, v.x + v.w);
      T v2y = __min__(y + h, v.y + v.h);
      
      // If origin is less than ending, 
      // it means a null intersection
      if ((v2x < v1x) || (v2y < v1y))
         return utils::Nullable<Rect<T>>();
      
      // Fill results
      result.x = v1x;
      result.y = v1y;
      
      result.w = v2x - v1x;
      result.h = v2y - v1y;
      
      return result;
   }
   
   /**
    * Check whether this rectangle overlaps with the rectangle defined by
    * given top-left coordinates and size. 
    */
   inline bool overlap(const Vector<T>& p, const Vector<T>& s) const
   { return overlap(Rect(p, s)); }
   
   /**
    * Check whether this rectangle overlaps with rectangle passed as
    * argument.
    */
   inline bool overlap(const Rect& v) const
   {
      utils::Nullable<Rect<T>> intersection = *this - v;
      
      // True if intersection is not null.
      return !intersection.isNull();
   }   
   
   /**
    * Check whether given rectangle is contained inside this one.
    */
   inline bool isInside(const Rect& v) const
   {
      Rect<T> sub = (*this - v);
      return *this == sub;
   }
   
   /**
    * Grow the rectangle the given amount of units.
    */
   inline void grow(const T& inc)
   {
      x -= inc;
      y -= inc;
      w += inc * 2.0;
      h += inc * 2.0;
   }
   
   /**
    * Shrink this rectangle the given amount of units.
    */
   inline void shrink(const T& dec)
   {
      x += dec;
      y += dec;
      w -= dec * 2.0;
      h -= dec * 2.0;
   }

};

typedef Rect<double> DRect;
typedef Rect<float>  FRect;
typedef Rect<int>    IRect;
typedef Rect<long>   LRect;

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

/**
 * Font type. This struct encapsulates the properties of a typographical
 * font.
 */
struct Font
{
   /**
    * Font color property.
    */
   Color          color;
   
   /**
    * Font size property.
    */
   unsigned int   size;
   
   /**
    * Font boldface property.
    */
   bool           boldface;
   
   /**
    * Font italic property.
    */
   bool           italic;
};

/**
 * Pixel format class. This class encapsulates the pixel format data. 
 */
class PixelFormat
{
public:

   /**
    * Pixel format mask. This struct provides the binary mask used to
    * retrieve the RGBA values from each pixel in a 32-bit length binary 
    * representation.
    */
   struct Mask
   {
      UInt32 r;
      UInt32 g;
      UInt32 b;
      UInt32 a;
      
      /**
       * Create a mask object from given RGBA mask values. 
       */
      inline Mask(UInt32 theR, UInt32 theG, UInt32 theB, UInt32 theA)
       : r(theR), g(theG), b(theB), a(theA)
      {
      }
      
      /**
       * Equals to operator.
       */
      bool operator == (const Mask& m) const
      { return r == m.r && g == m.g && b == m.b && a == m.a; }
      
      /**
       * Not equals to operator.
       */
      bool operator != (const Mask& m) const
      { return !(*this == m); }

   };
   
   /**
    * Pixel format shift. This struct provides the left shift of the RGBA 
    * values of each pixel in a 32-bit length binary representation.  
    */
   struct Shift
   {
      unsigned int r;
      unsigned int g;
      unsigned int b;
      unsigned int a;

      /**
       * Create a mask object from given RGBA mask values. 
       */
      Shift(unsigned int theR, unsigned int theG, 
            unsigned int theB, unsigned int theA)
       : r(theR), g(theG), b(theB), a(theA)
      {
      }
      
      /**
       * Equals to operator.
       */
      bool operator == (const Shift& s) const
      { return r == s.r && g == s.g && b == s.b && a == s.a; }
      
      /**
       * Not equals to operator.
       */
      bool operator != (const Shift& s) const
      { return !(*this == s); }

   };
   
   /**
    * 8 bits per pixel, single greyscale byte.
    */
   static const PixelFormat FORMAT_8BPP_GREYSCALE;
   
   /**
    * 16 bits per pixel, double greyscale byte.
    */
   static const PixelFormat FORMAT_16BPP_GREYSCALE;
   
   /**
    * 24 bits per pixel, RGB channels.
    */
   static const PixelFormat FORMAT_24BPP_RGB;
   
   /**
    * 24 bits per pixel, BGR channels.
    */
   static const PixelFormat FORMAT_24BPP_BGR;
   
   /**
    * 32 bits per pixel, RGB channels, right padding.
    */
   static const PixelFormat FORMAT_32BPP_RGBX;
   
   /**
    * 32 bits per pixel, RGB channels, left padding.
    */
   static const PixelFormat FORMAT_32BPP_XRGB;
   
   /**
    * 32 bits per pixel, BGR channels, right padding.
    */
   static const PixelFormat FORMAT_32BPP_BGRX;
   
   /**
    * 32 bits per pixel, BGR channels, left padding.
    */
   static const PixelFormat FORMAT_32BPP_XBGR;
   
   /**
    * 32 bits per pixel, RGBA channels.
    */
   static const PixelFormat FORMAT_32BPP_RGBA;
   
   /**
    * 32 bits per pixel, ABGR channels.
    */
   static const PixelFormat FORMAT_32BPP_ABGR;
   
   /**
    * Copy constructor.
    */
   inline PixelFormat(const PixelFormat& fmt)
    : _mask(fmt._mask), _shift(fmt._shift), _bitsPerPixel(fmt._bitsPerPixel)
   {
   }
   
   /**
    * Equals to operator.
    */
   inline bool operator == (const PixelFormat& fmt) const
   { return _mask == fmt._mask && _bitsPerPixel == fmt._bitsPerPixel; }
   
   /**
    * Not equals to operator.
    */
   inline bool operator != (const PixelFormat& fmt) const
   { return !(*this == fmt); }
   
   /**
    * Obtain pixel format mask.
    */
   inline const Mask& mask() const
   { return _mask; }
   
   /**
    * Obtain pixel format shift.
    */
   inline const Shift& shift() const
   { return _shift; }
   
   /**
    * Obtain the bits per pixel for this format.
    */
   inline unsigned int bitsPerPixel() const
   { return _bitsPerPixel; }
   
   /**
    * Obtain the bytes per pixel for this format.
    */
   inline unsigned int bytesPerPixel() const
   { return _bitsPerPixel / 8; }
   
private:

   /**
    * Private constructor.
    */
   inline PixelFormat(const Mask& mask, const Shift& shift, unsigned int bpp)
    : _mask(mask), _shift(shift), _bitsPerPixel(bpp)
   {
   }

   Mask           _mask;
   Shift          _shift;
   unsigned int   _bitsPerPixel;
   
};

}} /* Namespace karen::ui */

#endif
