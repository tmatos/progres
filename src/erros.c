/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "strutil.h"
#include "erros.h"

void show_error_identifier_duplicate(const char* tok, int lin, int col)
{
    print("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n",
          lin, col, tok);
}

void show_error_lexical(const char* msg, int lin, int col)
{
    return show_error_msg(msg, lin, col, NULL, NULL);
}

void show_error_size_exceeded(const char* msg, int lin, int col, const char* tok, int max)
{
    print("%d:%d: erro: '%s' relativo a '%s'. Maximo permitido: %d.\n",
          lin, col, msg, tok, max);
}

void show_error_msg(const char* msg, int lin, int col, const char* expected, const char* found)
{
    if (lin > 0) {
        print("%d:", lin);

        if(col > 0)
            print("%d:", col);
    }

    if (!msg) {
        print("Erro desconhecido.\n");
        return;
    }

    printf(" erro: %s.", msg);

    if (expected) {
        print(" Esperava-se: '%s'", expected);

        if(found)
            print(", mas foi encontrado: '%s'", found);

        print(".");
    }

    print("\n");
}

void fatal_error_no_memory()
{
    print("\nERRO FATAL: Sem memoria para alocar.\n");

    exit(-1);
}
