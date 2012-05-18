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

#include "ui/core/ft.h"
#include "utils/exception.h"
#include "utils/platform.h"

namespace karen { namespace ui { namespace core {

FreeTypeRenderer::FreeTypeRenderer(
   const String& font, 
   unsigned int fontSize, 
   const Color& fontColor)
throw (utils::InvalidInputException)
 : _pen(0.0, 0.0),
   _fontSize(fontSize),
   _fontColor(fontColor)
{
   FT_Error err = FT_Init_FreeType(&_ft);
   if (err)
      KAREN_THROW(utils::InternalErrorException, 
         "cannot initialize FreeType2 library for unknown reasons");

   err = FT_New_Face(_ft, font, 0, &_face);
   if (err == FT_Err_Unknown_File_Format)
   {
      KAREN_THROW(utils::InvalidInputException, 
            "cannot render text: unknow file format for given font file");
   }
   else if (err == FT_Err_Cannot_Open_Resource)
   {
      KAREN_THROW(utils::InvalidInputException, 
            "cannot render text: font file not found");
   }
   else if (err)
   {
      KAREN_THROW(utils::InvalidInputException, 
            "cannot render text: unexpected error %d while opening font %s",
            err,
            (const char*) font);
   }
   err = FT_Set_Char_Size(_face, 0, _fontSize * 64, 300, 300);
   if (err)
   {
      KAREN_THROW(utils::InvalidInputException, 
            "cannot render text: unexpected error %d while setting font size",
            err);
   }
}

FreeTypeRenderer::~FreeTypeRenderer()
{
   FT_Done_Face(_face);
   FT_Done_FreeType(_ft);
}

void
FreeTypeRenderer::render(
   const String& text,
   const String& fontLocation,
   unsigned int fontSize,
   Bitmap& renderTo)
throw (utils::InvalidInputException)
{
   FT_Error err;
   const char* rawText = text;
   for (int i = 0; i < text.length(); i++)
   {
      err = FT_Load_Char(_face, rawText[i], FT_LOAD_RENDER);
      if (err)
         KAREN_THROW(utils::InvalidInputException, 
               "cannot render text: error code %d while rendering symbol %d; ",
               err, rawText[i]);
      blitToImage(renderTo);
   }
}

void
FreeTypeRenderer::blitToImage(Bitmap& target)
throw (utils::InvalidStateException)
{
   FT_GlyphSlot slot = _face->glyph;
   if (slot->format != FT_GLYPH_FORMAT_BITMAP)
      KAREN_THROW(utils::InvalidStateException, 
            "cannot render text: rendered character is not a bitmap");
   
   Color pixCol;
   for (int i = slot->bitmap_left; i < slot->bitmap.width; i++)
      for (int j = 0; j < slot->bitmap.rows; j++)
      {
         unsigned long offset = (j * slot->bitmap.pitch) + i;
         pixCol.r = pixCol.g = pixCol.b = slot->bitmap.buffer[offset];
         // TODO: fix this: target.setPixelAt(_pen, pixCol);
      }
   _pen.x += slot->advance.x >> 6;
}

}}}; /* Namespace karen::ui::core */

