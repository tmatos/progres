#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../verilog.h"

class Testes_verilog : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_verilog );
  CPPUNIT_TEST( test_isPortaLogica );
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

};