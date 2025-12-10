#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>
#include <list>
#include <string>

#include "../strutil.h"
#include "../lex.h"

class Testes_lex : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_lex );
  CPPUNIT_TEST( test_iguais );
  CPPUNIT_TEST( test_has_only_digits );
  CPPUNIT_TEST( test_new_list_token );
  CPPUNIT_TEST( test_insert_token_of_string );
  CPPUNIT_TEST( test_remove_tokens_by_value );
  CPPUNIT_TEST( test_is_single_char_symbol );
  CPPUNIT_TEST( test_is_allowed_identifier );
  CPPUNIT_TEST( test_has_item_of_string_value );
  CPPUNIT_TEST( test_is_reserverd_word );
  CPPUNIT_TEST( test_has_only_digits_outro );
  CPPUNIT_TEST( test_is_valid_natural_number );
  CPPUNIT_TEST( test_get_token_class );
  CPPUNIT_TEST( test_tokeniza_operators_v );
  CPPUNIT_TEST( test_tokeniza_top_v );
  CPPUNIT_TEST( test_tokeniza_tudo_v );
  CPPUNIT_TEST( test_tokeniza_multiline_v );
  CPPUNIT_TEST( test_tokeniza_strings_v );
  CPPUNIT_TEST( test_tokeniza_bad_lexical_v );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_new_list_token()
  {
    ListToken* l = new_list_token();
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( ! l->primeiro );
    CPPUNIT_ASSERT( ! l->ultimo );
    CPPUNIT_ASSERT_EQUAL( 0, l->tamanho );
  }  

  void test_insert_token_of_string()
  {
    ListToken* l = new_list_token();

    std::string token0("my_token");
    std::string token1("<=");
    
    insert_token_of_string(l, token0.c_str(), 3000, 200);
    
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( l->primeiro );
    CPPUNIT_ASSERT( l->ultimo );
    CPPUNIT_ASSERT( l->primeiro == l->ultimo );
    CPPUNIT_ASSERT( ! l->primeiro->seguinte );
    CPPUNIT_ASSERT( ! l->primeiro->anterior );
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );
    CPPUNIT_ASSERT_EQUAL( 3000, l->primeiro->linha );
    CPPUNIT_ASSERT_EQUAL( 200, l->primeiro->coluna );
    CPPUNIT_ASSERT( !strcmp(token0.c_str(), l->primeiro->valor) );
    
    insert_token_of_string(l, token1.c_str(), 3000, 210);
    
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( l->primeiro );
    CPPUNIT_ASSERT( l->ultimo );
    CPPUNIT_ASSERT( l->primeiro != l->ultimo );
    CPPUNIT_ASSERT_EQUAL( 2, l->tamanho );
    CPPUNIT_ASSERT_EQUAL( 3000, l->primeiro->linha );
    CPPUNIT_ASSERT_EQUAL( 200, l->primeiro->coluna );
    CPPUNIT_ASSERT( !strcmp(token0.c_str(), l->primeiro->valor) );
    CPPUNIT_ASSERT_EQUAL( 3000, l->ultimo->linha );
    CPPUNIT_ASSERT_EQUAL( 210, l->ultimo->coluna );
    CPPUNIT_ASSERT( !strcmp(token1.c_str(), l->ultimo->valor) );
    CPPUNIT_ASSERT( ! l->primeiro->anterior );
    CPPUNIT_ASSERT( ! l->ultimo->seguinte );
    CPPUNIT_ASSERT( l->primeiro->seguinte == l->ultimo );
    CPPUNIT_ASSERT( l->primeiro == l->ultimo->anterior );
  }

  void test_remove_tokens_by_value()
  {
    ListToken* l = new_list_token();

    std::string token0("none");
    std::string token1("<=");
    std::string token2("wire");
    std::string token3("#");
    std::string tokenZ("zzzzz");
    
    insert_token_of_string(l, token0.c_str(), 500, 210);
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );
    
    remove_tokens_by_value(l, tokenZ.c_str()); // nao sera encontrado
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );

    remove_tokens_by_value(l, token0.c_str());
    CPPUNIT_ASSERT_EQUAL( 0, l->tamanho );
    CPPUNIT_ASSERT( ! l->primeiro );
    CPPUNIT_ASSERT( ! l->ultimo );

    insert_token_of_string(l, token0.c_str(), 10, 1);
    insert_token_of_string(l, token1.c_str(), 20, 1);
    insert_token_of_string(l, token2.c_str(), 50, 1);
    insert_token_of_string(l, token3.c_str(), 500, 1);
    CPPUNIT_ASSERT_EQUAL( 4, l->tamanho );

    remove_tokens_by_value(l, token0.c_str());
    CPPUNIT_ASSERT_EQUAL( 3, l->tamanho );

    remove_tokens_by_value(l, token2.c_str());
    CPPUNIT_ASSERT_EQUAL( 2, l->tamanho );
    CPPUNIT_ASSERT( l->primeiro != l->ultimo );

    remove_tokens_by_value(l, token3.c_str());
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );
    CPPUNIT_ASSERT( l->primeiro == l->ultimo );
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

  void test_has_only_digits()
  {
    char str_0[] = "001123456789";
    char str_1[] = "001123456789a";
    char str_2[] = "001123456789 ";

    CPPUNIT_ASSERT(   has_only_digits(str_0) );
    CPPUNIT_ASSERT( ! has_only_digits(str_1) );
    CPPUNIT_ASSERT( ! has_only_digits(str_2) );
    CPPUNIT_ASSERT( ! has_only_digits(NULL) );
  }

  void test_is_single_char_symbol()
  {
    int n = 26;
    char simbolos[n] = "(),;:{}[]?=<>~&|!+-*/#@$\"\'";

    for( int i=0 ; i<n ; i++ ) {
      CPPUNIT_ASSERT( is_single_char_symbol(simbolos[i]) );
    }

    CPPUNIT_ASSERT( ! is_single_char_symbol('5') );
    CPPUNIT_ASSERT( ! is_single_char_symbol('b') );
    CPPUNIT_ASSERT( ! is_single_char_symbol('B') );
    CPPUNIT_ASSERT( ! is_single_char_symbol('_') );
    CPPUNIT_ASSERT( ! is_single_char_symbol(' ') );
  }

  void test_is_allowed_identifier()
  {
    std::list<std::string> list_invalid = {
      "module", "wire", "reg", "input", "output",
      "123", "123abc", "+", "++", "$", "$$", "$_"
    };

    std::list<std::string> list_valid = {
      "a", "b", "x", "y", "clk", "clock", "CLK",
      "clr", "CLR", "reset", "_x", "_1", "a_0", "_"
    };

    Token tk;
    tk.linha = 3;
    tk.coluna = 1;
    tk.seguinte = NULL;

    for (auto s : list_valid) {
      copy(tk.valor, s.c_str());
      CPPUNIT_ASSERT( is_allowed_identifier(&tk) );
    }

    for (auto s : list_invalid) {
      copy(tk.valor, s.c_str());
      CPPUNIT_ASSERT( !is_allowed_identifier(&tk) );  
    }

    CPPUNIT_ASSERT( ! is_allowed_identifier((Token*)NULL) );
  }

  void test_has_item_of_string_value()
  {
    std::string str;
    ListToken* lst_tk = NULL;

    CPPUNIT_ASSERT( !has_item_of_string_value(lst_tk, str.c_str()) );

    lst_tk = new_list_token();
    CPPUNIT_ASSERT(lst_tk);

    CPPUNIT_ASSERT( !has_item_of_string_value(lst_tk, (const char*)NULL ) );
    CPPUNIT_ASSERT( !has_item_of_string_value(lst_tk, str.c_str()) );
    CPPUNIT_ASSERT( !has_item_of_string_value(lst_tk, "aa") );

    std::list<std::string> lst_str = { "a", "aa", "abc", "+", "(", ")", "**" };

    int line = 1;
    for (auto s : lst_str) {
      insert_token_of_string(lst_tk, s.c_str(), line, 1);
      line++;
    }

    for (auto s : lst_str) {
      CPPUNIT_ASSERT( has_item_of_string_value(lst_tk, s.c_str()) );
    }
  }

  void test_is_reserverd_word()
  {
    Token tk;
    tk.linha = 10;
    tk.coluna = 5;
    tk.seguinte = NULL;

    copy(tk.valor, "aaa");
    CPPUNIT_ASSERT( ! is_reserverd_word(&tk) );

    copy(tk.valor, "123");
    CPPUNIT_ASSERT( ! is_reserverd_word(&tk) );

    copy(tk.valor, "wire");
    CPPUNIT_ASSERT( is_reserverd_word(&tk) );

    copy(tk.valor, "always");
    CPPUNIT_ASSERT( is_reserverd_word(&tk) );

    copy(tk.valor, "xor");
    CPPUNIT_ASSERT( is_reserverd_word(&tk) );

    CPPUNIT_ASSERT( ! is_reserverd_word((Token*)NULL) );
  }

  void test_has_only_digits_outro()
  {
    char str[50];

    copy(str, "1234567890");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "0000000000");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "1");
    CPPUNIT_ASSERT( has_only_digits(str) );

    copy(str, "A");
    CPPUNIT_ASSERT( ! has_only_digits(str) );

    copy(str, "555555x555555");
    CPPUNIT_ASSERT( ! has_only_digits(str) );

    copy(str, "999999999999m");
    CPPUNIT_ASSERT( ! has_only_digits(str) );
  }

  void test_is_valid_natural_number()
  {
    std::list<std::string> valid_nums = {
      "0", "00", "000", "0000",
      "01", "001", "0001", "0009",
      "1", "9", "20", "123", "1000",
      "09", "99", "999", "9090", "9999"
    };

    for ( std::string s : valid_nums ) {
      CPPUNIT_ASSERT( is_valid_natural_number(s.c_str()) );
    }

    std::list<std::string> invalid_nums = {
      "-1", "a", "a1a", "1a1", "1aaaa", "0000a", "9999999999999999"
    };

    for ( std::string s : invalid_nums ) {
      CPPUNIT_ASSERT( ! is_valid_natural_number(s.c_str()) );
    }

    CPPUNIT_ASSERT( ! is_valid_natural_number((char*)NULL) );
  }

  void test_get_token_class()
  {
    std::vector<std::tuple<std::string, TokenClass>> pairs = {
      {"module", KW_MODULE},
      {"endmodule", KW_ENDMODULE},
      {"input", KW_INPUT},
      {"output", KW_OUTPUT},
      {"wire", KW_WIRE},
      {"reg", KW_REG},
      {"signed", KW_SIGNED},
      {"assign", KW_ASSIGN},
      {"begin", KW_BEGIN},
      {"end", KW_END},
      {"initial", KW_INITIAL},
      {"parameter", KW_PARAMETER},
      {"defparam", KW_DEFPARAM},
      {"localparam", KW_LOCALPARAM},
      {"buf", KW_BUF},
      {"not", KW_NOT},
      {"and", KW_AND},
      {"nand", KW_NAND},
      {"or", KW_OR},
      {"nor", KW_NOR},
      {"xor", KW_XOR},
      {"xnor", KW_XNOR},
      {"bufif0", KW_BUFIF0},
      {"bufif1", KW_BUFIF1},
      {"notif0", KW_NOTIF0},
      {"notif1", KW_NOTIF1},
      {"=", SYM_EQ},
      {",", SYM_COMMA},
      {":", SYM_COLON},
      {";", SYM_SEMICOLON},
      {"(", SYM_OPEN_BRACKET},
      {")", SYM_CLOSE_BRACKET},
      {"[", SYM_OPEN_SQUAREBRACKET},
      {"]", SYM_CLOSE_SQUAREBRACKET},
      {"{", SYM_OPEN_BRACE},
      {"}", SYM_CLOSE_BRACE},
      {"{{", SYM_DOUBLE_OPEN_BRACE},
      {"}}", SYM_DOUBLE_CLOSE_BRACE},
      {"+", SYM_PLUS},
      {"-", SYM_MINUS},
      {"*", SYM_ASTERISK},
      {"/", SYM_SLASH},
      {"%", SYM_PERCENT},
      {"~", SYM_TILDE},
      {"&", SYM_AMPERSAND},
      {"|", SYM_PIPE},
      {"^", SYM_CIRCUMFLEX},
      {"$", SYM_DOLLAR},
      {"**", SYM_DOUBLE_ASTERISK},
      {">", SYM_GREATER_THAN},
      {">=", SYM_GREATER_OR_EQUAL},
      {"<", SYM_LESS_THAN},
      {"<=", SYM_LESS_OR_EQUAL},
      {"&&", SYM_DOUBLE_AMPERSAND},
      {"||", SYM_DOUBLE_PIPE},
      {"==", SYM_DOUBLE_EQ},
      {"!=", SYM_EXCLAMATION_EQ},
      {"^~", SYM_CIRCUMFLEX_TILDE},
      {"~^", SYM_TILDE_CIRCUMFLEX},
      {"~&", SYM_TILDE_AMPERSAND},
      {"~|", SYM_TILDE_PIPE},
      {"<<", SYM_DOUBLE_LESS_THAN},
      {">>", SYM_DOUBLE_GREATER_THAN},
      {"?:", SYM_QUESTION_COLON},
      {"`", SYM_GRAVE_ACCENT},
      {"===", SYM_TRIPLE_EQ},
      {"!==", SYM_EXCLAMATION_DOUBLE_EQ},
      {"<<<", SYM_TRIPLE_LESS_THAN},
      {">>>", SYM_TRIPLE_GREATER_THAN},
      {"123456", NUM_BASE_DECIMAL},
      {"\"This is a string\"", STRING},
      {"!unknown_token!", _UNKNOWN}
    };

    for ( auto p : pairs )
    {
      std::string str = std::get<0>(p);
      TokenClass tc_expected = std::get<1>(p);
      
      TokenClass tc_returned = get_token_class(str.c_str());

      CPPUNIT_ASSERT_EQUAL( tc_expected, tc_returned );
    }
  }

  void test_tokeniza_operators_v()
  {
    std::list<std::string> tokens_esperados = {
      "=",
      ",",
      ":",
      ";",
      "(",
      ")",
      "[",
      "]",
      "{",
      "}",
      "#",
      "+",
      "-",
      "*",
      "/",
      "%",
      "~",
      "&",
      "|",
      "^",
      "$",
      ">",
      "<",
      "!",
      "{{",
      "}}",
      "**",
      ">=",
      "<=",
      "&&",
      "||",
      "==",
      "!=",
      "^~",
      "~^",
      "~&",
      "~|",
      "<<",
      ">>",
      "?:",
      "===",
      "!==",
      "<<<",
      ">>>",
      "==",
      "+",
      "!=",
      "+",
      ">>",
      "~",
      "<<",
      "~",
      "`" };
    
    helper_test_tokeniza("./verilog_sample_src/operators.v", tokens_esperados);
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
    
    ListToken* lt = helper_test_tokeniza("./verilog_sample_src/multiline.v", tokens_esperados);

    show_token_list(lt);
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

  ListToken* helper_test_tokeniza(const char* file_path, const std::list<std::string>& tokens_esperados)
  {
    FILE* arquivo = fopen(file_path, "r");
    CPPUNIT_ASSERT(arquivo);

    ListToken* tokens = tokeniza(arquivo);
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

    ListToken* tokens = tokeniza(f_bad_lexical);
    CPPUNIT_ASSERT_EQUAL( 21, tokens->tamanho );
  }

};
