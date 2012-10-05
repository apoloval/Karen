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

#ifndef KAREN_CORE_PARSING_H
#define KAREN_CORE_PARSING_H

#include "KarenCore/collection.h"
#include "KarenCore/string.h"

namespace karen {

/**
 * String tokenizer. Tokenizes a string, spliting it in tokens by searching 
 * for certain separator.
 */
KAREN_EXPORT void tokenizeString(const String& str, 
                                 Array<String>& tokens,
                                 char separator = ' '); 

}; // namespace karen

#endif
