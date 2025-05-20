/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "erros.h"

void* show_error_identifier_duplicate(const char* tok, int lin, int col)
{
    if (global_silent_mode)
        return NULL;

    printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n",
           lin, col, tok);
    
    return NULL;
}

void* show_error_lexical(const char *msg, int lin, int col)
{
    return show_error_msg(msg, lin, col, NULL, NULL);
}

void* show_error_msg(const char* msg, int linha, int coluna, const char* esperado, char *encontrado)
{
    if (global_silent_mode)
        return NULL;

    if(linha > 0) {
        printf("%d:", linha);

        if(coluna > 0)
            printf("%d:", coluna);
    }

    if(!msg) {
        printf("Erro desconhecido.\n");
        return NULL;
    }

    printf(" erro: %s.", msg);

    if(esperado) {
        printf(" Esperava-se: '%s'", esperado);

        if(encontrado)
            printf(", mas foi encontrado: '%s'", encontrado);

        printf(".");
    }

    printf("\n");

    return NULL;
}

void fatal_error_no_memory()
{
    if (!global_silent_mode)
        printf("\nERRO FATAL: Sem memoria para alocar.\n");

    exit(-1);
}
