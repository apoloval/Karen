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

#ifndef KAREN_UTILS_PARSING_H
#define KAREN_UTILS_PARSING_H

#include "collection.h"
#include "string.h"

namespace karen { namespace utils {

/**
 * String tokenizer class. This class provides functionality to tokenize
 * a string (i.e., split a string in tokens by searching for certain 
 * separator).
 */
class StringTokenizer
{
public:

   /**
    * Split this string in tokens considering given separator.
    */
   static Array<String> tokenize(
         const String& str, 
         String::Element separator = ' '); 
   
};

}}; // namespace karen::utils

#endif