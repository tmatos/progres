
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

ListaToken* novaListaToken() {
    ListaToken* l = (ListaToken*) malloc(sizeof(ListaToken));
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;

    return l;
}

int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna) {
    char s[2] = {tok, '\0'};

    return insereTokenString(lista, s, p_linha, p_coluna);
}

int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna) {
    Token* novo = (Token*) malloc(sizeof(Token));
    strcpy(novo->valor, tok);
    novo->linha = p_linha;
    novo->coluna = p_coluna;
    novo->seguinte = NULL;

    // TODO: Preencher o tipo
    // TODO: Checagens...

    if(lista->tamanho == 0) {
        lista->primeiro = novo;
        lista->ultimo = novo;
    }
    else if(lista->tamanho == 1) {
        lista->primeiro->seguinte = novo;
        lista->ultimo = novo;
    }
    else {
        lista->ultimo->seguinte = novo;
        lista->ultimo = novo;
    }

    lista->tamanho++;

    return 1;
}

// Faz o apend de um char numa string qualquer
int anexa(char* str, char c) {
    char tmp[2] = {c, '\0'};

    strcat(str, tmp);

    // TODO: Checagens...

    return 1;
}

int isSimbolo(char c) {
    return (c == '(' || c == ')' || c == ',' || c == ';');
}

void exibeListaDeToken(ListaToken* tokens) {
    printf(" -=-=- LISTA DE TOKENS CAPTURADOS -=-=-\n\n");
    Token* it = tokens->primeiro;
    while(it) {
        printf("%s\n", it->valor);
        it = it->seguinte;
    }
    printf("\n");
}

int iguais(char* a, char* b) {
    return !strcmp(a, b);
}
