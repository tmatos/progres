#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "estruturasTests.cpp"
#include "sinaisTests.cpp"
#include "lexTests.cpp"
#include "preprocessorTests.cpp"
#include "eventosTests.cpp"
#include "simulaTests.cpp"
#include "verilogTests.cpp"
#include "inoutTests.cpp"
#include "errosTests.cpp"
#include "strutilTests.cpp"

int global_silent_mode;

int main(int argc, char **argv)
{
  global_silent_mode = 0; // if 0, will not output msgs to stdout
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TextOutputter textOut( &runner.result(), std::cout );
  runner.addTest( Testes_estruturas::suite() );
  runner.addTest( Testes_sinais::suite() );
  runner.addTest( Testes_lex::suite() );
  runner.addTest( Testes_preprocessor::suite() );
  runner.addTest( Testes_eventos::suite() );
  runner.addTest( Testes_simula::suite() );
  runner.addTest( Testes_verilog::suite() );
  runner.addTest( Testes_inout::suite() );
  runner.addTest( Testes_erros::suite() );
  runner.addTest( Testes_strutil::suite() );
  int returnVal = runner.run() ? 0 : 1;
  textOut.printStatistics();
  return returnVal;
}

