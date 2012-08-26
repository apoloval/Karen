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

#include "KarenUI/core/gl.h"
#include <KarenCore/collection.h>
#include <KarenCore/platform.h>
#include <KarenCore/pointer.h>

#include <cmath>

namespace karen { namespace ui { namespace core {

static void 
toGLPixelFormat(const PixelFormat& fmt, GLenum& format, GLenum& type)
throw (InvalidInputException)
{
   if (fmt == PixelFormat::FORMAT_8BPP_GREYSCALE)
   { format = GL_LUMINANCE; type = GL_UNSIGNED_BYTE; }
   else if (fmt == PixelFormat::FORMAT_16BPP_GREYSCALE)
   { format = GL_LUMINANCE; type = GL_UNSIGNED_SHORT; }
   else if (fmt == PixelFormat::FORMAT_24BPP_RGB)
   { format = GL_RGB; type = GL_UNSIGNED_BYTE; }
   else if (fmt == PixelFormat::FORMAT_24BPP_BGR)
   { format = GL_BGR; type = GL_UNSIGNED_BYTE; }
   else if (fmt == PixelFormat::FORMAT_32BPP_RGBX)
   { KAREN_THROW(InvalidInputException, 
                 "OpenGL is unable to work with RGBX pixel format"); }
   else if (fmt == PixelFormat::FORMAT_32BPP_XRGB)
   { KAREN_THROW(InvalidInputException, 
                 "OpenGL is unable to work with XRGB pixel format"); }
   else if (fmt == PixelFormat::FORMAT_32BPP_BGRX)
   { KAREN_THROW(InvalidInputException, 
                 "OpenGL is unable to work with BGRX pixel format"); }
   else if (fmt == PixelFormat::FORMAT_32BPP_XBGR)
   { KAREN_THROW(InvalidInputException, 
                 "OpenGL is unable to work with XBGR pixel format"); }
   else if (fmt == PixelFormat::FORMAT_32BPP_RGBA)
   { format = GL_RGBA; type = GL_UNSIGNED_BYTE; }
   else if (fmt == PixelFormat::FORMAT_32BPP_ABGR)
   { KAREN_THROW(InvalidInputException, 
                 "OpenGL is unable to work with ABGR pixel format"); }
}

bool
GLTextureStore::isLocked(const Bitmap& bmp) const
{
   try { return _bitmapInfo.get(&bmp)->locked; }
   catch (NotFoundException&) { return false; }
}

void
GLTextureStore::lock(const Bitmap& bmp)
{
   try
   {
      Ptr<BitmapInfo>& info = _bitmapInfo.get(&bmp);
      info->locked = true;
   }
   catch (NotFoundException&) {}
}

void
GLTextureStore::unlock(const Bitmap& bmp)
{
   try
   {
      Ptr<BitmapInfo>& info = _bitmapInfo.get(&bmp);
      info->locked = false;
      updateTextureName(*info);
   }
   catch (NotFoundException&) {}
}

void
GLTextureStore::onBind(const Bitmap& bmp)
{
   Ptr<BitmapInfo> info = new BitmapInfo(bmp);
   _bitmapInfo.put(&bmp, info);
   updateTextureName(*info);
}

void
GLTextureStore::onDispose(const Bitmap& bmp)
{
   try
   {
      Ptr<BitmapInfo>& info(_bitmapInfo.get(&bmp));
      releaseTextureName(*info);
      _bitmapInfo.remove(&bmp);

   }
   catch (NotFoundException&) {}   
}

GLint
GLTextureStore::textureName(const Bitmap& bmp)
{
   try
   {
      Ptr<BitmapInfo>& info(_bitmapInfo.get(&bmp));
      return info->textureName;
   }
   catch (NotFoundException&) { return 0; }   
}

void
GLTextureStore::updateTextureName(BitmapInfo& info)
{
   releaseTextureName(info);

   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &info.textureName);
   glBindTexture(GL_TEXTURE_2D, info.textureName);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
   GLsizei width = info.bitmap->pitch().x, height = info.bitmap->pitch().y;   
   GLenum pixelFormat, pixelType;
   toGLPixelFormat(info.bitmap->pixelFormat(), pixelFormat, pixelType);
   const GLvoid* data = info.bitmap->pixels();
   
   glTexImage2D(GL_TEXTURE_2D,      // the target
                0,                  // texture level
                GL_RGBA,            // internal format
                width,              // image width
                height,             // image height
                0,                  // border width
                pixelFormat,        // image format
                pixelType,          // pixel data type
                data);              // pixel data      

   /* TODO:
    * For some unknown reason, glTexImage2D is not updating glError properly.
    * In my 10.7 laptop, it indicates a GL_INVALID_FRAMEBUFFER_OPERATION but
    * it loads the bitmap succesfully after all. For now, this check is only 
    * enabled for the rest of platforms until the problem is located.
    */
   #if KAREN_PLATFORM != KAREN_PLATFORM_OSX
   GLenum error = glGetError();
   if (error != GL_NO_ERROR)
      KAREN_THROW(InternalErrorException,
         "cannot create GL texture for given bitmap: %s",
         gluErrorString(error));
   #endif
}

void
GLTextureStore::releaseTextureName(BitmapInfo& info)
{
   glEnable(GL_TEXTURE_2D);
   if (info.textureName && glIsTexture(info.textureName))
   {
      glDeleteTextures(1, &info.textureName);
      info.textureName = 0;
   }
}

OpenGLCanvas::OpenGLCanvas(
         const DrawingContext& parentContext, 
         const IVector& size)
 : Canvas(parentContext), _size(size)
{
   glEnable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);

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
   DVector vector(line.endPos - line.beginPos);
   float mod = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
   DVector normal(vector.y / mod, -vector.x / mod);
   
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
   DVector path = arc.endPos - arc.beginPos;  // The begin-end line
   DVector npath(path.y, -path.x);            // Normal vector...
   npath.normalise();                        // ...unitary
   
   // Calculate the axis point
   DVector mid = arc.beginPos + (path * 0.5f);     // middle point of path
   DVector axis = mid + (npath * arc.radious);     // Final axis point
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
      
      DVector pathCut = arc.beginPos + (path * (i / ((float) arc.precision - 1)));
      DVector dir = (pathCut - axis).normalise();
      DVector p1 = axis + (dir * (rad + arc.lineWidth * 0.5f));
      DVector p2 = axis + (dir * (rad - arc.lineWidth * 0.5f));

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
      DVector orig;
      DVector dest;
      DVector ctrl1;
      DVector ctrl2;
      
      inline bool operator == (const BezierCurve& bc) const
      { return orig == bc.orig && dest == bc.dest && 
               ctrl1 == bc.ctrl1 && ctrl2 == bc.ctrl2; }
   };
   
   DynArray<BezierCurve> curves(numPoints - 1);
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

      GLdouble cp[2][4][3] = { 
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
      
      glMap2d(GL_MAP2_VERTEX_3, 
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
   glPolygonMode(GL_FRONT_AND_BACK, quad.fill ? GL_FILL : GL_LINE);

   glBegin(GL_QUADS);
      for (int i = 0; i < 4; i++)
      {
         glColor4f(quad.vertexColor[i].r / 255.0f, 
                   quad.vertexColor[i].g / 255.0f, 
                   quad.vertexColor[i].b / 255.0f, 
                   quad.vertexColor[i].a / 255.0f);
         glVertex2f(quad.vertex[i].x, quad.vertex[i].y);
      }
   glEnd();
}

void
OpenGLCanvas::drawImage(const ImageParams& img)
{
   if (&img.bitmap->lockCoordinator() != &_textureStore)
      img.bitmap->setLockCoordinator(&_textureStore);
   
   GLuint texName = _textureStore.textureName(*img.bitmap);
   if (!glIsTexture(texName))
      KAREN_THROW(InternalErrorException, 
         "cannot draw image: the GL texture store contains "
         "an invalid texture name for that bitmap");
   
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texName);
   
   DVector imgSize = img.bitmap->size();
   DVector imgPitch = img.bitmap->pitch();
   
   double imgW = imgSize.x / imgPitch.x, 
          imgH = imgSize.y / imgPitch.y;
   
   double canvasX = img.canvasRect.x,
          canvasY = img.canvasRect.y,
          canvasW = img.canvasRect.w,
          canvasH = img.canvasRect.h;
   
   // This implementation just provides a DISPLAY_MODE_SCALE display mode
   // TODO: implement this to support other display modes
   glBegin(GL_QUADS);
      glTexCoord2d(0.0, 0.0);
      glVertex2d(canvasX, canvasY);
      glTexCoord2d(imgW, 0.0);
      glVertex2d(canvasX + canvasW, canvasY);
      glTexCoord2d(imgW, imgH);
      glVertex2d(canvasX + canvasW, canvasY + canvasH);
      glTexCoord2d(0.0, imgH);
      glVertex2d(canvasX, canvasY + canvasH);
   glEnd();
}

void
OpenGLCanvas::drawText(const TextParams& txt)
{
   /* TODO: code this. */
}

}}}; /* namespace karen::ui::core */
