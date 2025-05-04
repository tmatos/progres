#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../sinais.h"

class Testes_sinais : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_sinais );
  CPPUNIT_TEST( test_novoSinal );
  CPPUNIT_TEST( test_setSinalNome );
  CPPUNIT_TEST( test_setPulsoNulo );
  CPPUNIT_TEST( test_addPulso );
  CPPUNIT_TEST( test_novaSinais );
  CPPUNIT_TEST( test_addSinal );
  CPPUNIT_TEST( test_addSinalPronto );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novoSinal()
  {
    Sinal *sinal_unnamed = novoSinal(NULL);
    
    CPPUNIT_ASSERT( sinal_unnamed );
    CPPUNIT_ASSERT( !strcmp( (char*)"", (char*)sinal_unnamed->nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal_unnamed->duracaoTotal );

    Sinal *sinal_teste = novoSinal( (char*)"teste" );
    
    CPPUNIT_ASSERT( sinal_teste );
    CPPUNIT_ASSERT( !strcmp( (char*)"teste", (char*)sinal_teste->nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal_teste->duracaoTotal );
  }

  void test_setSinalNome()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );

    int ret = setSinalNome(sinal, (char*)"123" );
    
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );

    ret = setSinalNome(NULL, NULL);

    CPPUNIT_ASSERT(!ret);
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );
  }

  void test_setPulsoNulo()
  {
    Pulso p;

    int ret = setPulsoNulo(&p);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, p.valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, p.tempo );
    CPPUNIT_ASSERT_EQUAL( UN_S, p.unidade );

    ret = setPulsoNulo(NULL);
    CPPUNIT_ASSERT(!ret);
  }

  void test_addPulso()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );

    CPPUNIT_ASSERT(sinal);
    
    int ret = addPulso(sinal, VAL_0, (Tempo)5);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, sinal->duracaoTotal );
    
    ret = addPulso(sinal, VAL_0, (Tempo)30000);
    CPPUNIT_ASSERT(ret);
    CPPUNIT_ASSERT_EQUAL( (Tempo)30005, sinal->duracaoTotal );

    ret = addPulso(NULL, VAL_0, (Tempo)30);
    CPPUNIT_ASSERT(!ret);

    free(sinal->pulsos);
    sinal->pulsos = NULL;
    ret = addPulso(sinal, VAL_0, (Tempo)30);
    CPPUNIT_ASSERT(!ret);
  }

  void test_novaSinais()
  {
    Sinais *sinais = novaSinais();
    
    CPPUNIT_ASSERT_EQUAL( 0, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Sinal*)NULL, sinais->lista );
  }

  void test_addSinal()
  {
    Sinais *sinais = novaSinais();
    
    char str_nome_1[50] = "primeiro_sinal";
    char str_nome_2[50] = "segundo_sinal";
    
    addSinal(sinais, str_nome_1);
    CPPUNIT_ASSERT_EQUAL( 1, sinais->quantidade );
    CPPUNIT_ASSERT( sinais->lista );
    CPPUNIT_ASSERT( sinais->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[0].duracaoTotal );
    
    addSinal(sinais, str_nome_2);
    CPPUNIT_ASSERT_EQUAL( 2, sinais->quantidade );
    CPPUNIT_ASSERT( sinais->lista );
    CPPUNIT_ASSERT( sinais->lista[1].pulsos );
    CPPUNIT_ASSERT_EQUAL( VAL_BLANK, sinais->lista[1].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[1].duracaoTotal );
  }

  void test_addSinalPronto()
  {
    Sinais *sinais = novaSinais();
    
    Sinal *sinal_0 = novoSinal( (char*)"sinal_teste_0" );
    addPulso(sinal_0, VAL_1, (Tempo)255);   // 0
    addPulso(sinal_0, VAL_0, (Tempo)250); // 1
    addPulso(sinal_0, VAL_1, (Tempo)10);    // 2
    
    addSinalPronto(sinais, sinal_0);
    
    CPPUNIT_ASSERT_EQUAL( 1, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Tempo)(255+250+10), sinais->lista[0].duracaoTotal );
    CPPUNIT_ASSERT( sinais->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].tempo, (Tempo)(255) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].valor, VAL_0 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].tempo, (Tempo)(250) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].valor, VAL_1 );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].tempo, (Tempo)(10) );
    
    Sinal *sinal_1 = novoSinal( (char*)"sinal_teste_1" );
    addPulso(sinal_1, VAL_1, (Tempo)1010); // 0
    addPulso(sinal_1, VAL_1, (Tempo)200);  // 1
    addPulso(sinal_1, VAL_0, (Tempo)5);  // 2
    
    addSinalPronto(sinais, sinal_1);
    
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

};

