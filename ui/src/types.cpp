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

#include "KarenUI/types.h"

#include <cmath>

namespace karen { namespace ui {

KAREN_EXPORT const Color Color::BLACK   (0x00, 0x00, 0x00);
KAREN_EXPORT const Color Color::WHITE   (0xff, 0xff, 0xff);
KAREN_EXPORT const Color Color::RED     (0xff, 0x00, 0x00);
KAREN_EXPORT const Color Color::GREEN   (0x00, 0xff, 0x00);
KAREN_EXPORT const Color Color::BLUE    (0x00, 0x00, 0xff);
KAREN_EXPORT const Color Color::YELLOW  (0xff, 0xff, 0x00);
KAREN_EXPORT const Color Color::CYAN    (0x00, 0xff, 0xff);
KAREN_EXPORT const Color Color::MAGENTA (0xff, 0x00, 0xff);
KAREN_EXPORT const Color Color::GREY    (0xc0, 0xc0, 0xc0);

#if (KAREN_ENDIANNESS == KAREN_BIG_ENDIAN)
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_8BPP_GREYSCALE(
   PixelFormat::Mask(0xff000000, 0xff000000, 0xff000000, 0x00000000), 
   PixelFormat::Shift(24, 24, 24, 0),
   8);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_16BPP_GREYSCALE(
   PixelFormat::Mask(0xffff0000, 0xffff0000, 0xffff0000, 0x00000000), 
   PixelFormat::Shift(16, 16, 16, 0),
   16);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_24BPP_RGB(
   PixelFormat::Mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000), 
   PixelFormat::Shift(24, 16, 8, 0),
   24);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_24BPP_BGR(
   PixelFormat::Mask(0x0000ff00, 0x00ff0000, 0xff000000, 0x00000000),
   PixelFormat::Shift(8, 16, 24, 0),
   24);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_RGBX(
   PixelFormat::Mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000), 
   PixelFormat::Shift(24, 16, 8, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_XRGB(
   PixelFormat::Mask(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000),
   PixelFormat::Shift(16, 8, 0, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_BGRX(
   PixelFormat::Mask(0x0000ff00, 0x00ff0000, 0xff000000, 0x00000000), 
   PixelFormat::Shift(8, 16, 24, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_XBGR(
   PixelFormat::Mask(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000), 
   PixelFormat::Shift(0, 8, 16, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_RGBA(
   PixelFormat::Mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff), 
   PixelFormat::Shift(24, 16, 8, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_ABGR(
   PixelFormat::Mask(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000), 
   PixelFormat::Shift(0, 8, 16, 24),
   32);
#else
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_8BPP_GREYSCALE(
   PixelFormat::Mask(0x000000ff, 0x000000ff, 0x000000ff, 0x00000000), 
   PixelFormat::Shift(0, 0, 0, 0),
   8);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_16BPP_GREYSCALE(
   PixelFormat::Mask(0x0000ffff, 0x0000ffff, 0x0000ffff, 0x00000000), 
   PixelFormat::Shift(0, 0, 0, 0),
   16);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_24BPP_RGB(
   PixelFormat::Mask(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000), 
   PixelFormat::Shift(0, 8, 16, 0),
   24);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_24BPP_BGR(
   PixelFormat::Mask(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000), 
   PixelFormat::Shift(16, 8, 0, 0),
   24);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_RGBX(
   PixelFormat::Mask(0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000), 
   PixelFormat::Shift(0, 8, 16, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_XRGB(
   PixelFormat::Mask(0x0000ff00, 0x00ff0000, 0xff000000, 0x00000000), 
   PixelFormat::Shift(8, 16, 24, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_BGRX(
   PixelFormat::Mask(0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000), 
   PixelFormat::Shift(16, 8, 0, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_XBGR(
   PixelFormat::Mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x00000000), 
   PixelFormat::Shift(24, 16, 8, 0),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_RGBA(
   PixelFormat::Mask(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000), 
   PixelFormat::Shift(0, 8, 16, 24),
   32);
KAREN_EXPORT const PixelFormat PixelFormat::FORMAT_32BPP_ABGR(
   PixelFormat::Mask(0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff), 
   PixelFormat::Shift(24, 16, 8, 0),
   32);
#endif

}} /* Namespace karen::ui */
