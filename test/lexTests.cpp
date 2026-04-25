#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <list>
#include <string>

#include "strutil.h"
#include "lex.h"

class Testes_lex : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_lex );
  CPPUNIT_TEST( test_avanca );
  CPPUNIT_TEST( test_backtrack );
  CPPUNIT_TEST( test_new_token );
  CPPUNIT_TEST( test_new_token_to_detect_class );
  CPPUNIT_TEST( test_new_list_token );
  CPPUNIT_TEST( test_insert_token_of_string );
  CPPUNIT_TEST( test_remove_tokens_by_value );
  CPPUNIT_TEST( test_is_single_char_symbol );
  CPPUNIT_TEST( test_is_allowed_identifier );
  CPPUNIT_TEST( test_has_item_of_string_value );
  CPPUNIT_TEST( test_is_reserverd_word );
  CPPUNIT_TEST( test_get_token_class );
  CPPUNIT_TEST( test_tokeniza_operators_v );
  CPPUNIT_TEST( test_tokeniza_top_v );
  CPPUNIT_TEST( test_tokeniza_tudo_v );
  CPPUNIT_TEST( test_tokeniza_multiline_v );
  CPPUNIT_TEST( test_tokeniza_strings_v );
  CPPUNIT_TEST( test_tokeniza_bad_lexical_v );
  CPPUNIT_TEST( test_get_bit_size_from_literal_token );
  CPPUNIT_TEST( test_convert_value_string_to_uint );
  CPPUNIT_TEST( test_get_value_from_literal_token );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_avanca()
  {
    Token* t_0 = new_token("module", 1, 1, KW_MODULE);
    Token* t_1 = new_token("(", 1, 8, SYM_OPEN_BRACKET);
    Token* t_2 = new_token(")", 1, 9, SYM_CLOSE_BRACKET);
    
    CPPUNIT_ASSERT(t_0);
    CPPUNIT_ASSERT(t_1);
    CPPUNIT_ASSERT(t_2);

    t_0->seguinte = t_1;
    t_1->anterior = t_0;
    t_1->seguinte = t_2;
    t_2->anterior = t_1;
    
    Token* cursor = NULL;
    CPPUNIT_ASSERT_EQUAL( (Token*)NULL, avanca(&cursor) );

    cursor = t_2;
    CPPUNIT_ASSERT_EQUAL( (Token*)NULL, avanca(&cursor) );

    cursor = t_0;
    CPPUNIT_ASSERT_EQUAL( t_1, avanca(&cursor) );

    cursor = t_1;
    CPPUNIT_ASSERT_EQUAL( t_2, avanca(&cursor) );
  }

  void test_backtrack()
  {
    Token* t_0 = new_token("module", 1, 1, KW_MODULE);
    Token* t_1 = new_token("(", 1, 8, SYM_OPEN_BRACKET);
    Token* t_2 = new_token(")", 1, 9, SYM_CLOSE_BRACKET);
    Token* t_3 = new_token(";", 1, 10, SYM_SEMICOLON);
    
    CPPUNIT_ASSERT(t_0);
    CPPUNIT_ASSERT(t_1);
    CPPUNIT_ASSERT(t_2);
    CPPUNIT_ASSERT(t_3);

    t_0->seguinte = t_1;
    t_1->anterior = t_0;
    t_1->seguinte = t_2;
    t_2->anterior = t_1;
    t_2->seguinte = t_3;
    t_3->anterior = t_2;
    
    Token* cursor = NULL;
    CPPUNIT_ASSERT_EQUAL( (Token*)NULL, backtrack(&cursor) );

    cursor = t_0;
    CPPUNIT_ASSERT_EQUAL( (Token*)NULL, backtrack(&cursor) );

    cursor = t_1;
    CPPUNIT_ASSERT_EQUAL( t_0, backtrack(&cursor) );

    cursor = t_2;
    CPPUNIT_ASSERT_EQUAL( t_1, backtrack(&cursor) );

    cursor = t_3;
    CPPUNIT_ASSERT_EQUAL( t_2, backtrack(&cursor) );
  }

  void test_new_token()
  {
    std::string str_value = "some_token";
    int line = 500;
    int column = 10;
    TokenClass t_class = KW_MODULE;

    Token* t = new_token(str_value.c_str(), line, column, t_class);
    CPPUNIT_ASSERT(t);
    CPPUNIT_ASSERT( ! t->anterior );
    CPPUNIT_ASSERT( ! t->seguinte );
    CPPUNIT_ASSERT_EQUAL( line, t->linha );
    CPPUNIT_ASSERT_EQUAL( column, t->coluna );
    std::string str_token_value(t->valor);
    CPPUNIT_ASSERT_EQUAL( str_value, str_token_value );
    CPPUNIT_ASSERT_EQUAL( t_class, t->classe );
  }

  void test_new_token_to_detect_class()
  {
    std::string str_value = "some_token_0";
    TokenClass detected_class = get_token_class(str_value.c_str());
    int line = 1;
    int column = 1;

    Token* t = new_token(str_value.c_str(), line, column, _TO_DETECT);
    CPPUNIT_ASSERT(t);
    CPPUNIT_ASSERT( ! t->anterior );
    CPPUNIT_ASSERT( ! t->seguinte );
    CPPUNIT_ASSERT_EQUAL( line, t->linha );
    CPPUNIT_ASSERT_EQUAL( column, t->coluna );
    std::string str_token_value(t->valor);
    CPPUNIT_ASSERT_EQUAL( str_value, str_token_value );
    CPPUNIT_ASSERT_EQUAL( detected_class, t->classe );
  }

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
    
    insert_token_of_string(l, token0.c_str(), 3000, 200, IDENTIFIER);
    
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( l->primeiro );
    CPPUNIT_ASSERT( l->ultimo );
    CPPUNIT_ASSERT( l->primeiro == l->ultimo );
    CPPUNIT_ASSERT( ! l->primeiro->seguinte );
    CPPUNIT_ASSERT( ! l->primeiro->anterior );
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );
    CPPUNIT_ASSERT_EQUAL( 3000, l->primeiro->linha );
    CPPUNIT_ASSERT_EQUAL( 200, l->primeiro->coluna );
    std::string str_l_first_value(l->primeiro->valor);
    CPPUNIT_ASSERT_EQUAL( token0, str_l_first_value );
    
    insert_token_of_string(l, token1.c_str(), 3000, 210, SYM_LESS_OR_EQUAL);
    
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT( l->primeiro );
    CPPUNIT_ASSERT( l->ultimo );
    CPPUNIT_ASSERT( l->primeiro != l->ultimo );
    CPPUNIT_ASSERT_EQUAL( 2, l->tamanho );
    CPPUNIT_ASSERT_EQUAL( 3000, l->primeiro->linha );
    CPPUNIT_ASSERT_EQUAL( 200, l->primeiro->coluna );
    CPPUNIT_ASSERT_EQUAL( token0, str_l_first_value );
    CPPUNIT_ASSERT_EQUAL( 3000, l->ultimo->linha );
    CPPUNIT_ASSERT_EQUAL( 210, l->ultimo->coluna );
    std::string str_l_last_value(l->ultimo->valor);
    CPPUNIT_ASSERT_EQUAL( token1, str_l_last_value );
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
    
    insert_token_of_string(l, token0.c_str(), 500, 210, IDENTIFIER);
    CPPUNIT_ASSERT(l);
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );
    
    remove_tokens_by_value(l, tokenZ.c_str()); // nao sera encontrado
    CPPUNIT_ASSERT_EQUAL( 1, l->tamanho );

    remove_tokens_by_value(l, token0.c_str());
    CPPUNIT_ASSERT_EQUAL( 0, l->tamanho );
    CPPUNIT_ASSERT( ! l->primeiro );
    CPPUNIT_ASSERT( ! l->ultimo );

    insert_token_of_string(l, token0.c_str(), 10, 1, IDENTIFIER);
    insert_token_of_string(l, token1.c_str(), 20, 1, SYM_LESS_OR_EQUAL);
    insert_token_of_string(l, token2.c_str(), 50, 1, KW_WIRE);
    insert_token_of_string(l, token3.c_str(), 500, 1, SYM_HASHTAG);
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
      insert_token_of_string(lst_tk, s.c_str(), line, 1, _TO_DETECT);
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

    for ( std::string s : tokens_esperados )
    {
      CPPUNIT_ASSERT(it);
      std::string str_it_valor(it->valor);
      CPPUNIT_ASSERT_EQUAL( s, str_it_valor );
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

  void test_get_bit_size_from_literal_token()
  {
    std::vector<std::tuple<TokenClass, std::string, unsigned int>> tups_tok_str_uint = {
      {_UNKNOWN, "unknown_token", 0},
      {NUM_BASE_DECIMAL, "", 0},
      {NUM_BASE_DECIMAL, "0", 32},
      {NUM_BASE_DECIMAL, "1", 32},
      {NUM_BASE_DECIMAL, "123", 32},
      {NUM_BASE_DECIMAL, "255", 32},
      {NUM_BASE_DECIMAL, "256", 32},
      {NUM_BASE_DECIMAL, "1023", 32},
      {NUM_BASE_DECIMAL, "1024", 32},
      {NUM_BASE_BINARY, "1'b1", 1},
      {NUM_BASE_BINARY, "2'b00", 2},
      {NUM_BASE_BINARY, "3'b010", 3},
      {NUM_BASE_BINARY, "8'b10101010", 8},
      {NUM_BASE_BINARY, "10'b1010101010", 10},
      {NUM_BASE_BINARY, "16'b1010101010101010", 16},
      {NUM_BASE_BINARY, "16'b1010101_010101010", 16},
      {NUM_BASE_HEX, "16'hDEAD", 16},
      {NUM_BASE_HEX, "32'hDEADBEEF", 32},
      {NUM_BASE_HEX, "32'hDEAD_BEEF", 32},
      {NUM_BASE_HEX, "64'hBEEF_BABA_CADA_FADA", 64},
      {NUM_BASE_OCTAL, "3'o0", 3},
      {NUM_BASE_OCTAL, "6'o77", 6},
      {NUM_BASE_OCTAL, "9'o770", 9},
      {NUM_BASE_OCTAL, "12'o7701", 12},
      {NUM_BASE_OCTAL, "18'o770155", 18},
      {NUM_BASE_OCTAL, "18'o77_01_55", 18},
      {NUM_BASE_OCTAL, "36'o770155770155", 36}
    };

    for ( auto t : tups_tok_str_uint )
    {
      TokenClass tok_class = std::get<0>(t);
      std::string tok_str = std::get<1>(t);
      unsigned int expected_size = std::get<2>(t);

      Token tk;
      tk.linha = 1;
      tk.coluna = 1;
      tk.anterior = NULL;
      tk.seguinte = NULL;
      tk.classe = tok_class;
      copy(tk.valor, tok_str.c_str());

      unsigned int bit_size = get_bit_size_from_literal_token(&tk);

      CPPUNIT_ASSERT_EQUAL( expected_size, bit_size );
    }
  }

  void test_convert_value_string_to_uint()
  {
    std::vector<std::tuple<char, std::string, unsigned int>> tups_base_str_uint = {
      
      // incorrect cases shall return 0
      {'x', "FF", 0},
      {'h', "", 0},

      {'d', "0", 0},
      {'d', "1", 1},
      {'d', "123", 123},
      {'d', "255", 255},
      {'d', "256", 256},
      {'d', "1023", 1023},
      {'d', "1024", 1024},

      {'b', "0", 0},
      {'b', "1", 1},
      {'b', "10101010", 170},
      {'b', "1010101010", 682},
      {'b', "101010101010", 2730},
      {'b', "1010101010101010", 43690},

      {'h', "0", 0},
      {'h', "1", 1},
      {'h', "DEAD", 57005},
      {'h', "DEADBEEF", 3735928559},

      {'o', "0", 0},
      {'o', "1", 1},
      {'o', "770", 504},
      {'o', "7701", 4033},
      {'o', "770155", 258157},
      {'o', "770155770155", 67674566765}
    };

    for ( auto t : tups_base_str_uint )
    {
      char base = std::get<0>(t);
      std::string str = std::get<1>(t);
      unsigned int expected_uint = std::get<2>(t);

      char c_str[MAX_TOKEN_SIZE];
      copy(c_str, str.c_str());

      unsigned int uint_returned = convert_value_string_to_uint(c_str, base);

      CPPUNIT_ASSERT_EQUAL( expected_uint, uint_returned );
    }
  }

  void test_get_value_from_literal_token()
  { 
    std::vector<std::tuple<TokenClass, std::string, unsigned int>> tups_tokcls_str_uint = {
      {_UNKNOWN, "unknown_token", 0},
      {NUM_BASE_DECIMAL, "", 0},
      {NUM_BASE_DECIMAL, "0", 0},
      {NUM_BASE_DECIMAL, "1", 1},
      {NUM_BASE_DECIMAL, "123", 123},
      {NUM_BASE_DECIMAL, "255", 255},
      {NUM_BASE_DECIMAL, "256", 256},
      {NUM_BASE_DECIMAL, "1023", 1023},
      {NUM_BASE_DECIMAL, "1024", 1024}
    };

    for ( auto t : tups_tokcls_str_uint )
    {
      TokenClass tok_class = std::get<0>(t);
      std::string tok_str = std::get<1>(t);
      unsigned int expected_uint = std::get<2>(t);

      Token tk;
      tk.linha = 1;
      tk.coluna = 1;
      tk.anterior = NULL;
      tk.seguinte = NULL;
      tk.classe = tok_class;
      copy(tk.valor, tok_str.c_str());

      unsigned int returned_uint = get_value_from_literal_token(&tk);

      CPPUNIT_ASSERT_EQUAL( expected_uint, returned_uint );
    }
  }

};
