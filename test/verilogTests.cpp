#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <iostream>
#include <cstring>
#include <list>
#include <string>

#include "../verilog.h"

class Testes_verilog : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_verilog );
  CPPUNIT_TEST( test_isPortaLogica );
  CPPUNIT_TEST( test_carregaCircuito_fileEmpty );
  CPPUNIT_TEST( test_carregaCircuito_fileTop_module );
  CPPUNIT_TEST( test_carregaCircuito_fileTudo_module );
  CPPUNIT_TEST( test_carregaCircuito_reg_v );
  CPPUNIT_TEST( test_carregaCircuito_localparam_test_v );
  CPPUNIT_TEST( test_carregaCircuito_badverilog_XX_v );
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

  void test_carregaCircuito_fileEmpty()
  {
    Module* circuit = NULL;
    FILE* arquivoVerilogVazio = fopen("./verilog_sample_src/empty.v", "r");
    CPPUNIT_ASSERT( arquivoVerilogVazio );
    circuit = carregaCircuito(arquivoVerilogVazio);
    CPPUNIT_ASSERT( !circuit );
    fclose(arquivoVerilogVazio);
  }

  void test_carregaCircuito_fileTop_module()
  {
    Module* circuit = NULL;
    FILE* arquivoVerilogTop = fopen("./verilog_sample_src/top.v", "r");
    CPPUNIT_ASSERT( arquivoVerilogTop );
    circuit = carregaCircuito(arquivoVerilogTop);
    CPPUNIT_ASSERT( circuit );
    fclose(arquivoVerilogTop);
  }

  void test_carregaCircuito_fileTudo_module()
  {
    Module* circuit = NULL;
    FILE* arquivo = fopen("./verilog_sample_src/tudo.v", "r");
    CPPUNIT_ASSERT( arquivo );
    circuit = carregaCircuito(arquivo);
    CPPUNIT_ASSERT( circuit );
    fclose(arquivo);
  }

  void test_carregaCircuito_reg_v()
  {
    Module* circuit = NULL;
    FILE* file = fopen("./verilog_sample_src/reg.v", "r");
    CPPUNIT_ASSERT( file );
    circuit = carregaCircuito(file);
    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT( circuit->listaReg.total == 5 );
    CPPUNIT_ASSERT( !strcmp(circuit->listaReg.itens[0]->name, "ra") );
    CPPUNIT_ASSERT( !strcmp(circuit->listaReg.itens[1]->name, "rb") );
    CPPUNIT_ASSERT( !strcmp(circuit->listaReg.itens[2]->name, "rc") );
    CPPUNIT_ASSERT( !strcmp(circuit->listaReg.itens[3]->name, "rd") );
    CPPUNIT_ASSERT( !strcmp(circuit->listaReg.itens[4]->name, "r_flag") );
    fclose(file);
  }

  void test_carregaCircuito_localparam_test_v()
  {
    Module* circuit = NULL;
    FILE* file = fopen("./verilog_sample_src/localparam_test.v", "r");
    CPPUNIT_ASSERT( file );
    circuit = carregaCircuito(file);
    CPPUNIT_ASSERT( circuit );
    CPPUNIT_ASSERT( circuit->listaParam.total == 2 );
    CPPUNIT_ASSERT( !strcmp(circuit->listaParam.itens[0]->name, "VER_NUM") );
    CPPUNIT_ASSERT_EQUAL( circuit->listaParam.itens[0]->value, 123 );
    CPPUNIT_ASSERT( !strcmp(circuit->listaParam.itens[1]->name, "_AUTHOR_ID") );
    CPPUNIT_ASSERT_EQUAL( circuit->listaParam.itens[1]->value, 1 );
    fclose(file);
  }

  void test_carregaCircuito_badverilog_XX_v()
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
      "./verilog_sample_src/badverilog_59.v"
    };

    Module* circuit = NULL;
    FILE* fp = NULL;

    for ( std::string path : list_bad_files )
    {
      //std::cout << "test_carregaCircuito_badverilog_XX_v: " << path << std::endl;
      fp = fopen( path.c_str(), "r");
      CPPUNIT_ASSERT( fp );
      circuit = carregaCircuito(fp);
      CPPUNIT_ASSERT( !circuit );
      fclose(fp);
    }
  }

};
