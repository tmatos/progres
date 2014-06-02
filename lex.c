#include <stdlib.h>
#include <string.h>
#include "lex.h"

ListaToken* novaListaToken()
{
    ListaToken* l = (ListaToken*) malloc(sizeof(ListaToken));
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;

    return l;
}

int insereToken(ListaToken* lista, char tok)
{
    char s[2] = {tok, '\0'};

    return insereTokenString(lista, s);
}

int insereTokenString(ListaToken* lista, char* tok)
{
    Token novo;
    strcpy(novo.valor, tok);
    novo.seguinte = NULL;

    // TODO: Preencher o tipo
    // TODO: Checagens...

    if(lista->tamanho == 0)
    {
        lista->primeiro = &novo;
        lista->ultimo = &novo;
    }
    else
    {
        lista->ultimo->seguinte = &novo;
        lista->ultimo = &novo;
    }

    lista->tamanho++;

    return 1;
}

// Faz o apend de um char numa string qualquer
int anexa(char* str, char c)
{
    char tmp[2] = {c, '\0'};

    strcat(str, tmp);

    // TODO: Checagens...

    return 1;
}
