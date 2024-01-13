#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../estruturas.h"
#include "../sinais.h"
#include "../eventos.h"

class Testes_eventos : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_eventos );
  CPPUNIT_TEST( test_insereEvento );
  //CPPUNIT_TEST(  );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_insereEvento()
  {
	Evento *fila = NULL;

	Tempo t = (Tempo)5000;
	Componente c0 = novoComponente( (char*)"entrada_componente_0", input );
	ValorLogico v = um;

	insereEvento( &fila, t, c0, v);
	
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT_EQUAL(fila->quando, t);
    CPPUNIT_ASSERT(fila->listaTransicao);
    CPPUNIT_ASSERT_EQUAL(fila->listaTransicao->fio, c0);
    CPPUNIT_ASSERT_EQUAL(fila->listaTransicao->novoValor, v);
    CPPUNIT_ASSERT(!fila->listaTransicao->proximo);
    CPPUNIT_ASSERT(!fila->proximo);

	t = 11000;
    v = zero;
	Componente c1 = novoComponente( (char*)"entrada_componente_1", input );

	insereEvento( &fila, t, c1, v);

	CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT_EQUAL(fila->quando, (Tempo)5000);
	CPPUNIT_ASSERT(fila->listaTransicao);
	CPPUNIT_ASSERT_EQUAL(fila->listaTransicao->fio, c0);	

	//TODO: terminar e testar as outras possibilidades

  }

};

