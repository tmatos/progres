/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdlib.h>
#include "erros.h"
#include "mem.h"

void* xmalloc(size_t t) {
    void* p = malloc(t);

    if(!p)
        fatal_error_no_memory();

    return p;
}

void* xrealloc(void* m, size_t t) {
    void* p = realloc(m, t);

    if(!p)
        fatal_error_no_memory();

    return p;
}

void* xcalloc(size_t n, size_t t) {
    void* p = calloc(n, t);

    if(!p)
        fatal_error_no_memory();

    return p;
}
