#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../lex.h"

class Testes_lex : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_lex );
  CPPUNIT_TEST( test_iguais );
  CPPUNIT_TEST( test_apenasDigitos );
  CPPUNIT_TEST( test_novaListaToken );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novaListaToken()
  {
    ListaToken* l = novaListaToken();

	CPPUNIT_ASSERT(l);

    CPPUNIT_ASSERT( ! l->primeiro );
    CPPUNIT_ASSERT( ! l->ultimo );
    CPPUNIT_ASSERT_EQUAL( l->tamanho, 0 );
  }

  void test_iguais()
  {
    char str_0[] = "";
    char str_1[] = "";
    char str_2[] = "p";
    char str_3[] = "p";
    char str_4[] = "wp";
    char str_5[] = "Wp";
    char str_6[] = "abc def";
    char str_7[] = "abc_def";
    char str_8[] = "a1b2c3_defACZ-780";
    char str_9[] = "a1b2c3_defACZ-780";
    
    CPPUNIT_ASSERT(   iguais(str_0, str_1) );
    CPPUNIT_ASSERT( ! iguais(str_1, str_2) );
    CPPUNIT_ASSERT(   iguais(str_2, str_3) );
    CPPUNIT_ASSERT( ! iguais(str_3, str_4) );
    CPPUNIT_ASSERT( ! iguais(str_4, str_5) );
    CPPUNIT_ASSERT( ! iguais(str_5, str_6) );
    CPPUNIT_ASSERT( ! iguais(str_6, str_7) );
    CPPUNIT_ASSERT(   iguais(str_8, str_9) );
    CPPUNIT_ASSERT(   iguais(str_7, str_7) );
  }

  void test_apenasDigitos()
  {
    char str_0[] = "001123456789";
    char str_1[] = "001123456789a";
    char str_2[] = "001123456789 ";

    CPPUNIT_ASSERT(   apenasDigitos(str_0) );
    CPPUNIT_ASSERT( ! apenasDigitos(str_1) );
    CPPUNIT_ASSERT( ! apenasDigitos(str_2) );
    CPPUNIT_ASSERT( ! apenasDigitos(NULL) );
  }

};

