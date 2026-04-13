/**
 * @file lex.h
 * @brief Funcoes elementares de processamento lexico dos arquivos fonte.
 */

#ifndef LEX_H
#define LEX_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__vxworks)
    #define MAX_PATH_LENGTH 255
#else
    #define MAX_PATH_LENGTH 1023
#endif

#define MAX_TOKEN_SIZE 128 /// Qtde maxima de caracteres permitidos em um Token
#define MAX_DIGITOS_NUM 13 /// Qtde maxima de digitos num numero inteiro a ser reconhecido
#define MAX_SIZE_NUMBER 1024 /// Tamanho maximo permitido para um numero literal sized

/** @brief Enumeracao para a classe do token detectado.
 */
typedef enum en_token_class {
    KW_ALWAYS = 0,
    KW_AND,
    KW_ASSIGN,
    KW_BEGIN,
    KW_BUF,
    KW_END,
    KW_ENDMODULE,
    KW_INITIAL,
    KW_INOUT,
    KW_INPUT,
    KW_MODULE,
    KW_NAND,
    KW_NOR,
    KW_NOT,
    KW_OR,
    KW_OUTPUT,
    KW_PARAMETER,
    KW_LOCALPARAM,
    KW_DEFPARAM,
    KW_REAL,
    KW_REALTIME,
    KW_REG,
    KW_SIGNED,
    KW_TIME,
    KW_TRI,
    KW_WIRE,
    KW_XNOR,
    KW_XOR,
    KW_BUFIF0,
    KW_BUFIF1,
    KW_NOTIF0,
    KW_NOTIF1,

    SYM_AT, // @
    SYM_COMMA, // ,
    SYM_HASHTAG, // #
    SYM_DOT, // .
    SYM_EQ, // =
    SYM_COLON, // :
    SYM_SEMICOLON, // ; 
    SYM_OPEN_BRACKET, // (
    SYM_CLOSE_BRACKET, // )
    SYM_OPEN_SQUAREBRACKET, // [
    SYM_CLOSE_SQUAREBRACKET, // ]
    SYM_OPEN_BRACE, // {
    SYM_CLOSE_BRACE, // }
    SYM_DOUBLE_OPEN_BRACE, // {{
    SYM_DOUBLE_CLOSE_BRACE, // }}
    SYM_PLUS, // +
    SYM_MINUS, // -
    SYM_ASTERISK, // *
    SYM_SLASH, // /
    SYM_PERCENT, // %
    SYM_TILDE, // ~
    SYM_AMPERSAND, // &
    SYM_PIPE, // |
    SYM_CIRCUMFLEX, // ^
    SYM_DOLLAR, // $
    SYM_GRAVE_ACCENT, // `
    SYM_DOUBLE_ASTERISK, // **
    SYM_GREATER_THAN, // >
    SYM_GREATER_OR_EQUAL, // >=
    SYM_LESS_THAN, // <
    SYM_LESS_OR_EQUAL, // <=
    SYM_DOUBLE_AMPERSAND, // &&
    SYM_DOUBLE_PIPE, // ||
    SYM_DOUBLE_EQ, // ==
    SYM_TRIPLE_EQ, // ===
    SYM_EXCLAMATION, // !
    SYM_EXCLAMATION_EQ, // !=
    SYM_EXCLAMATION_DOUBLE_EQ, // !==
    SYM_CIRCUMFLEX_TILDE, // ^~
    SYM_TILDE_CIRCUMFLEX, // ~^
    SYM_TILDE_AMPERSAND, // ~&
    SYM_TILDE_PIPE, // ~|
    SYM_DOUBLE_LESS_THAN, // <<
    SYM_TRIPLE_LESS_THAN, // <<<
    SYM_DOUBLE_GREATER_THAN, // >>
    SYM_TRIPLE_GREATER_THAN, // >>>
    SYM_QUESTION_COLON, // ?:

    NUM_BASE_BINARY,
    NUM_BASE_DECIMAL,
    NUM_BASE_OCTAL,
    NUM_BASE_HEX,

    STRING,
    IDENTIFIER,
    _UNKNOWN,
    _TO_DETECT // usado internamente no processo de tokenizacao
} TokenClass;

/** @brief Struct for pairs that represents the mapping between
 *         token strings and their respective TokenClass.
 */
typedef struct st_pair_string_tokenclass
{
    char token_string[MAX_TOKEN_SIZE];
    TokenClass token_class;
} PairStringTokenClass;

/** @brief Tipo basico para o elemento que representa um token.
 */
typedef struct st_token {
    char valor[MAX_TOKEN_SIZE];
    int linha;
    int coluna;
    TokenClass classe;
    struct st_token* anterior;
    struct st_token* seguinte;
} Token;

/** @brief Tipo para uma lista encadeada de Tokens.
 */
typedef struct st_list_token {
    Token* primeiro;
    Token* ultimo;
    int tamanho;
    char file[MAX_PATH_LENGTH];
} ListToken;

/** @brief Inicializa uma lista vazia, isto é, com zero elementos.
 */
ListToken* new_list_token();

/** @brief Delete the list and its contents from memory.
 */
void delete_lista_token(ListToken* list);

/** @brief Cria um novo token com os dados especificados.
 *  @param value String que representa o valor textual do token.
 *  @param line Linha no arquivo onde o token foi encontrado.
 *  @param column Coluna no arquivo onde o token inicia-se.
 *  @param class Classe do token, conforme enum TokenClass.
 *  @return Ponteiro para o token criado.
 */
Token* new_token(const char* value, int line, int column, TokenClass t_class);

/** @brief Adiciona um token inicializado ao final da lista de tokens.
 *  @param list Ponteiro para a struct da lista onde o token sera adicionado.
 *  @param tok Ponteiro para o token a ser adicionado.
 *  @return void.
 */
void add_token_to_list(ListToken* list, Token* tok);

/** @brief Insere na lista um novo token a partir de um caractere,
           deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Um único caractere que representa o token.
 *  @param p_linha Linha no arquivo onde está o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @return void.
 */
void insert_token_of_char(ListToken* lista, char tok, int p_linha, int p_coluna);

/** @brief Insere na lista um novo token a partir de uma string,
           deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Uma string que representa o token.
 *  @param p_linha Linha no arquivo onde está o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @param tok_class A classe do token, conforme a enum TokenClass.
 *  @return void.
 */
void insert_token_of_string(ListToken* lista, const char* tok, int p_linha, int p_coluna, TokenClass tok_class);

/** @brief Remove the token, specified by its pointer, from the list.
 *  @param list The list from which the token will be removed.
 *  @param tok Pointer to the token to be removed.
 *  @return void.
 */
void remove_token(ListToken* list, Token* tok);

/** @brief Remove all tokens matching the indicated token string, from the list.
 *  @param lst The list from which the tokens will be removed.
 *  @param tok The string representing the tokens to be removed.
 *  @return ...
 */
int remove_tokens_by_value(ListToken* lst, const char* tok);

/** @brief Retorna verdadeiro se c for um simbolo em Verilog.
 *  @param c Um char qualquer.
 *  @return True se c for simbolo, False caso contrario.
 */
int is_single_char_symbol(char c);

/** @brief Imprime na tela os tokens dessa lista, um por linha.
 *  @return Void.
 */
void show_token_list(ListToken* tokens);

/**
 * @brief Avanca o iterador de token para o próximo da lista encadeada respectiva.
 * @param it Um ponteiro para um ponteiro de um Token.
 * @return O endereço do próximo token, que já estará atualizado no iterador.
 *         Este pode ser NULL, que indica o final da lista.
 *         Caso o argumento passado seja NULL ou a derreferenciação deste argumento
 *         também o seja, será retornado NULL.
 */
Token* avanca(Token** it);

/** 
 * @brief Retrocede o iterador de token para o anterior da lista encadeada respectiva.
 * @param it Um ponteiro para um ponteiro de um Token.
 * @return O endereço do token anterior, que já estará atualizado no iterador.
*/
Token* backtrack(Token** it);

/** @brief Verifica se um token é uma palavra reservada em Verilog.
 *  @param tk Um ponteiro para Token.
 *  @return Verdadeiro se o valor do token for palavra reservada em Verilog, falso c. c.
 */
int is_reserverd_word(Token* tk);

/** @brief Verifica se um token possui um nome permitido para identificador.
 *  @param tk Um ponteiro para Token.
 *  @return Verdadeiro se o valor do token for um nome permitido de identificador, falso c.c.
 */
int is_allowed_identifier(Token* tk);

/** @brief Retorna verdadeiro se a string esta contida em algum token da lista.
 *  @param lst Uma lista de Tokens.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se str é o valor de algum Token em lst, falso caso contrário.
 */
int has_item_of_string_value(ListToken* lst, const char* str);

/** @brief Cria uma lista de Tokens que tem significado para o processamento
            sintatico, a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A lista de tokens.
 */
ListToken* tokeniza(FILE *arquivo);

/** @brief Get the TokenClass enum value from its respective token string.
 *  @param s_tok Token string.
 *  @return Respective TokenClass value, in case of a match. Otherwise: _UNKNOWN
 */
TokenClass get_token_class(const char* s_tok);

/** @brief Returns the number of bits represented in a numeric literal token,
 *         according to the Verilog standard.
 *  @param tok Pointer to a Token struct that represents a literal token.
 *  @return The number of bits represented in the literal token.
 *          If the token is not a valid literal, returns 0.
 */
unsigned int get_bit_size_from_literal_token(const Token* tok);

/** @brief Returns the value represented in a numeric literal token,
 *         according to the Verilog standard.
 *  @param tok Pointer to a Token struct that represents a literal token.
 *  @return The value represented in the literal token as an unsigned int.
 *          If the token is not a valid literal, returns 0.
 */
unsigned int get_value_from_literal_token(const Token* tok);

#ifdef __cplusplus
}
#endif

#endif // LEX_H
