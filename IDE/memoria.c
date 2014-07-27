
#include <stdio.h>
#include <stdlib.h>

#include "memoria.h"

void* xmalloc(size_t t) {
    void* p = malloc(t);

    if(!p)
        erroFatalMemoria();

    return p;
}

void* xrealloc(void* m, size_t t) {
    void* p = realloc(m, t);

    if(!p)
        erroFatalMemoria();

    return p;
}

void* xcalloc(size_t n, size_t t) {
    void* p = calloc(n, t);

    if(!p)
        erroFatalMemoria();

    return p;
}

void* exibeMsgErro(char* msg, int linha, int coluna, char* esperado, char *encontrado) {
//    if(linha > 0)
//    {
//        printf("%d:", linha);
//
//        if(coluna > 0)
//            printf("%d:", coluna);
//    }
//
//    if(msg)
//    {
//        printf(" erro: %s.", msg);
//
//        if(esperado)
//        {
//            printf(" Esperava-se '%s', mas foi encontrado '%s'.", esperado, encontrado);
//        }
//    }
//
//    printf("\n");

    return NULL;
}

void erroFatalMemoria() {
//    printf("\nERRO FATAL: Sem memoria para alocar.\n");
//    exit(-1);
}
