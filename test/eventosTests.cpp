#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <string>

#include "estruturas.h"
#include "sinais.h"
#include "eventos.h"

class Testes_eventos : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_eventos );
  CPPUNIT_TEST( test_new_event_at );
  CPPUNIT_TEST( test_insert_event );
  CPPUNIT_TEST( test_delete_event_queue );
  CPPUNIT_TEST( test_get_transitions_at_time );
  CPPUNIT_TEST( test_pop_event );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_new_event_at()
  {
    Tempo t = (Tempo) 255000111;
    EventKind k = EVT_NET_TRANSITION;

    Evento* q = new_event_at(t, k);

    CPPUNIT_ASSERT(q);

    free(q);
  }

  void test_insert_event()
  {
    Evento* fila = NULL;

    insert_event( &fila, (Tempo)1, EVT_NET_TRANSITION, NULL, NULL, VAL_0 );

    CPPUNIT_ASSERT(!fila);

    Tempo t = (Tempo)5000;
    std::string nome_componente_0 = "entrada_componente_0";
    Component* c0 = new_component( nome_componente_0.c_str(), ROLE_INPUT );
    ValorLogico v = VAL_1;

    insert_event( &fila, t, EVT_NET_TRANSITION, c0, NULL, v );
  
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT_EQUAL(fila->quando, t);
    CPPUNIT_ASSERT(fila->listaTransicao);
    CPPUNIT_ASSERT_EQUAL(fila->listaTransicao->fio, c0);
    CPPUNIT_ASSERT_EQUAL(fila->listaTransicao->novoValor, v);
    CPPUNIT_ASSERT(!fila->listaTransicao->proximo);
    CPPUNIT_ASSERT(!fila->proximo);

    t = 11000;
    v = VAL_0;
    std::string nome_componente_1 = "entrada_componente_1";
    Component* c1 = new_component( nome_componente_1.c_str(), ROLE_INPUT );

    insert_event( &fila, t, EVT_NET_TRANSITION, c1, NULL, v );

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

  void test_delete_event_queue()
  {
    Evento* q = NULL;

    Tempo t = (Tempo)10;
    ValorLogico v = VAL_1;
    std::string nome_componente_0 = "in_component_0";
    Component* c_0 = new_component( nome_componente_0.c_str(), ROLE_INPUT );

    insert_event( &q, t, EVT_NET_TRANSITION, c_0, NULL, v );

    t = 11;
    v = VAL_0;
    std::string nome_componente_1 = "in_component_1";
    Component* c_1 = new_component( nome_componente_1.c_str(), ROLE_INPUT );

    insert_event( &q, t, EVT_NET_TRANSITION, c_1, NULL, v );

    // check list existence
    CPPUNIT_ASSERT(q);
    CPPUNIT_ASSERT(q->listaTransicao);

    delete_event_queue(&q);

    // check that func had set q to NULL
    CPPUNIT_ASSERT(!q);

    delete_event_queue(&q);

    // check that is ok to call on NULL queue
    CPPUNIT_ASSERT(!q);
  }

  void test_get_transitions_at_time()
  {
    Evento* fila = NULL;

    Tempo t = (Tempo)5000;
    std::string nome_componente_0 = "wire_component_0";
    Component* c0 = new_component( nome_componente_0.c_str(), ROLE_WIRE );
    ValorLogico v = VAL_1;

    insert_event( &fila, t, EVT_NET_TRANSITION, c0, NULL, v );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->listaTransicao);
    CPPUNIT_ASSERT( !fila->proximo );

    Transicao* tr = get_transitions_at_time(fila, t);
    CPPUNIT_ASSERT(tr);
    std::string str_tr_fio_nome(tr->fio->nome);
    CPPUNIT_ASSERT_EQUAL( nome_componente_0, str_tr_fio_nome );

    CPPUNIT_ASSERT( ! get_transitions_at_time(fila, (Tempo)50 ) );
    CPPUNIT_ASSERT( ! get_transitions_at_time(fila, (Tempo)15000 ) );

    std::string nome_componente_1 = "wire_component_1";
    Component* c1 = new_component( nome_componente_1.c_str(), ROLE_WIRE );
    insert_event( &fila, (Tempo)7000, EVT_NET_TRANSITION, c1, NULL, VAL_0 );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->proximo);

    std::string nome_componente_2 = "wire_component_2";
    Component* c2 = new_component( nome_componente_2.c_str(), ROLE_WIRE );
    insert_event( &fila, (Tempo)5550, EVT_NET_TRANSITION, c2, NULL, VAL_1 );
    CPPUNIT_ASSERT(fila);
    CPPUNIT_ASSERT(fila->proximo);
    CPPUNIT_ASSERT(fila->proximo->proximo);

    Transicao* tr_no_meio = get_transitions_at_time(fila, (Tempo)5550);
    CPPUNIT_ASSERT(tr);

    CPPUNIT_ASSERT( ! get_transitions_at_time(fila, (Tempo)5551 ) );

    free(fila->proximo->proximo->listaTransicao);
    free(fila->proximo->proximo);
    free(fila->proximo->listaTransicao);
    free(fila->proximo);
    free(fila->listaTransicao);
    free(fila);

    // what more?
  }

  void test_pop_event()
  {
    Evento* q = NULL;

    CPPUNIT_ASSERT( ! pop_event(NULL) );
    CPPUNIT_ASSERT( ! pop_event(&q) );

    std::string nome_componente_0 = "in_component_0";
    std::string nome_componente_1 = "in_component_1";

    insert_event( &q,
                  (Tempo)10,
                  EVT_NET_TRANSITION,
                  new_component( nome_componente_0.c_str(), ROLE_INPUT ),
                  NULL,
                  VAL_0 );

    insert_event( &q,
                  (Tempo)10,
                  EVT_NET_TRANSITION,
                  new_component( nome_componente_1.c_str(), ROLE_INPUT ),
                  NULL,
                  VAL_X );

    Transicao* tr = pop_event(&q);

    CPPUNIT_ASSERT(tr);
    CPPUNIT_ASSERT(tr->proximo);

    tr = pop_event(&q);

    CPPUNIT_ASSERT(!tr);
  }

};
