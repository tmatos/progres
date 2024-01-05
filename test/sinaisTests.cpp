#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../sinais.h"

class Testes_sinais : public CppUnit::TestFixture  {

  CPPUNIT_TEST_SUITE( Testes_sinais );
  CPPUNIT_TEST( test_novoSinal );
  CPPUNIT_TEST( test_setSinalNome );
  CPPUNIT_TEST( test_addPulso );
  CPPUNIT_TEST( test_novaSinais );
  CPPUNIT_TEST( test_addSinal );
  CPPUNIT_TEST( test_addSinalPronto );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novoSinal()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );
    
    CPPUNIT_ASSERT( sinal );
    CPPUNIT_ASSERT( !strcmp( (char*)"teste", (char*)sinal->nome ) );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal->duracaoTotal );
  }
  
  void test_setSinalNome()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );
    setSinalNome(sinal, (char*)"123" );
    
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );
  }
  
  void test_addPulso()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );
    
    addPulso(sinal, zero, (Tempo)5);
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, sinal->duracaoTotal );
    
    addPulso(sinal, zero, (Tempo)30000);
    CPPUNIT_ASSERT_EQUAL( (Tempo)30005, sinal->duracaoTotal );
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
    CPPUNIT_ASSERT_EQUAL( nulo, sinais->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[0].duracaoTotal );
    
    addSinal(sinais, str_nome_2);
    CPPUNIT_ASSERT_EQUAL( 2, sinais->quantidade );
    CPPUNIT_ASSERT( sinais->lista );
    CPPUNIT_ASSERT( sinais->lista[1].pulsos );
    CPPUNIT_ASSERT_EQUAL( nulo, sinais->lista[1].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinais->lista[1].duracaoTotal );
  }

  void test_addSinalPronto()
  {
    Sinais *sinais = novaSinais();
    
    Sinal *sinal_0 = novoSinal( (char*)"sinal_teste_0" );
    addPulso(sinal_0, um, (Tempo)255);   // 0
    addPulso(sinal_0, zero, (Tempo)250); // 1 
    addPulso(sinal_0, um, (Tempo)10);    // 2
    
    addSinalPronto(sinais, sinal_0);
    
    CPPUNIT_ASSERT_EQUAL( 1, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Tempo)(255+250+10), sinais->lista[0].duracaoTotal );
    CPPUNIT_ASSERT( sinais->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].valor, um );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[0].tempo, (Tempo)(255) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].valor, zero );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[1].tempo, (Tempo)(250) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].valor, um );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[0].pulsos[2].tempo, (Tempo)(10) );
    
    Sinal *sinal_1 = novoSinal( (char*)"sinal_teste_1" );
    addPulso(sinal_1, um, (Tempo)1010); // 0
    addPulso(sinal_1, um, (Tempo)200);  // 1
    addPulso(sinal_1, zero, (Tempo)5);    // 2
    
    addSinalPronto(sinais, sinal_1);
    
    CPPUNIT_ASSERT_EQUAL( 2, sinais->quantidade );
    CPPUNIT_ASSERT_EQUAL( (Tempo)(1010+200+5), sinais->lista[1].duracaoTotal );
    CPPUNIT_ASSERT( sinais->lista[1].pulsos );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[0].valor, um );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[0].tempo, (Tempo)(1010) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[1].valor, um );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[1].tempo, (Tempo)(200) );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[2].valor, zero );
    CPPUNIT_ASSERT_EQUAL( sinais->lista[1].pulsos[2].tempo, (Tempo)(5) );
  }
  
};

int main(int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TextOutputter textOut( &runner.result(), std::cout );
  runner.addTest( Testes_sinais::suite() );
  runner.run();
  textOut.printStatistics();
  return 0;
}
