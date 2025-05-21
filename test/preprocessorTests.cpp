#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <list>
#include <string>
#include <cstring>

#include "../lex.h"
#include "../preprocessor.h"

class Testes_preprocessor : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_preprocessor );
  CPPUNIT_TEST( test_pre_processor_define );
  CPPUNIT_TEST( test_pre_processor_badpreproc_XX_v );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_pre_processor_define()
  {
    FILE* fp = fopen("./verilog_sample_src/preproc_define.v", "r");
    CPPUNIT_ASSERT(fp);

    ListaToken* list_tok = tokeniza(fp);
    CPPUNIT_ASSERT(list_tok);

    //exibeListaDeToken(list_tok);

    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->valor, "VER") );
    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->anterior->valor, "\x60") );

    int n_tok_in = list_tok->tamanho;

    int ret = pre_processor(list_tok);

    //exibeListaDeToken(list_tok);

    CPPUNIT_ASSERT_EQUAL(1, ret);

    int n_tok_out = list_tok->tamanho;

    CPPUNIT_ASSERT_EQUAL(n_tok_out + 1, n_tok_in);

    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->valor, "5") );
    CPPUNIT_ASSERT( !strcmp(list_tok->ultimo->anterior->anterior->anterior->valor, "=") );
  }

  void test_pre_processor_badpreproc_XX_v()
  {
    std::list<std::string> list_bad_files = {
    //  "./verilog_sample_src/badpreproc_00.v",
    //  "./verilog_sample_src/badpreproc_01.v",
        "./verilog_sample_src/badpreproc_02.v",
    //  "./verilog_sample_src/badpreproc_03.v",
        "./verilog_sample_src/badpreproc_04.v",
        "./verilog_sample_src/badpreproc_05.v"
    };

    FILE* fp = NULL;

    ListaToken* list_tok = NULL;

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
