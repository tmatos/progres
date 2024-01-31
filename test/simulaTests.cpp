#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../estruturas.h"
#include "../sinais.h"
#include "../eventos.h"
#include "../simula.h"

class Testes_simula : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_simula );
  CPPUNIT_TEST( test_simula_CircuitoNull );
  CPPUNIT_TEST( test_simula_CircuitoVazio );
  CPPUNIT_TEST( test_simula_CircuitoUmaEntrada );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_simula_CircuitoNull()
  {
    t_circuito *circ = NULL;
    Sinais *inputs = NULL;
    Sinais *outputs = NULL;

    outputs = simula(circ, inputs);

    CPPUNIT_ASSERT_EQUAL( (Sinais*)NULL, outputs );
  }

  void test_simula_CircuitoVazio()
  {
    t_circuito *circ = novoCircuito();
    Sinais *inputs = novaSinais();
    Sinais *outputs = NULL;

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    outputs = simula(circ, inputs);

    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL( outputs->quantidade, 0 );
    CPPUNIT_ASSERT_EQUAL( (Sinal*)NULL, outputs->lista );
  }

  void test_simula_CircuitoUmaEntrada()
  {
    t_circuito *circ = novoCircuito();
    Sinais *inputs = novaSinais();
    Sinais *outputs = NULL;

    char str_nome_entrada_1[50] = "sinal_in_1";

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    addSinal( inputs, str_nome_entrada_1 );
    addPulso( &(inputs->lista[0]), um, (Tempo)20 );    // 0
    addPulso( &(inputs->lista[0]), zero, (Tempo)50 );  // 1
    addPulso( &(inputs->lista[0]), um, (Tempo)105 );   // 2

    CPPUNIT_ASSERT_EQUAL( 1, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( inputs->lista[0].pulsos );

    Componente cp_in_wire = novoComponente(str_nome_entrada_1, wire);

    adicionaEntrada(circ, cp_in_wire);

    CPPUNIT_ASSERT_EQUAL( 1, circ->listaFiosEntrada->tamanho );
    CPPUNIT_ASSERT( circ->listaFiosEntrada->itens );
    CPPUNIT_ASSERT( ! strcmp(str_nome_entrada_1, circ->listaFiosEntrada->itens[0]->nome) );

    outputs = simula(circ, inputs);

    CPPUNIT_ASSERT(outputs);
  }

};
