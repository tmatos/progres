/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>

void exibeMsgErro(char* msg, int linha, int coluna, char* esperado, char *encontrado) {
    if(linha > 0)
    {
        printf("%d:", linha);

        if(coluna > 0)
            printf("%d:", coluna);
    }

    if(msg)
    {
        printf(" erro: %s.", msg);

        if(esperado)
        {
            printf(" Esperava-se '%s', mas foi encontrado '%s'.", esperado, encontrado);
        }
    }

    printf("\n");
}