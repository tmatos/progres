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
  CPPUNIT_TEST( test_simula_samplefile_delays_v );
  CPPUNIT_TEST( test_simula_samplefile_display_v );
  CPPUNIT_TEST( test_simula_samplefile_dumpfile_v );
  CPPUNIT_TEST( test_simula_samplefile_tri_state_gates_v );
  CPPUNIT_TEST( test_simula_samplefile_numbers_v );
  CPPUNIT_TEST( test_compute_buf_if0_gate );
  CPPUNIT_TEST( test_compute_buf_if1_gate );
  CPPUNIT_TEST( test_compute_not_if0_gate );
  CPPUNIT_TEST( test_compute_not_if1_gate );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_simula_CircuitoNull()
  {
    Module *circ = NULL;
    Sinais *inputs = NULL;
    Sinais *outputs = NULL;

    outputs = simula(circ, inputs, NULL);

    CPPUNIT_ASSERT_EQUAL( (Sinais*)NULL, outputs );

    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_CircuitoVazio()
  {
    Module *circ = new_module();
    Sinais *inputs = new_signal_list();
    Sinais *outputs = NULL;

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    outputs = simula(circ, inputs, NULL);

    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL( outputs->quantidade, 0 );
    CPPUNIT_ASSERT_EQUAL( (Sinal*)NULL, outputs->lista );

    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_CircuitoUmaEntrada()
  {
    Module *circ = new_module();
    Sinais *inputs = new_signal_list();
    Sinais *outputs = NULL;

    char str_nome_entrada_1[50] = "sinal_in_1";

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    add_new_signal( inputs, str_nome_entrada_1 );
    add_new_pulse( &(inputs->lista[0]), VAL_1, (Tempo)20 );  // 0
    add_new_pulse( &(inputs->lista[0]), VAL_0, (Tempo)50 );  // 1
    add_new_pulse( &(inputs->lista[0]), VAL_1, (Tempo)105 ); // 2

    CPPUNIT_ASSERT_EQUAL( 1, inputs->quantidade );
    CPPUNIT_ASSERT( inputs->lista );
    CPPUNIT_ASSERT( inputs->lista[0].pulsos );

    Component* cp_in_wire = new_component(str_nome_entrada_1, ROLE_WIRE);

    add_input(circ, cp_in_wire);

    CPPUNIT_ASSERT_EQUAL( 1, circ->listaFiosEntrada->tamanho );
    CPPUNIT_ASSERT( circ->listaFiosEntrada->itens );
    CPPUNIT_ASSERT( ! strcmp(str_nome_entrada_1, circ->listaFiosEntrada->itens[0]->nome) );

    outputs = simula(circ, inputs, NULL);

    CPPUNIT_ASSERT(outputs);

    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_samplefile_andgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    char s_andgates_v[] = "./verilog_sample_src/andgates.v";
    FILE* f_andgates_in = fopen("./inout_sample_files/andgates.in", "r");

    CPPUNIT_ASSERT( f_andgates_in );

    circuit = load_module(s_andgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_andgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
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

    fclose(f_andgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_orgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    char s_orgates_v[] = "./verilog_sample_src/orgates.v";
    FILE* f_orgates_in = fopen("./inout_sample_files/orgates.in", "r");

    CPPUNIT_ASSERT( f_orgates_in );

    circuit = load_module(s_orgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_orgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
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

    fclose(f_orgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_nandgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    char s_nandgates_v[] = "./verilog_sample_src/nandgates.v";
    FILE* f_nandgates_in = fopen("./inout_sample_files/nandgates.in", "r");

    CPPUNIT_ASSERT( f_nandgates_in );

    circuit = load_module(s_nandgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_nandgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
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

    fclose(f_nandgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_norgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    char s_norgates_v[] = "./verilog_sample_src/norgates.v";
    FILE* f_norgates_in = fopen("./inout_sample_files/norgates.in", "r");

    CPPUNIT_ASSERT( f_norgates_in );

    circuit = load_module(s_norgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_norgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
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

    fclose(f_norgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_notgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    Sinal s;
    char s_notgates_v[] = "./verilog_sample_src/notgates.v";
    FILE* f_notgates_in = fopen("./inout_sample_files/notgates.in", "r");

    CPPUNIT_ASSERT( f_notgates_in );

    circuit = load_module(s_notgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_notgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
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

    fclose(f_notgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_bufgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    Sinal s;
    char s_bufgates_v[] = "./verilog_sample_src/bufgates.v";
    FILE* f_bufgates_in = fopen("./inout_sample_files/bufgates.in", "r");

    CPPUNIT_ASSERT( f_bufgates_in );

    circuit = load_module(s_bufgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_bufgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    s = outputs->lista[0];

    CPPUNIT_ASSERT( ! strcmp("y", s.nome ) );
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)48, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)13, s.pulsos[3].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[4].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[4].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[5].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[5].tempo );

    fclose(f_bufgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }
  
  void test_simula_samplefile_xorgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    Sinal s;
    char s_xorgates_v[] = "./verilog_sample_src/xorgates.v";
    FILE* f_xorgates_in = fopen("./inout_sample_files/xorgates.in", "r");

    CPPUNIT_ASSERT( f_xorgates_in );

    circuit = load_module(s_xorgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_xorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );

    s = outputs->lista[0];
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)35, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[4].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[4].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[5].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[5].tempo );

    fclose(f_xorgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_xnorgates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    Sinal s;
    char s_xnorgates_v[] = "./verilog_sample_src/xnorgates.v";
    FILE* f_xnorgates_in = fopen("./inout_sample_files/xnorgates.in", "r");

    CPPUNIT_ASSERT( f_xnorgates_in );

    circuit = load_module(s_xnorgates_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_xnorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->lista[0].nome ) );

    s = outputs->lista[0];
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)35, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)10, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[3].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[4].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[4].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[5].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)5, s.pulsos[5].tempo );

    fclose(f_xnorgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_delays_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinais* sim_outputs = NULL;
    Evento* q = new_empty_event();

    char s_delays_v[] = "./verilog_sample_src/delays.v";
    FILE* f_delays_in = fopen("./inout_sample_files/delays.in", "r");
    FILE* f_delays_in_out = fopen("./inout_sample_files/delays.in.out", "r");

    CPPUNIT_ASSERT(f_delays_in);
    CPPUNIT_ASSERT(f_delays_in_out);

    circuit = load_module(s_delays_v, &q);
    CPPUNIT_ASSERT(circuit);

    inputs = load_input_signals(f_delays_in);
    CPPUNIT_ASSERT(inputs);

    outputs = load_input_signals(f_delays_in_out);
    CPPUNIT_ASSERT(outputs);

    CPPUNIT_ASSERT_EQUAL(8, outputs->quantidade);
    CPPUNIT_ASSERT(outputs->lista);

    sim_outputs = simula(circuit, inputs, &q);

    CPPUNIT_ASSERT(sim_outputs);
    CPPUNIT_ASSERT_EQUAL(8, sim_outputs->quantidade);
    CPPUNIT_ASSERT(sim_outputs->lista);

    CPPUNIT_ASSERT( helper_compare_signal_lists(outputs, sim_outputs) );
    
    fclose(f_delays_in);
    fclose(f_delays_in_out);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_display_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* sim_outputs = NULL;
    Evento* q = new_empty_event();

    char s_display_v[] = "./verilog_sample_src/display.v";

    circuit = load_module(s_display_v, &q);
    CPPUNIT_ASSERT(circuit);

    inputs = new_signal_list();
    CPPUNIT_ASSERT(inputs);

    sim_outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT(sim_outputs);

    free_signal_list(&inputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_dumpfile_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* sim_outputs = NULL;
    Evento* q = new_empty_event();

    char s_dumpfile_v[] = "./verilog_sample_src/dumpfile.v";
    char s_dumpfile_vcd[] = "./dumpfile.vcd";

    inputs = new_signal_list();
    CPPUNIT_ASSERT(inputs);

    circuit = load_module(s_dumpfile_v, &q);
    CPPUNIT_ASSERT(circuit);
    CPPUNIT_ASSERT(q->listaTransicao);
    CPPUNIT_ASSERT_EQUAL(TASK_DUMPFILE, q->listaTransicao->task_type);

    sim_outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT(sim_outputs);

    FILE* f_dumpfile_vcd = fopen(s_dumpfile_vcd, "r");
    CPPUNIT_ASSERT(f_dumpfile_vcd);

    fclose(f_dumpfile_vcd);
    remove(s_dumpfile_vcd);

    free_signal_list(&inputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_simula_samplefile_tri_state_gates_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Evento* q = new_empty_event();
    Sinal s;
    char s_v[] = "./verilog_sample_src/tri_state_gates.v";
    FILE* f_in = fopen("./inout_sample_files/tri_state_gates.in", "r");

    CPPUNIT_ASSERT( f_in );

    circuit = load_module(s_v, &q);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 4, outputs->quantidade );
    CPPUNIT_ASSERT( outputs->lista );

    CPPUNIT_ASSERT( ! strcmp("o0", outputs->lista[0].nome ) );

    s = outputs->lista[0];
    CPPUNIT_ASSERT( s.pulsos );
    CPPUNIT_ASSERT_EQUAL( (Tempo)12, s.duracaoTotal );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulsos[0].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)4, s.pulsos[0].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulsos[1].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)2, s.pulsos[1].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_Z, s.pulsos[2].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)2, s.pulsos[2].tempo );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulsos[3].valor );
    CPPUNIT_ASSERT_EQUAL( (Tempo)2, s.pulsos[3].tempo );

    fclose(f_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_module(&circuit);
    
    // TODO: check all cases in the Table 7-5 of Std 1364-2005
  }

  void test_simula_samplefile_numbers_v()
  {
    Module* circuit = NULL;
    Sinais* inputs = NULL;
    Sinais* outputs = NULL;
    Sinais* sim_outputs = NULL;
    Evento* q = new_empty_event();

    char s_v[] = "./verilog_sample_src/numbers.v";
    FILE* f_in = fopen("./inout_sample_files/numbers.in", "r");
    FILE* f_out = fopen("./inout_sample_files/numbers.in.out", "r");

    CPPUNIT_ASSERT(f_in);
    CPPUNIT_ASSERT(f_out);

    circuit = load_module(s_v, &q);
    CPPUNIT_ASSERT(circuit);

    inputs = load_input_signals(f_in);
    CPPUNIT_ASSERT(inputs);
    CPPUNIT_ASSERT_EQUAL(1, inputs->quantidade);
    CPPUNIT_ASSERT(inputs->lista);

    outputs = load_input_signals(f_out);
    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL(2, outputs->quantidade);
    CPPUNIT_ASSERT(outputs->lista);

    sim_outputs = simula(circuit, inputs, &q);
    CPPUNIT_ASSERT(sim_outputs);
    CPPUNIT_ASSERT_EQUAL(2, sim_outputs->quantidade);
    CPPUNIT_ASSERT(sim_outputs->lista);

    CPPUNIT_ASSERT( helper_compare_signal_lists(outputs, sim_outputs) );

    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_module(&circuit);
  }

  /** @brief Single test case that covers all logic paths for compute_buf_if0_gate.
   */
  void test_compute_buf_if0_gate()
  {
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if0_gate(VAL_1, VAL_H));

    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if0_gate(VAL_0, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if0_gate(VAL_0, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if0_gate(VAL_X, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if0_gate(VAL_L, VAL_Z));

    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if0_gate(VAL_X, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if0_gate(VAL_Z, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if0_gate(VAL_L, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if0_gate(VAL_H, VAL_0));

    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if0_gate(VAL_X, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if0_gate(VAL_Z, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if0_gate(VAL_L, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if0_gate(VAL_H, VAL_1));

    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_buf_if0_gate(VAL_0, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_buf_if0_gate(VAL_0, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if0_gate(VAL_0, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if0_gate(VAL_0, VAL_H));
  }

  /** @brief Single test case that covers all logic paths for compute_buf_if1_gate.
   */
  void test_compute_buf_if1_gate()
  {
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_buf_if1_gate(VAL_0, VAL_H));

    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if1_gate(VAL_1, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if1_gate(VAL_1, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if1_gate(VAL_X, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_buf_if1_gate(VAL_L, VAL_Z));

    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if1_gate(VAL_X, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if1_gate(VAL_Z, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if1_gate(VAL_L, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if1_gate(VAL_H, VAL_0));

    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if1_gate(VAL_X, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if1_gate(VAL_Z, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if1_gate(VAL_L, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if1_gate(VAL_H, VAL_1));

    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_buf_if1_gate(VAL_1, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_buf_if1_gate(VAL_1, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_buf_if1_gate(VAL_1, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_buf_if1_gate(VAL_1, VAL_H));
  }

  /** @brief Single test case that covers all logic paths for compute_not_if0_gate.
   */
  void test_compute_not_if0_gate()
  {
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if0_gate(VAL_1, VAL_H));

    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if0_gate(VAL_0, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if0_gate(VAL_0, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if0_gate(VAL_X, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if0_gate(VAL_L, VAL_Z));

    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if0_gate(VAL_X, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if0_gate(VAL_Z, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if0_gate(VAL_L, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if0_gate(VAL_H, VAL_0));

    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if0_gate(VAL_X, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if0_gate(VAL_Z, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if0_gate(VAL_L, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if0_gate(VAL_H, VAL_1));

    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_not_if0_gate(VAL_0, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_not_if0_gate(VAL_0, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_not_if0_gate(VAL_0, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_not_if0_gate(VAL_0, VAL_H));
  }

  /** @brief Single test case that covers all logic paths for compute_not_if1_gate.
   */
  void test_compute_not_if1_gate()
  {
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_Z, compute_not_if1_gate(VAL_0, VAL_H));

    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if1_gate(VAL_1, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if1_gate(VAL_1, VAL_Z));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if1_gate(VAL_X, VAL_X));
    CPPUNIT_ASSERT_EQUAL(VAL_X, compute_not_if1_gate(VAL_L, VAL_Z));

    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if1_gate(VAL_X, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if1_gate(VAL_Z, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if1_gate(VAL_L, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_H, compute_not_if1_gate(VAL_H, VAL_0));

    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if1_gate(VAL_X, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if1_gate(VAL_Z, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if1_gate(VAL_L, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_L, compute_not_if1_gate(VAL_H, VAL_1));

    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_not_if1_gate(VAL_1, VAL_0));
    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_not_if1_gate(VAL_1, VAL_1));
    CPPUNIT_ASSERT_EQUAL(VAL_1, compute_not_if1_gate(VAL_1, VAL_L));
    CPPUNIT_ASSERT_EQUAL(VAL_0, compute_not_if1_gate(VAL_1, VAL_H));
  }

  bool helper_compare_signal_lists(Sinais* list_a, Sinais* list_b)
  {
    Sinal os;
    Sinal ss;
    Pulso* pos;
    Pulso* pss;

    // basic len check
    if ( list_a->quantidade != list_b->quantidade ) {
      return false;
    }

    // compare all pairs
    for (int i = 0; i < list_a->quantidade; i++)
    {
      os = list_a->lista[i];
      ss = list_b->lista[i];

      pos = os.pulsos;
      pss = ss.pulsos;

      // check all the signal in the pair
      while ( pos->valor != VAL_BLANK && pss->valor != VAL_BLANK )
      {
        if ( pos->tempo != pss->tempo ||
             pos->unidade != pss->unidade ||
             pos->valor != pss->valor ) {
          return false;
        }

        pos = pos + 1;
        pss = pss + 1;
      }
    }

    return true;
  }

};
