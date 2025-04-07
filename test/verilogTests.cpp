#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
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
    t_circuito* circuit = NULL;
    FILE* arquivoVerilogVazio = fopen("./verilog_sample_src/empty.v", "r");
    CPPUNIT_ASSERT( arquivoVerilogVazio );
    circuit = carregaCircuito(arquivoVerilogVazio);
    CPPUNIT_ASSERT( !circuit );
    fclose(arquivoVerilogVazio);
  }

  void test_carregaCircuito_fileTop_module()
  {
    t_circuito* circuit = NULL;
    FILE* arquivoVerilogTop = fopen("./verilog_sample_src/top.v", "r");
    CPPUNIT_ASSERT( arquivoVerilogTop );
    circuit = carregaCircuito(arquivoVerilogTop);
    CPPUNIT_ASSERT( circuit );
    fclose(arquivoVerilogTop);
  }

  void test_carregaCircuito_fileTudo_module()
  {
    t_circuito* circuit = NULL;
    FILE* arquivo = fopen("./verilog_sample_src/tudo.v", "r");
    CPPUNIT_ASSERT( arquivo );
    circuit = carregaCircuito(arquivo);
    CPPUNIT_ASSERT( circuit );
    fclose(arquivo);
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
      "./verilog_sample_src/badverilog_18.v",
      "./verilog_sample_src/badverilog_19.v",
      "./verilog_sample_src/badverilog_20.v",
      "./verilog_sample_src/badverilog_21.v"
    };

    t_circuito* circuit = NULL;
    FILE* fp = NULL;

    for ( std::string path : list_bad_files )
    {
      fp = fopen( path.c_str(), "r");
      CPPUNIT_ASSERT( fp );
      circuit = carregaCircuito(fp);
      CPPUNIT_ASSERT( !circuit );
      fclose(fp);
    }
  }

};
