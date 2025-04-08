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
  CPPUNIT_TEST( test_getTransicoesEm );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_insereEvento()
  {
    Evento* fila = NULL;

    Tempo t = (Tempo)5000;
    Componente c0 = novoComponente( (char*)"entrada_componente_0", input );
    ValorLogico v = um;

    insereEvento( &fila, t, c0, v );
  
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

    insereEvento( &fila, t, c1, v );

    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT_EQUAL( (Tempo)5000, fila->quando );
    CPPUNIT_ASSERT(fila->listaTransicao);
    CPPUNIT_ASSERT_EQUAL( c0, fila->listaTransicao->fio );

    CPPUNIT_ASSERT(fila->proximo);
    CPPUNIT_ASSERT_EQUAL( t, fila->proximo->quando );
    CPPUNIT_ASSERT(fila->proximo->listaTransicao);
    CPPUNIT_ASSERT_EQUAL( c1, fila->proximo->listaTransicao->fio );
    CPPUNIT_ASSERT_EQUAL( v, fila->proximo->listaTransicao->novoValor );
    CPPUNIT_ASSERT(!fila->proximo->listaTransicao->proximo);
    CPPUNIT_ASSERT(!fila->proximo->proximo);
    
    free(fila->proximo->listaTransicao);
    free(fila->proximo);
    free(fila->listaTransicao);
    free(fila);

    //TODO: testar ainda mais outras possibilidades
  }

  void test_getTransicoesEm()
  {
    Evento* fila = NULL;

    Tempo t = (Tempo)5000;
    Componente c0 = novoComponente( (char*)"wire_component_0", wire );
    ValorLogico v = um;

    insereEvento( &fila, t, c0, v );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->listaTransicao);
    CPPUNIT_ASSERT( !fila->proximo );

    Transicao* tr = getTransicoesEm(fila, t);
    CPPUNIT_ASSERT(tr);
    // CPPUNIT_ASSERT( !strcmp("wire_component_0", (char*)tr->fio->nome) );

    CPPUNIT_ASSERT( ! getTransicoesEm(fila, (Tempo)50 ) );
    CPPUNIT_ASSERT( ! getTransicoesEm(fila, (Tempo)15000 ) );

    Componente c1 = novoComponente( (char*)"wire_component_1", wire );
    insereEvento( &fila, (Tempo)7000, c1, zero );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->proximo);

    Componente c2 = novoComponente( (char*)"wire_component_2", wire );
    insereEvento( &fila, (Tempo)5550, c2, um );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->proximo);
    CPPUNIT_ASSERT(fila->proximo->proximo);

    Transicao* tr_no_meio = getTransicoesEm(fila, (Tempo)5550);
    CPPUNIT_ASSERT(tr);

    CPPUNIT_ASSERT( ! getTransicoesEm(fila, (Tempo)5551 ) );

    free(fila->proximo->proximo->listaTransicao);
    free(fila->proximo->proximo);
    free(fila->proximo->listaTransicao);
    free(fila->proximo);
    free(fila->listaTransicao);
    free(fila);

    // what more?
  }

};
