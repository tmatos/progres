/**
 * @file lex.h
 * @brief Funcoes elementares de processamento lexico dos arquivos fonte.
 */

#ifndef LEX_H

#define LEX_H

#define MAX_TOKEN_SIZE 80

/** @brief .
 */
typedef enum en_keyword {
    kw_module, kw_endmodule // nao sei se isso vai ficar mesmo
} KeywordId;

/** @brief .
 */
typedef enum en_grupoToken {
    tokenSimbolo, tokenPalavra, tokenIdent
} GrupoToken;

/** @brief Tipo basico para o elemento que representa um Token.
 */
typedef struct st_token {
    char valor[MAX_TOKEN_SIZE];
    int linha;
    int coluna;
    GrupoToken tipo;
    struct st_token* seguinte;
} Token;

/** @brief Tipo para uma lista encadeada de Tokens.
 */
typedef struct st_listaToken {
    Token* primeiro;
    Token* ultimo;
    int tamanho;
} ListaToken;

/** @brief Inicializa uma lista vazia, i. e., com zero elementos.
 */
ListaToken* novaListaToken();

/** @brief Insere na lista um novo token a partir de um caractere, deve-se especificar a posicao do mesmo.
 */
int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna);

/** @brief Insere na lista um novo token a partir de uma string, deve-se especificar a posicao do mesmo.
 */
int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna);

/** @brief Faz o apend de um char numa string qualquer.
 */
int anexa(char* str, char c);

/** @brief Retorna verdadeiro se c for um simbolo em verilog.
 *  @param c Um char qualquer.
 *  @return True se c for simbolo, False caso contrario.
 */
int isSimbolo(char c);

/** @brief Imprime na tela os tokens dessa lista, um por linha.
 *  @return Void.
 */
void exibeListaDeToken(ListaToken* tokens);

/** @brief Retorna verdadeiro se duas strings são iguais.
 */
int iguais(char* a, char* b);

/** @brief Avanca o iterador de token para o proximo da lista.
 *  @param t Um ponteiro para um ponteiro de um Token.
 *  @return Void.
 */
void avanca(Token** t);

/** @brief Retorna verdadeiro se um token for uma palavra reservada em Verilog.
 */
int isPalavra(Token* tk);

/** @brief .
 */
int isIdentificador(Token* tk);

/** @brief Retorna true se a string esta contida em algum token da lista.
 */
int identExiste(ListaToken* lst, char* str);

#endif
