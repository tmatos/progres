#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "../lex.h"

class Testes_lex : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_lex );
  CPPUNIT_TEST( test_iguais );
  CPPUNIT_TEST( test_apenasDigitos );
  CPPUNIT_TEST( test_novaListaToken );
  CPPUNIT_TEST( test_isSimbolo );
  CPPUNIT_TEST( test_isIdentificador );
  CPPUNIT_TEST( test_isPalavra );
  CPPUNIT_TEST( test_apenasDigitos_outro );
  CPPUNIT_TEST( test_isNumNaturalValido );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_novaListaToken()
  {
    ListaToken* l = novaListaToken();

    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( ! l->primeiro );
    CPPUNIT_ASSERT( ! l->ultimo );
    CPPUNIT_ASSERT_EQUAL( l->tamanho, 0 );
  }

  void test_iguais()
  {
    char str_0[] = "";
    char str_1[] = "";
    char str_2[] = "p";
    char str_3[] = "p";
    char str_4[] = "wp";
    char str_5[] = "Wp";
    char str_6[] = "abc def";
    char str_7[] = "abc_def";
    char str_8[] = "a1b2c3_defACZ-780";
    char str_9[] = "a1b2c3_defACZ-780";
    
    CPPUNIT_ASSERT(   iguais(str_0, str_1) );
    CPPUNIT_ASSERT( ! iguais(str_1, str_2) );
    CPPUNIT_ASSERT(   iguais(str_2, str_3) );
    CPPUNIT_ASSERT( ! iguais(str_3, str_4) );
    CPPUNIT_ASSERT( ! iguais(str_4, str_5) );
    CPPUNIT_ASSERT( ! iguais(str_5, str_6) );
    CPPUNIT_ASSERT( ! iguais(str_6, str_7) );
    CPPUNIT_ASSERT(   iguais(str_8, str_9) );
    CPPUNIT_ASSERT(   iguais(str_7, str_7) );
  }

  void test_apenasDigitos()
  {
    char str_0[] = "001123456789";
    char str_1[] = "001123456789a";
    char str_2[] = "001123456789 ";

    CPPUNIT_ASSERT(   apenasDigitos(str_0) );
    CPPUNIT_ASSERT( ! apenasDigitos(str_1) );
    CPPUNIT_ASSERT( ! apenasDigitos(str_2) );
    CPPUNIT_ASSERT( ! apenasDigitos(NULL) );
  }

  void test_isSimbolo()
  {
    int n = 26;
    char simbolos[n] = "(),;:{}[]?=<>~&|!+-*/#@$\"\'";

    int i = 0;

    for( i=0 ; i<n ; i++ )
    {
      CPPUNIT_ASSERT( isSimbolo(simbolos[i]) );
    }

    CPPUNIT_ASSERT( ! isSimbolo('5') );
    CPPUNIT_ASSERT( ! isSimbolo('b') );
    CPPUNIT_ASSERT( ! isSimbolo('B') );
    CPPUNIT_ASSERT( ! isSimbolo('_') );
    CPPUNIT_ASSERT( ! isSimbolo(' ') );
  }

  void test_isIdentificador()
  {
    Token tk;
    tk.linha = 3;
    tk.coluna = 1;
    tk.seguinte = NULL;

    strcpy(tk.valor, "module");
    CPPUNIT_ASSERT( !isIdentificador(&tk) );

    strcpy(tk.valor, "wire");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "reg");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "input");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "output");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "123");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "123abc");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "+");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    strcpy(tk.valor, "a");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "b");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "x");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "y");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "clk");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "clock");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "CLK");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "clr");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "CLR");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    strcpy(tk.valor, "reset");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

  }

  void test_isPalavra()
  {
    Token tk;
    tk.linha = 10;
    tk.coluna = 5;
    tk.seguinte = NULL;

    strcpy(tk.valor, "aaa");
    CPPUNIT_ASSERT( ! isPalavra(&tk) );

    strcpy(tk.valor, "123");
    CPPUNIT_ASSERT( ! isPalavra(&tk) );

    strcpy(tk.valor, "wire");
    CPPUNIT_ASSERT( isPalavra(&tk) );
  }

  void test_apenasDigitos_outro()
  {
    char str[50];

    strcpy(str, "1234567890");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    strcpy(str, "0000000000");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    strcpy(str, "1");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    strcpy(str, "A");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );

    strcpy(str, "555555x555555");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );

    strcpy(str, "999999999999m");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );
  }

  void test_isNumNaturalValido()
  {
    char str[50];

    strcpy(str, "0");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    strcpy(str, "1");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    strcpy(str, "20");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    strcpy(str, "123");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    strcpy(str, "5050");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    strcpy(str, "-1");
    CPPUNIT_ASSERT( ! isNumNaturalValido(str) );

    strcpy(str, "a");
    CPPUNIT_ASSERT( ! isNumNaturalValido(str) );
  }

};
