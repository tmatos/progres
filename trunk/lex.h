#ifndef LEX_H

#define LEX_H

typedef enum en_grupoToken {tokenSimbolo, tokenPalavra, tokenIdent} GrupoToken;

typedef struct st_token {
    char valor[80];
    GrupoToken tipo;
    struct st_token* seguinte;
} Token;

typedef struct st_listaToken {
    Token* primeiro;
    Token* ultimo;
    int tamanho;
} ListaToken;

ListaToken* novaListaToken();

int insereToken(ListaToken* lista, char tok);

int insereTokenString(ListaToken* lista, char* tok);

int anexa(char* str, char c);

#endif
