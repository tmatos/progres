#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "sinais.h"
#include "strutil.h"

class Testes_sinais : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_sinais );
  CPPUNIT_TEST( test_new_signal_unamed );
  CPPUNIT_TEST( test_new_signal_named );
  CPPUNIT_TEST( test_set_signal_name );
  CPPUNIT_TEST( test_set_pulse_blank );
  CPPUNIT_TEST( test_add_new_pulse );
  CPPUNIT_TEST( test_new_signal_list );
  CPPUNIT_TEST( test_add_new_signal );
  CPPUNIT_TEST( test_insert_signal );
  CPPUNIT_TEST( test_get_timeunit_from_str_valid_units );
  CPPUNIT_TEST( test_get_timeunit_from_str_invalid_units );
  CPPUNIT_TEST( test_get_str_from_timeunit );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_new_signal_unamed()
  {
    std::string str_empty = "";

    Signal *sinal_unnamed = new_signal(NULL);
    CPPUNIT_ASSERT( sinal_unnamed );

    std::string str_nome_sinal_unnamed( sinal_unnamed->name );

    CPPUNIT_ASSERT_EQUAL( str_empty, str_nome_sinal_unnamed );
    CPPUNIT_ASSERT_EQUAL( (Time)0, sinal_unnamed->total_time );
  }

  void test_new_signal_named()
  {
    std::string str_nome_teste = "teste";

    Signal *sinal_teste = new_signal( str_nome_teste.c_str() );
    CPPUNIT_ASSERT( sinal_teste );
    
    std::string str_nome_teste_sinal( sinal_teste->name );

    CPPUNIT_ASSERT_EQUAL( str_nome_teste, str_nome_teste_sinal );
    CPPUNIT_ASSERT_EQUAL( (Time)0, sinal_teste->total_time );
  }

  void test_set_signal_name()
  {
    std::string str_original_name = "teste";

    Signal *sinal = new_signal( str_original_name.c_str() );
    CPPUNIT_ASSERT( sinal );

    std::string str_new_name = "123";

    int ret = set_signal_name(sinal, str_new_name.c_str() );
    CPPUNIT_ASSERT(ret);

    std::string str_new_name_sinal( sinal->name );

    CPPUNIT_ASSERT_EQUAL( str_new_name, str_new_name_sinal );

    ret = set_signal_name(NULL, NULL);
    CPPUNIT_ASSERT(!ret);

    std::string str_new_name_sinal_again( sinal->name );

    CPPUNIT_ASSERT_EQUAL( str_new_name, str_new_name_sinal_again );
  }

  void test_set_pulse_blank()
  {
    Pulse p;

    int ret = set_pulse_blank(&p);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, p.value );
    CPPUNIT_ASSERT_EQUAL( (Time)0, p.time );
    CPPUNIT_ASSERT_EQUAL( UN_S, p.unit );

    ret = set_pulse_blank(NULL);
    CPPUNIT_ASSERT(!ret);
  }

  void test_add_new_pulse()
  {
    std::string str_signal_name = "teste";

    Signal *sinal = new_signal( str_signal_name.c_str() );

    CPPUNIT_ASSERT(sinal);
    
    int ret = add_new_pulse(sinal, VAL_0, (Time)5);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Time)5, sinal->total_time );
    
    ret = add_new_pulse(sinal, VAL_0, (Time)30000);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Time)30005, sinal->total_time );

    ret = add_new_pulse(NULL, VAL_0, (Time)30);
    CPPUNIT_ASSERT(!ret);

    free(sinal->pulses);
    sinal->pulses = NULL;
    ret = add_new_pulse(sinal, VAL_0, (Time)30);
    CPPUNIT_ASSERT(!ret);
  }

  void test_new_signal_list()
  {
    SignalArray *sinais = new_signal_list();
    
    CPPUNIT_ASSERT_EQUAL( 0, sinais->count );
    CPPUNIT_ASSERT_EQUAL( (Signal*)NULL, sinais->itens );
  }

  void test_add_new_signal()
  {
    SignalArray *sinais = new_signal_list();
    
    char str_nome_1[50] = "primeiro_sinal";
    char str_nome_2[50] = "segundo_sinal";
    
    add_new_signal(sinais, str_nome_1);
    CPPUNIT_ASSERT_EQUAL( 1, sinais->count );
    CPPUNIT_ASSERT( sinais->itens );
    CPPUNIT_ASSERT( sinais->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)0, sinais->itens[0].total_time );
    
    add_new_signal(sinais, str_nome_2);
    CPPUNIT_ASSERT_EQUAL( 2, sinais->count );
    CPPUNIT_ASSERT( sinais->itens );
    CPPUNIT_ASSERT( sinais->itens[1].pulses );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->itens[1].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)0, sinais->itens[1].total_time );
  }

  void test_insert_signal()
  {
    std::string str_signal_name_teste_0 = "sinal_teste_0";
    std::string str_signal_name_teste_1 = "sinal_teste_0";

    SignalArray *sinais = new_signal_list();
    
    Signal *sinal_0 = new_signal( str_signal_name_teste_0.c_str() );
    add_new_pulse(sinal_0, VAL_1, (Time)255);   // 0
    add_new_pulse(sinal_0, VAL_0, (Time)250);   // 1
    add_new_pulse(sinal_0, VAL_1, (Time) 10);   // 2
    
    insert_signal(sinais, sinal_0);
    
    CPPUNIT_ASSERT_EQUAL( 1, sinais->count );
    CPPUNIT_ASSERT_EQUAL( (Time)(255+250+10), sinais->itens[0].total_time );
    CPPUNIT_ASSERT( sinais->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL(      VAL_1, sinais->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)255, sinais->itens[0].pulses[0].time );
    CPPUNIT_ASSERT_EQUAL(      VAL_0, sinais->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)250, sinais->itens[0].pulses[1].time );
    CPPUNIT_ASSERT_EQUAL(      VAL_1, sinais->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL(  (Time)10, sinais->itens[0].pulses[2].time );
    
    Signal *sinal_1 = new_signal( str_signal_name_teste_1.c_str() );
    add_new_pulse(sinal_1, VAL_1, (Time)1010);  // 0
    add_new_pulse(sinal_1, VAL_1, (Time) 200);  // 1
    add_new_pulse(sinal_1, VAL_0, (Time)   5);  // 2
    add_new_pulse(sinal_1, VAL_X, (Time)   1);  // 3
    // IMPORTANT: pulses 0 and 1 are combined because they have same value.
    
    insert_signal(sinais, sinal_1);
    
    CPPUNIT_ASSERT_EQUAL( 2, sinais->count );
    CPPUNIT_ASSERT_EQUAL( (Time)(1010+200+5+1), sinais->itens[1].total_time );
    CPPUNIT_ASSERT( sinais->itens[1].pulses );
    CPPUNIT_ASSERT_EQUAL(             VAL_1, sinais->itens[1].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)(1010+200), sinais->itens[1].pulses[0].time );
    CPPUNIT_ASSERT_EQUAL(             VAL_0, sinais->itens[1].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL(        (Time)(5), sinais->itens[1].pulses[1].time );
    CPPUNIT_ASSERT_EQUAL(             VAL_X, sinais->itens[1].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL(        (Time)(1), sinais->itens[1].pulses[2].time );
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

  void test_get_str_from_timeunit()
  {
    CPPUNIT_ASSERT( iguais("s",  get_str_from_timeunit(UN_S)) );
    CPPUNIT_ASSERT( iguais("ms", get_str_from_timeunit(UN_MS)) );
    CPPUNIT_ASSERT( iguais("us", get_str_from_timeunit(UN_US)) );
    CPPUNIT_ASSERT( iguais("ns", get_str_from_timeunit(UN_NS)) );
    CPPUNIT_ASSERT( iguais("ps", get_str_from_timeunit(UN_PS)) );
    CPPUNIT_ASSERT( iguais("fs", get_str_from_timeunit(UN_FS)) );
    CPPUNIT_ASSERT( iguais("ns", get_str_from_timeunit(UN_INVALID)) ); // Testing default case
  }

};
