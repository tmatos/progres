#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "estruturas.h"
#include "sinais.h"
#include "eventos.h"
#include "verilog.h"
#include "inout.h"
#include "simula.h"

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
  CPPUNIT_TEST( test_simula_samplefile_finish_v );
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
    SignalArray *inputs = NULL;
    SignalArray *outputs = NULL;
    FILE* f_dump = NULL;

    outputs = simula(circ, inputs, NULL, &f_dump);
    CPPUNIT_ASSERT_EQUAL( (SignalArray*)NULL, outputs );

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_CircuitoVazio()
  {
    Module *circ = new_module();
    SignalArray *inputs = new_signal_list();
    SignalArray *outputs = NULL;
    FILE* f_dump = NULL;

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    outputs = simula(circ, inputs, NULL, &f_dump);
    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL( outputs->count, 0 );
    CPPUNIT_ASSERT_EQUAL( (Signal*)NULL, outputs->itens );

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_CircuitoUmaEntrada()
  {
    Module *circ = new_module();
    SignalArray *inputs = new_signal_list();
    SignalArray *outputs = NULL;
    FILE* f_dump = NULL;

    char str_name_entrada_1[50] = "sinal_in_1";

    CPPUNIT_ASSERT(circ);
    CPPUNIT_ASSERT(inputs);

    add_new_signal( inputs, str_name_entrada_1 );
    add_new_pulse( &(inputs->itens[0]), VAL_1, (Time)20 );  // 0
    add_new_pulse( &(inputs->itens[0]), VAL_0, (Time)50 );  // 1
    add_new_pulse( &(inputs->itens[0]), VAL_1, (Time)105 ); // 2

    CPPUNIT_ASSERT_EQUAL( 1, inputs->count );
    CPPUNIT_ASSERT( inputs->itens );
    CPPUNIT_ASSERT( inputs->itens[0].pulses );

    Component* cp_in_wire = new_component(str_name_entrada_1, ROLE_WIRE);

    add_input(circ, cp_in_wire);

    CPPUNIT_ASSERT_EQUAL( 1, circ->list_input_net->total );
    CPPUNIT_ASSERT( circ->list_input_net->itens );
    CPPUNIT_ASSERT( ! strcmp(str_name_entrada_1, circ->list_input_net->itens[0]->name) );

    outputs = simula(circ, inputs, NULL, &f_dump);
    CPPUNIT_ASSERT(outputs);

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_module(&circ);
  }

  void test_simula_samplefile_andgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();

    char s_andgates_v[] = "./verilog_sample_src/andgates.v";
    FILE* f_andgates_v = fopen(s_andgates_v, "r");
    CPPUNIT_ASSERT( f_andgates_v );

    FILE* f_andgates_in = fopen("./inout_sample_files/andgates.in", "r");
    CPPUNIT_ASSERT( f_andgates_in );

    circuit = load_circuit(f_andgates_v, &q, s_andgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_andgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );
    CPPUNIT_ASSERT( outputs->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)25, outputs->itens[0].total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)15, outputs->itens[0].pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[2].time );

    helper_close_dump_file(&f_dump);
    fclose(f_andgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_orgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    
    char s_orgates_v[] = "./verilog_sample_src/orgates.v";
    FILE* f_orgates_v = fopen(s_orgates_v, "r");
    CPPUNIT_ASSERT( f_orgates_v );

    FILE* f_orgates_in = fopen("./inout_sample_files/orgates.in", "r");
    CPPUNIT_ASSERT( f_orgates_in );

    circuit = load_circuit(f_orgates_v, &q, s_orgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_orgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );
    CPPUNIT_ASSERT( outputs->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)25, outputs->itens[0].total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)15, outputs->itens[0].pulses[2].time );

    helper_close_dump_file(&f_dump);
    fclose(f_orgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_nandgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    
    char s_nandgates_v[] = "./verilog_sample_src/nandgates.v";
    FILE* f_nandgates_v = fopen(s_nandgates_v, "r");
    CPPUNIT_ASSERT( f_nandgates_v );

    FILE* f_nandgates_in = fopen("./inout_sample_files/nandgates.in", "r");
    CPPUNIT_ASSERT( f_nandgates_in );

    circuit = load_circuit(f_nandgates_v, &q, s_nandgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_nandgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );
    CPPUNIT_ASSERT( outputs->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)25, outputs->itens[0].total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)15, outputs->itens[0].pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[2].time );

    helper_close_dump_file(&f_dump);
    fclose(f_nandgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_norgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();

    char s_norgates_v[] = "./verilog_sample_src/norgates.v";
    FILE* f_norgates_v = fopen(s_norgates_v, "r");
    CPPUNIT_ASSERT( f_norgates_v );

    FILE* f_norgates_in = fopen("./inout_sample_files/norgates.in", "r");
    CPPUNIT_ASSERT( f_norgates_in );

    circuit = load_circuit(f_norgates_v, &q, s_norgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_norgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );
    CPPUNIT_ASSERT( outputs->itens[0].pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)25, outputs->itens[0].total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, outputs->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, outputs->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, outputs->itens[0].pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, outputs->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)15, outputs->itens[0].pulses[2].time );

    helper_close_dump_file(&f_dump);
    fclose(f_norgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_notgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    Signal s;
    
    char s_notgates_v[] = "./verilog_sample_src/notgates.v";
    FILE* f_notgates_v = fopen(s_notgates_v, "r");
    CPPUNIT_ASSERT( f_notgates_v );

    FILE* f_notgates_in = fopen("./inout_sample_files/notgates.in", "r");
    CPPUNIT_ASSERT( f_notgates_in );

    circuit = load_circuit(f_notgates_v, &q, s_notgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_notgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 2, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    s = outputs->itens[0];

    CPPUNIT_ASSERT( ! strcmp("na", s.name ) );
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)20, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[3].time );

    s = outputs->itens[1];

    CPPUNIT_ASSERT( ! strcmp("nnb", s.name ) );
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)20, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[3].time );

    helper_close_dump_file(&f_dump);
    fclose(f_notgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_bufgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    Signal s;
    
    char s_bufgates_v[] = "./verilog_sample_src/bufgates.v";
    FILE* f_bufgates_v = fopen(s_bufgates_v, "r");
    CPPUNIT_ASSERT( f_bufgates_v );

    FILE* f_bufgates_in = fopen("./inout_sample_files/bufgates.in", "r");
    CPPUNIT_ASSERT( f_bufgates_in );

    circuit = load_circuit(f_bufgates_v, &q, s_bufgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_bufgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    s = outputs->itens[0];

    CPPUNIT_ASSERT( ! strcmp("y", s.name ) );
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)48, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)10, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)10, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)13, s.pulses[3].time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[4].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[4].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[5].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[5].time );

    helper_close_dump_file(&f_dump);
    fclose(f_bufgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }
  
  void test_simula_samplefile_xorgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    Signal s;
    
    char s_xorgates_v[] = "./verilog_sample_src/xorgates.v";
    FILE* f_xorgates_v = fopen(s_xorgates_v, "r");
    CPPUNIT_ASSERT( f_xorgates_v );

    FILE* f_xorgates_in = fopen("./inout_sample_files/xorgates.in", "r");
    CPPUNIT_ASSERT( f_xorgates_in );

    circuit = load_circuit(f_xorgates_v, &q, s_xorgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_xorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );

    s = outputs->itens[0];
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)35, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)10, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[3].time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[4].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[4].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[5].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[5].time );

    helper_close_dump_file(&f_dump);
    fclose(f_xorgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_xnorgates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    Signal s;

    char s_xnorgates_v[] = "./verilog_sample_src/xnorgates.v";
    FILE * f_xnorgates_v = fopen(s_xnorgates_v, "r");
    CPPUNIT_ASSERT( f_xnorgates_v );

    FILE* f_xnorgates_in = fopen("./inout_sample_files/xnorgates.in", "r");
    CPPUNIT_ASSERT( f_xnorgates_in );

    circuit = load_circuit(f_xnorgates_v, &q, s_xnorgates_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_xnorgates_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 1, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("y", outputs->itens[0].name ) );

    s = outputs->itens[0];
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)35, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)10, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[3].time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[4].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[4].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[5].value );
    CPPUNIT_ASSERT_EQUAL( (Time)5, s.pulses[5].time );

    helper_close_dump_file(&f_dump);
    fclose(f_xnorgates_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_delays_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    SignalArray* sim_outputs = NULL;
    Event* q = new_empty_event();

    char s_delays_v[] = "./verilog_sample_src/delays.v";
    FILE* f_delays_v = fopen(s_delays_v, "r");
    CPPUNIT_ASSERT( f_delays_v );

    FILE* f_delays_in = fopen("./inout_sample_files/delays.in", "r");
    FILE* f_delays_in_out = fopen("./inout_sample_files/delays.in.out", "r");

    CPPUNIT_ASSERT(f_delays_in);
    CPPUNIT_ASSERT(f_delays_in_out);

    circuit = load_circuit(f_delays_v, &q, s_delays_v);
    CPPUNIT_ASSERT(circuit);

    inputs = load_input_signals(f_delays_in);
    CPPUNIT_ASSERT(inputs);

    outputs = load_input_signals(f_delays_in_out);
    CPPUNIT_ASSERT(outputs);

    CPPUNIT_ASSERT_EQUAL(8, outputs->count);
    CPPUNIT_ASSERT(outputs->itens);

    sim_outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(sim_outputs);
    CPPUNIT_ASSERT_EQUAL(8, sim_outputs->count);
    CPPUNIT_ASSERT(sim_outputs->itens);

    CPPUNIT_ASSERT( helper_compare_signal_lists(outputs, sim_outputs) );
    
    helper_close_dump_file(&f_dump);
    fclose(f_delays_in);
    fclose(f_delays_in_out);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_display_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* sim_outputs = NULL;
    Event* q = new_empty_event();

    char s_display_v[] = "./verilog_sample_src/display.v";
    FILE* f_display_v = fopen(s_display_v, "r");
    CPPUNIT_ASSERT( f_display_v );

    circuit = load_circuit(f_display_v, &q, s_display_v);
    CPPUNIT_ASSERT(circuit);

    inputs = new_signal_list();
    CPPUNIT_ASSERT(inputs);

    sim_outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(sim_outputs);

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_finish_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* sim_outputs = NULL;
    Event* q = new_empty_event();

    char s_finish_v[] = "./verilog_sample_src/finish.v";
    FILE* f_finish_v = fopen(s_finish_v, "r");
    CPPUNIT_ASSERT( f_finish_v );

    circuit = load_circuit(f_finish_v, &q, s_finish_v);
    CPPUNIT_ASSERT(circuit);

    inputs = new_signal_list();
    CPPUNIT_ASSERT(inputs);

    sim_outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(sim_outputs);

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_dumpfile_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* sim_outputs = NULL;
    Event* q = new_empty_event();

    char s_dumpfile_v[] = "./verilog_sample_src/dumpfile.v";
    char s_dumpfile_vcd[] = "./dumpfile.vcd";

    FILE* f_dumpfile_v = fopen(s_dumpfile_v, "r");
    CPPUNIT_ASSERT(f_dumpfile_v);

    inputs = new_signal_list();
    CPPUNIT_ASSERT(inputs);

    circuit = load_circuit(f_dumpfile_v, &q, s_dumpfile_v);
    CPPUNIT_ASSERT(circuit);
    CPPUNIT_ASSERT(q->list_transition);
    CPPUNIT_ASSERT_EQUAL(TASK_DUMPFILE, q->list_transition->task_type);

    sim_outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(sim_outputs);

    FILE* f_dumpfile_vcd = fopen(s_dumpfile_vcd, "r");
    CPPUNIT_ASSERT(f_dumpfile_vcd);

    fclose(f_dumpfile_vcd);
    remove(s_dumpfile_vcd);
    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
  }

  void test_simula_samplefile_tri_state_gates_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    Signal s;

    char s_v[] = "./verilog_sample_src/tri_state_gates.v";
    FILE* f_v = fopen(s_v, "r");
    CPPUNIT_ASSERT( f_v );

    FILE* f_in = fopen("./inout_sample_files/tri_state_gates.in", "r");
    CPPUNIT_ASSERT( f_in );

    circuit = load_circuit(f_v, &q, s_v);
    CPPUNIT_ASSERT( circuit );

    inputs = load_input_signals(f_in);
    CPPUNIT_ASSERT( inputs );

    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT( outputs );
    CPPUNIT_ASSERT_EQUAL( 4, outputs->count );
    CPPUNIT_ASSERT( outputs->itens );

    CPPUNIT_ASSERT( ! strcmp("o0", outputs->itens[0].name ) );

    s = outputs->itens[0];
    CPPUNIT_ASSERT( s.pulses );
    CPPUNIT_ASSERT_EQUAL( (Time)12, s.total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, s.pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( (Time)4, s.pulses[0].time );

    CPPUNIT_ASSERT_EQUAL( VAL_0, s.pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( (Time)2, s.pulses[1].time );

    CPPUNIT_ASSERT_EQUAL( VAL_Z, s.pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( (Time)2, s.pulses[2].time );

    CPPUNIT_ASSERT_EQUAL( VAL_1, s.pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( (Time)2, s.pulses[3].time );

    helper_close_dump_file(&f_dump);
    fclose(f_in);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
    
    // TODO: check all cases in the Table 7-5 of Std 1364-2005
  }

  void test_simula_samplefile_numbers_v()
  {
    FILE* f_dump = NULL;
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    SignalArray* sim_outputs = NULL;
    Event* q = new_empty_event();

    char s_v[] = "./verilog_sample_src/numbers.v";
    FILE* f_v = fopen(s_v, "r");
    CPPUNIT_ASSERT( f_v );

    FILE* f_in = fopen("./inout_sample_files/numbers.in", "r");
    FILE* f_out = fopen("./inout_sample_files/numbers.in.out", "r");

    CPPUNIT_ASSERT(f_in);
    CPPUNIT_ASSERT(f_out);

    circuit = load_circuit(f_v, &q, s_v);
    CPPUNIT_ASSERT(circuit);

    inputs = load_input_signals(f_in);
    CPPUNIT_ASSERT(inputs);
    CPPUNIT_ASSERT_EQUAL(1, inputs->count);
    CPPUNIT_ASSERT(inputs->itens);

    outputs = load_input_signals(f_out);
    CPPUNIT_ASSERT(outputs);
    CPPUNIT_ASSERT_EQUAL(2, outputs->count);
    CPPUNIT_ASSERT(outputs->itens);

    sim_outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(sim_outputs);
    CPPUNIT_ASSERT_EQUAL(2, sim_outputs->count);
    CPPUNIT_ASSERT(sim_outputs->itens);

    CPPUNIT_ASSERT( helper_compare_signal_lists(outputs, sim_outputs) );

    helper_close_dump_file(&f_dump);
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_signal_list(&sim_outputs);
    delete_event_queue(&q);
    free_circuit(&circuit);
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

  bool helper_compare_signal_lists(SignalArray* list_a, SignalArray* list_b)
  {
    Signal os;
    Signal ss;
    Pulse* pos;
    Pulse* pss;

    // basic len check
    if ( list_a->count != list_b->count ) {
      return false;
    }

    // compare all pairs
    for (int i = 0; i < list_a->count; i++)
    {
      os = list_a->itens[i];
      ss = list_b->itens[i];

      pos = os.pulses;
      pss = ss.pulses;

      // check all the signal in the pair
      while ( pos->value != VAL_BLANK && pss->value != VAL_BLANK )
      {
        if ( pos->time != pss->time ||
             pos->unit != pss->unit ||
             pos->value != pss->value ) {
          return false;
        }

        pos = pos + 1;
        pss = pss + 1;
      }
    }

    return true;
  }

  void helper_close_dump_file(FILE** f_dump)
  {
    if (f_dump && *f_dump) {
        fclose(*f_dump);
        *f_dump = NULL;
    }
  }

};
