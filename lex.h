#ifndef LEX_H

#define LEX_H

typedef enum en_grupoToken {tokenSimbolo, tokenPalavra, tokenIdent} GrupoToken;

// Tipo basico para o elemento que representa um Token
typedef struct st_token {
    char valor[80];
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

// Insere na lista um novo token a partir de um caractere
int insereToken(ListaToken* lista, char tok);

// Insere na lista um novo token a partir de uma string
int insereTokenString(ListaToken* lista, char* tok);

// Faz o apend de um char numa string qualquer
int anexa(char* str, char c);

#endif
