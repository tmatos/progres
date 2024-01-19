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
  CPPUNIT_TEST( test_simula_CircuitoVazio );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_simula_CircuitoVazio()
  {
    t_circuito* circ = novoCircuito();
    Sinais *inputs = novaSinais();
    Sinais *outputs = NULL;

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    outputs = simula(circ, inputs);

    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL( outputs->quantidade, 0 );
    CPPUNIT_ASSERT_EQUAL( (Sinal*)NULL, outputs->lista );
  }

};
