#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>
#include <list>
#include <string>

#include "../sinais.h"
#include "../eventos.h"
#include "../inout.h"

class Testes_inout : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_inout );
  CPPUNIT_TEST( test_carregaEntradas_emptyFile );
  CPPUNIT_TEST( test_carregaEntradas_empty_signal );
  CPPUNIT_TEST( test_carregaEntradas_oneInputFile );
  CPPUNIT_TEST( test_carregaEntradas_twoInputFile );
  CPPUNIT_TEST( test_carregaEntradas_file_notgates_in );
  CPPUNIT_TEST( test_carregaEntradas_file_badinput_XX_in );
  CPPUNIT_TEST( test_salvarSinais );
  CPPUNIT_TEST_SUITE_END();

public:
  void test_carregaEntradas_emptyFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/empty.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);
    CPPUNIT_ASSERT( !inputs );
    fclose(file_inputs);
  }

  void test_carregaEntradas_empty_signal()
  {
    FILE* file_inputs = fopen("./inout_sample_files/empty_signal.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);
    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 1, inputs->quantidade );

    free(inputs);
    fclose(file_inputs);
  }

  void test_carregaEntradas_oneInputFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/one.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    Sinais* inputs = carregaEntradas(file_inputs);
    CPPUNIT_ASSERT( inputs );
    fclose(file_inputs);

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
    fclose(file_inputs);

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
    fclose(f_notgates_in);

    CPPUNIT_ASSERT( !strcmp( (char*)"a", (char*)inputs->lista[0].nome ) );
    CPPUNIT_ASSERT( !strcmp( (char*)"b", (char*)inputs->lista[1].nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[0].duracaoTotal );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, inputs->lista[1].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, inputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( VAL_X, inputs->lista[1].pulsos[0].valor );
  }

  void test_carregaEntradas_file_badinput_XX_in()
  {
    std::list<std::string> list_bad_files = {
      "./inout_sample_files/badinput_0.in",
      "./inout_sample_files/badinput_1.in",
      "./inout_sample_files/badinput_2.in",
      "./inout_sample_files/badinput_3.in",
      "./inout_sample_files/badinput_4.in",
      "./inout_sample_files/badinput_5.in",
      "./inout_sample_files/badinput_5a.in",
      "./inout_sample_files/badinput_5b.in",
      "./inout_sample_files/badinput_6.in",
      "./inout_sample_files/badinput_6a.in",
      "./inout_sample_files/badinput_7.in",
      "./inout_sample_files/badinput_7a.in",
      "./inout_sample_files/badinput_8.in",
      "./inout_sample_files/badinput_9.in"
    };

    Sinais* inputs = NULL;
    FILE* fp = NULL;

    for ( std::string path : list_bad_files )
    {
      fp = fopen( path.c_str(), "r");
      CPPUNIT_ASSERT( fp );
      Sinais* inputs = carregaEntradas(fp);
      CPPUNIT_ASSERT( !inputs );
      fclose(fp);
    }
  }

  void test_salvarSinais()
  {
    FILE* fp = fopen("./inout_sample_files/allpulses.in", "r");
    CPPUNIT_ASSERT( fp );

    Sinais* inputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( inputs );
    fclose(fp);

    FILE* fp_out = fopen("./inout_sample_files/allpulses.in.out", "w");
    CPPUNIT_ASSERT( fp_out );

    salvarSinais(inputs, fp_out);
    fclose(fp_out);

    fp = fopen("./inout_sample_files/allpulses.in.out", "r");
    CPPUNIT_ASSERT( fp );

    Sinais* outputs = carregaEntradas(fp);
    CPPUNIT_ASSERT( outputs );
    fclose(fp);

    // verificando se o sinal original e o que foi salvo sao iguais

    CPPUNIT_ASSERT_EQUAL( inputs->quantidade, outputs->quantidade );

    for ( int i = 0; i < inputs->quantidade; ++i ) {
      CPPUNIT_ASSERT( !strcmp(inputs->lista[i].nome, outputs->lista[i].nome) );
      CPPUNIT_ASSERT_EQUAL( inputs->lista[i].duracaoTotal, outputs->lista[i].duracaoTotal );
      
      for ( int j = 0; j < 3; ++j )
      {
        CPPUNIT_ASSERT_EQUAL( inputs->lista[i].pulsos[j].tempo,
                              outputs->lista[i].pulsos[j].tempo );
        CPPUNIT_ASSERT_EQUAL( inputs->lista[i].pulsos[j].valor,
                              outputs->lista[i].pulsos[j].valor );
      }
    }

    remove("./inout_sample_files/allpulses.in.out");
  }

};
