/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdlib.h>
#include "erros.h"
#include "mem.h"

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
