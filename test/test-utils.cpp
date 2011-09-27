/*
 * ---------------------------------------------------------------------
 * This file is part of Karen
 *
 * Copyright (c) 2007-2011 Alvaro Polo
 * All rights reserved
 *
 * ---------------------------------------------------------------------
 */

#include "utils/test.h"
#include "utils/string.h"
#include "utils/numeric.h"

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
      KAREN_UTEST_ASSERT(tokens.length() == 4);
      KAREN_UTEST_ASSERT(tokens[0] == "Welcome");
      KAREN_UTEST_ASSERT(tokens[1] == "to");
      KAREN_UTEST_ASSERT(tokens[2] == "the");
      KAREN_UTEST_ASSERT(tokens[3] == "jungle");
   }

   void tokenizeOneTokenString()
   {
      String s("Welcome");
      Array<String> tokens = StringTokenizer::tokenize(s);
      KAREN_UTEST_ASSERT(tokens.length() == 1);
      KAREN_UTEST_ASSERT(tokens[0] == "Welcome");
   }

   void tokenizeZeroLengthString()
   {
      String s("");
      Array<String> tokens = StringTokenizer::tokenize(s);
      KAREN_UTEST_ASSERT(tokens.length() == 0);
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

int main(int argc, char* argv[])
{
   StdOutUnitTestReporter rep;
   {
      StringTestSuite suite;
      suite.run(&rep, NULL, 0);
   }

}
