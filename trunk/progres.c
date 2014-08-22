/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "inout.h"
#include "verilog.h"
#include "simula.h"

int main(int argc, char* argv[])
{
    FILE *arquivoVerilog = NULL;
    FILE *wavein = NULL;
    FILE *waveout = NULL;

    Sinais* entradas = NULL;
    Sinais* saidas = NULL;

    t_circuito *circuto1 = NULL;

    if(argc < 2) {
        printf("Uso: progres [arquivo verilog] [arquivo de entradas]\n");
        exit(0);
    }

    arquivoVerilog = fopen(argv[1], "r");

    if(!arquivoVerilog) {
        printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
        exit(1);
    }

    printf("Abrindo o arquivo de circuito: %s\n", argv[1]);

    circuto1 = carregaCircuito(arquivoVerilog);

    fclose(arquivoVerilog);

    if(circuto1) {
        printf("Circuito carregado com sucesso.\n");
    }
    else {
        printf("Erro com o fonte.\n");
    }

    // se foi dado um path do arquivo de entrada para simulação, simularemos
    if(argc > 2)
    {
        wavein = fopen(argv[2], "r");

        if(!wavein) {
            printf("Impossibilitado de abrir o arquivo de entrada: %s\n", argv[2]);
            exit(1);
        }

        printf("Abrindo o arquivo de entrada: %s\n", argv[2]);

        entradas = carregaEntradas(wavein);

        fclose(wavein);

        if(entradas)
        {
            char pathArquivoSaida[MAX_FILE_PATH_SIZE] = "";

            if(argc > 3) // se foi passado o argumento do arquivo de saída
            {
                strcpy(pathArquivoSaida, argv[3]);
            }
            else
            {
                strcpy(pathArquivoSaida, argv[2]);
                strcat(pathArquivoSaida, ".out");
            }

            saidas = simula(circuto1, entradas);

            if(saidas)
                printf("Simulacao concluida com saidas geradas.\n");

            free(entradas);

            waveout = fopen(pathArquivoSaida, "w");

            if(!waveout)
            {
                printf("Erro ao tentar abrir arquivo de saida '%s' para gravacao.\n", pathArquivoSaida);
            }
            else
            {
                salvarSinais(saidas, waveout);

                fclose(waveout);

                printf("Arquivo de saida salvo em '%s'.\n", pathArquivoSaida);

                free(saidas);
            }
        }
        else
        {
            printf("Nao ha entradas para a simulacao do circuito.\n");
        }
    }

    return 0;
}
