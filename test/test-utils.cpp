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

#include "utils/buffer.h"
#include "utils/numeric.h"
#include "utils/parsing.h"
#include "utils/string.h"
#include "utils/test.h"

using namespace karen::utils;

class StringTestSuite : public UnitTestSuite
{
public:

   StringTestSuite() : UnitTestSuite("String class")
   {
      KAREN_UTEST_ADD(StringTestSuite::createEmptyString);
      KAREN_UTEST_ADD(StringTestSuite::createStringFromNullTerminatedCharPointer);
      KAREN_UTEST_ADD(StringTestSuite::createStringFromOtherString);
      KAREN_UTEST_ADD(StringTestSuite::appendToSingleString);
      KAREN_UTEST_ADD(StringTestSuite::appendToDeduplicatedString);
      KAREN_UTEST_ADD(StringTestSuite::appendCharsToSingleString);
      KAREN_UTEST_ADD(StringTestSuite::appendCharsToDeduplicatedString);
      KAREN_UTEST_ADD(StringTestSuite::concatStrings);
      KAREN_UTEST_ADD(StringTestSuite::concatNullString);
      KAREN_UTEST_ADD(StringTestSuite::concatStringWithChars);
      KAREN_UTEST_ADD(StringTestSuite::concatStringWithZeroChars);
      KAREN_UTEST_ADD(StringTestSuite::obtainSlice);
      KAREN_UTEST_ADD(StringTestSuite::obtainSliceOutOfRange);
      KAREN_UTEST_ADD(StringTestSuite::obtainHead);
      KAREN_UTEST_ADD(StringTestSuite::obtainHeadOutOfRange);
      KAREN_UTEST_ADD(StringTestSuite::obtainTail);
      KAREN_UTEST_ADD(StringTestSuite::obtainTailOutOfRange);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHead);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHeadZero);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHeadOutOfRange);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTail);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTailZero);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTailOutOfRange);
      KAREN_UTEST_ADD(StringTestSuite::tokenizeString);
      KAREN_UTEST_ADD(StringTestSuite::tokenizeOneTokenString);
      KAREN_UTEST_ADD(StringTestSuite::tokenizeZeroLengthString);
      KAREN_UTEST_ADD(StringTestSuite::startsWithString);
      KAREN_UTEST_ADD(StringTestSuite::startsWithZeroLengthString);
      KAREN_UTEST_ADD(StringTestSuite::startsWithItself);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHeadMatching);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHeadNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::removeFromHeadByZeroLengthString);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTailMatching);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTailNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::removeFromTailByZeroLengthString);
      KAREN_UTEST_ADD(StringTestSuite::findCharFromBeginAndMatching);
      KAREN_UTEST_ADD(StringTestSuite::findCharFromBeginAndNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::findCharFromNotBeginAndMatching);
      KAREN_UTEST_ADD(StringTestSuite::findCharFromNotBeginAndNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::reverseFindCharFromEndAndMatching);
      KAREN_UTEST_ADD(StringTestSuite::reverseFindCharFromEndAndNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::reverseFindCharFromNotEndAndMatching);
      KAREN_UTEST_ADD(StringTestSuite::reverseFindCharFromNotEndAndNotMatching);
      KAREN_UTEST_ADD(StringTestSuite::capitalizeNonZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::capitalizeZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::toLowerCaseNonZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::toLowerCaseZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::toUpperCaseNonZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::toUpperCaseZeroLength);
      KAREN_UTEST_ADD(StringTestSuite::toIntPositiveBase10);
      KAREN_UTEST_ADD(StringTestSuite::toIntPositiveBase16);
      KAREN_UTEST_ADD(StringTestSuite::toIntPositiveBase10InvalidValue);
      KAREN_UTEST_ADD(StringTestSuite::toIntPositiveBase16InvalidValue);
      KAREN_UTEST_ADD(StringTestSuite::toFloatWithoutPoint);
      KAREN_UTEST_ADD(StringTestSuite::toFloatWithPoint);
      KAREN_UTEST_ADD(StringTestSuite::toFloatInvalidValue);
      KAREN_UTEST_ADD(StringTestSuite::toFloatNotCompletelyValidValue);
      KAREN_UTEST_ADD(StringTestSuite::formatZeroLengthString);
      KAREN_UTEST_ADD(StringTestSuite::formatWithEmbedString);
      KAREN_UTEST_ADD(StringTestSuite::formatWithEmbedInteger);
      KAREN_UTEST_ADD(StringTestSuite::formatWithEmbedFloat);
   }
   
   void createEmptyString()
   {
      String s;
      KAREN_UTEST_ASSERT(s.length() == 0);
      KAREN_UTEST_ASSERT(s.empty());
   }

   void createStringFromNullTerminatedCharPointer()
   {
      String s("foobar");
      KAREN_UTEST_ASSERT(s.length() == 6);
      KAREN_UTEST_ASSERT(!s.empty());
      KAREN_UTEST_ASSERT(s == "foobar");
      KAREN_UTEST_ASSERT(s != "foo");
   }
   
   void createStringFromOtherString()
   {
      String s1("foobar");
      String s2(s1);
      KAREN_UTEST_ASSERT(s1 == s2);
      KAREN_UTEST_ASSERT(s1.length() == s2.length());
   }

   void appendToSingleString()
   {
      String s("foo");
      s.append("bar");
      KAREN_UTEST_ASSERT(s == "foobar");
   }

   void appendToDeduplicatedString()
   {
      String s1("foo");
      String s2(s1);
      s1.append("bar");
      KAREN_UTEST_ASSERT(s1 == "foobar");
      KAREN_UTEST_ASSERT(s1.length() == 6);
      KAREN_UTEST_ASSERT(s2 == "foo");
      KAREN_UTEST_ASSERT(s2.length() == 3);
   }

   void appendCharsToSingleString()
   {
      String s("f");
      s.append('o', 2);
      KAREN_UTEST_ASSERT(s == "foo");
      KAREN_UTEST_ASSERT(s.length() == 3);
   }

   void appendCharsToDeduplicatedString()
   {
      String s1("f");
      String s2(s1);
      s1.append('o', 2);
      KAREN_UTEST_ASSERT(s1 == "foo");
      KAREN_UTEST_ASSERT(s1.length() == 3);
      KAREN_UTEST_ASSERT(s2 == "f");
      KAREN_UTEST_ASSERT(s2.length() == 1);
   }
   
   void concatStrings()
   {
      String s1("Welcome");
      String s2(" to the jungle");
      String s3 = s1.concat(s2);
      KAREN_UTEST_ASSERT(s1 == "Welcome");
      KAREN_UTEST_ASSERT(s2 == " to the jungle");
      KAREN_UTEST_ASSERT(s3 == "Welcome to the jungle");
   }

   void concatNullString()
   {
      String s1("Welcome");
      String s2("");
      String s3 = s1.concat(s2);
      KAREN_UTEST_ASSERT(s1 == "Welcome");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s3 == "Welcome");
   }

   void concatStringWithChars()
   {
      String s1("Welcome");
      String s2 = s1.concat(' ', 3);
      KAREN_UTEST_ASSERT(s1 == "Welcome");
      KAREN_UTEST_ASSERT(s2 == "Welcome   ");
   }
   
   void concatStringWithZeroChars()
   {
      String s1("Welcome");
      String s2 = s1.concat('X', 0);
      KAREN_UTEST_ASSERT(s1 == "Welcome");
      KAREN_UTEST_ASSERT(s2 == "Welcome");
   }
   
   void obtainSlice()
   {
      String s1("Welcome Foobar");
      String s2 = s1.slice(3, 4);
      String s3 = s1.slice(8, 6);
      String s4 = s1.slice(11, 3);
      String s5 = s1.slice(11, 6);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "come");
      KAREN_UTEST_ASSERT(s3 == "Foobar");
      KAREN_UTEST_ASSERT(s4 == "bar");
      KAREN_UTEST_ASSERT(s5 == "bar");
   }

   void obtainSliceOutOfRange()
   {
      String s1("Welcome Foobar");
      String s2 = s1.slice(8, 255);
      String s3 = s1.slice(64, 255);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Foobar");
      KAREN_UTEST_ASSERT(s3 == "");
   }

   void obtainHead()
   {
      String s1("Welcome Foobar");
      String s2 = s1.head(7);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Welcome");
   }

   void obtainHeadOutOfRange()
   {
      String s1("Welcome Foobar");
      String s2 = s1.head(255);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Welcome Foobar");
   }

   void obtainTail()
   {
      String s1("Welcome Foobar");
      String s2 = s1.tail(6);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Foobar");
   }

   void obtainTailOutOfRange()
   {
      String s1("Welcome Foobar");
      String s2 = s1.tail(255);
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Welcome Foobar");
   }

   void removeFromHead()
   {
      String s("Welcome Foobar");
      s.removeFromHead(8);
      KAREN_UTEST_ASSERT(s == "Foobar");
   }

   void removeFromHeadZero()
   {
      String s("Welcome Foobar");
      s.removeFromHead(0);
      KAREN_UTEST_ASSERT(s == "Welcome Foobar");
   }

   void removeFromHeadOutOfRange()
   {
      String s("Welcome Foobar");
      s.removeFromHead(255);
      KAREN_UTEST_ASSERT(s == "");
      KAREN_UTEST_ASSERT(s.empty());
   }

   void removeFromTail()
   {
      String s("Welcome Foobar");
      s.removeFromTail(7);
      KAREN_UTEST_ASSERT(s == "Welcome");
   }

   void removeFromTailZero()
   {
      String s("Welcome Foobar");
      s.removeFromTail(0);
      KAREN_UTEST_ASSERT(s == "Welcome Foobar");
   }

   void removeFromTailOutOfRange()
   {
      String s("Welcome Foobar");
      s.removeFromTail(255);
      KAREN_UTEST_ASSERT(s == "");
      KAREN_UTEST_ASSERT(s.empty());
   }
   
   void tokenizeString()
   {
      String s("Welcome to the jungle");
      Array<String> tokens = StringTokenizer::tokenize(s);
      KAREN_UTEST_ASSERT(tokens.size() == 4);
      KAREN_UTEST_ASSERT(tokens[0] == "Welcome");
      KAREN_UTEST_ASSERT(tokens[1] == "to");
      KAREN_UTEST_ASSERT(tokens[2] == "the");
      KAREN_UTEST_ASSERT(tokens[3] == "jungle");
   }

   void tokenizeOneTokenString()
   {
      String s("Welcome");
      Array<String> tokens = StringTokenizer::tokenize(s);
      KAREN_UTEST_ASSERT(tokens.size() == 1);
      KAREN_UTEST_ASSERT(tokens[0] == "Welcome");
   }

   void tokenizeZeroLengthString()
   {
      String s("");
      Array<String> tokens = StringTokenizer::tokenize(s);
      KAREN_UTEST_ASSERT(tokens.size() == 0);
   }
   
   void startsWithString()
   {
      String s1("Welcome Foobar");
      String s2("Welc");
      KAREN_UTEST_ASSERT(s1.startsWith(s2));
   }

   void startsWithZeroLengthString()
   {
      String s1("Welcome Foobar");
      String s2("");
      KAREN_UTEST_ASSERT(s1.startsWith(s2));
   }

   void startsWithItself()
   {
      String s1("Welcome Foobar");
      String s2("Welcome Foobar");
      KAREN_UTEST_ASSERT(s1.startsWith(s2));
   }
   
   void removeFromHeadMatching()
   {
      String s1("Welcome Foobar");
      String s2("Welcome ");
      String s3(s1.removeHead(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Welcome ");
      KAREN_UTEST_ASSERT(s3 == "Foobar");
   }

   void removeFromHeadNotMatching()
   {
      String s1("Welcome Foobar");
      String s2("Foobar");
      String s3(s1.removeHead(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Foobar");
      KAREN_UTEST_ASSERT(s3 == "Welcome Foobar");
   }

   void removeFromHeadByZeroLengthString()
   {
      String s1("Welcome Foobar");
      String s2("");
      String s3(s1.removeHead(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s3 == "Welcome Foobar");
   }

   void removeFromTailMatching()
   {
      String s1("Welcome Foobar");
      String s2(" Foobar");
      String s3(s1.removeTail(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == " Foobar");
      KAREN_UTEST_ASSERT(s3 == "Welcome");
   }

   void removeFromTailNotMatching()
   {
      String s1("Welcome Foobar");
      String s2("Kepler");
      String s3(s1.removeTail(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "Kepler");
      KAREN_UTEST_ASSERT(s3 == "Welcome Foobar");
   }

   void removeFromTailByZeroLengthString()
   {
      String s1("Welcome Foobar");
      String s2("");
      String s3(s1.removeTail(s2));
      KAREN_UTEST_ASSERT(s1 == "Welcome Foobar");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s3 == "Welcome Foobar");
   }
   
   void findCharFromBeginAndMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('c');
      KAREN_UTEST_ASSERT(p == 3);
      KAREN_UTEST_ASSERT(!p.isNull());
   }

   void findCharFromBeginAndNotMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('x');
      KAREN_UTEST_ASSERT(p.isNull());
   }

   void findCharFromNotBeginAndMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('o', 5);
      KAREN_UTEST_ASSERT(p == 9);
      KAREN_UTEST_ASSERT(!p.isNull());
   }

   void findCharFromNotBeginAndNotMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('x', 5);
      KAREN_UTEST_ASSERT(p.isNull());
   }
   
   void reverseFindCharFromEndAndMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('F');
      KAREN_UTEST_ASSERT(!p.isNull());
      KAREN_UTEST_ASSERT(p == 8);
   }

   void reverseFindCharFromEndAndNotMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('x');
      KAREN_UTEST_ASSERT(p.isNull());
   }

   void reverseFindCharFromNotEndAndMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('o', 7);
      KAREN_UTEST_ASSERT(!p.isNull());
      KAREN_UTEST_ASSERT(p == 4);
   }

   void reverseFindCharFromNotEndAndNotMatching()
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('F', 7);
      KAREN_UTEST_ASSERT(p.isNull());
   }

   void capitalizeNonZeroLength()
   {
      String s1("foobar");
      String s2(s1.capitalize());
      KAREN_UTEST_ASSERT(s2 == "Foobar");
      KAREN_UTEST_ASSERT(s1 == "foobar");
   }

   void capitalizeZeroLength()
   {
      String s1("");
      String s2(s1.capitalize());
      KAREN_UTEST_ASSERT(s1 == "");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s1 == s2);
   }
   
   void toLowerCaseNonZeroLength()
   {
      String s1("WelCOme FooBaR");
      String s2(s1.toLowerCase());
      KAREN_UTEST_ASSERT(s1 == "WelCOme FooBaR");
      KAREN_UTEST_ASSERT(s2 == "welcome foobar");
   }

   void toLowerCaseZeroLength()
   {
      String s1("");
      String s2(s1.toLowerCase());
      KAREN_UTEST_ASSERT(s1 == "");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s1 == s2);
   }

   void toUpperCaseNonZeroLength()
   {
      String s1("WelCOme FooBaR");
      String s2(s1.toUpperCase());
      KAREN_UTEST_ASSERT(s1 == "WelCOme FooBaR");
      KAREN_UTEST_ASSERT(s2 == "WELCOME FOOBAR");
   }

   void toUpperCaseZeroLength()
   {
      String s1("");
      String s2(s1.toUpperCase());
      KAREN_UTEST_ASSERT(s1 == "");
      KAREN_UTEST_ASSERT(s2 == "");
      KAREN_UTEST_ASSERT(s1 == s2);
   }
   
   void toIntPositiveBase10()
   {
      String s("1234");
      int i = Integer::toLong(s);
      KAREN_UTEST_ASSERT(s == "1234");
      KAREN_UTEST_ASSERT(i = 1234);
   }

   void toIntPositiveBase16()
   {
      String s("0xff");
      int i = Integer::toLong(s, 16);
      KAREN_UTEST_ASSERT(s == "0xff");
      KAREN_UTEST_ASSERT(i = 255);
   }

   void toIntPositiveBase10InvalidValue()
   {
      String s("foobar");
      try
      {
         Integer::toLong(s);
         KAREN_UTEST_FAILED("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&)
      {
         KAREN_UTEST_ASSERT(s == "foobar");
      }
   }

   void toIntPositiveBase16InvalidValue()
   {
      String s("foobar");
      try
      {
         Integer::toLong(s, 16);
         KAREN_UTEST_FAILED("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&)
      {
         KAREN_UTEST_ASSERT(s == "foobar");
      }
   }
   
   void toFloatWithoutPoint()
   {
      String s("1234");
      try
      {
         float f = Float::toDouble(s);
         KAREN_UTEST_ASSERT(f == 1234.0f);
      }
      catch (InvalidConversionException&)
      {
         KAREN_UTEST_FAILED("unexpected InvalidConversion exception");
      }
   }

   void toFloatWithPoint()
   {
      String s("1234.56");
      try
      {
         float f = Float::toDouble(s);
         KAREN_UTEST_ASSERT(f == 1234.56f);
      }
      catch (InvalidConversionException&)
      {
         KAREN_UTEST_FAILED("unexpected InvalidConversion exception");
      }
   }

   void toFloatInvalidValue()
   {
      String s("foobar");
      try
      {
         Float::toDouble(s);
         KAREN_UTEST_FAILED("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&) {}
   }

   void toFloatNotCompletelyValidValue()
   {
      String s("1234.56foobar");
      try
      {
         Float::toDouble(s);
         KAREN_UTEST_FAILED("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&) {}
   }
   
   void formatZeroLengthString()
   {
      String s(String::format(""));
      KAREN_UTEST_ASSERT(s.length() == 0);
      KAREN_UTEST_ASSERT(s == "");
   }

   void formatWithEmbedString()
   {
      String s(String::format("Welcome %s", "Foobar"));
      KAREN_UTEST_ASSERT(s == "Welcome Foobar");
   }

   void formatWithEmbedInteger()
   {
      String s(String::format("Welcome %i", 12));
      KAREN_UTEST_ASSERT(s == "Welcome 12");
   }

   void formatWithEmbedFloat()
   {
      String s(String::format("Welcome %f", 12.34f));
      KAREN_UTEST_ASSERT(s == "Welcome 12.340000");
   }

};

class ArrayTestSuite : public UnitTestSuite
{
public:

   ArrayTestSuite() : UnitTestSuite("Collections - Array")
   {
      KAREN_UTEST_ADD(ArrayTestSuite::shouldCreateEmptyArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldCreateFromRawArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldNotIndexWhenEmpty);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldPushBackItems);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIndexWhenNotEmpty);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIterateArray);
      KAREN_UTEST_ADD(ArrayTestSuite::shouldIterateArrayUsingForRange);
   }
   
   void shouldCreateEmptyArray()
   {
      Array<int> a;
      KAREN_UTEST_ASSERT(a.empty());
      KAREN_UTEST_ASSERT(a.size() == 0);
   }
   
   void shouldNotIndexWhenEmpty()
   {
      Array<int> a;
      try
      {
         a[0];
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (OutOfBoundsException&) {}
   }
   
   void shouldCreateFromRawArray()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      Array<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 6);
   }
   
   void shouldPushBackItems()
   {
      Array<int> a;
      a.pushBack(10);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 1);
      KAREN_UTEST_ASSERT(a[0] == 10);
      a.pushBack(11);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 2);
      KAREN_UTEST_ASSERT(a[1] == 11);
      a.pushBack(12);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 3);
      KAREN_UTEST_ASSERT(a[2] == 12);   
   }

   void shouldIndexWhenNotEmpty()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      Array<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      for (int i = 0; i < a.size(); i++)
         KAREN_UTEST_ASSERT(a[i] == raw[i]);
   }
   
   void shouldIterateArray()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      Array<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      int i = 0;
      for (Iterator<int> it = a.begin(), end = a.end(); it != end; it++)
         KAREN_UTEST_ASSERT(*it == raw[i++]);
   }

   void shouldIterateArrayUsingForRange()
   {
      int raw[] = { 10, 11, 12, 13, 14, 15 };
      Array<int> a(raw, 6);
      KAREN_UTEST_ASSERT(!a.empty());
      KAREN_UTEST_ASSERT(a.size() == 6);
      int i = 0;
      for (int n : a)
         KAREN_UTEST_ASSERT(n == raw[i++]);
   }

};

class ListTestSuite : public UnitTestSuite
{
public:
   ListTestSuite() : UnitTestSuite("Collections - List")
   {
      KAREN_UTEST_ADD(ListTestSuite::createEmptyList);
      KAREN_UTEST_ADD(ListTestSuite::insertOneElementToHead);
      KAREN_UTEST_ADD(ListTestSuite::insertOneElementToTail);
      KAREN_UTEST_ADD(ListTestSuite::insertSeveralElementsToHead);
      KAREN_UTEST_ADD(ListTestSuite::insertSeveralElementsToTail);
      KAREN_UTEST_ADD(ListTestSuite::iterateFromHead);
      KAREN_UTEST_ADD(ListTestSuite::iterateFromTail);
      KAREN_UTEST_ADD(ListTestSuite::contains);
      KAREN_UTEST_ADD(ListTestSuite::removeIterator);
      KAREN_UTEST_ADD(ListTestSuite::removeWithInvalidIterator);
      KAREN_UTEST_ADD(ListTestSuite::removeFirst);
      KAREN_UTEST_ADD(ListTestSuite::removeLast);
      KAREN_UTEST_ADD(ListTestSuite::clearEmpty);
      KAREN_UTEST_ADD(ListTestSuite::clearNotEmpty);
      KAREN_UTEST_ADD(ListTestSuite::insertBefore);
      KAREN_UTEST_ADD(ListTestSuite::insertAfter);
      KAREN_UTEST_ADD(ListTestSuite::iterateForRange);
   }
   
   void createEmptyList()
   {
      List<int> l;
      KAREN_UTEST_ASSERT(l.empty());
      KAREN_UTEST_ASSERT(l.size() == 0);      
   }
   
   void insertOneElementToHead()
   {
      List<int> l;
      l.toHead(11);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 1);
      KAREN_UTEST_ASSERT(l.head() == 11);
      KAREN_UTEST_ASSERT(l.tail() == 11);
   }

   void insertOneElementToTail()
   {
      List<int> l;
      l.toTail(10);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 1);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 10);
   }

   void insertSeveralElementsToHead()
   {
      List<int> l;
      l.toHead(11);
      l.toHead(12);
      l.toHead(13);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 3);
      KAREN_UTEST_ASSERT(l.head() == 13);
      KAREN_UTEST_ASSERT(l.tail() == 11);
   }

   void insertSeveralElementsToTail()
   {
      List<int> l;
      l.toTail(11);
      l.toTail(12);
      l.toTail(13);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 3);
      KAREN_UTEST_ASSERT(l.head() == 11);
      KAREN_UTEST_ASSERT(l.tail() == 13);
   }

   void iterateFromHead()
   {
      List<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.toTail(i);
            
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 19);
      Iterator<int> it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == j++);
   }

   void iterateFromTail()
   {
      List<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.toTail(i);
            
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 19);
      j = 19;
      Iterator<int> it = l.begin(BEGIN_AT_BACK);
      KAREN_UTEST_ASSERT(it);
      for (; it; it--)
         KAREN_UTEST_ASSERT(*it == j--);
   }
   
   void contains()
   {
      List<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.toTail(i);
            
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 19);
      KAREN_UTEST_ASSERT(l.contains(15));
      KAREN_UTEST_ASSERT(!l.contains(25));
   }

   void removeIterator()
   {
      List<int> l;
      int j = 10;
      for (int i = j; i < 20; i++)
         l.toTail(i);
            
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 10);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 19);

      Iterator<int> it = l.begin();
      for (int i = 0; i < 5; i++)
         it++;
      l.remove(it);
      KAREN_UTEST_ASSERT(*it == 16);
      KAREN_UTEST_ASSERT(!l.contains(15));
   }

   void removeFirst()
   {
      List<int> l;
      for (int i = 10; i < 20; i++)
         l.toTail(i);
      Iterator<int> it = l.begin();
      l.remove(it);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 9);
      KAREN_UTEST_ASSERT(l.head() == 11);
      KAREN_UTEST_ASSERT(l.tail() == 19);
   }
   
   void removeLast()
   {
      List<int> l;
      for (int i = 10; i < 20; i++)
         l.toTail(i);
      Iterator<int> it = l.begin(BEGIN_AT_BACK);
      l.remove(it);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 9);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 18);
   }
   
   void removeWithInvalidIterator()
   {
      List<int> l;
      for (int i = 10; i < 20; i++)
         l.toTail(i);
      
      try
      {
         Iterator<int> it;
         l.remove(it);
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (InvalidInputException&) {}
      try
      {
         List<int> ll;
         Iterator<int> it = ll.begin();
         l.remove(it);
         KAREN_UTEST_FAILED("expected exception not raised");
      }
      catch (InvalidInputException&) {}
   }
   
   void clearNotEmpty()
   {
      List<int> l;
      for (int i = 10; i < 20; i++)
         l.toTail(i);
      l.clear();
      KAREN_UTEST_ASSERT(l.empty());
      KAREN_UTEST_ASSERT(l.size() == 0);
   }
   
   void clearEmpty()
   {
      List<int> l;
      l.clear();
      KAREN_UTEST_ASSERT(l.empty());
      KAREN_UTEST_ASSERT(l.size() == 0);
   }
   
   void insertBefore()
   {
      List<int> l;
      l.toTail(10);
      l.toTail(11);
      l.toTail(12);
      l.toTail(14);
      Iterator<int> it = l.begin(BEGIN_AT_BACK);
      l.insertBefore(13, it);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 5);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 14);
      KAREN_UTEST_ASSERT(*it == 14);
      int j = 10;
      it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == j++);
   }

   void insertAfter()
   {
      List<int> l;
      l.toTail(10);
      l.toTail(12);
      l.toTail(13);
      l.toTail(14);
      Iterator<int> it = l.begin();
      l.insertAfter(11, it);
      KAREN_UTEST_ASSERT(!l.empty());
      KAREN_UTEST_ASSERT(l.size() == 5);
      KAREN_UTEST_ASSERT(l.head() == 10);
      KAREN_UTEST_ASSERT(l.tail() == 14);
      KAREN_UTEST_ASSERT(*it == 10);
      int j = 10;
      it = l.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == j++);
   }
   
   void iterateForRange()
   {
      List<int> l;
      l.toTail(10);
      l.toTail(12);
      l.toTail(13);
      l.toTail(14);
      int i = 0;
      int values[] = { 10, 12, 13, 14 };
      for (int num : l)
      {
         KAREN_UTEST_ASSERT(num == values[i++]);
      }
      KAREN_UTEST_ASSERT(i == 4);
      
   }
   
};

class SetTestSuite : public UnitTestSuite
{
public:

   SetTestSuite() : UnitTestSuite("Collections - Set")
   {
      KAREN_UTEST_ADD(SetTestSuite::createEmptySet);
      KAREN_UTEST_ADD(SetTestSuite::insertOneElement);
      KAREN_UTEST_ADD(SetTestSuite::insertSeveralElements);
      KAREN_UTEST_ADD(SetTestSuite::insertDuplicatedElements);
      KAREN_UTEST_ADD(SetTestSuite::containsElement);
      KAREN_UTEST_ADD(SetTestSuite::iterate);
      KAREN_UTEST_ADD(SetTestSuite::removeElement);
      KAREN_UTEST_ADD(SetTestSuite::clear);
   }
   
   void createEmptySet()
   {
      Set<int> s;
      KAREN_UTEST_ASSERT(s.empty());
      KAREN_UTEST_ASSERT(s.size() == 0);
   }
   
   void insertOneElement()
   {
      Set<int> s;
      s.insert(10);
      KAREN_UTEST_ASSERT(!s.empty());
      KAREN_UTEST_ASSERT(s.size() == 1);
   }
   
   void insertSeveralElements()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      KAREN_UTEST_ASSERT(!s.empty());
      KAREN_UTEST_ASSERT(s.size() == 3);
   }
   
   void insertDuplicatedElements()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      KAREN_UTEST_ASSERT(!s.empty());
      KAREN_UTEST_ASSERT(s.size() == 3);
   }
   
   void containsElement()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      KAREN_UTEST_ASSERT(s.contains(7));
      KAREN_UTEST_ASSERT(!s.contains(13));
   }
   
   void iterate()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);
      
      int seq[] = { 2, 7, 10, 15, 17, 20 };
      int i = 0;
      ConstIterator<int> it = s.begin();
      KAREN_UTEST_ASSERT(it);
      for (; it; it++)
         KAREN_UTEST_ASSERT(*it == seq[i++]);
   }
   
   void removeElement()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);

      s.remove(10);
      
      int seq[] = { 2, 7, 15, 17, 20 };
      int i = 0;
      for (ConstIterator<int> it = s.begin(); it; it++)
         KAREN_UTEST_ASSERT(*it == seq[i++]);
   }
   
   void clear()
   {
      Set<int> s;
      s.insert(10);
      s.insert(15);
      s.insert(7);
      s.insert(10);
      s.insert(15);
      s.insert(20);
      s.insert(17);
      s.insert(2);
      
      s.clear();
      KAREN_UTEST_ASSERT(s.empty());
      KAREN_UTEST_ASSERT(s.size() == 0);
   }
   
};

class DictTestSuite : public UnitTestSuite
{
public:

   DictTestSuite() : UnitTestSuite("Collections - Dictionary")
   {
      KAREN_UTEST_ADD(DictTestSuite::shouldCreateAnEmptyDictionary);
      KAREN_UTEST_ADD(DictTestSuite::shouldInsertOneElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldInsertOneElementUsingIndexingOperator);
      KAREN_UTEST_ADD(DictTestSuite::shouldUpdateAnExistingElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldRemoveAnExistingElement);
      KAREN_UTEST_ADD(DictTestSuite::shouldClear);
      KAREN_UTEST_ADD(DictTestSuite::shouldIterate);
   }
   
   void shouldCreateAnEmptyDictionary()
   {
      Dictionary<String, int> d;
      KAREN_UTEST_ASSERT(d.empty());
      KAREN_UTEST_ASSERT(d.size() == 0);
   }
   
   void shouldInsertOneElement()
   {
      Dictionary<String, int> d;
      d.insert("Mark", 45);
      KAREN_UTEST_ASSERT(!d.empty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.defined("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 45);
   }
   
   void shouldInsertOneElementUsingIndexingOperator()
   {
      Dictionary<String, int> d;
      d["Mark"] = 45;
      KAREN_UTEST_ASSERT(!d.empty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.defined("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 45);
   }
      
   void shouldUpdateAnExistingElement()
   {
      Dictionary<String, int> d;
      d.insert("Mark", 45);
      d["Mark"] = 40;
      KAREN_UTEST_ASSERT(!d.empty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.defined("Mark"));
      KAREN_UTEST_ASSERT(d["Mark"] == 40);
   }
      
   void shouldRemoveAnExistingElement()
   {
      Dictionary<String, int> d;
      d.insert("Mark", 45);
      d.insert("John", 35);
      d.remove("Mark");
      KAREN_UTEST_ASSERT(!d.empty());
      KAREN_UTEST_ASSERT(d.size() == 1);
      KAREN_UTEST_ASSERT(d.defined("John"));
   }
      
   void shouldClear()
   {
      Dictionary<String, int> d;
      d.insert("Mark", 45);
      d.insert("John", 35);
      d.clear();
      KAREN_UTEST_ASSERT(d.empty());
      KAREN_UTEST_ASSERT(d.size() == 0);
   }
   
   void shouldIterate()
   {
      KeyValuePair<String, int> elems[] =
      {
         KeyValuePair<String, int>("John",   12 ),
         KeyValuePair<String, int>("Laura",  33 ),
         KeyValuePair<String, int>("Mark",   45 ),
         KeyValuePair<String, int>("Patty",  18 ),
      };
      Dictionary<String, int> d(elems, 4);
      ConstIterator<KeyValuePair<String, int> > it;
      int i = 0;
      for (it = d.begin(); it; it++)
      {
         KAREN_UTEST_ASSERT(it->key == elems[i].key);
         KAREN_UTEST_ASSERT(it->value == elems[i].value);
         i++;
      }
   }
      
};

class PriorityQueueTestSuite : public UnitTestSuite
{
public:

   struct Entry
   {
      int priority;
      String name;
   };
   
   struct EntryLessThan
   {
      bool operator()(const Entry &e1, const Entry &e2) const
      { return e1.priority < e2.priority; }
   };

   PriorityQueueTestSuite()
      : UnitTestSuite("Collections - Priority Queue")
   {
      KAREN_UTEST_ADD(PriorityQueueTestSuite::createEmptyQueue);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertOneElement);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertSeveralElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::removeElement);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::insertDuplicatedElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::iterateOnDuplicatedElements);
      KAREN_UTEST_ADD(PriorityQueueTestSuite::pullDuplicatedElements);
   }
   
   void createEmptyQueue()
   {
      PriorityQueue<int> q;
      KAREN_UTEST_ASSERT(q.empty());
      KAREN_UTEST_ASSERT(q.size() == 0);
   }
   
   void insertOneElement()
   {
      PriorityQueue<int> q;
      q.push(10);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 1);
      KAREN_UTEST_ASSERT(q.contains(10));
   }
   
   void insertSeveralElements()
   {
      PriorityQueue<int> q;
      q.push(10);
      q.push(4);
      q.push(15);
      q.push(1);
      q.push(3);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 5);
      KAREN_UTEST_ASSERT(q.contains(1));
      KAREN_UTEST_ASSERT(q.contains(3));
      KAREN_UTEST_ASSERT(q.contains(4));
      KAREN_UTEST_ASSERT(q.contains(10));
      KAREN_UTEST_ASSERT(q.contains(15));
      KAREN_UTEST_ASSERT(q.next() == 15);
   }
   
   void removeElement()
   {
      PriorityQueue<int> q;
      q.push(10);
      q.push(4);
      q.push(15);
      q.push(1);
      q.push(3);
      q.remove(15);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      KAREN_UTEST_ASSERT(q.contains(1));
      KAREN_UTEST_ASSERT(q.contains(3));
      KAREN_UTEST_ASSERT(q.contains(4));
      KAREN_UTEST_ASSERT(q.contains(10));
      KAREN_UTEST_ASSERT(!q.contains(15));
      KAREN_UTEST_ASSERT(q.next() == 10);
   }
   
   void insertDuplicatedElements()
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      for (int i = 0; i < 4; i++)
         q.push(ent[i]);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 4);
   }
   
   void iterateOnDuplicatedElements()
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      Entry ord[] = {
         ent[0],
         ent[2],
         ent[1],
         ent[3],
      };
      for (int i = 0; i < 4; i++)
         q.push(ent[i]);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      PriorityQueue<Entry, EntryLessThan>::ConstIterator it = q.begin();
      for (int i = 0; i < 4; i++)
      {
         const Entry &e = *it;
         KAREN_UTEST_ASSERT(e.name == ord[i].name);
         it++;
      }
   }
   
   void pullDuplicatedElements()
   {
      PriorityQueue<Entry, EntryLessThan> q;
      Entry ent[] = {
         { 10, "Jack" },
         { 15, "John" },
         { 12, "Mary" },
         { 15, "Stephen" },
      };
      Entry ord[] = {
         ent[1],
         ent[3],
         ent[2],
         ent[0],
      };
      for (int i = 0; i < 4; i++)
         q.push(ent[i]);
      KAREN_UTEST_ASSERT(!q.empty());
      KAREN_UTEST_ASSERT(q.size() == 4);
      for (int i = 0; i < 4; i++)
      {
         Entry e = q.pull();
         KAREN_UTEST_ASSERT(e.name == ord[i].name);
      }
   }
   
};

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
      using namespace karen::utils;
      
      Buffer buf(1024);
      KAREN_UTEST_ASSERT(buf.length() == 1024);
   }
   
   void shouldInitiateFromMemoryRegion()
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      KAREN_UTEST_ASSERT(buf.length() == 64);
      for (int i = 0; i < 64; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
   }
   
   void shouldIndicateRightBoundaries()
   {
      using namespace karen::utils;
      
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
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[64];
      buf.read(dst, 64);
      
      for (int i = 0; i < 64; i++)
         KAREN_UTEST_ASSERT(data[i] == dst[i]);
   }
   
   void shouldReadFirstPartOfBuffer() 
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16);
      
      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(data[i] == dst[i]);
   }
   
   void shouldReadLastPartOfBuffer() 
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[16];
      buf.read(dst, 16, 48);
      
      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(data[i + 48] == dst[i]);
   }
   
   void shouldReadFromTheMiddleOfBuffer()
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(data, 64);
      UInt8 dst[32];
      buf.read(dst, 32, 16);
      
      for (int i = 0; i < 32; i++)
         KAREN_UTEST_ASSERT(data[i + 16] == dst[i]);
   }
   
   void shouldFailWhileReadingBeyondBuffer()
   {
      using namespace karen::utils;
      
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
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(64);
      Buffer buf(64);
      buf.write(data, 64);

      for (int i = 0; i < 32; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
      delete []data;
   }
   
   void shouldWriteToFirstPartOfBuffer()
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16);

      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i) == data[i]);
      delete []data;
   }
   
   void shouldWriteToLastPartOfBuffer()
   {
      using namespace karen::utils;
      
      UInt8* data = allocRawBuffer(16);
      Buffer buf(64);
      buf.write(data, 16, 48);

      for (int i = 0; i < 16; i++)
         KAREN_UTEST_ASSERT(buf.get<UInt8>(i + 48) == data[i]);
      delete []data;
   }
   
   void shouldFailWhileWritingBeyondBuffer()
   {
      using namespace karen::utils;
      
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
      using namespace karen::utils;

      Buffer buf(64);
      buf.set<UInt8>(7, 32);
      buf.set<UInt32>(9, 16);
      KAREN_UTEST_ASSERT(buf.get<UInt8>(32) == 7);
      KAREN_UTEST_ASSERT(buf.get<UInt32>(16) == 9);
   }
   
   void shouldSetLastElement()
   {
      using namespace karen::utils;

      Buffer buf(64);
      buf.set<UInt8>(7, 63);
   }
   
   void shouldFailWhileSettingBeyondBuffer()
   {
      using namespace karen::utils;

      Buffer buf(64);
      try
      {
         buf.set<UInt8>(7, 128);
         KAREN_UTEST_FAILED("expected out of bounds exception not raised");
      } catch (OutOfBoundsException& e) {}
   }
   
   void shouldBeCleanAfterInitialization()
   {
      using namespace karen::utils;

      Buffer buf(64);
      KAREN_UTEST_ASSERT(!buf.isDirty());
   }
   
   void shouldBeDirtyAfterWrite()
   {
      using namespace karen::utils;

      Buffer buf(64);
      UInt8* data = allocRawBuffer(32);
      buf.write(data, 32);
      KAREN_UTEST_ASSERT(buf.isDirty());
      delete []data;
   }
   
   void shouldBeDirtyAfterSet()
   {
      using namespace karen::utils;

      Buffer buf(64);
      buf.set<UInt8>(32, 7);
      KAREN_UTEST_ASSERT(buf.isDirty());
   }
   
   void shouldBeCleanAfterMarkedAsClean()
   {
      using namespace karen::utils;

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
      using namespace karen::utils;
      
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
      using namespace karen::utils;
      
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
      using namespace karen::utils;
      
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
      using namespace karen::utils;
      
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

   static karen::utils::Buffer* allocBuffer(unsigned long len)
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
      StringTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      ArrayTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      ListTestSuite suite;
      suite.run(&rep, NULL, 0);
   }   
   {
      SetTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      DictTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      PriorityQueueTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      BufferTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
   {
      BufferStreamsTestSuite suite;
      suite.run(&rep, NULL, 0);
   }
}
