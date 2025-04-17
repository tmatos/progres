#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>
#include <list>
#include <string>

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
  CPPUNIT_TEST( test_tokeniza_top_v );
  CPPUNIT_TEST( test_tokeniza_tudo_v );
  CPPUNIT_TEST( test_tokeniza_multiline_v );
  CPPUNIT_TEST( test_tokeniza_strings_v );
  CPPUNIT_TEST( test_tokeniza_bad_lexical_v );
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

    for( int i=0 ; i<n ; i++ ) {
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

    copy(tk.valor, "module");
    CPPUNIT_ASSERT( !isIdentificador(&tk) );

    copy(tk.valor, "wire");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "reg");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "input");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "output");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "123");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "123abc");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "+");
    CPPUNIT_ASSERT( ! isIdentificador(&tk) );

    copy(tk.valor, "a");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "b");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "x");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "y");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "clk");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "clock");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "CLK");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "clr");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "CLR");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    copy(tk.valor, "reset");
    CPPUNIT_ASSERT( isIdentificador(&tk) );

    CPPUNIT_ASSERT( ! isIdentificador((Token*)NULL) );
  }

  void test_isPalavra()
  {
    Token tk;
    tk.linha = 10;
    tk.coluna = 5;
    tk.seguinte = NULL;

    copy(tk.valor, "aaa");
    CPPUNIT_ASSERT( ! isPalavra(&tk) );

    copy(tk.valor, "123");
    CPPUNIT_ASSERT( ! isPalavra(&tk) );

    copy(tk.valor, "wire");
    CPPUNIT_ASSERT( isPalavra(&tk) );

    copy(tk.valor, "always");
    CPPUNIT_ASSERT( isPalavra(&tk) );

    copy(tk.valor, "xor");
    CPPUNIT_ASSERT( isPalavra(&tk) );

    CPPUNIT_ASSERT( ! isPalavra((Token*)NULL) );
  }

  void test_apenasDigitos_outro()
  {
    char str[50];

    copy(str, "1234567890");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    copy(str, "0000000000");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    copy(str, "1");
    CPPUNIT_ASSERT( apenasDigitos(str) );

    copy(str, "A");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );

    copy(str, "555555x555555");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );

    copy(str, "999999999999m");
    CPPUNIT_ASSERT( ! apenasDigitos(str) );
  }

  void test_isNumNaturalValido()
  {
    char str[50];

    copy(str, "0");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    copy(str, "1");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    copy(str, "20");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    copy(str, "123");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    copy(str, "5050");
    CPPUNIT_ASSERT( isNumNaturalValido(str) );

    copy(str, "-1");
    CPPUNIT_ASSERT( ! isNumNaturalValido(str) );

    copy(str, "a");
    CPPUNIT_ASSERT( ! isNumNaturalValido(str) );

    CPPUNIT_ASSERT( ! isNumNaturalValido((char*)NULL) );
  }

  void test_tokeniza_top_v()
  {
    std::list<std::string> tokens_esperados = {
      "module", "top", "(", ")", ";",
      "endmodule" };
    
    helper_test_tokeniza("./verilog_sample_src/top.v", tokens_esperados);
  }

  void test_tokeniza_tudo_v()
  {
    std::list<std::string> tokens_esperados = {
      "module", "modTudo", "(", "a0", ",", "b0", ",", "x0", ",", "y0", ")", ";",
      "input",  "a0", ",", "b0", ";",
      "output", "x0", ",", "y0", ";",
      "wire", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ";",
      "wire", "wo0", ",", "wo1", ",", "wo2", ",", "wo3", ",", "wo4", ",", "wo5", ";",
      "buf",  "(", "wi0", ",", "a0", ")", ";",
      "buf",  "(", "wi1", ",", "b0", ")", ";",
      "not",  "(", "wi2", ",", "a0", ")", ";",
      "not",  "(", "wi3", ",", "b0", ")", ";",
      "and",  "(", "wo0", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "or",   "(", "wo1", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "nand", "(", "wo2", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "nor",  "(", "wo3", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "xor",  "(", "wo4", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "xnor", "(", "wo5", ",", "wi0", ",", "wi1", ",", "wi2", ",", "wi3", ")", ";",
      "not",  "(", "x0",  ",", "wo0", ")", ";",
      "buf",  "(", "y0",  ",", "wo0", ")", ";",
      "endmodule" };

    helper_test_tokeniza("./verilog_sample_src/tudo.v", tokens_esperados);
  }

  void test_tokeniza_multiline_v()
  {
    std::list<std::string> tokens_esperados = {
      "module", "multiline", "(", ")", ";",
      "wire", "w", ";",
      "endmodule" };
    
    ListaToken* lt = helper_test_tokeniza("./verilog_sample_src/multiline.v", tokens_esperados);

    exibeListaDeToken(lt);
  }

  void test_tokeniza_strings_v()
  {
    std::list<std::string> tokens_esperados = {
      "module", "strings", "(", ")", ";",
      "reg", "[", "255", ":", "0", "]", "str1", ";",
      "initial", "begin",
      "str1", "=", "\"Hello\n world\"", ";",
      "end",
      "endmodule" };

    helper_test_tokeniza("./verilog_sample_src/strings.v", tokens_esperados);
  }

  ListaToken* helper_test_tokeniza(const char* file_path, const std::list<std::string>& tokens_esperados)
  {
    FILE* arquivo = fopen(file_path, "r");
    CPPUNIT_ASSERT(arquivo);

    ListaToken* tokens = tokeniza(arquivo);
    CPPUNIT_ASSERT(tokens);

    CPPUNIT_ASSERT_EQUAL( (size_t)tokens_esperados.size(), (size_t)tokens->tamanho );

    Token* it = tokens->primeiro;

    for ( auto s : tokens_esperados ) {
      CPPUNIT_ASSERT(it);
      CPPUNIT_ASSERT( !strcmp(s.c_str(), it->valor) );
      it = it->seguinte;
    }

    CPPUNIT_ASSERT_EQUAL(tokens->ultimo->seguinte, it);
    CPPUNIT_ASSERT(!it);

    fclose(arquivo);

    return tokens;
  }

  void test_tokeniza_bad_lexical_v()
  {
    FILE* f_bad_lexical = fopen("./verilog_sample_src/bad_lexical.v", "r");
    CPPUNIT_ASSERT(f_bad_lexical);

    ListaToken* tokens = tokeniza(f_bad_lexical);
    CPPUNIT_ASSERT_EQUAL( 21, tokens->tamanho );
  }

};
