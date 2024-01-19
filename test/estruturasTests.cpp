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
  CPPUNIT_TEST( test_novoCircuito );
  CPPUNIT_TEST( test_insereComponente );
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

  void test_novoCircuito()
  {
    t_circuito* circ = novoCircuito();

    CPPUNIT_ASSERT(circ);

    CPPUNIT_ASSERT(circ->listaFiosEntrada);
    CPPUNIT_ASSERT_EQUAL(circ->listaFiosEntrada->tamanho, 0);
    CPPUNIT_ASSERT(!circ->listaFiosEntrada->itens);

    CPPUNIT_ASSERT(circ->listaFiosSaida);
    CPPUNIT_ASSERT_EQUAL(circ->listaFiosSaida->tamanho, 0);
    CPPUNIT_ASSERT(!circ->listaFiosSaida->itens);

    CPPUNIT_ASSERT(circ->listaWires);
    CPPUNIT_ASSERT_EQUAL(circ->listaWires->tamanho, 0);
    CPPUNIT_ASSERT(!circ->listaWires->itens);

    CPPUNIT_ASSERT(circ->listaPortas);
    CPPUNIT_ASSERT_EQUAL(circ->listaPortas->tamanho, 0);
    CPPUNIT_ASSERT(!circ->listaPortas->itens);
  }

  void test_insereComponente()
  {
    char str_c0[] = "entrada_0";
    char str_c1[] = "entrada_1"; 
    Componente c0 = novoComponente( (char*)str_c0, input );
    Componente c1 = novoComponente( (char*)str_c1, input );

    ListaComponente* list = novaListaComponente();

    CPPUNIT_ASSERT(c0);
    CPPUNIT_ASSERT(c1);
    CPPUNIT_ASSERT(list);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 0);

    insereComponente(list, c0);

    CPPUNIT_ASSERT_EQUAL(list->tamanho, 1);
    CPPUNIT_ASSERT(list->itens);
    CPPUNIT_ASSERT_EQUAL(list->itens[0], c0);

    insereComponente(list, c1);

    CPPUNIT_ASSERT_EQUAL(list->tamanho, 2);
    CPPUNIT_ASSERT(list->itens);
    CPPUNIT_ASSERT_EQUAL(list->itens[1], c1);
  }

};

