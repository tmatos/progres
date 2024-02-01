#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../sinais.h"
#include "../inout.h"

class Testes_inout : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_inout );
  CPPUNIT_TEST( test_carregaEntradas_emptyFile );
  CPPUNIT_TEST( test_carregaEntradas_oneInputFile );
  CPPUNIT_TEST( test_carregaEntradas_twoInputFile );
  CPPUNIT_TEST_SUITE_END();

public:
  void test_carregaEntradas_emptyFile()
  {
    FILE *file_inputs = fopen("./inout_sample_files/empty.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais *inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_oneInputFile()
  {
    FILE *file_inputs = fopen("./inout_sample_files/one.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais *inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 1, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
  }

  void test_carregaEntradas_twoInputFile()
  {
    FILE *file_inputs = fopen("./inout_sample_files/two.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais *inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 2, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT( !strcmp( (char*)"b", (char*)inputs->lista[1].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, inputs->lista[1].duracaoTotal );
  }

};
