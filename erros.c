/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "erros.h"

void show_error_identifier_duplicate(const char* tok, int lin, int col)
{
    if (!global_silent_mode) {
        printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n",
               lin, col, tok);
    }
}

void show_error_lexical(const char* msg, int lin, int col)
{
    return show_error_msg(msg, lin, col, NULL, NULL);
}

void show_error_size_exceeded(const char* msg, int lin, int col, const char* tok, int max)
{
    if (!global_silent_mode) {
        printf("%d:%d: erro: '%s' relativo a '%s'. Maximo permitido: %d.\n",
               lin, col, msg, tok, max);
    }
}

void show_error_msg(const char* msg, int lin, int col, const char* expected, const char* found)
{
    if (!global_silent_mode) {
        if(lin > 0) {
            printf("%d:", lin);

            if(col > 0)
                printf("%d:", col);
        }

        if(!msg) {
            printf("Erro desconhecido.\n");
            return;
        }

        printf(" erro: %s.", msg);

        if(expected) {
            printf(" Esperava-se: '%s'", expected);

            if(found)
                printf(", mas foi encontrado: '%s'", found);

            printf(".");
        }

        printf("\n");
    }
}

void fatal_error_no_memory()
{
    if (!global_silent_mode)
        printf("\nERRO FATAL: Sem memoria para alocar.\n");

    exit(-1);
}
