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
  CPPUNIT_TEST( test_remove_underscores_from_literal );
  CPPUNIT_TEST_SUITE_END();

public:

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
