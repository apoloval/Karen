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

#include <KarenUI/image.h>
#include <KarenCore/test.h>

using namespace karen;

static ui::Vector SMALL_IMG_SIZE(3, 3);

class ImageBlendingTest : public UnitTestSuite
{
public:

   ImageBlendingTest() : UnitTestSuite("Image blending")
   {
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillGreyscale8WithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillGreyscale16WithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillRGBWithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillRGBAWithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillABGRWithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldFillRGBARegionWithColor);
      KAREN_UTEST_ADD(ImageBlendingTest::shouldBlendFullRowWithSamePixelFormat);
   }
   
   void shouldFillGreyscale8WithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_8BPP_GREYSCALE);

      ui::ImageBlender::fillWithColor(img, ui::Color::RED);
      UInt8 epix = 0xff * 0.3;
      const UInt8* pixels = (const UInt8*) img.pixels();
      const ui::Vector& pitch = img.pitch();
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {            
            UInt8 pix = pixels[(unsigned long) (j * pitch.x + i)];
            KAREN_UTEST_ASSERT(pix == epix);
         }
   }
   
   void shouldFillGreyscale16WithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_16BPP_GREYSCALE);

      ui::ImageBlender::fillWithColor(img, ui::Color::RED);
      UInt16 epix = 0xff * 0.3 * 255;
      const UInt16* pixels = (const UInt16*) img.pixels();
      const ui::Vector& pitch = img.pitch();
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {
            UInt16 pix = pixels[(unsigned long) (j * pitch.x + i)];
            KAREN_UTEST_ASSERT(pix == epix);
         }
   }
   
   void shouldFillRGBWithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_24BPP_RGB);

      ui::ImageBlender::fillWithColor(img, ui::Color::RED);
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {
            const ui::Color& col = img.pixelAt(ui::Vector(i, j));
            KAREN_UTEST_ASSERT(col == ui::Color::RED);
         }
   }
   
   void shouldFillRGBAWithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_32BPP_RGBA);

      ui::ImageBlender::fillWithColor(img, ui::Color::RED);
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {
            const ui::Color& col = img.pixelAt(ui::Vector(i, j));
            KAREN_UTEST_ASSERT(col == ui::Color::RED);
         }
   }

   void shouldFillABGRWithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_32BPP_ABGR);

      ui::ImageBlender::fillWithColor(img, ui::Color::CYAN);
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {
            const ui::Color& col = img.pixelAt(ui::Vector(i, j));
            KAREN_UTEST_ASSERT(col == ui::Color::CYAN);
         }
   }

   void shouldFillRGBARegionWithColor()
   {
      ui::Image img(SMALL_IMG_SIZE, ui::PixelFormat::FORMAT_32BPP_RGBA);

      ui::ImageBlender::fillWithColor(img, ui::Color::BLACK);
      ui::ImageBlender::fillRegionWithColor(
            img, ui::Rect(0, 1, 3, 1), ui::Color::RED);
      for (int j = 0; j < SMALL_IMG_SIZE.y; j++)
         for (int i = 0; i < SMALL_IMG_SIZE.x; i++)
         {
            const ui::Color& col = img.pixelAt(ui::Vector(i, j));
            if (j % 2 == 0)
            {
               KAREN_UTEST_ASSERT(col == ui::Color::BLACK);
            }
            else
            {
               KAREN_UTEST_ASSERT(col == ui::Color::RED);
            }
         }
   }

   void shouldBlendFullRowWithSamePixelFormat()
   {
      ui::Image srcImg(ui::Vector(5, 3), ui::PixelFormat::FORMAT_32BPP_RGBA);
      ui::Image dstImg(ui::Vector(5, 3), ui::PixelFormat::FORMAT_32BPP_RGBA);
      // ui::ImageBlender::blend(srcImg, dstImg);
   }

};

class FontRenderTest : public UnitTestSuite
{
public:

   FontRenderTest() : UnitTestSuite("Font render provider")
   {
   }

};

int
main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   {
      ImageBlendingTest test;
      test.run(&rep, NULL, 0);
   }
   {
      FontRenderTest test;
      test.run(&rep, NULL, 0);
   }
   return 0;
}