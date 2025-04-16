
#ifndef MEMORIA_H

#define MEMORIA_H

#include <stdio.h>
#include <stdlib.h>

void* xmalloc(size_t t);

void* xrealloc(void* m, size_t t);

void* xcalloc(size_t n, size_t t);

void* show_error_msg(char* msg, int linha, int coluna, char* esperado, char *encontrado);

void fatal_error_no_memory();

#endif // MEMORIA_H
