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
  CPPUNIT_TEST( test_getXPorNome );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novoComponente()
  {
    char nome[] = "entrada_0"; 
    Componente c0 = novoComponente( (char*)nome, input );
    CPPUNIT_ASSERT(c0);
    CPPUNIT_ASSERT( !strcmp( (char*)(c0->nome), (char*)nome ) );
    CPPUNIT_ASSERT_EQUAL( c0->tipo.operador, input );
    CPPUNIT_ASSERT_EQUAL( c0->valorDinamico, VAL_X );
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
    Module* circ = novoCircuito();
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

  void test_getXPorNome()
  {
    char* null_name = NULL;
    Module* circ = novoCircuito();
    CPPUNIT_ASSERT(circ);    

    CPPUNIT_ASSERT( !getPortaPorNome(circ, null_name) );
    CPPUNIT_ASSERT( !getWirePorNome(circ, null_name) );
    CPPUNIT_ASSERT( !getInputPorNome(circ, null_name) );
    CPPUNIT_ASSERT( !getOutputPorNome(circ, null_name) );

    char str_porta[] = "porta_01";
    char str_wire[] = "fio_01"; 
    char str_in[] = "entrada_01";
    char str_out[] = "saida_01"; 
    Componente c_porta = novoComponente( (char*)str_porta, op_and );
    Componente c_wire = novoComponente( (char*)str_wire, wire );
    Componente c_in = novoComponente( (char*)str_in, input );
    Componente c_out = novoComponente( (char*)str_out, output );

    insereComponente(circ->listaPortas, c_porta);
    insereComponente(circ->listaWires, c_wire);
    insereComponente(circ->listaFiosEntrada, c_in);
    insereComponente(circ->listaFiosSaida, c_out);

    CPPUNIT_ASSERT( getPortaPorNome(circ, str_porta) );
    CPPUNIT_ASSERT( getWirePorNome(circ, str_wire) );
    CPPUNIT_ASSERT( getInputPorNome(circ, str_in) );
    CPPUNIT_ASSERT( getOutputPorNome(circ, str_out) );

    CPPUNIT_ASSERT( !getPortaPorNome(circ, str_in) );
    CPPUNIT_ASSERT( !getWirePorNome(circ, str_in) );
    CPPUNIT_ASSERT( !getInputPorNome(circ, str_out) );
    CPPUNIT_ASSERT( !getOutputPorNome(circ, str_in) );
  }

};

