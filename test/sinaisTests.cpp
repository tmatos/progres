#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstring>

#include "../sinais.h"

class Testes_sinais : public CppUnit::TestFixture  {

  CPPUNIT_TEST_SUITE( Testes_sinais );
  CPPUNIT_TEST( test_novoSinal );
  CPPUNIT_TEST( test_setSinalNome );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novoSinal()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );
    
    CPPUNIT_ASSERT_EQUAL( (Tempo)0, sinal->duracaoTotal );
  }
  
  void test_setSinalNome()
  {
    Sinal *sinal = novoSinal( (char*)"teste" );
    setSinalNome(sinal, (char*)"123" );
    
    CPPUNIT_ASSERT( !strcmp( (char*)"123", (char*)sinal->nome ) );
  }
  
};

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( Testes_sinais::suite() );
  runner.run();
  return 0;
}
