#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <iostream>
#include <cstring>
#include <list>
#include <string>
#include <vector>
#include <tuple>

#include "../verilog.h"

class Testes_verilog : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_verilog );
  CPPUNIT_TEST( test_isPortaLogica );
  CPPUNIT_TEST( test_load_module_fileEmpty );
  CPPUNIT_TEST( test_load_module_fileTop_module );
  CPPUNIT_TEST( test_load_module_fileTudo_module );
  CPPUNIT_TEST( test_load_module_reg_v );
  CPPUNIT_TEST( test_load_module_localparam_test_v );
  CPPUNIT_TEST( test_load_module_named_gates_test_v );
  CPPUNIT_TEST( test_load_module_initial_single_test_v );
  CPPUNIT_TEST( test_load_module_initial_block_test_v );
  CPPUNIT_TEST( test_load_module_display_v );
  CPPUNIT_TEST( test_load_module_assigns_v );
  CPPUNIT_TEST( test_load_module_tri_state_gates_v );
  CPPUNIT_TEST( test_load_module_badverilog_XX_v );
  CPPUNIT_TEST( test_load_module_badtimescale_XX_v );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_isPortaLogica()
  {
    CPPUNIT_ASSERT( isPortaLogica( (char*)"and") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"or") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"xor") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"nand") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"nor") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"xnor") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"not") );
    CPPUNIT_ASSERT( isPortaLogica( (char*)"buf") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)"a") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)"aa") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)"n") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)"nn") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)"") );
    CPPUNIT_ASSERT( ! isPortaLogica( (char*)" ") );
  }

  void test_load_module_fileEmpty()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/empty.v", &q);

    CPPUNIT_ASSERT( !circuit );

    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_fileTop_module()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/top.v", &q);

    CPPUNIT_ASSERT( circuit );
    
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_fileTudo_module()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/tudo.v", &q);

    CPPUNIT_ASSERT( circuit );
    
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_reg_v()
  {
    std::vector<std::tuple<std::string, int, int>> regs_info {
      {"bit_is_on", 1, 0},
      {"bit_is_ok", 1, 0},
      {"silly_bit", 1, 0},
      {"ra", 16, 0},
      {"rb", 16, 0},
      {"ral", 8, 0},
      {"rah", 8, 0},
      {"rax", 32, 0},
      {"rbx", 32, 0},
      {"r_flag", 20, 0},
      {"r_extra_flag", 3, 0},
      {"number_signed", 64, 1}
    };

    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/reg.v", &q);

    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT( circuit->listaReg.total == 12 );

    for ( int i=0 ; i < circuit->listaReg.total ; i++ )
    {
      std::string name = std::get<0>(regs_info[i]);
      unsigned size = std::get<1>(regs_info[i]);
      int is_signed = std::get<2>(regs_info[i]);
      CPPUNIT_ASSERT( !strcmp(name.c_str(), circuit->listaReg.itens[i]->name) );
      CPPUNIT_ASSERT_EQUAL( size, circuit->listaReg.itens[i]->size );
      CPPUNIT_ASSERT_EQUAL( is_signed, circuit->listaReg.itens[i]->is_signed );
    }

    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_localparam_test_v()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/localparam_test.v", &q);

    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT( circuit->listaParam.total == 2 );
    CPPUNIT_ASSERT( !strcmp(circuit->listaParam.itens[0]->name, "VER_NUM") );
    CPPUNIT_ASSERT_EQUAL( circuit->listaParam.itens[0]->value, 123 );
    CPPUNIT_ASSERT( !strcmp(circuit->listaParam.itens[1]->name, "_AUTHOR_ID") );
    CPPUNIT_ASSERT_EQUAL( circuit->listaParam.itens[1]->value, 1 );

    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_named_gates_test_v()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/named_gates_test.v", &q);
    
    CPPUNIT_ASSERT( circuit );
    
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_initial_single_test_v()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/initial_single_test.v", &q);

    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT_EQUAL( 1, circuit->listaParam.total );
    CPPUNIT_ASSERT_EQUAL( 0, circuit->listaParam.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( 2, circuit->listaReg.total );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, circuit->listaReg.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, circuit->listaReg.itens[1]->value );

    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_initial_block_test_v()
  {
    Evento* q = new_empty_event();
    Module* mod = load_module("./verilog_sample_src/initial_block_test.v", &q);

    const int expected_param_value = 1;
    const int expected_liter_value = 0;

    CPPUNIT_ASSERT( mod );
    CPPUNIT_ASSERT_EQUAL( 1, mod->listaParam.total );
    CPPUNIT_ASSERT_EQUAL( expected_param_value, mod->listaParam.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( 2, mod->listaReg.total );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)expected_param_value, mod->listaReg.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)expected_liter_value, mod->listaReg.itens[1]->value );

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_display_v()
  {
    Evento* q = new_empty_event();
    Module* m = load_module("./verilog_sample_src/display.v", &q);

    CPPUNIT_ASSERT(m);

    // TODO: more inspections

    delete_event_queue(&q);
    free_module(&m);
  }
  
  void test_load_module_assigns_v()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/assigns.v", &q);

    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT_EQUAL(1, circuit->listaFiosEntrada->tamanho);
    CPPUNIT_ASSERT_EQUAL(2, circuit->listaFiosSaida->tamanho);
    CPPUNIT_ASSERT_EQUAL(2, circuit->listaWires->tamanho);

    Component* x;
    x = circuit->listaFiosSaida->itens[0];
    // x (output) tem uma entrada que vem de um assign simples
    CPPUNIT_ASSERT_EQUAL(1, x->listaEntrada->tamanho);
    CPPUNIT_ASSERT_EQUAL(assign, x->listaEntrada->itens[0]->tipo.operador);

    // TODO: more inspections

    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_tri_state_gates_v()
  {
    Evento* q = new_empty_event();
    Module* circuit = load_module("./verilog_sample_src/tri_state_gates.v", &q);

    CPPUNIT_ASSERT(circuit);
    
    delete_event_queue(&q);
    free_module(&circuit);
  }

  void test_load_module_badverilog_XX_v()
  {
    std::list<std::string> list_bad_files = {
      "./verilog_sample_src/badverilog_00.v",
      "./verilog_sample_src/badverilog_01.v",
      "./verilog_sample_src/badverilog_02.v",
      "./verilog_sample_src/badverilog_03.v",
      "./verilog_sample_src/badverilog_04.v",
      "./verilog_sample_src/badverilog_05.v",
      "./verilog_sample_src/badverilog_06.v",
      "./verilog_sample_src/badverilog_07.v",
      "./verilog_sample_src/badverilog_08.v",
      "./verilog_sample_src/badverilog_09.v",
      "./verilog_sample_src/badverilog_10.v",
      "./verilog_sample_src/badverilog_11.v",
      "./verilog_sample_src/badverilog_12.v",
      "./verilog_sample_src/badverilog_13.v",
      "./verilog_sample_src/badverilog_14.v",
      "./verilog_sample_src/badverilog_15.v",
      "./verilog_sample_src/badverilog_16.v",
      "./verilog_sample_src/badverilog_17.v",
      "./verilog_sample_src/badverilog_17a.v",
      "./verilog_sample_src/badverilog_17b.v",
      "./verilog_sample_src/badverilog_18.v",
      "./verilog_sample_src/badverilog_19.v",
      "./verilog_sample_src/badverilog_20.v",
      "./verilog_sample_src/badverilog_21.v",
      "./verilog_sample_src/badverilog_22.v",
      "./verilog_sample_src/badverilog_23.v",
      "./verilog_sample_src/badverilog_24.v",
      "./verilog_sample_src/badverilog_25.v",
      "./verilog_sample_src/badverilog_26.v",
      "./verilog_sample_src/badverilog_27.v",
      "./verilog_sample_src/badverilog_28.v",
      "./verilog_sample_src/badverilog_29.v",
      "./verilog_sample_src/badverilog_29a.v",
      "./verilog_sample_src/badverilog_30.v",
      "./verilog_sample_src/badverilog_30a.v",
      "./verilog_sample_src/badverilog_31.v",
      "./verilog_sample_src/badverilog_32.v",
      "./verilog_sample_src/badverilog_33.v",
      "./verilog_sample_src/badverilog_34.v",
      "./verilog_sample_src/badverilog_35.v",
      "./verilog_sample_src/badverilog_36.v",
      "./verilog_sample_src/badverilog_37.v",
      "./verilog_sample_src/badverilog_38.v",
      "./verilog_sample_src/badverilog_39.v",
      "./verilog_sample_src/badverilog_40.v",
      "./verilog_sample_src/badverilog_41.v",
      "./verilog_sample_src/badverilog_42.v",
      "./verilog_sample_src/badverilog_43.v",
      "./verilog_sample_src/badverilog_44.v",
      "./verilog_sample_src/badverilog_45.v",
      "./verilog_sample_src/badverilog_46.v",
      "./verilog_sample_src/badverilog_47.v",
      "./verilog_sample_src/badverilog_48.v",
      "./verilog_sample_src/badverilog_49.v",
      "./verilog_sample_src/badverilog_50.v",
      "./verilog_sample_src/badverilog_51.v",
      "./verilog_sample_src/badverilog_52.v",
      "./verilog_sample_src/badverilog_53.v",
      "./verilog_sample_src/badverilog_53a.v",
      "./verilog_sample_src/badverilog_54.v",
      "./verilog_sample_src/badverilog_55.v",
      "./verilog_sample_src/badverilog_56.v",
      "./verilog_sample_src/badverilog_57.v",
      "./verilog_sample_src/badverilog_58.v",
      "./verilog_sample_src/badverilog_58a.v",
      "./verilog_sample_src/badverilog_59.v",
      "./verilog_sample_src/badverilog_60.v",
      "./verilog_sample_src/badverilog_61.v",
      "./verilog_sample_src/badverilog_62.v",
      "./verilog_sample_src/badverilog_63.v",
      "./verilog_sample_src/badverilog_64.v",
      "./verilog_sample_src/badverilog_65.v",
      "./verilog_sample_src/badverilog_66.v",
      "./verilog_sample_src/badverilog_67.v",
      "./verilog_sample_src/badverilog_68.v",
      "./verilog_sample_src/badverilog_69.v",
      "./verilog_sample_src/badverilog_70.v",
      "./verilog_sample_src/badverilog_71.v",
      "./verilog_sample_src/badverilog_71a.v",
      "./verilog_sample_src/badverilog_72.v",
      "./verilog_sample_src/badverilog_73.v",
      "./verilog_sample_src/badverilog_74.v",
      "./verilog_sample_src/badverilog_75.v",
      "./verilog_sample_src/badverilog_76.v",
      "./verilog_sample_src/badverilog_77.v",
      "./verilog_sample_src/badverilog_78.v",
      "./verilog_sample_src/badverilog_79.v",
      "./verilog_sample_src/badverilog_80.v",
      "./verilog_sample_src/badverilog_81.v",
      "./verilog_sample_src/badverilog_82.v",
      "./verilog_sample_src/badverilog_83.v",
      "./verilog_sample_src/badverilog_83a.v",
      "./verilog_sample_src/badverilog_83b.v",
      "./verilog_sample_src/badverilog_84.v",
      "./verilog_sample_src/badverilog_84a.v",
      "./verilog_sample_src/badverilog_85.v",
      "./verilog_sample_src/badverilog_85a.v",
      "./verilog_sample_src/badverilog_86.v",
      "./verilog_sample_src/badverilog_86a.v",
      "./verilog_sample_src/badverilog_87.v",
      "./verilog_sample_src/badverilog_87a.v",
      "./verilog_sample_src/badverilog_87b.v",
      "./verilog_sample_src/badverilog_87c.v",
      "./verilog_sample_src/badverilog_87d.v",
      "./verilog_sample_src/badverilog_87e.v",
      "./verilog_sample_src/badverilog_87f.v",
      "./verilog_sample_src/badverilog_87g.v",
      "./verilog_sample_src/badverilog_87h.v",
      "./verilog_sample_src/badverilog_87i.v",
      "./verilog_sample_src/badverilog_87j.v",
      "./verilog_sample_src/badverilog_87k.v",
      "./verilog_sample_src/badverilog_88.v",
      "./verilog_sample_src/badverilog_88a.v",
      "./verilog_sample_src/badverilog_88b.v",
      "./verilog_sample_src/badverilog_88c.v",
      "./verilog_sample_src/badverilog_88d.v",
      "./verilog_sample_src/badverilog_88e.v"
    };

    Evento* q = new_empty_event();
    Module* circuit = NULL;

    for ( std::string path : list_bad_files )
    {
      //std::cout << "test_load_module_badverilog_XX_v: " << path << std::endl;
      circuit = load_module(path.c_str(), &q);

      CPPUNIT_ASSERT(!circuit);

      free_module(&circuit);
    }

    delete_event_queue(&q);
  }

  void test_load_module_badtimescale_XX_v()
  {
    std::list<std::string> list_bad_files = {
      "./verilog_sample_src/badtimescale_00.v",
      "./verilog_sample_src/badtimescale_01.v",
      "./verilog_sample_src/badtimescale_01a.v",
      "./verilog_sample_src/badtimescale_02.v",
      "./verilog_sample_src/badtimescale_03.v",
      "./verilog_sample_src/badtimescale_04.v",
      "./verilog_sample_src/badtimescale_05.v",
      "./verilog_sample_src/badtimescale_06.v",
      "./verilog_sample_src/badtimescale_07.v",
      "./verilog_sample_src/badtimescale_08.v",
      "./verilog_sample_src/badtimescale_09.v",
      "./verilog_sample_src/badtimescale_10.v",
      "./verilog_sample_src/badtimescale_11.v"
    };

    Evento* q = new_empty_event();
    Module* circuit = NULL;

    for ( std::string path : list_bad_files )
    {
      circuit = load_module(path.c_str(), &q);

      CPPUNIT_ASSERT(!circuit);

      free_module(&circuit);
    }

    delete_event_queue(&q);
  }

};
