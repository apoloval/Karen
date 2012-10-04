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

#include "KarenUI/color.h"

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

}}
