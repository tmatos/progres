#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <iostream>
#include <cstring>
#include <list>
#include <string>
#include <vector>
#include <tuple>

#include "lex.h"
#include "preprocessor.h"
#include "verilog.h"

class Testes_verilog : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_verilog );
  CPPUNIT_TEST( test_is_string_logic_gate );
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

  Token* helper_tokenize_preproc(const char* filepath)
  {
    FILE* f = fopen(filepath, "r");
    CPPUNIT_ASSERT( f );

    ListToken* tokens = tokeniza(f);
    CPPUNIT_ASSERT( tokens );

    int preproc_result = pre_processor(tokens);
    CPPUNIT_ASSERT_EQUAL( 1, preproc_result );

    fclose(f);

    return tokens->primeiro;
  }

  ListToken* helper_tokenize_only(const char* filepath)
  {
    FILE* f = fopen(filepath, "r");
    CPPUNIT_ASSERT( f );

    ListToken* tokens = tokeniza(f);
    CPPUNIT_ASSERT( tokens );

    fclose(f);

    return tokens;
  }

  void test_is_string_logic_gate()
  {
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"and") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"or") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"xor") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"nand") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"nor") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"xnor") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"not") );
    CPPUNIT_ASSERT( is_string_logic_gate( (char*)"buf") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)"a") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)"aa") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)"n") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)"nn") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)"") );
    CPPUNIT_ASSERT( ! is_string_logic_gate( (char*)" ") );
  }

  void test_load_module_fileEmpty()
  {
    std::string path_file = "./verilog_sample_src/empty.v";
    Evento* q = new_empty_event();
    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( !mod );
    CPPUNIT_ASSERT_EQUAL( END_OF_TOKENS, err );

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_fileTop_module()
  {
    std::string path_file = "./verilog_sample_src/top.v";
    Evento* q = new_empty_event();
    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );
    
    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_fileTudo_module()
  {
    std::string path_file = "./verilog_sample_src/tudo.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );
    
    delete_event_queue(&q);
    free_module(&mod);
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
      {"number_signed", 64, 1},
      {"r_0", 1, 0},
      {"r_1", 1, 0},
      {"r_2", 1, 0},
      {"r_3", 1, 0},
      {"r_4", 1, 0},
      {"r_5", 1, 0},
      {"r_6", 1, 0},
      {"r_7", 1, 0}
    };

    std::string path_file = "./verilog_sample_src/reg.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );

    CPPUNIT_ASSERT_EQUAL( (int)regs_info.size(), mod->list_register.total );

    for ( int i=0 ; i < mod->list_register.total ; i++ )
    {
      std::string name = std::get<0>(regs_info[i]);
      unsigned size = std::get<1>(regs_info[i]);
      int is_signed = std::get<2>(regs_info[i]);
      CPPUNIT_ASSERT( !strcmp(name.c_str(), mod->list_register.itens[i]->name) );
      CPPUNIT_ASSERT_EQUAL( size, mod->list_register.itens[i]->size );
      CPPUNIT_ASSERT_EQUAL( is_signed, mod->list_register.itens[i]->is_signed );
    }

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_localparam_test_v()
  {
    std::string path_file = "./verilog_sample_src/localparam_test.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );
    CPPUNIT_ASSERT( mod->list_param.total == 2 );
    CPPUNIT_ASSERT( !strcmp(mod->list_param.itens[0]->name, "VER_NUM") );
    CPPUNIT_ASSERT_EQUAL( mod->list_param.itens[0]->value, 123 );
    CPPUNIT_ASSERT( !strcmp(mod->list_param.itens[1]->name, "_AUTHOR_ID") );
    CPPUNIT_ASSERT_EQUAL( mod->list_param.itens[1]->value, 1 );

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_named_gates_test_v()
  {
    std::string path_file = "./verilog_sample_src/named_gates_test.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );
    
    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_initial_single_test_v()
  {
    std::string path_file = "./verilog_sample_src/initial_single_test.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );

    CPPUNIT_ASSERT_EQUAL( 1, mod->list_param.total );
    CPPUNIT_ASSERT_EQUAL( 0, mod->list_param.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( 2, mod->list_register.total );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)0, mod->list_register.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)1, mod->list_register.itens[1]->value );

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_initial_block_test_v()
  {
    std::string path_file = "./verilog_sample_src/initial_block_test.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);

    const int expected_param_value = 1;
    const int expected_liter_value = 0;

    CPPUNIT_ASSERT( mod );
    CPPUNIT_ASSERT_EQUAL( 1, mod->list_param.total );
    CPPUNIT_ASSERT_EQUAL( expected_param_value, mod->list_param.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( 2, mod->list_register.total );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)expected_param_value, mod->list_register.itens[0]->value );
    CPPUNIT_ASSERT_EQUAL( (unsigned int)expected_liter_value, mod->list_register.itens[1]->value );

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_display_v()
  {
    std::string path_file = "./verilog_sample_src/display.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT(mod);

    // TODO: more inspections

    delete_event_queue(&q);
    free_module(&mod);
  }
  
  void test_load_module_assigns_v()
  {
    std::string path_file = "./verilog_sample_src/assigns.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT( mod );
    CPPUNIT_ASSERT_EQUAL(1, mod->list_input_net->tamanho);
    CPPUNIT_ASSERT_EQUAL(2, mod->list_output_net->tamanho);
    CPPUNIT_ASSERT_EQUAL(2, mod->list_wire_net->tamanho);

    Component* x;
    x = mod->list_output_net->itens[0];
    // x (output) tem uma entrada que vem de um assign simples
    CPPUNIT_ASSERT_EQUAL(1, x->list_input->tamanho);
    CPPUNIT_ASSERT_EQUAL(ROLE_ASSIGN, x->list_input->itens[0]->atributos.role);

    // TODO: more inspections

    delete_event_queue(&q);
    free_module(&mod);
  }

  void test_load_module_tri_state_gates_v()
  {
    std::string path_file = "./verilog_sample_src/tri_state_gates.v";
    Evento* q = new_empty_event();

    Module* mod = NULL;
    Token* it = helper_tokenize_preproc(path_file.c_str());

    VerilogError err = load_module(&it, &q, &mod);
    CPPUNIT_ASSERT(mod);
    
    delete_event_queue(&q);
    free_module(&mod);
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
      "./verilog_sample_src/badverilog_46a.v",
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
      "./verilog_sample_src/badverilog_88e.v",
      "./verilog_sample_src/badverilog_88f.v",
      "./verilog_sample_src/badverilog_88g.v",
      "./verilog_sample_src/badverilog_88h.v"
    };

    FILE* f = NULL;
    Evento* q = new_empty_event();
    Module* mod = NULL;
    Token* it = NULL;
    VerilogError err;

    for ( std::string path : list_bad_files )
    {
      it = helper_tokenize_preproc(path.c_str());

      //std::cout << "test_load_module_badverilog_XX_v: " << path << std::endl;
      do {
        if (mod)
          free_module(&mod);
        
        err = load_module(&it, &q, &mod);
      }
      while (err == NO_ERROR);
      
      CPPUNIT_ASSERT( !mod );
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

    FILE* f = NULL;
    Evento* q = new_empty_event();
    Module* mod = NULL;
    ListToken* tokens = NULL;
    Token* it = NULL;
    PreprocesorResult result;

    for ( std::string path : list_bad_files )
    {
      tokens = helper_tokenize_only(path.c_str());
      CPPUNIT_ASSERT( tokens->primeiro );

      result = pre_processor(tokens); 
      if (result == PREPROCESSOR_SUCCESS) {
        CPPUNIT_ASSERT( tokens->primeiro );
      
        it = tokens->primeiro;

        VerilogError err = load_module(&it, &q, &mod);
        CPPUNIT_ASSERT( !mod );
        CPPUNIT_ASSERT( err != NO_ERROR );

        delete_lista_token(tokens);
        free_module(&mod);
      }
      else if (tokens) {
        delete_lista_token(tokens);
      }
    }

    delete_event_queue(&q);
  }

};
