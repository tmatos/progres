#ifndef LEX_H

#define LEX_H

#define MAX_TOKEN_SIZE 80

typedef enum en_grupoToken {tokenSimbolo, tokenPalavra, tokenIdent} GrupoToken;

// Tipo basico para o elemento que representa um Token
typedef struct st_token {
    char valor[MAX_TOKEN_SIZE];
    int linha;
    int coluna;
    GrupoToken tipo;
    struct st_token* seguinte;
} Token;

// Tipo para uma lista encadeada de Tokens
typedef struct st_listaToken {
    Token* primeiro;
    Token* ultimo;
    int tamanho;
} ListaToken;

// Inicializa uma lista vazia, i. e., com zero elementos
ListaToken* novaListaToken();

// Insere na lista um novo token a partir de um caractere, deve-se especificar a posicao do mesmo
int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna);

// Insere na lista um novo token a partir de uma string, deve-se especificar a posicao do mesmo
int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna);

// Faz o apend de um char numa string qualquer
int anexa(char* str, char c);

// Retorna verdadeiro se c for um simbolo em verilog
int isSimbolo(char c);

// Imprime na tela os tokens dessa lista, um por linha
void exibeListaDeToken(ListaToken* tokens);

// Retorna verdadeiro se duas strings são iguais
int iguais(char* a, char* b);

// Avanca o iterador de token para o proximo da lista
void avanca(Token** t);

// Retorna verdadeiro se um token for uma palavra reservada em Verilog
int isPalavra(Token* tk);

//
int isIdentificador(Token* tk);

#endif
