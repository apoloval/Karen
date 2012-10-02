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

#include <KarenCore/file.h>
#include <KarenCore/test.h>

using namespace karen;

KAREN_BEGIN_UNIT_TEST(FileTestSuite);

   KAREN_DECL_TEST(shouldCreateFile,
   {
      File f("/tmp/foobar1", FileOpenMode::CREATE_AND_WRITE_MODE);      
   });
   
   KAREN_DECL_TEST(shouldWriteAndWriteFile,
   {
      const String masterMessage("Hello World!");
      {
         File f("/tmp/foobar1", FileOpenMode::WRITE_ONLY_MODE);
         f.write<String>(masterMessage);
      }
      {
         File f("/tmp/foobar1", FileOpenMode::READ_ONLY_MODE);
         String msg = f.read<String>();
         assertEquals<String>(masterMessage, msg);
      }
   });
   
   KAREN_DECL_TEST(shouldFailWhenNotCreatingAndFileDoesNotExist,
   {
      try
      { 
         File f("/tmp/foobar2", FileOpenMode::WRITE_ONLY_MODE);
         assertionFailed("Expected IOException not thrown");
      }
      catch (IOException&) {}
   });
   
   KAREN_DECL_TEST(shouldFailWhenReadingNotReadableFile,
   {
      try
      { 
         File f("/tmp/foobar1", FileOpenMode::WRITE_ONLY_MODE);
         f.read<UInt8>();
         assertionFailed("Expected IOException not thrown");
      }
      catch (IOException&) {}
   });

   KAREN_DECL_TEST(shouldFailWhenWritingNotWriteableFile,
   {
      try
      { 
         File f("/tmp/foobar1", FileOpenMode::READ_ONLY_MODE);
         f.write<UInt8>(7);
         assertionFailed("Expected IOException not thrown");
      }
      catch (IOException&) {}
   });
   
KAREN_END_UNIT_TEST(FileTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   FileTestSuite suite;
   suite.run(&rep, NULL, 0);
}
