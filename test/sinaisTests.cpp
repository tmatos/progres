#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../sinais.h"

class Testes_sinais : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_sinais );
  CPPUNIT_TEST( test_new_signal );
  CPPUNIT_TEST( test_set_signal_name );
  CPPUNIT_TEST( test_set_pulse_blank );
  CPPUNIT_TEST( test_add_new_pulse );
  CPPUNIT_TEST( test_new_signal_list );
  CPPUNIT_TEST( test_add_new_signal );
  CPPUNIT_TEST( test_insert_signal );
  CPPUNIT_TEST( test_get_timeunit_from_str_valid_units );
  CPPUNIT_TEST( test_get_timeunit_from_str_invalid_units );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_new_signal()
  {
    Sinal *sinal_unnamed = new_signal(NULL);
    
    CPPUNIT_ASSERT( sinal_unnamed );
    CPPUNIT_ASSERT( !strcmp( (char*)"", (char*)sinal_unnamed->nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal_unnamed->duracaoTotal );

    Sinal *sinal_teste = new_signal( (char*)"teste" );
    
    CPPUNIT_ASSERT( sinal_teste );
    CPPUNIT_ASSERT( !strcmp( (char*)"teste", (char*)sinal_teste->nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal_teste->duracaoTotal );
  }

  void test_set_signal_name()
  {
    Sinal *sinal = new_signal( (char*)"teste" );

    int ret = set_signal_name(sinal, (char*)"123" );
    
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );

    ret = set_signal_name(NULL, NULL);

    CPPUNIT_ASSERT(!ret);
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );
  }

  void test_set_pulse_blank()
  {
    Pulso p;

    int ret = set_pulse_blank(&p);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, p.valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, p.tempo );
    CPPUNIT_ASSERT_EQUAL( UN_S, p.unidade );

    ret = set_pulse_blank(NULL);
    CPPUNIT_ASSERT(!ret);
  }

  void test_add_new_pulse()
  {
    Sinal *sinal = new_signal( (char*)"teste" );

    CPPUNIT_ASSERT(sinal);
    
    int ret = add_new_pulse(sinal, VAL_0, (Tempo)5);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, sinal->duracaoTotal );
    
    ret = add_new_pulse(sinal, VAL_0, (Tempo)30000);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Tempo)30005, sinal->duracaoTotal );

    ret = add_new_pulse(NULL, VAL_0, (Tempo)30);
    CPPUNIT_ASSERT(!ret);

    free(sinal->pulsos);
    sinal->pulsos = NULL;
    ret = add_new_pulse(sinal, VAL_0, (Tempo)30);
    CPPUNIT_ASSERT(!ret);
  }

  void test_new_signal_list()
  {
    Sinais *sinais = new_signal_list();
    
    CPPUNIT_ASSERT_EQUAL( 0, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Sinal*)NULL, sinais->lista );
  }

  void test_add_new_signal()
  {
    Sinais *sinais = new_signal_list();
    
    char str_nome_1[50] = "primeiro_sinal";
    char str_nome_2[50] = "segundo_sinal";
    
    add_new_signal(sinais, str_nome_1);
    CPPUNIT_ASSERT_EQUAL( 1, sinais->quantidade );
    CPPUNIT_ASSERT( sinais->lista );
    CPPUNIT_ASSERT( sinais->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[0].duracaoTotal );
    
    add_new_signal(sinais, str_nome_2);
    CPPUNIT_ASSERT_EQUAL( 2, sinais->quantidade );
    CPPUNIT_ASSERT( sinais->lista );
    CPPUNIT_ASSERT( sinais->lista[1].pulsos );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->lista[1].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[1].duracaoTotal );
  }

  void test_insert_signal()
  {
    Sinais *sinais = new_signal_list();
    
    Sinal *sinal_0 = new_signal( (char*)"sinal_teste_0" );
    add_new_pulse(sinal_0, VAL_1, (Tempo)255);   // 0
    add_new_pulse(sinal_0, VAL_0, (Tempo)250); // 1
    add_new_pulse(sinal_0, VAL_1, (Tempo)10);    // 2
    
    insert_signal(sinais, sinal_0);
    
    CPPUNIT_ASSERT_EQUAL( 1, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Tempo)(255+250+10), sinais->lista[0].duracaoTotal );
    CPPUNIT_ASSERT( sinais->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].tempo, (Tempo)(255) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].valor, VAL_0 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].tempo, (Tempo)(250) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].tempo, (Tempo)(10) );
    
    Sinal *sinal_1 = new_signal( (char*)"sinal_teste_1" );
    add_new_pulse(sinal_1, VAL_1, (Tempo)1010); // 0
    add_new_pulse(sinal_1, VAL_1, (Tempo)200);  // 1
    add_new_pulse(sinal_1, VAL_0, (Tempo)5);  // 2
    
    insert_signal(sinais, sinal_1);
    
    CPPUNIT_ASSERT_EQUAL( 2, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Tempo)(1010+200+5), sinais->lista[1].duracaoTotal );
    CPPUNIT_ASSERT( sinais->lista[1].pulsos );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[0].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[0].tempo, (Tempo)(1010) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[1].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[1].tempo, (Tempo)(200) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[2].valor, VAL_0 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[2].tempo, (Tempo)(5) );
  }

  void test_get_timeunit_from_str_valid_units()
  {
    // Testa todas as strings de entrada válidas
    CPPUNIT_ASSERT_EQUAL((int)UN_S,  (int)get_timeunit_from_str("s"));
    CPPUNIT_ASSERT_EQUAL((int)UN_MS, (int)get_timeunit_from_str("ms"));
    CPPUNIT_ASSERT_EQUAL((int)UN_US, (int)get_timeunit_from_str("us"));
    CPPUNIT_ASSERT_EQUAL((int)UN_NS, (int)get_timeunit_from_str("ns"));
    CPPUNIT_ASSERT_EQUAL((int)UN_PS, (int)get_timeunit_from_str("ps"));
    CPPUNIT_ASSERT_EQUAL((int)UN_FS, (int)get_timeunit_from_str("fs"));
  }
  
  void test_get_timeunit_from_str_invalid_units()
  {
    // Testa strings que não representam uma unidade válida
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str("GHz"));
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str("segundos"));
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str(" random "));

    // NULL pointer e string vazia
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str(NULL));
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str(""));

    // Sensibilidade a maiúsculas/minúsculas
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str("S"));
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str("Ms"));
    CPPUNIT_ASSERT_EQUAL((int)UN_INVALID, (int)get_timeunit_from_str("US"));
  }

};
