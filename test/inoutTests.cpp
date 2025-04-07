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
  CPPUNIT_TEST( test_carregaEntradas_file_notgates_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_0_in );  
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_1_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_2_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_3_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_4_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_5_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_6_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_7_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_8_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_9_in );
  CPPUNIT_TEST_SUITE_END();

public:
  void test_carregaEntradas_emptyFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/empty.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_oneInputFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/one.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 1, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
  }

  void test_carregaEntradas_twoInputFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/two.in", "r");

    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);

    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 2, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT( !strcmp( (char*)"b", (char*)inputs->lista[1].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, inputs->lista[1].duracaoTotal );
  }

  void test_carregaEntradas_file_notgates_in()
  {
    FILE* f_notgates_in = fopen("./inout_sample_files/notgates.in", "r");

    CPPUNIT_ASSERT( f_notgates_in );

    Sinais* inputs = carregaEntradas(f_notgates_in);
    
    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 2, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT( !strcmp( (char*)"b", (char*)inputs->lista[1].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[1].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( x, inputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( x, inputs->lista[1].pulsos[0].valor );
  }

  void test_carregaEntradas_file_badinput_0_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_0.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_1_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_1.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_2_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_2.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_3_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_3.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_4_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_4.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_5_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_5.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_6_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_6.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_7_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_7.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_8_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_8.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

  void test_carregaEntradas_file_badinput_9_in()
  {
    FILE* fp = fopen("./inout_sample_files/badinput_9.in", "r");
    CPPUNIT_ASSERT( fp );
    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( !inputs );
  }

};
