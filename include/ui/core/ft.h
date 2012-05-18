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

#ifndef KAREN_UI_CORE_FT_H
#define KAREN_UI_CORE_FT_H

#if KAREN_PLATFORM == KAREN_PLATFORM_OSX
#include <FreeType/ft2build.h>
#include FT_FREETYPE_H
#endif

#include "ui/bitmap.h"
#include "utils/pointer.h"

namespace karen { namespace ui { namespace core {

/**
 * Free type renderer class. This class provides a font renderer based on
 * FreeType 2 library. 
 */
class FreeTypeRenderer
{
public:

   /*
    * Create a new rendeder with given properties. If given font is not found,
    * or given size is not valid, InvalidInputException is thrown.
    */
   FreeTypeRenderer(const String& font, 
                    unsigned int fontSize, 
                    const Color& fontColor)
         throw (utils::InvalidInputException);
   
   /**
    * Destroy the renderer and release its resources. 
    */
   virtual ~FreeTypeRenderer();
   
   /**
    * Render text into a new image. The contents of text object are rendered
    * into a new image by using given font of given size. 
    */
   void render(const String& text,
               const String& fontLocation,
               unsigned int fontSize,
               Bitmap& renderTo) throw (utils::InvalidInputException);

private:

   void blitToImage(Bitmap& target) throw (utils::InvalidStateException);
         
   DVector        _pen;   
   unsigned int   _fontSize;
   Color          _fontColor;
   
   FT_Library     _ft;
   FT_Face        _face;

};

}}}; /* Namespace karen::ui::core */

#endif
