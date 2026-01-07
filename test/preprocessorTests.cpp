#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <list>
#include <string>
#include <cstring>
#include <vector>
#include <tuple>

#include "lex.h"
#include "preprocessor.h"

class Testes_preprocessor : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_preprocessor );
  CPPUNIT_TEST( test_pre_processor_insert_macro_get_macro_by_name );
  CPPUNIT_TEST( test_pre_processor_remove_macro_by_name );
  CPPUNIT_TEST( test_pre_processor_define );
  CPPUNIT_TEST( test_pre_processor_directives_v );
  CPPUNIT_TEST( test_pre_processor_undef_v );
  CPPUNIT_TEST( test_pre_processor_badpreproc_XX_v );
  CPPUNIT_TEST_SUITE_END();

public:

  std::list<std::tuple<std::string, std::string>> test_macros_data {
    {"MACRO_01", "000"},
    {"MACRO_02", "123"},
    {"MACRO_03", "xxx"},
    {"MACRO_04", "___"},
    {"MACRO_05", "[10:0]"}
  };

  void test_pre_processor_insert_macro_get_macro_by_name()
  {
    ListMacro list_macro;
    list_macro.total = 0;
    list_macro.itens = NULL;

    for ( auto t : test_macros_data ) {
      // insert_macro(&list_macro, macro_name, macro_value)
      insert_macro( &list_macro, std::get<0>(t).c_str(), std::get<1>(t).c_str() );
    }

    int size = test_macros_data.size();
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    Macro* m = NULL;

    for ( auto t : test_macros_data ) {
      m = get_macro_by_name( list_macro, std::get<0>(t).c_str() );
      CPPUNIT_ASSERT( m );
      CPPUNIT_ASSERT( !strcmp(std::get<1>(t).c_str(), m->value) );
    }

    char* no_name = NULL;

    CPPUNIT_ASSERT( !get_macro_by_name(list_macro, no_name) );
  }

  void test_pre_processor_remove_macro_by_name()
  {
    ListMacro list_macro;
    list_macro.total = 0;
    list_macro.itens = NULL;

    for ( auto t : test_macros_data ) {
      // insert_macro(&list_macro, macro_name, macro_value)
      insert_macro( &list_macro, std::get<0>(t).c_str(), std::get<1>(t).c_str() );
    }

    remove_macro_by_name(&list_macro, NULL);

    int size = test_macros_data.size();
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    remove_macro_by_name(&list_macro, "MACRO_05");

    size--;
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    remove_macro_by_name(&list_macro, "MACRO_04");

    size--;
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    remove_macro_by_name(&list_macro, "MACRO_01");

    CPPUNIT_ASSERT( !get_macro_by_name(list_macro, "MACRO_01") );
    CPPUNIT_ASSERT( get_macro_by_name(list_macro, "MACRO_03") );

    size--;
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    remove_macro_by_name(&list_macro, "MACRO_02");

    size--;
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    remove_macro_by_name(&list_macro, "MACRO_03");

    size--;
    CPPUNIT_ASSERT_EQUAL( size, list_macro.total );

    Macro* m = NULL;

    for ( auto t : test_macros_data ) {
      m = get_macro_by_name( list_macro, std::get<0>(t).c_str() );
      CPPUNIT_ASSERT( !m );
    }

    insert_macro(&list_macro, "POST_MACRO_SET", "110011");
    CPPUNIT_ASSERT( get_macro_by_name(list_macro, "POST_MACRO_SET") );
  }

  void test_pre_processor_define()
  {
    FILE* fp = fopen("./verilog_sample_src/preproc_define.v", "r");
    CPPUNIT_ASSERT(fp);

    ListToken* list_tok = tokeniza(fp);
    CPPUNIT_ASSERT(list_tok);

    //show_token_list(list_tok);

    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->valor, "VER") );
    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->anterior->valor, "\x60") );

    int n_tok_in = list_tok->tamanho;

    PreprocesorResult ret = pre_processor(list_tok);

    //show_token_list(list_tok);

    CPPUNIT_ASSERT_EQUAL(PREPROCESSOR_SUCCESS, ret);

    int n_tok_out = list_tok->tamanho;

    CPPUNIT_ASSERT_EQUAL(n_tok_out + 5, n_tok_in);

    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->valor, "5") );
    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->anterior->valor, "=") );
  }

  void test_pre_processor_undef_v()
  {
    FILE* fp = fopen("./verilog_sample_src/preproc_undef.v", "r");
    CPPUNIT_ASSERT(fp);

    ListToken* list_tok = tokeniza(fp);
    CPPUNIT_ASSERT(list_tok);

    int n_tok_in = list_tok->tamanho;

    PreprocesorResult ret = pre_processor(list_tok);
    CPPUNIT_ASSERT_EQUAL(PREPROCESSOR_SUCCESS, ret);

    int n_tok_out = list_tok->tamanho;

    CPPUNIT_ASSERT_EQUAL(n_tok_out + (4+4+3+1), n_tok_in);

    CPPUNIT_ASSERT( !strcmp(list_tok->primeiro->valor, "module") );
  }

  void test_pre_processor_directives_v()
  {
    FILE* fp = fopen("./verilog_sample_src/preproc_directives.v", "r");
    CPPUNIT_ASSERT(fp);

    ListToken* list_tok = tokeniza(fp);
    CPPUNIT_ASSERT(list_tok);

    PreprocesorResult ret = pre_processor(list_tok);
    CPPUNIT_ASSERT_EQUAL(PREPROCESSOR_SUCCESS, ret);
  }

  void test_pre_processor_badpreproc_XX_v()
  {
    std::list<std::string> list_bad_files = {
      "./verilog_sample_src/badpreproc_00.v",
      "./verilog_sample_src/badpreproc_01.v",
      "./verilog_sample_src/badpreproc_02.v",
      "./verilog_sample_src/badpreproc_03.v",
      "./verilog_sample_src/badpreproc_04.v",
      "./verilog_sample_src/badpreproc_05.v",
      "./verilog_sample_src/badpreproc_06.v",
      "./verilog_sample_src/badpreproc_07.v",
      "./verilog_sample_src/badpreproc_08.v",
      "./verilog_sample_src/badpreproc_09.v",
      "./verilog_sample_src/badpreproc_10.v",
    };

    FILE* fp = NULL;

    ListToken* list_tok = NULL;

    for ( std::string path : list_bad_files )
    {
      fp = fopen(path.c_str(), "r");
      CPPUNIT_ASSERT(fp);

      list_tok = tokeniza(fp);
      CPPUNIT_ASSERT(list_tok);
      
      CPPUNIT_ASSERT( !pre_processor(list_tok) );

      fclose(fp);
    }
  }

};
