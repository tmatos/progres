/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>

#include "erros.h"


void* show_error_lexical(char *msg, int lin, int col)
{
    return show_error_msg(msg, lin, col, NULL, NULL);
}

void* show_error_msg(char* msg, int linha, int coluna, char* esperado, char *encontrado)
{
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

void fatal_error_no_memory() {
    printf("\nERRO FATAL: Sem memoria para alocar.\n");
    exit(-1);
}
