/**
 * @file lex.h
 * @brief Funcoes elementares de processamento lexico dos arquivos fonte.
 */

#ifndef LEX_H

#define LEX_H

#define MAX_TOKEN_SIZE 80 // Qtde m�xima de caracteres permitidos em um Token
#define MAX_DIGITOS_NUM 4 // Qtde m�xima de digitos num n�mero inteiro a ser reconhecido

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

/** @brief Tipo basico para o elemento que representa um token.
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

/** @brief Insere na lista um novo token a partir de um caractere,
            deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Um token de apenas um caractere.
 *  @param p_linha Linha no arquivo onde est� o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @return Verdadeiro caso sucesso, falso caso falhe.
 */
int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna);

/** @brief Insere na lista um novo token a partir de uma string,
            deve-se especificar a posicao do mesmo no arquivo.
 *  @param lista Onde sera inserido o token.
 *  @param tok Uma string contendo o token.
 *  @param p_linha Linha no arquivo onde est� o token.
 *  @param p_coluna Coluna no arquivo onde inicia-se o token.
 *  @return Verdadeiro caso sucesso, falso caso falhe.
 */
int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna);

/** @brief Remove todos os tokens com o valor indicado da lista.
 */
int removeTokensPorValor(ListaToken* lst, char* tok);

/** @brief Faz o apend de um char numa string qualquer.
 */
int anexa(char* str, char c);

/** @brief Retorna verdadeiro se c for um simbolo em Verilog.
 *  @param c Um char qualquer.
 *  @return True se c for simbolo, False caso contrario.
 */
int isSimbolo(char c);

/** @brief Imprime na tela os tokens dessa lista, um por linha.
 *  @return Void.
 */
void exibeListaDeToken(ListaToken* tokens);

/** @brief Retorna verdadeiro se duas strings s�o iguais.
 */
int iguais(char* a, char* b);

/** @brief Avanca o iterador de token para o pr�ximo da lista.
 *  @param t Um ponteiro para um ponteiro de um Token.
 *  @return Void.
 */
void avanca(Token** t);

/** @brief Verifica se um token � uma palavra reservada em Verilog.
 *  @param tk Um objeto Token.
 *  @return Verdadeiro se o valor do token for palavra reservada em Verilog, falso c. c.
 */
int isPalavra(Token* tk);

/** @brief Verifica se um token � um nome permitido de identificador.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se o valor do token for um nome permitido de identificador, falso c.c.
 */
int isIdentificador(Token* tk);

/** @brief Retorna verdadeiro se a string esta contida em algum token da lista.
 *  @param lst Uma lista de Tokens.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se str � o valor de algum Token em lst, falso caso contr�rio.
 */
int identExiste(ListaToken* lst, char* str);

/** @brief Cria uma lista de Tokens que tem significado para o processamento
            sintatico, a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A lista de tokens.
 */
ListaToken* tokeniza(FILE *arquivo);

/** @brief Verifica se uma string cont�m apenas d�gitos (0, 1, 2, ..., 9).
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se h� apenas d�gitos, falso na ocorr�ncia de qualquer outro tipo de caractere.
 */
int apenasDigitos(char* str);

/** @brief Verifica se uma string cont�m um n�mero que pode ser convertido.
            Mais especeificamente, se � um natural menor que 10000.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se pode ser convertido.
 */
int isNumNaturalValido(char* str);

/** @brief Wrapper to strlen()
 *  @param str A null terminated C string
 *  @return Lenght of str
 */
unsigned long len(const char* str);

#endif // LEX_H
