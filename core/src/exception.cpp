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

#include "KarenCore/exception.h"
#include "KarenCore/string.h"

namespace karen {

Exception::Exception(
   const String& cause, 
   const String& sourceFile,
   long sourceLine,
   Exception* nestedException)
{
   this->init(cause, sourceFile, sourceLine, nestedException);
}

Exception::Exception(
   const String& cause,
   const String& sourceFile,
   long sourceLine) 
{
   this->init(cause, sourceFile, sourceLine, nullptr);
}

Exception::Exception(
   const String& cause,
   Exception* nestedException) 
{
   this->init(cause, "unknown file", -1, nestedException);
}

Exception::~Exception()
throw ()
{
   delete _cause;
   delete _sourceFile;
}

void Exception::init(const String &cause,
                     const String &sourceFile,
                     long sourceLine,
                     Exception *nestedException)
{
   _cause = new String(
            nestedException ? cause + "\n" + nestedException->cause() : cause);
   _sourceFile = new String(sourceFile);
   _sourceLine = sourceLine;
   _nestedException = nestedException;
}

};
