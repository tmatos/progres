#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <list>
#include <tuple>
#include <string>
#include <cstring>

#include "strutil.h"

class Testes_strutil : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_strutil );
  CPPUNIT_TEST( test_iguais );
  CPPUNIT_TEST( test_has_only_digits );
  CPPUNIT_TEST( test_has_only_digits_outro );
  CPPUNIT_TEST( test_is_valid_natural_number );
  CPPUNIT_TEST( test_remove_underscores_from_literal );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_iguais()
  {
    char str_0[] = "";
    char str_1[] = "";
    char str_2[] = "p";
    char str_3[] = "p";
    char str_4[] = "wp";
    char str_5[] = "Wp";
    char str_6[] = "abc def";
    char str_7[] = "abc_def";
    char str_8[] = "a1b2c3_defACZ-780";
    char str_9[] = "a1b2c3_defACZ-780";
    
    CPPUNIT_ASSERT(   iguais(str_0, str_1) );
    CPPUNIT_ASSERT( ! iguais(str_1, str_2) );
    CPPUNIT_ASSERT(   iguais(str_2, str_3) );
    CPPUNIT_ASSERT( ! iguais(str_3, str_4) );
    CPPUNIT_ASSERT( ! iguais(str_4, str_5) );
    CPPUNIT_ASSERT( ! iguais(str_5, str_6) );
    CPPUNIT_ASSERT( ! iguais(str_6, str_7) );
    CPPUNIT_ASSERT(   iguais(str_8, str_9) );
    CPPUNIT_ASSERT(   iguais(str_7, str_7) );
  }

  void test_has_only_digits()
  {
    char str_0[] = "001123456789";
    char str_1[] = "001123456789a";
    char str_2[] = "001123456789 ";

    CPPUNIT_ASSERT(   has_only_digits(str_0) );
    CPPUNIT_ASSERT( ! has_only_digits(str_1) );
    CPPUNIT_ASSERT( ! has_only_digits(str_2) );
    CPPUNIT_ASSERT( ! has_only_digits(NULL) );
  }

  void test_has_only_digits_outro()
  {
    char str[50];

    copy(str, "1234567890");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "0000000000");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "1");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "A");
    CPPUNIT_ASSERT( ! has_only_digits(str) );

    copy(str, "555555x555555");
    CPPUNIT_ASSERT( ! has_only_digits(str) );

    copy(str, "999999999999m");
    CPPUNIT_ASSERT( ! has_only_digits(str) );
  }

  void test_is_valid_natural_number()
  {
    std::list<std::string> valid_nums = {
      "0", "00", "000", "0000",
      "01", "001", "0001", "0009",
      "1", "9", "20", "123", "1000",
      "09", "99", "999", "9090", "9999"
    };

    for ( std::string s : valid_nums ) {
      CPPUNIT_ASSERT( is_valid_natural_number(s.c_str()) );
    }

    std::list<std::string> invalid_nums = {
      "-1", "a", "a1a", "1a1", "1aaaa", "0000a", "9999999999999999"
    };

    for ( std::string s : invalid_nums ) {
      CPPUNIT_ASSERT( ! is_valid_natural_number(s.c_str()) );
    }

    CPPUNIT_ASSERT( ! is_valid_natural_number((char*)NULL) );
  }

  void test_remove_underscores_from_literal()
  {
    std::list<std::tuple<std::string, std::string>> test_str_literals = {
      {"1_000_000",           "1000000"},
      {"_123_456_789_",       "123456789"},
      {"____",                ""},
      {"no_underscores_here", "nounderscoreshere"},
      {"",                    ""},
      {"none",                "none"},
      {"-1-1",                 "-1-1"},
      {"0xDE_AD_BE_EF",       "0xDEADBEEF"},
      {"0b1010_1010",         "0b10101010"}
    };

    for (auto pair : test_str_literals)
    {
      auto in = std::get<0>(pair);
      auto expected = std::get<1>(pair);

      char c_str[150];
      copy(c_str, in.c_str());

      remove_underscores_from_literal(c_str);

      std::string cpp_str_out(c_str);

      CPPUNIT_ASSERT_EQUAL(expected, cpp_str_out);
    }
  }

};
