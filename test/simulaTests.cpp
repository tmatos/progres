#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../estruturas.h"
#include "../sinais.h"
#include "../eventos.h"
#include "../verilog.h"
#include "../inout.h"
#include "../simula.h"

class Testes_simula : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_simula );
  CPPUNIT_TEST( test_simula_CircuitoNull );
  CPPUNIT_TEST( test_simula_CircuitoVazio );
  CPPUNIT_TEST( test_simula_CircuitoUmaEntrada );
  CPPUNIT_TEST( test_simula_samplefile_andgates_v );
  CPPUNIT_TEST( test_simula_samplefile_orgates_v );
  CPPUNIT_TEST( test_simula_samplefile_nandgates_v );
  CPPUNIT_TEST( test_simula_samplefile_norgates_v );
  CPPUNIT_TEST( test_simula_samplefile_notgates_v );
  CPPUNIT_TEST( test_simula_samplefile_bufgates_v );
  CPPUNIT_TEST( test_simula_samplefile_xorgates_v );
  CPPUNIT_TEST( test_simula_samplefile_xnorgates_v );
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
    addPulso( &(inputs->lista[0]), VAL_1, (Tempo)20 );    // 0
    addPulso( &(inputs->lista[0]), VAL_0, (Tempo)50 );  // 1
    addPulso( &(inputs->lista[0]), VAL_1, (Tempo)105 );   // 2

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

  void test_simula_samplefile_andgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    FILE* f_andgates_v = fopen("./verilog_sample_src/andgates.v", "r");
    FILE* f_andgates_in = fopen("./inout_sample_files/andgates.in", "r");

    CPPUNIT_ASSERT( f_andgates_v );
    CPPUNIT_ASSERT( f_andgates_in );

    circuit = carregaCircuito(f_andgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_andgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );
    CPPUNIT_ASSERT( outputs->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, outputs->lista[0].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->lista[0].pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)15, outputs->lista[0].pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->lista[0].pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[2].tempo );
  }

  void test_simula_samplefile_orgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    FILE* f_orgates_v = fopen("./verilog_sample_src/orgates.v", "r");
    FILE* f_orgates_in = fopen("./inout_sample_files/orgates.in", "r");

    CPPUNIT_ASSERT( f_orgates_v );
    CPPUNIT_ASSERT( f_orgates_in );

    circuit = carregaCircuito(f_orgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_orgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );
    CPPUNIT_ASSERT( outputs->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, outputs->lista[0].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->lista[0].pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->lista[0].pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)15, outputs->lista[0].pulsos[2].tempo );
  }

  void test_simula_samplefile_nandgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    FILE* f_nandgates_v = fopen("./verilog_sample_src/nandgates.v", "r");
    FILE* f_nandgates_in = fopen("./inout_sample_files/nandgates.in", "r");

    CPPUNIT_ASSERT( f_nandgates_v );
    CPPUNIT_ASSERT( f_nandgates_in );

    circuit = carregaCircuito(f_nandgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_nandgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );
    CPPUNIT_ASSERT( outputs->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, outputs->lista[0].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->lista[0].pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)15, outputs->lista[0].pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->lista[0].pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[2].tempo );
  }

  void test_simula_samplefile_norgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    FILE* f_norgates_v = fopen("./verilog_sample_src/norgates.v", "r");
    FILE* f_norgates_in = fopen("./inout_sample_files/norgates.in", "r");

    CPPUNIT_ASSERT( f_norgates_v );
    CPPUNIT_ASSERT( f_norgates_in );

    circuit = carregaCircuito(f_norgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_norgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );
    CPPUNIT_ASSERT( outputs->lista[0].pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, outputs->lista[0].duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->lista[0].pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->lista[0].pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, outputs->lista[0].pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->lista[0].pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)15, outputs->lista[0].pulsos[2].tempo );
  }

  void test_simula_samplefile_notgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinal s;
    FILE* f_notgates_v = fopen("./verilog_sample_src/notgates.v", "r");
    FILE* f_notgates_in = fopen("./inout_sample_files/notgates.in", "r");

    CPPUNIT_ASSERT( f_notgates_v );
    CPPUNIT_ASSERT( f_notgates_in );

    circuit = carregaCircuito(f_notgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_notgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 2, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    s = outputs->lista[0];

    CPPUNIT_ASSERT( ! strcmp("na", s.nome ) );
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );

    s = outputs->lista[1];

    CPPUNIT_ASSERT( ! strcmp("nnb", s.nome ) );
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)20, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );
  }

  void test_simula_samplefile_bufgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinal s;
    FILE* f_bufgates_v = fopen("./verilog_sample_src/bufgates.v", "r");
    FILE* f_bufgates_in = fopen("./inout_sample_files/bufgates.in", "r");

    CPPUNIT_ASSERT( f_bufgates_v );
    CPPUNIT_ASSERT( f_bufgates_in );

    circuit = carregaCircuito(f_bufgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_bufgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    s = outputs->lista[0];

    CPPUNIT_ASSERT( ! strcmp("y", s.nome ) );
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)38, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)13, s.pulsos[3].tempo );
  }
  
  void test_simula_samplefile_xorgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinal s;
    FILE* f_xorgates_v = fopen("./verilog_sample_src/xorgates.v", "r");
    FILE* f_xorgates_in = fopen("./inout_sample_files/xorgates.in", "r");

    CPPUNIT_ASSERT( f_xorgates_v );
    CPPUNIT_ASSERT( f_xorgates_in );

    circuit = carregaCircuito(f_xorgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_xorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );

    s = outputs->lista[0];
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );
  }

  void test_simula_samplefile_xnorgates_v()
  {
    t_circuito* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinal s;
    FILE* f_xnorgates_v = fopen("./verilog_sample_src/xnorgates.v", "r");
    FILE* f_xnorgates_in = fopen("./inout_sample_files/xnorgates.in", "r");

    CPPUNIT_ASSERT( f_xnorgates_v );
    CPPUNIT_ASSERT( f_xnorgates_in );

    circuit = carregaCircuito(f_xnorgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = carregaEntradas(f_xnorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );

    s = outputs->lista[0];
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)25, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );
  }

};