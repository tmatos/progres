/**
 * @file lex.h
 * @brief Funcoes elementares de processamento lexico dos arquivos fonte.
 */

#ifndef LEX_H

#define LEX_H

#include <stdio.h>

#if defined(_WIN32) || defined(__CYGWIN__) || defined(__vxworks)
    #define MAX_PATH_LENGTH 255
#else
    #define MAX_PATH_LENGTH 1023
#endif

#define MAX_TOKEN_SIZE 128 /// Qtde maxima de caracteres permitidos em um Token
#define MAX_DIGITOS_NUM 13 /// Qtde maxima de digitos num numero inteiro a ser reconhecido

/** @brief .
 */
typedef enum en_keyword {
    kw_module,
    kw_endmodule
    // nao sei se isso vai ficar mesmo
} KeywordId;

/** @brief Enumeracao para a classe do token detectado.
 */
typedef enum en_token_class {
    KW_ALWAYS,
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

    NUM_BASE_BINARY,
    NUM_BASE_DECIMAL,
    NUM_BASE_OCTAL,
    NUM_BASE_HEX,

    STRING,
    IDENTIFIER,
    _UNKNOWN
} TokenClass;

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
typedef struct st_listaToken {
    Token* primeiro;
    Token* ultimo;
    int tamanho;
    char file[MAX_PATH_LENGTH];
} ListaToken;

/** @brief Inicializa uma lista vazia, isto é, com zero elementos.
 */
ListaToken* novaListaToken();

/** @brief Delete the list and its contents from memory.
 */
void delete_lista_token(ListaToken* list);

/** @brief Insere na lista um novo token a partir de um caractere,
            deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Um token de apenas um caractere.
 *  @param p_linha Linha no arquivo onde está o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @return Verdadeiro caso sucesso, falso caso falhe.
 */
int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna);

/** @brief Insere na lista um novo token a partir de uma string,
            deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Uma string contendo o token.
 *  @param p_linha Linha no arquivo onde está o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @return Verdadeiro caso sucesso, falso caso falhe.
 */
int insereTokenString(ListaToken* lista, const char* tok, int p_linha, int p_coluna);

/** @brief Remove the token, specified by its pointer, from the list. 
 */
void remove_token(ListaToken* list, Token* tok);

/** @brief Remove todos os tokens com o valor indicado da lista.
 */
int removeTokensPorValor(ListaToken* lst, const char* tok);

/** @brief Retorna verdadeiro se c for um simbolo em Verilog.
 *  @param c Um char qualquer.
 *  @return True se c for simbolo, False caso contrario.
 */
int isSimbolo(char c);

/** @brief Imprime na tela os tokens dessa lista, um por linha.
 *  @return Void.
 */
void exibeListaDeToken(ListaToken* tokens);

/** @brief Avanca o iterador de token para o próximo da lista encadeada respectiva.
 *  @param it Um ponteiro para um ponteiro de um Token.
 *  @return O endereço do próximo token, que já estará atualizado no iterador.
 *          Este pode ser NULL, que indica o final da lista.
 *          Caso o argumento passado seja NULL ou a derreferenciação deste argumento
 *          também o seja, será retornado NULL.
 */
Token* avanca(Token** it);

/** @brief Verifica se um token é uma palavra reservada em Verilog.
 *  @param tk Um objeto Token.
 *  @return Verdadeiro se o valor do token for palavra reservada em Verilog, falso c. c.
 */
int isPalavra(Token* tk);

/** @brief Verifica se um token é um nome permitido de identificador.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se o valor do token for um nome permitido de identificador, falso c.c.
 */
int isIdentificador(Token* tk);

/** @brief Retorna verdadeiro se a string esta contida em algum token da lista.
 *  @param lst Uma lista de Tokens.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se str é o valor de algum Token em lst, falso caso contrário.
 */
int identExiste(ListaToken* lst, const char* str);

/** @brief Cria uma lista de Tokens que tem significado para o processamento
            sintatico, a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A lista de tokens.
 */
ListaToken* tokeniza(FILE *arquivo);

/** @brief Get the TokenClass enum value from its respective token string.
 *  @param s_tok Token string.
 *  @return Respective TokenClass value, in case of a match. Otherwise: _UNKNOWN
 */
TokenClass get_token_class(const char* s_tok);

#endif // LEX_H
