#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <list>
#include <string>

#include "sinais.h"
#include "eventos.h"
#include "inout.h"
#include "strutil.h"
#include "verilog.h"
#include "simula.h"

class Testes_inout : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_inout );
  CPPUNIT_TEST( test_load_input_signals_emptyFile );
  CPPUNIT_TEST( test_load_input_signals_empty_signal );
  CPPUNIT_TEST( test_load_input_signals_oneInputFile );
  CPPUNIT_TEST( test_load_input_signals_twoInputFile );
  CPPUNIT_TEST( test_load_input_signals_file_notgates_in );
  CPPUNIT_TEST( test_load_input_signals_file_badinput_XX_in );
  CPPUNIT_TEST( test_save_signals );
  CPPUNIT_TEST( test_get_char_from_logic_value );
  CPPUNIT_TEST( test_save_vcd );
  CPPUNIT_TEST_SUITE_END();

public:
  void test_load_input_signals_emptyFile()
  {
    FILE* file_inputs = fopen("./inout_sample_files/empty.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    SignalArray* inputs = load_input_signals(file_inputs);
    CPPUNIT_ASSERT( !inputs );
    fclose(file_inputs);
  }

  void test_load_input_signals_empty_signal()
  {
    FILE* file_inputs = fopen("./inout_sample_files/empty_signal.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    SignalArray* inputs = load_input_signals(file_inputs);
    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 1, inputs->count );

    free(inputs);
    fclose(file_inputs);
  }

  void test_load_input_signals_oneInputFile()
  {
    std::string expected_name_a = "a";

    FILE* file_inputs = fopen("./inout_sample_files/one.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    SignalArray* inputs = load_input_signals(file_inputs);
    CPPUNIT_ASSERT( inputs );
    fclose(file_inputs);

    CPPUNIT_ASSERT_EQUAL( 1, inputs->count );
    CPPUNIT_ASSERT( inputs->itens );

    std::string str_name_a(inputs->itens[0].name);
    CPPUNIT_ASSERT_EQUAL( expected_name_a, str_name_a );
    CPPUNIT_ASSERT_EQUAL( (Time)20, inputs->itens[0].total_time );
  }

  void test_load_input_signals_twoInputFile()
  {
    std::string expected_name_a = "a";
    std::string expected_name_b = "b";

    FILE* file_inputs = fopen("./inout_sample_files/two.in", "r");
    CPPUNIT_ASSERT( file_inputs );

    SignalArray* inputs = load_input_signals(file_inputs);
    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 2, inputs->count );
    CPPUNIT_ASSERT( inputs->itens );
    fclose(file_inputs);

    std::string str_name_a(inputs->itens[0].name);
    std::string str_name_b(inputs->itens[1].name);
    CPPUNIT_ASSERT_EQUAL( expected_name_a, str_name_a );
    CPPUNIT_ASSERT_EQUAL( expected_name_b, str_name_b );
    CPPUNIT_ASSERT_EQUAL( (Time)20, inputs->itens[0].total_time );
    CPPUNIT_ASSERT_EQUAL( (Time)10, inputs->itens[1].total_time );
  }

  void test_load_input_signals_file_notgates_in()
  {
    std::string expected_name_a = "a";
    std::string expected_name_b = "b";

    FILE* f_notgates_in = fopen("./inout_sample_files/notgates.in", "r");
    CPPUNIT_ASSERT( f_notgates_in );

    SignalArray* inputs = load_input_signals(f_notgates_in);
    CPPUNIT_ASSERT( inputs );
    CPPUNIT_ASSERT_EQUAL( 2, inputs->count );
    CPPUNIT_ASSERT( inputs->itens );
    fclose(f_notgates_in);

    std::string str_name_a(inputs->itens[0].name);
    std::string str_name_b(inputs->itens[1].name);
    CPPUNIT_ASSERT_EQUAL( expected_name_a, str_name_a );
    CPPUNIT_ASSERT_EQUAL( expected_name_b, str_name_b );
    CPPUNIT_ASSERT_EQUAL( (Time)20, inputs->itens[0].total_time );
    CPPUNIT_ASSERT_EQUAL( (Time)20, inputs->itens[1].total_time );

    CPPUNIT_ASSERT_EQUAL( VAL_X, inputs->itens[0].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( VAL_X, inputs->itens[1].pulses[0].value );
    CPPUNIT_ASSERT_EQUAL( VAL_0, inputs->itens[0].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( VAL_0, inputs->itens[1].pulses[1].value );
    CPPUNIT_ASSERT_EQUAL( VAL_1, inputs->itens[0].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( VAL_1, inputs->itens[1].pulses[2].value );
    CPPUNIT_ASSERT_EQUAL( VAL_0, inputs->itens[0].pulses[3].value );
    CPPUNIT_ASSERT_EQUAL( VAL_0, inputs->itens[1].pulses[3].value );
  }

  void test_load_input_signals_file_badinput_XX_in()
  {
    std::list<std::string> list_bad_files = {
      "./inout_sample_files/badinput_0.in",
      "./inout_sample_files/badinput_1.in",
      "./inout_sample_files/badinput_2.in",
      "./inout_sample_files/badinput_3.in",
      "./inout_sample_files/badinput_4.in",
      "./inout_sample_files/badinput_5.in",
      "./inout_sample_files/badinput_5a.in",
      "./inout_sample_files/badinput_5b.in",
      "./inout_sample_files/badinput_6.in",
      "./inout_sample_files/badinput_6a.in",
      "./inout_sample_files/badinput_7.in",
      "./inout_sample_files/badinput_7a.in",
      "./inout_sample_files/badinput_8.in",
      "./inout_sample_files/badinput_9.in"
    };

    for ( std::string path : list_bad_files )
    {
      FILE* fp = fopen( path.c_str(), "r");
      CPPUNIT_ASSERT( fp );

      SignalArray* inputs = load_input_signals(fp);
      CPPUNIT_ASSERT( !inputs );

      fclose(fp);
    }
  }

  void test_save_signals()
  {
    FILE* fp = fopen("./inout_sample_files/allpulses.in", "r");
    CPPUNIT_ASSERT( fp );

    SignalArray* inputs = load_input_signals(fp);
    CPPUNIT_ASSERT( inputs );
    fclose(fp);

    FILE* fp_out = fopen("./inout_sample_files/allpulses.in.out", "w");
    CPPUNIT_ASSERT( fp_out );

    save_signals(inputs, fp_out);
    fclose(fp_out);

    fp = fopen("./inout_sample_files/allpulses.in.out", "r");
    CPPUNIT_ASSERT( fp );

    SignalArray* outputs = load_input_signals(fp);
    CPPUNIT_ASSERT( outputs );
    fclose(fp);

    // verificando se o sinal original e o que foi salvo sao iguais

    CPPUNIT_ASSERT_EQUAL( inputs->count, outputs->count );

    for ( int i = 0; i < inputs->count; ++i )
    {
      std::string str_name_input_i(inputs->itens[i].name);
      std::string str_name_output_i(outputs->itens[i].name);
      CPPUNIT_ASSERT_EQUAL( str_name_input_i, str_name_output_i );
      CPPUNIT_ASSERT_EQUAL( inputs->itens[i].total_time, outputs->itens[i].total_time );
      
      for ( int j = 0; j < 3; ++j )
      {
        CPPUNIT_ASSERT_EQUAL( inputs->itens[i].pulses[j].time,
                              outputs->itens[i].pulses[j].time );
        CPPUNIT_ASSERT_EQUAL( inputs->itens[i].pulses[j].value,
                              outputs->itens[i].pulses[j].value );
      }
    }

    remove("./inout_sample_files/allpulses.in.out");
  }

  void test_get_char_from_logic_value() 
  {
    CPPUNIT_ASSERT_EQUAL('1', get_char_from_logic_value(VAL_1));
    CPPUNIT_ASSERT_EQUAL('0', get_char_from_logic_value(VAL_0));
    CPPUNIT_ASSERT_EQUAL('x', get_char_from_logic_value(VAL_X));
    CPPUNIT_ASSERT_EQUAL('z', get_char_from_logic_value(VAL_Z));
    CPPUNIT_ASSERT_EQUAL('1', get_char_from_logic_value(VAL_H)); // High logic value
    CPPUNIT_ASSERT_EQUAL('0', get_char_from_logic_value(VAL_L)); // Low logic value
    CPPUNIT_ASSERT_EQUAL('x', get_char_from_logic_value(VAL_BLANK)); // Testing default case
  }

  void test_save_vcd()
  {
    ListModule* circuit = NULL;
    SignalArray* inputs = NULL;
    SignalArray* outputs = NULL;
    Event* q = new_empty_event();
    FILE* file_v = NULL;
    FILE* f_dump = NULL;

    std::string path_file_v = "./verilog_sample_src/bufgates.v";

    file_v = fopen(path_file_v.c_str(), "r");
    CPPUNIT_ASSERT(file_v);

    circuit = load_circuit(file_v, &q, path_file_v.c_str());
    CPPUNIT_ASSERT(circuit);
    
    FILE* f_in = fopen("./inout_sample_files/bufgates.in", "r");
    CPPUNIT_ASSERT(f_in);

    inputs = load_input_signals(f_in);
    fclose(f_in);
    CPPUNIT_ASSERT(inputs);
    
    outputs = simula(circuit->itens[0], inputs, &q, &f_dump);
    CPPUNIT_ASSERT(outputs);

    FILE* file_vcd = fopen("test_output.vcd", "w");
    CPPUNIT_ASSERT(file_vcd);

    // Call the function to test
    save_vcd(circuit->itens[0], outputs, file_vcd);
    fclose(file_vcd);

    // Open the file to read and validate output
    file_vcd = fopen("test_output.vcd", "r");
    CPPUNIT_ASSERT(file_vcd);
    
    // Read and validate SOME contents of the VCD file
    char buffer[256];
    fgets(buffer, sizeof(buffer), file_vcd);
    std::string str_expected = "$date\n";
    std::string str_buffer(buffer);
    CPPUNIT_ASSERT_EQUAL( str_expected, str_buffer );

    fclose(file_vcd);
    
    // TODO:_Continue with more assertions to validate the output...

    // Clean up
    remove("test_output.vcd");
    free_signal_list(&inputs);
    free_signal_list(&outputs);
    free_circuit(&circuit);
    if (f_dump) {
      fclose(f_dump);
    }
  }

};
