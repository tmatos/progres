#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "sinaisTests.cpp"
#include "lexTests.cpp"

int main(int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TextOutputter textOut( &runner.result(), std::cout );
  runner.addTest( Testes_sinais::suite() );
  runner.addTest( Testes_lex::suite() );
  int returnVal = runner.run() ? 0 : 1;
  textOut.printStatistics();
  return returnVal;
}

