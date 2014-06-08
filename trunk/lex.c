
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
        avanca(&it);
    }
    printf("\n");
}

int iguais(char* a, char* b) {
    return !strcmp(a, b);
}

void avanca(Token** t) {
    if(*t)
        *t = (*t)->seguinte;
}

int isPalavra(Token* tk) {
    if(!tk)
        return 0;

    char* valor = (char*) malloc(sizeof(char) * MAX_TOKEN_SIZE);
    strcpy(valor, tk->valor);

    if(iguais(valor, "module")) return 1;
    if(iguais(valor, "parameter")) return 1;
    if(iguais(valor, "localparam")) return 1;
    if(iguais(valor, "input")) return 1;
    if(iguais(valor, "output")) return 1;
    if(iguais(valor, "reg")) return 1;
    if(iguais(valor, "integer")) return 1;
    if(iguais(valor, "wire")) return 1;
    if(iguais(valor, "always")) return 1;
    if(iguais(valor, "assign")) return 1;
    if(iguais(valor, "while")) return 1;
    if(iguais(valor, "begin")) return 1;
    if(iguais(valor, "if")) return 1;
    if(iguais(valor, "else")) return 1;
    if(iguais(valor, "end")) return 1;
    if(iguais(valor, "case")) return 1;
    if(iguais(valor, "endcase")) return 1;
    if(iguais(valor, "endmodule")) return 1;

    return 0;
}

int isIdentificador(Token* tk) {
    int i = 0;
    int simbol = 0;

    if(!tk)
        return 0;

    if(!isalpha(tk->valor[0])) // se nao comeca com letra, nao eh identificador valido
        return 0;

    for(i = 1 ; i < strlen(tk->valor) ; i++) {
        if(!isalnum(tk->valor[i])) {
            simbol = 1;
            break;
        }
    }

    if(simbol) // se contem algo a mais que letras ou numeros, nao eh identificador valido
        return 0;

    if(isPalavra(tk)) // palavra reservada nao pode ser identificador
        return 0;

    return 1;
}
