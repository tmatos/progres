/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

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
    FILE* f_verilog_source = NULL;
    FILE* f_wave_in = NULL;
    FILE* f_wave_out = NULL;

    Sinais* sinais_entradas = NULL;
    Sinais* sinais_saidas = NULL;
    Module* circuto1 = NULL;

    char str_wave_out_filepath[MAX_FILE_PATH_SIZE] = "";

    if(argc < 2) {
        printf("Uso: progres [arquivo verilog] [arquivo de entradas]\n");
        exit(0);
    }

    f_verilog_source = fopen(argv[1], "r");

    if(!f_verilog_source) {
        printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
        exit(1);
    }

    printf("Abrindo o arquivo de circuito: %s\n", argv[1]);

    circuto1 = carregaCircuito(f_verilog_source);

    fclose(f_verilog_source);

    if(circuto1) {
        printf("Circuito carregado com sucesso.\n");
    }
    else {
        printf("Erro com o carregamento do codigo fonte do cicuito.\n");
        exit(1);
    }

    // caso onde apenas o circuito, sem sinais de entradas, foi fornecido
    if(argc == 2) {
        printf("Para haver simulacao, um arquivo de entrada deve ser fornecido.\n");
        exit(0);
    }

    // foi fornecido um path para arquivo de entradas da simulacao (argc > 2)
    f_wave_in = fopen(argv[2], "r");

    if(!f_wave_in) {
        printf("Impossibilitado de abrir o arquivo de entrada: %s\n", argv[2]);
        exit(1);
    }

    printf("Abrindo o arquivo de entrada: %s\n", argv[2]);

    sinais_entradas = carregaEntradas(f_wave_in);

    fclose(f_wave_in);

    if(!sinais_entradas) {
        printf("Nao ha entradas para a simulacao do circuito.\n");
        exit(1);
    }

    // se foi fornecido o argumento com path para arquivo de saida
    if(argc > 3) {
        strcpy(str_wave_out_filepath, argv[3]);
    }
    else {
        // senao, deriva-se do arquivo de entrada
        strcpy(str_wave_out_filepath, argv[2]);
        strcat(str_wave_out_filepath, ".out");
    }

    sinais_saidas = simula(circuto1, sinais_entradas);

    if(sinais_saidas) {
        printf("Simulacao concluida com saidas geradas.\n");
    }

    if(sinais_entradas->lista)
        free(sinais_entradas->lista);
    free(sinais_entradas);

    f_wave_out = fopen(str_wave_out_filepath, "w");

    if(!f_wave_out) {
        printf("Erro ao tentar abrir arquivo de saida '%s' para gravacao.\n",
               str_wave_out_filepath);
        exit(1);
    }
    
    salvarSinais(sinais_saidas, f_wave_out);
    fclose(f_wave_out);
    printf("Arquivo de saida salvo em '%s'.\n", str_wave_out_filepath);

    if(sinais_saidas->lista)
        free(sinais_saidas->lista);
    free(sinais_saidas);

    return 0;
}
