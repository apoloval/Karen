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

class BufferTestSuite : public UnitTestSuite
{
public:

   BufferTestSuite()
      : UnitTestSuite("Buffers")
   {
      KAREN_UTEST_ADD(BufferTestSuite::shouldInitiateBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldInitiateFromMemoryRegion);
      KAREN_UTEST_ADD(BufferTestSuite::shouldIndicateRightBoundaries);
      KAREN_UTEST_ADD(BufferTestSuite::shouldReadWholeBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldReadFirstPartOfBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldReadLastPartOfBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldReadFromTheMiddleOfBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldFailWhileReadingBeyondBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldWriteWholeBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldWriteToFirstPartOfBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldWriteToLastPartOfBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldFailWhileWritingBeyondBuffer);
      KAREN_UTEST_ADD(BufferTestSuite::shouldSetData);
      KAREN_UTEST_ADD(BufferTestSuite::shouldBeCleanAfterInitialization);
      KAREN_UTEST_ADD(BufferTestSuite::shouldBeDirtyAfterWrite);
      KAREN_UTEST_ADD(BufferTestSuite::shouldBeDirtyAfterSet);
      KAREN_UTEST_ADD(BufferTestSuite::shouldBeCleanAfterMarkedAsClean);
      KAREN_UTEST_ADD(BufferTestSuite::shouldSetLastElement);
   }
   
   void shouldInitiateBuffer()
   {
      Buffer buf(1024);
      KAREN_UTEST_ASSERT(buf.length() == 1024);
   }
   
   void shouldInitiateFromMemoryRegion()
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      KAREN_UTEST_ASSERT(buf.length() == 64);
      for (int i = 0; i < 64; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
   }
   
   void shouldIndicateRightBoundaries()
   {
      Buffer buf(1024);
      KAREN_UTEST_ASSERT(buf.isValidRange(0, 1024));
      KAREN_UTEST_ASSERT(buf.isValidRange(0, 512));
      KAREN_UTEST_ASSERT(buf.isValidRange(1, 512));
      KAREN_UTEST_ASSERT(buf.isValidRange(512, 0));
      KAREN_UTEST_ASSERT(buf.isValidRange(1024, 0));
      KAREN_UTEST_ASSERT(!buf.isValidRange(1025, 0));
      KAREN_UTEST_ASSERT(!buf.isValidRange(1024, 64));
      KAREN_UTEST_ASSERT(!buf.isValidRange(512, 1024));
   }
   
   void shouldReadWholeBuffer() 
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[64];
      buf.read(dst, 64);
      
      for (int i = 0; i < 64; i++)
         KAREN_UTEST_ASSERT(data[i] == dst[i]);
   }
   
   void shouldReadFirstPartOfBuffer() 
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16);
      
      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(data[i] == dst[i]);
   }
   
   void shouldReadLastPartOfBuffer() 
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16, 48);
      
      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(data[i + 48] == dst[i]);
   }
   
   void shouldReadFromTheMiddleOfBuffer()
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[32];
      buf.read(dst, 32, 16);
      
      for (int i = 0; i < 32; i++)
         KAREN_UTEST_ASSERT(data[i + 16] == dst[i]);
   }
   
   void shouldFailWhileReadingBeyondBuffer()
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[32];
      try
      { 
         buf.read(dst, 32, 48);
         KAREN_UTEST_FAILED("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
   }
   
   void shouldWriteWholeBuffer()
   {
      UInt8* data = allocRawBuffer(64);
      Buffer buf(64);
      buf.write(data, 64);

      for (int i = 0; i < 32; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
      delete []data;
   }
   
   void shouldWriteToFirstPartOfBuffer()
   {
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16);

      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
      delete []data;
   }
   
   void shouldWriteToLastPartOfBuffer()
   {
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16, 48);

      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i + 48) == data[i]);
      delete []data;
   }
   
   void shouldFailWhileWritingBeyondBuffer()
   {
      UInt8* data = allocRawBuffer(32);
      Buffer buf(64);
      try
      { 
         buf.write(data, 32, 48);
         KAREN_UTEST_FAILED("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
      delete data;
   }
   
   void shouldSetData()
   {
      Buffer buf(64);
      buf.set<UInt8>(7, 32);
      buf.set<UInt32>(9, 16);
      KAREN_UTEST_ASSERT(buf.get<UInt8>(32) == 7);
      KAREN_UTEST_ASSERT(buf.get<UInt32>(16) == 9);
   }
   
   void shouldSetLastElement()
   {
      Buffer buf(64);
      buf.set<UInt8>(7, 63);
   }
   
   void shouldFailWhileSettingBeyondBuffer()
   {
      Buffer buf(64);
      try
      {
         buf.set<UInt8>(7, 128);
         KAREN_UTEST_FAILED("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
   }
   
   void shouldBeCleanAfterInitialization()
   {
      Buffer buf(64);
      KAREN_UTEST_ASSERT(!buf.isDirty());
   }
   
   void shouldBeDirtyAfterWrite()
   {
      Buffer buf(64);
      UInt8* data = allocRawBuffer(32);
      buf.write(data, 32);
      KAREN_UTEST_ASSERT(buf.isDirty());
      delete []data;
   }
   
   void shouldBeDirtyAfterSet()
   {
      Buffer buf(64);
      buf.set<UInt8>(32, 7);
      KAREN_UTEST_ASSERT(buf.isDirty());
   }
   
   void shouldBeCleanAfterMarkedAsClean()
   {
      Buffer buf(64);
      buf.set<UInt8>(32, 7);
      buf.markAsClean();
      KAREN_UTEST_ASSERT(!buf.isDirty());
   }
   
private:

   static UInt8* allocRawBuffer(unsigned long len)
   {
      UInt8* data = new UInt8[len];
      for (unsigned long i = 0; i < len; i++)
         data[i] = (i % 256);
      return data;
   }
   
};

class BufferStreamsTestSuite : public UnitTestSuite
{
public:

   BufferStreamsTestSuite()
      : UnitTestSuite("Buffer Streams")
   {
      KAREN_UTEST_ADD(BufferStreamsTestSuite::shouldReadFromInputStream);
      KAREN_UTEST_ADD(BufferStreamsTestSuite::shouldFailWhileReadingBeyondInputStream);
      KAREN_UTEST_ADD(BufferStreamsTestSuite::shouldWriteToOutputStream);
      KAREN_UTEST_ADD(BufferStreamsTestSuite::shouldFailWhileWritingBeyondOutputStream);
   }
   
   void shouldReadFromInputStream()
   {
      Ptr<Buffer> buf = allocBuffer(64);
      BufferInputStream bis(buf);
      for (int i = 0; i < 64; i++)
      {
         KAREN_UTEST_ASSERT(bis.bytesLeftToRead() == (64 - i));
         KAREN_UTEST_ASSERT(bis.read<UInt8>() == i);
      }
   }
   
   void shouldFailWhileReadingBeyondInputStream()
   {
      Ptr<Buffer> buf = allocBuffer(64);
      BufferInputStream bis(buf);
      while (bis.bytesLeftToRead())
         bis.read<UInt8>();
      try
      {
         bis.read<UInt8>();
         KAREN_UTEST_FAILED("expected invalid state exception not raised");
      }
      catch (IOException& e) {}
   }
   
   void shouldWriteToOutputStream()
   {
      Buffer buf(64);
      BufferOutputStream bos(&buf);
      for (int i = 0; i < 64; i++)
      {
         KAREN_UTEST_ASSERT(bos.bytesLeftToWrite() == 64 - i);
         bos.write<UInt8>(i);
      }
      for (int i = 0; i < 64; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == i);
   }
   
   void shouldFailWhileWritingBeyondOutputStream()
   {
      Buffer buf(64);
      BufferOutputStream bos(&buf);
      for (int i = 0; i < 64; i++)
         bos.write<UInt8>(i);
      try
      {
         bos.write<UInt8>(7);
         KAREN_UTEST_FAILED("expected invalid state exception not raised");
      } catch (IOException&) {}
   }
   
private:

   static karen::Buffer* allocBuffer(unsigned long len)
   {
      UInt8* data = new UInt8[len];
      for (unsigned long i = 0; i < len; i++)
         data[i] = (i % 256);
      return new Buffer(data, len);
   }
   
};

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
