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

#include <KarenCore/numeric.h>
#include <KarenCore/collection.h>
#include <KarenCore/parsing.h>
#include <KarenCore/string.h>
#include <KarenCore/test.h>
#include <KarenCore/types.h>

using namespace karen;

KAREN_BEGIN_UNIT_TEST(StringTestSuite);

   KAREN_DECL_TEST(createEmptyString,
   {
      String s;
      assertEquals<int>(0, s.length());
      assertTrue(s.isEmpty());
   });

   KAREN_DECL_TEST(createStringFromNullTerminatedCharPointer,
   {
      String s("foobar");
      assertEquals<int>(6, s.length());
      assertFalse(s.isEmpty());
      assertEquals<String>("foobar", s);
      assertNotEquals<String>("foo", s);
   });
   
   KAREN_DECL_TEST(createStringFromOtherString,
   {
      String s1("foobar");
      String s2(s1);
      assertEquals<String>(s1, s2);
      assertEquals<int>(s1.length(), s2.length());
   });

   KAREN_DECL_TEST(appendToSingleString,
   {
      String s("foo");
      s.append("bar");
      assertEquals<String>("foobar", s);
   });

   KAREN_DECL_TEST(appendToDeduplicatedString,
   {
      String s1("foo");
      String s2(s1);
      s1.append("bar");
      assertEquals<String>("foobar", s1);
      assertEquals<int>(6, s1.length());
      assertEquals<String>("foo", s2);
      assertEquals<int>(3, s2.length());
   });

   KAREN_DECL_TEST(appendCharsToSingleString,
   {
      String s("f");
      s.append('o', 2);
      assertEquals<String>("foo", s);
      assertEquals<int>(3, s.length());
   });

   KAREN_DECL_TEST(appendCharsToDeduplicatedString,
   {
      String s1("f");
      String s2(s1);
      s1.append('o', 2);
      assertEquals<String>("foo", s1);
      assertEquals<int>(3, s1.length());
      assertEquals<String>("f", s2);
      assertEquals<int>(1, s2.length());
   });
   
   KAREN_DECL_TEST(concatStrings,
   {
      String s1("Welcome");
      String s2(" to the jungle");
      String s3 = s1.concat(s2);
      assertEquals<String>("Welcome", s1);
      assertEquals<String>(" to the jungle", s2);
      assertEquals<String>("Welcome to the jungle", s3);
   });

   KAREN_DECL_TEST(concatNullString,
   {
      String s1("Welcome");
      String s2("");
      String s3 = s1.concat(s2);
      assertEquals<String>("Welcome", s1);
      assertEquals<String>("", s2);
      assertEquals<String>("Welcome", s3);
   });

   KAREN_DECL_TEST(concatStringWithChars,
   {
      String s1("Welcome");
      String s2 = s1.concat(' ', 3);
      assertEquals<String>("Welcome", s1);
      assertEquals<String>("Welcome   ", s2);
   });
   
   KAREN_DECL_TEST(concatStringWithZeroChars,
   {
      String s1("Welcome");
      String s2 = s1.concat('X', 0);
      assertEquals<String>("Welcome", s1);
      assertEquals<String>("Welcome", s2);
   });
   
   KAREN_DECL_TEST(obtainSlice,
   {
      String s1("Welcome Foobar");
      String s2 = s1.slice(3, 4);
      String s3 = s1.slice(8, 6);
      String s4 = s1.slice(11, 3);
      String s5 = s1.slice(11, 6);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("come", s2);
      assertEquals<String>("Foobar", s3);
      assertEquals<String>("bar", s4);
      assertEquals<String>("bar", s5);
   });

   KAREN_DECL_TEST(obtainSliceOutOfRange,
   {
      String s1("Welcome Foobar");
      String s2 = s1.slice(8, 255);
      String s3 = s1.slice(64, 255);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Foobar", s2);
      assertEquals<String>("", s3);
   });

   KAREN_DECL_TEST(obtainHead,
   {
      String s1("Welcome Foobar");
      String s2 = s1.head(7);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Welcome", s2);
   });

   KAREN_DECL_TEST(obtainHeadOutOfRange,
   {
      String s1("Welcome Foobar");
      String s2 = s1.head(255);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Welcome Foobar", s2);
   });

   KAREN_DECL_TEST(obtainTail,
   {
      String s1("Welcome Foobar");
      String s2 = s1.tail(6);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Foobar", s2);
   });

   KAREN_DECL_TEST(obtainTailOutOfRange,
   {
      String s1("Welcome Foobar");
      String s2 = s1.tail(255);
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Welcome Foobar", s2);
   });

   KAREN_DECL_TEST(removeFromHead,
   {
      String s("Welcome Foobar");
      s.removeFromHead(8);
      assertEquals<String>("Foobar", s);
   });

   KAREN_DECL_TEST(removeFromHeadZero,
   {
      String s("Welcome Foobar");
      s.removeFromHead(0);
      assertEquals<String>("Welcome Foobar", s);
   });

   KAREN_DECL_TEST(removeFromHeadOutOfRange,
   {
      String s("Welcome Foobar");
      s.removeFromHead(255);
      assertEquals<String>("", s);
      assertTrue(s.isEmpty());
   });

   KAREN_DECL_TEST(removeFromTail,
   {
      String s("Welcome Foobar");
      s.removeFromTail(7);
      assertEquals<String>("Welcome", s);
   });

   KAREN_DECL_TEST(removeFromTailZero,
   {
      String s("Welcome Foobar");
      s.removeFromTail(0);
      assertEquals<String>("Welcome Foobar", s);
   });

   KAREN_DECL_TEST(removeFromTailOutOfRange,
   {
      String s("Welcome Foobar");
      s.removeFromTail(255);
      assertEquals<String>("", s);
      assertTrue(s.isEmpty());
   });
   
   KAREN_DECL_TEST(tokenizeString,
   {
      String s("Welcome to the jungle");
      DynArray<String> tokens;
      karen::tokenizeString(s, tokens);
      assertEquals<int>(4, tokens.size());
      assertEquals<String>("Welcome", tokens[0]);
      assertEquals<String>("to", tokens[1]);
      assertEquals<String>("the", tokens[2]);
      assertEquals<String>("jungle", tokens[3]);
   });

   KAREN_DECL_TEST(tokenizeOneTokenString,
   {
      String s("Welcome");
      DynArray<String> tokens;
      karen::tokenizeString(s, tokens);
      assertEquals<int>(1, tokens.size());
      assertEquals<String>("Welcome", tokens[0]);
   });

   KAREN_DECL_TEST(tokenizeZeroLengthString,
   {
      String s("");
      DynArray<String> tokens;
      karen::tokenizeString(s, tokens);
      assertEquals<int>(0, tokens.size());
   });
   
   KAREN_DECL_TEST(startsWithString,
   {
      String s1("Welcome Foobar");
      String s2("Welc");
      assertTrue(s1.startsWith(s2));
   });

   KAREN_DECL_TEST(startsWithZeroLengthString,
   {
      String s1("Welcome Foobar");
      String s2("");
      assertTrue(s1.startsWith(s2));
   });

   KAREN_DECL_TEST(startsWithItself,
   {
      String s1("Welcome Foobar");
      String s2("Welcome Foobar");
      assertTrue(s1.startsWith(s2));
   });
   
   KAREN_DECL_TEST(removeFromHeadMatching,
   {
      String s1("Welcome Foobar");
      String s2("Welcome ");
      String s3(s1.removeHead(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Welcome ", s2);
      assertEquals<String>("Foobar", s3);
   });

   KAREN_DECL_TEST(removeFromHeadNotMatching,
   {
      String s1("Welcome Foobar");
      String s2("Foobar");
      String s3(s1.removeHead(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Foobar", s2);
      assertEquals<String>("Welcome Foobar", s3);
   });

   KAREN_DECL_TEST(removeFromHeadByZeroLengthString,
   {
      String s1("Welcome Foobar");
      String s2("");
      String s3(s1.removeHead(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("", s2);
      assertEquals<String>("Welcome Foobar", s3);
   });

   KAREN_DECL_TEST(removeFromTailMatching,
   {
      String s1("Welcome Foobar");
      String s2(" Foobar");
      String s3(s1.removeTail(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>(" Foobar", s2);
      assertEquals<String>("Welcome", s3);
   });

   KAREN_DECL_TEST(removeFromTailNotMatching,
   {
      String s1("Welcome Foobar");
      String s2("Kepler");
      String s3(s1.removeTail(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("Kepler", s2);
      assertEquals<String>("Welcome Foobar", s3);
   });

   KAREN_DECL_TEST(removeFromTailByZeroLengthString,
   {
      String s1("Welcome Foobar");
      String s2("");
      String s3(s1.removeTail(s2));
      assertEquals<String>("Welcome Foobar", s1);
      assertEquals<String>("", s2);
      assertEquals<String>("Welcome Foobar", s3);
   });
   
   KAREN_DECL_TEST(findCharFromBeginAndMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('c');
      assertEquals<int>(3, p);
      assertFalse(p.isNull());
   });

   KAREN_DECL_TEST(findCharFromBeginAndNotMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('x');
      assertTrue(p.isNull());
   });

   KAREN_DECL_TEST(findCharFromNotBeginAndMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('o', 5);
      assertEquals<int>(9, p);
      assertFalse(p.isNull());
   });

   KAREN_DECL_TEST(findCharFromNotBeginAndNotMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.findChar('x', 5);
      assertTrue(p.isNull());
   });
   
   KAREN_DECL_TEST(reverseFindCharFromEndAndMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('F');
      assertFalse(p.isNull());
      assertEquals<int>(8, p);
   });

   KAREN_DECL_TEST(reverseFindCharFromEndAndNotMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('x');
      assertTrue(p.isNull());
   });

   KAREN_DECL_TEST(reverseFindCharFromNotEndAndMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('o', 7);
      assertFalse(p.isNull());
      assertEquals<int>(4, p);
   });

   KAREN_DECL_TEST(reverseFindCharFromNotEndAndNotMatching,
   {
      String s("Welcome Foobar");
      String::Position p = s.reverseFindChar('F', 7);
      assertTrue(p.isNull());
   });

   KAREN_DECL_TEST(capitalizeNonZeroLength,
   {
      String s1("foobar");
      String s2(s1.capitalize());
      assertEquals<String>("Foobar", s2);
      assertEquals<String>("foobar", s1);
   });

   KAREN_DECL_TEST(capitalizeZeroLength,
   {
      String s1("");
      String s2(s1.capitalize());
      assertEquals<String>("", s1);
      assertEquals<String>("", s2);
      assertEquals<String>(s1, s2);
   });
   
   KAREN_DECL_TEST(toLowerCaseNonZeroLength,
   {
      String s1("WelCOme FooBaR");
      String s2(s1.toLowerCase());
      assertEquals<String>("WelCOme FooBaR", s1);
      assertEquals<String>("welcome foobar", s2);
   });

   KAREN_DECL_TEST(toLowerCaseZeroLength,
   {
      String s1("");
      String s2(s1.toLowerCase());
      assertEquals<String>("", s1);
      assertEquals<String>("", s2);
      assertEquals<String>(s1, s2);
   });

   KAREN_DECL_TEST(toUpperCaseNonZeroLength,
   {
      String s1("WelCOme FooBaR");
      String s2(s1.toUpperCase());
      assertEquals<String>("WelCOme FooBaR", s1);
      assertEquals<String>("WELCOME FOOBAR", s2);
   });

   KAREN_DECL_TEST(toUpperCaseZeroLength,
   {
      String s1("");
      String s2(s1.toUpperCase());
      assertEquals<String>("", s1);
      assertEquals<String>("", s2);
      assertEquals<String>(s1, s2);
   });
   
   KAREN_DECL_TEST(toIntPositiveBase10,
   {
      String s("1234");
      long i = Integer::toLong(s);
      assertEquals<String>("1234", s);
      assertEquals<int>(1234, i);
   });

   KAREN_DECL_TEST(toIntPositiveBase16,
   {
      String s("0xff");
      long i = Integer::toLong(s, 16);
      assertEquals<String>("0xff", s);
      assertEquals<int>(255, i);
   });

   KAREN_DECL_TEST(toIntPositiveBase10InvalidValue,
   {
      String s("foobar");
      try
      {
         Integer::toLong(s);
         assertionFailed(
               "expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&)
      {
         assertEquals<String>("foobar", s);
      }
   });

   KAREN_DECL_TEST(toIntPositiveBase16InvalidValue,
   {
      String s("foobar");
      try
      {
         Integer::toLong(s, 16);
         assertionFailed(
               "expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&)
      {
         assertEquals<String>("foobar", s);
      }
   });
   
   KAREN_DECL_TEST(toFloatWithoutPoint,
   {
      String s("1234");
      try
      {
         float f = Float::toDouble(s);
         assertEquals<float>(1234.0f, f);
      }
      catch (InvalidConversionException&)
      {
         assertionFailed("unexpected InvalidConversion exception");
      }
   });

   KAREN_DECL_TEST(toFloatWithPoint,
   {
      String s("1234.56");
      try
      {
         float f = Float::toDouble(s);
         assertEquals<float>(1234.56f, f);
      }
      catch (InvalidConversionException&)
      {
         assertionFailed("unexpected InvalidConversion exception");
      }
   });

   KAREN_DECL_TEST(toFloatInvalidValue,
   {
      String s("foobar");
      try
      {
         Float::toDouble(s);
         assertionFailed("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&) {}
   });

   KAREN_DECL_TEST(toFloatNotCompletelyValidValue,
   {
      String s("1234.56foobar");
      try
      {
         Float::toDouble(s);
         assertionFailed("expected InvalidConversion exception not raised");
      }
      catch (InvalidConversionException&) {}
   });
   
   KAREN_DECL_TEST(formatZeroLengthString,
   {
      String s(String::format(""));
      assertEquals<int>(0, s.length());
      assertEquals<String>("", s);
   });

   KAREN_DECL_TEST(formatWithEmbedString,
   {
      String s(String::format("Welcome %s", "Foobar"));
      assertEquals<String>("Welcome Foobar", s);
   });

   KAREN_DECL_TEST(formatWithEmbedInteger,
   {
      String s(String::format("Welcome %i", 12));
      assertEquals<String>("Welcome 12", s);
   });

   KAREN_DECL_TEST(formatWithEmbedFloat,
   {
      String s(String::format("Welcome %f", 12.34f));
      assertEquals<String>("Welcome 12.340000", s);
   });

KAREN_END_UNIT_TEST(StringTestSuite);

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   StringTestSuite suite;
   suite.run(&rep, NULL, 0);
}
