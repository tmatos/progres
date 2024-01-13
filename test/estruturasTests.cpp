#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../sinais.h"
#include "../estruturas.h"

class Testes_estruturas : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_estruturas );
  CPPUNIT_TEST( test_novoComponente );
  CPPUNIT_TEST( test_novaListaComponente );
  CPPUNIT_TEST( test_novaListaComponenteTamanho );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novoComponente()
  {
	char nome[] = "entrada_0"; 
	Componente c0 = novoComponente( (char*)nome, input );

    CPPUNIT_ASSERT(c0);
    CPPUNIT_ASSERT( !strcmp( (char*)(c0->nome), (char*)nome ) );
    CPPUNIT_ASSERT_EQUAL( c0->tipo.operador, input );
    CPPUNIT_ASSERT_EQUAL( c0->valorDinamico, x );
    CPPUNIT_ASSERT( c0->listaEntrada );
    CPPUNIT_ASSERT( c0->listaSaida );
  }

  void test_novaListaComponente()
  {
	ListaComponente* list = novaListaComponente();
	
	CPPUNIT_ASSERT(list);
	CPPUNIT_ASSERT_EQUAL(list->tamanho, 0);
	CPPUNIT_ASSERT(!list->itens);
  }

  void test_novaListaComponenteTamanho()
  {
    ListaComponente* list = novaListaComponenteTamanho(50);
	
    CPPUNIT_ASSERT(list);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 50);
    CPPUNIT_ASSERT(list->itens);
  }

};

