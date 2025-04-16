#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../erros.h"

class Testes_erros : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_erros );
  CPPUNIT_TEST( test_show_error_msg );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_show_error_msg()
  {
    char msg[] = "mensagem";
    int linha = 1;
    int coluna = 1;
    char esperado[] = "wire";
    char encontrado[] = "qwire";
    
    // show_error_msg(...) returna sempre um NULL
    CPPUNIT_ASSERT( ! show_error_msg(msg, linha, coluna, esperado, encontrado) );
  }

};
