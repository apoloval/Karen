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

#include "ui/core/gl.h"
#include "utils/collection.h"
#include "utils/platform.h"
#include "utils/pointer.h"

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <cmath>

namespace karen { namespace ui { namespace core {

class OpenGLTextureFactory : public ImageLifecycleObserver
{
public:

   static OpenGLTextureFactory& instance()
   {
      if (!_singleton)
         _singleton = new OpenGLTextureFactory();
      return *_singleton;
   }

   inline GLint imageToTexture(Image& img)
   {
      if (_store.defined(&img))
         return _store[&img];
      else
      {
         GLuint tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

         bool hasAlpha = Image::pixelFormatMask(img.pixelFormat()).a;
         Vector imgSize(img.size());

         glTexImage2D(
               GL_TEXTURE_2D,                      // Target texture
               0,                                  // Level of detail 
               hasAlpha ? 4 : 3,                   // Internal format 
               imgSize.x, imgSize.y,               // Dimensions
               0,                                  // Border width
               hasAlpha ? GL_RGBA : GL_RGB,        // Pixel data format
               GL_UNSIGNED_BYTE,                   // Pixel data type
               img.pixels());                      // Image data
               
         _store.insert(&img, tex);
         
         img.addLifecycleObserver(this);
         
         return tex;
      }
   }
   
   /**
    * Release the GL texture associated to given image.
    */
   inline void releaseTexture(const Image& img)
   {
      try
      {
         GLuint tex = _store.find(&img);
         glDeleteTextures(1, &tex);
         _store.remove(&img);
      }
      catch (utils::NotFoundException&)
      {
      }
   }
   
   /**
    * Callback method for image modification.
    */
   inline virtual void onImageModification(const Image& img)
   {
      /* TODO: this code should be improved. It is more efficient to
       * regenerate the texture rather than destroying it and create it
       * latter. 
       */
      releaseTexture(img);
   }

   /**
    * Callback method for image destruction.
    */
   inline virtual void onImageDestruction(const Image& img)
   {
      releaseTexture(img);
   }
   
private:

   /**
    * Private constructor.
    */
   inline OpenGLTextureFactory() {}

   static Ptr<OpenGLTextureFactory> _singleton;
   Dictionary<const Image*, GLuint> _store;

};

Ptr<OpenGLTextureFactory> OpenGLTextureFactory::_singleton;

OpenGLCanvas::OpenGLCanvas(const Vector& size)
 : _size(size)
{
   glEnable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDepthFunc(GL_LEQUAL);

   glViewport(0, 0, size.x, size.y);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(0.0, 
           (GLdouble) size.x, 
           (GLdouble) size.y, 
           0.0, 
           0.0, 
           1.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);

   glFlush();   
}

void
OpenGLCanvas::clear()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}

void
OpenGLCanvas::flush()
{ glFlush(); }

void
OpenGLCanvas::drawLine(const LineParams& line)
{
   Vector vector(line.endPos - line.beginPos);
   float mod = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
   Vector normal(vector.y / mod, -vector.x / mod);
   
   QuadParams quadParams =
   { 
      {
         line.beginPos - (normal * line.lineWidth * 0.5f),
         line.beginPos + (normal * line.lineWidth * 0.5f),
         line.endPos + (normal * line.lineWidth * 0.5f),
         line.endPos - (normal * line.lineWidth * 0.5f),
      },
      { line.beginColor, line.beginColor, line.endColor, line.endColor },
      1.0f, 
      true
   };
   drawQuad(quadParams);
}

void
OpenGLCanvas::drawArc(const ArcParams& arc)
{
   Vector path = arc.endPos - arc.beginPos;  // The begin-end line
   Vector npath(path.y, -path.x);            // Normal vector...
   npath.normalise();                        // ...unitary
   
   // Calculate the axis point
   Vector mid = arc.beginPos + (path * 0.5f);     // middle point of path
   Vector axis = mid + (npath * arc.radious);     // Final axis point
   float rad = sqrtf(powf(arc.radious, 2) +       // Curvature radius
                     powf(path.length() * 0.5f, 2));
   
   // Draw the step points in arc
   glDisable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_POLYGON_SMOOTH);
   glBegin(GL_QUAD_STRIP);
   for (int i = 0; i < arc.precision; i++)
   {
      float percent = ((float) i / (float) arc.precision);
      Color col(arc.beginColor + 
                (arc.endColor - arc.beginColor) * percent);
      glColor4f(col.r / 255.0f, 
                col.g / 255.0f, 
                col.b / 255.0f, 
                col.a / 255.0f);
      
      Vector pathCut = arc.beginPos + (path * (i / ((float) arc.precision - 1)));
      Vector dir = (pathCut - axis).normalise();
      Vector p1 = axis + (dir * (rad + arc.lineWidth * 0.5f));
      Vector p2 = axis + (dir * (rad - arc.lineWidth * 0.5f));

      glVertex2f(p1.x, p1.y);
      glVertex2f(p2.x, p2.y);
   }
   glEnd();
}

void
OpenGLCanvas::drawBezier(const BezierParams& line)
{
   unsigned int numPoints = line.points.size();
      
   if (numPoints < 2)
      return;
      
   if (numPoints == 2)
   {
      LineParams lineParams = 
      {
         line.points[0], 
         line.points[1],
         line.color,
         line.color,
         line.lineWidth
      };
      drawLine(lineParams);
      return;
   }
   
   struct BezierCurve
   {
      Vector orig;
      Vector dest;
      Vector ctrl1;
      Vector ctrl2;
   };
   
   Array<BezierCurve> curves(numPoints - 1);
   for (int i = 0; i < numPoints - 1; i++)
   {
      curves[i].orig = line.points[i];
      curves[i].dest = line.points[i + 1];
      if (i == 0) // the first curve
      {         
         curves[i].ctrl1 = line.points[i] + 
               (line.points[i + 1] - line.points[i]).normalise() * 
               line.curvature;
         curves[i].ctrl2 = line.points[i + 1] - 
               ((line.points[i + 2] - line.points[i]).normalise() * 
               line.curvature);
      }
      else if (i == numPoints - 2)
      {
         curves[i].ctrl1 = line.points[i] + 
               ((line.points[i + 1] - line.points[i - 1]).normalise() * 
               line.curvature);
         curves[i].ctrl2 = line.points[i + 1] -
               ((line.points[i + 1] - line.points[i]).normalise() * 
               line.curvature);
      }
      else
      {
         curves[i].ctrl1 = line.points[i] + 
               ((line.points[i + 1] - line.points[i - 1]).normalise() * 
               line.curvature);
         curves[i].ctrl2 = line.points[i + 1] - 
               ((line.points[i + 2] - line.points[i]).normalise() * 
               line.curvature);
      }
   }

   for (int i = 0; i < curves.size(); i++)
   {
      BezierCurve &c = curves[i];

      float halfWidth = line.lineWidth * 0.5f;
      BezierCurve c1 = 
      {
         c.orig + (c.ctrl1 - c.orig).perpendicular() * halfWidth,
         c.dest + (c.dest - c.ctrl2).perpendicular() * halfWidth,
         c.ctrl1 + (c.ctrl2 - c.orig).perpendicular() * halfWidth,
         c.ctrl2 + (c.dest - c.ctrl1).perpendicular() * halfWidth,
      };
      BezierCurve c2 = 
      {
         c.orig - (c.ctrl1 - c.orig).perpendicular() * halfWidth,
         c.dest - (c.dest - c.ctrl2).perpendicular() * halfWidth,
         c.ctrl1 - (c.ctrl2 - c.orig).perpendicular() * halfWidth,
         c.ctrl2 - (c.dest - c.ctrl1).perpendicular() * halfWidth,
      };

      GLfloat cp[2][4][3] = { 
         {
            { c1.orig.x,    c1.orig.y,   0.0f }, 
            { c1.ctrl1.x,   c1.ctrl1.y,  0.0f },
            { c1.ctrl2.x,   c1.ctrl2.y,  0.0f },
            { c1.dest.x,    c1.dest.y,   0.0f }
         },
         {
            { c2.orig.x,    c2.orig.y,   0.0f }, 
            { c2.ctrl1.x,   c2.ctrl1.y,  0.0f },
            { c2.ctrl2.x,   c2.ctrl2.y,  0.0f },
            { c2.dest.x,    c2.dest.y,   0.0f }
         },
      };
      
      glMap2f(GL_MAP2_VERTEX_3, 
              0.0f, 1.0f, 3, 4, 
              0.0f, 1.0f, 12, 2,
              &cp[0][0][0]);
      glEnable(GL_MAP2_VERTEX_3);
      glDisable(GL_TEXTURE_2D);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_POLYGON_SMOOTH);
      glColor4f(line.color.r / 255.0f, 
                line.color.g / 255.0f, 
                line.color.b / 255.0f, 
                line.color.a / 255.0f);
      glBegin(GL_QUAD_STRIP);
      for (int j = 0; j <= line.precision; j++)
      {
         glEvalCoord2f(j / (GLfloat) line.precision, 0.0f);
         glEvalCoord2f(j / (GLfloat) line.precision, 1.0f);
      }
      glEnd();
   }

}

void
OpenGLCanvas::drawTriangle(const TriangleParams& triangle)
{
   glDisable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable((triangle.fill) ? GL_POLYGON_SMOOTH : GL_LINE_SMOOTH);

   glBegin(GL_TRIANGLES);
      for (int i = 0; i < 3; i++)
      {
         glVertex2f(triangle.vertex[i].x, triangle.vertex[i].y);
         glColor4f(triangle.vertexColor[i].r / 255.0f, 
                   triangle.vertexColor[i].g / 255.0f, 
                   triangle.vertexColor[i].b / 255.0f, 
                   triangle.vertexColor[i].a / 255.0f);
      }
   glEnd();
}

void
OpenGLCanvas::drawQuad(const QuadParams& quad)
{
   glDisable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable((quad.fill) ? GL_POLYGON_SMOOTH : GL_LINE_SMOOTH);

   glBegin(GL_QUADS);
      for (int i = 0; i < 4; i++)
      {
         glVertex2f(quad.vertex[i].x, quad.vertex[i].y);
         glColor4f(quad.vertexColor[i].r / 255.0f, 
                   quad.vertexColor[i].g / 255.0f, 
                   quad.vertexColor[i].b / 255.0f, 
                   quad.vertexColor[i].a / 255.0f);
      }
   glEnd();
}

void
OpenGLCanvas::drawImage(const ImageParams& img)
{
   /* TODO: code this. */
}

void
OpenGLCanvas::drawText(const TextParams& txt)
{
   /* TODO: code this. */
}

}}}; /* namespace karen::ui::core */
