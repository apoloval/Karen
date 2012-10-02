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

#include <KarenCore/buffer.h>
#include <KarenCore/pointer.h>
#include <KarenCore/stream.h>
#include <KarenCore/test.h>

using namespace karen;

static UInt8* allocRawBuffer(unsigned long len)
{
   UInt8* data = new UInt8[len];
   for (unsigned long i = 0; i < len; i++)
      data[i] = (i % 256);
   return data;
}

static karen::Buffer* allocBuffer(unsigned long len)
{
   UInt8* data = allocRawBuffer(len);
   return new Buffer(data, len);
}

KAREN_BEGIN_UNIT_TEST(BufferTestSuite);

   KAREN_DECL_TEST(shouldInitiateBuffer,
   {
      Buffer buf(1024);
      assertEquals<int>(1024, buf.length());
   });
   
   KAREN_DECL_TEST(shouldInitiateFromMemoryRegion,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      assertEquals<int>(64, buf.length());
      for (int i = 0; i < 64; i++)
         assertEquals<int>(data[i], buf.get<UInt8>(i));
   });
   
   KAREN_DECL_TEST(shouldIndicateRightBoundaries,
   {
      Buffer buf(1024);
      assertTrue(buf.isValidRange(0, 1024));
      assertTrue(buf.isValidRange(0, 512));
      assertTrue(buf.isValidRange(1, 512));
      assertTrue(buf.isValidRange(512, 0));
      assertTrue(buf.isValidRange(1024, 0));
      assertFalse(buf.isValidRange(1025, 0));
      assertFalse(buf.isValidRange(1024, 64));
      assertFalse(buf.isValidRange(512, 1024));
   });
   
   KAREN_DECL_TEST(shouldReadWholeBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[64];
      buf.read(dst, 64);
      
      for (int i = 0; i < 64; i++)
         assertEquals<int>(dst[i], data[i]);
   });
   
   KAREN_DECL_TEST(shouldReadFirstPartOfBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16);
      
      for (int i = 0; i < 16; i++)
         assertEquals<int>(dst[i], data[i]);
   });
   
   KAREN_DECL_TEST(shouldReadLastPartOfBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16, 48);
      
      for (int i = 0; i < 16; i++)
         assertEquals<int>(dst[i], data[i + 48]);
   });
   
   KAREN_DECL_TEST(shouldReadFromTheMiddleOfBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[32];
      buf.read(dst, 32, 16);
      
      for (int i = 0; i < 32; i++)
         assertEquals<int>(dst[i], data[i + 16]);
   });
   
   KAREN_DECL_TEST(shouldFailWhileReadingBeyondBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[32];
      try
      { 
         buf.read(dst, 32, 48);
         assertionFailed("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
   });
   
   KAREN_DECL_TEST(shouldWriteWholeBuffer,
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(64);
      buf.write(data, 64);

      for (int i = 0; i < 32; i++)
         assertEquals<int>(data[i], buf.get<UInt8>(i));
      delete []data;
   });
   
   KAREN_DECL_TEST(shouldWriteToFirstPartOfBuffer,
   {
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16);

      for (int i = 0; i < 16; i++)
         assertEquals<int>(data[i], buf.get<UInt8>(i));
      delete []data;
   });
   
   KAREN_DECL_TEST(shouldWriteToLastPartOfBuffer,
   {
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16, 48);

      for (int i = 0; i < 16; i++)
         assertEquals<int>(data[i], buf.get<UInt8>(i + 48));
      delete []data;
   });
   
   KAREN_DECL_TEST(shouldFailWhileWritingBeyondBuffer,
   {
      UInt8* data = allocRawBuffer(32);
      Buffer buf(64);
      try
      { 
         buf.write(data, 32, 48);
         assertionFailed("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
      delete data;
   });
   
   KAREN_DECL_TEST(shouldSetData,
   {
      Buffer buf(64);
      buf.set<UInt8>(7, 32);
      buf.set<UInt32>(9, 16);
      assertEquals<int>(7, buf.get<UInt8>(32));
      assertEquals<int>(9, buf.get<UInt32>(16));
   });
   
   KAREN_DECL_TEST(shouldSetLastElement,
   {
      Buffer buf(64);
      buf.set<UInt8>(7, 63);
   });
   
   KAREN_DECL_TEST(shouldFailWhileSettingBeyondBuffer,
   {
      Buffer buf(64);
      try
      {
         buf.set<UInt8>(7, 128);
         assertionFailed("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
   });
   
   KAREN_DECL_TEST(shouldBeCleanAfterInitialization,
   {
      Buffer buf(64);
      assertFalse(buf.isDirty());
   });
   
   KAREN_DECL_TEST(shouldBeDirtyAfterWrite,
   {
      Buffer buf(64);
      UInt8* data = allocRawBuffer(32);
      buf.write(data, 32);
      assertTrue(buf.isDirty());
      delete []data;
   });
   
   KAREN_DECL_TEST(shouldBeDirtyAfterSet,
   {
      Buffer buf(64);
      buf.set<UInt8>(32, 7);
      assertTrue(buf.isDirty());
   });
   
   KAREN_DECL_TEST(shouldBeCleanAfterMarkedAsClean,
   {
      Buffer buf(64);
      buf.set<UInt8>(32, 7);
      buf.markAsClean();
      assertFalse(buf.isDirty());
   });
      
KAREN_END_UNIT_TEST(BufferTestSuite);

KAREN_BEGIN_UNIT_TEST(BufferStreamsTestSuite);

   KAREN_DECL_TEST(shouldReadFromInputStream,
   {
      Ptr<Buffer> buf = allocBuffer(64);
      BufferInputStream bis(buf);
      for (int i = 0; i < 64; i++)
      {
         assertEquals<int>((64 - i), bis.bytesLeftToRead());
         assertEquals<int>(i, bis.read<UInt8>());
      }
   });
   
   KAREN_DECL_TEST(shouldFailWhileReadingBeyondInputStream,
   {
      Ptr<Buffer> buf = allocBuffer(64);
      BufferInputStream bis(buf);
      while (bis.bytesLeftToRead())
         bis.read<UInt8>();
      try
      {
         bis.read<UInt8>();
         assertionFailed("expected invalid state exception not raised");
      }
      catch (IOException& e) {}
   });
   
   KAREN_DECL_TEST(shouldWriteToOutputStream,
   {
      Buffer buf(64);
      BufferOutputStream bos(&buf);
      for (int i = 0; i < 64; i++)
      {
         assertEquals<int>(64 - i, bos.bytesLeftToWrite());
         bos.write<UInt8>(i);
      }
      for (int i = 0; i < 64; i++)
         assertEquals<int>(i, buf.get<UInt8>(i));
   });
   
   KAREN_DECL_TEST(shouldFailWhileWritingBeyondOutputStream,
   {
      Buffer buf(64);
      BufferOutputStream bos(&buf);
      for (int i = 0; i < 64; i++)
         bos.write<UInt8>(i);
      try
      {
         bos.write<UInt8>(7);
         assertionFailed("expected invalid state exception not raised");
      } catch (IOException&) {}
   });

KAREN_END_UNIT_TEST(BufferStreamsTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   {
      BufferTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      BufferStreamsTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
}
