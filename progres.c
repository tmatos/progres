/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
  #include <unistd.h>
#else
  #include <getopt.h>
#endif

#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "inout.h"
#include "lex.h"
#include "verilog.h"
#include "simula.h"

int global_silent_mode;

int main(int argc, char* argv[])
{
    int i;
    char* s_verilog_source = NULL;
    char* s_wave_in = NULL;
    char* s_wave_out = NULL;
    FILE* f_verilog_source = NULL;
    FILE* f_wave_in = NULL;
    FILE* f_wave_out = NULL;

    Sinais* sinais_entradas = NULL;
    Sinais* sinais_saidas = NULL;
    Module* circuto1 = NULL;

    char str_wave_out_filepath[MAX_FILE_PATH_SIZE] = "";

    global_silent_mode = 0;

    if (argc < 2) {
        printf("%s", _HELP_STRING_BRIEF);
        exit(0);
    }

    int opt;

    while ( (opt = getopt(argc, argv, "vhsc:i:o:")) != -1 )
    {
        switch (opt)
        {
        case 'v':
            printf("Progres Verilog Simulator - version %s\n"
                   "(C) 2014-2025 Tiago Matos (tmatos.net)\n",
                   _PROGRES_VERSION);
            exit(0);
        case 'h':
            printf("%s", _HELP_STRING_BRIEF);
            exit(0);
        case 's':
            global_silent_mode = 1;
            break;
        case 'c':
            s_verilog_source = (char*) malloc( sizeof(char) * (len(optarg) + 1) );
            copy(s_verilog_source, optarg);
            break;
        case 'i':
            s_wave_in = (char*) malloc( sizeof(char) * (len(optarg) + 1) );
            copy(s_wave_in, optarg);
            break;
        case 'o':
            if (len(optarg) > MAX_FILE_PATH_SIZE) {
                printf("error: file name size exceeds maximum lenght of %d chars\n",
                       MAX_FILE_PATH_SIZE);
                exit(1);
            }
            s_wave_out = (char*) malloc( sizeof(char) * (len(optarg) + 1) );
            copy(s_wave_out, optarg);
            break;
        default: // '?'
            printf("%s", _HELP_STRING_BRIEF);
            exit(1);
        }
    }

    if (!s_verilog_source) {
        if (!global_silent_mode) {
            printf("error: a verilog file should be given with the -c option\n");
        }

        exit(1);
    }

    f_verilog_source = fopen(s_verilog_source, "r");

    if (!f_verilog_source) {
        if (!global_silent_mode) {
            printf("error: unnable to open the file: %s\n", s_verilog_source);
        }

        exit(1);
    }
    
    if (!global_silent_mode) {
        printf("Opening verilg file: %s\n", s_verilog_source);
    }

    circuto1 = carregaCircuito(f_verilog_source);

    fclose(f_verilog_source);

    if (circuto1) {
        if (!global_silent_mode) {
            printf("Circuito carregado com sucesso.\n");
        }
    }
    else {
        if (!global_silent_mode) {
            printf("error: loading the circuit source file.\n");
        }

        exit(1);
    }

    // caso onde apenas o circuito, sem sinais de entradas, foi fornecido
    if (!s_wave_in) {
        if (!global_silent_mode) {
            printf("For simulations, an input file should be given with the -i option.\n");
        }

        exit(0);
    }

    f_wave_in = fopen(s_wave_in, "r");

    if (!f_wave_in) {
        if (!global_silent_mode) {
            printf("error: unnable to open input file: %s\n", s_wave_in);
        }

        exit(1);
    }

    if (!global_silent_mode) {
        printf("Opening input file: %s\n", s_wave_in);
    }

    sinais_entradas = carregaEntradas(f_wave_in);

    fclose(f_wave_in);

    if (!sinais_entradas) {
        if (!global_silent_mode) {
            printf("error: Nao ha entradas para a simulacao do circuito.\n");
        }

        exit(1);
    }

    // se foi fornecido o argumento com path para arquivo de saida
    if (s_wave_out) {
        copy(str_wave_out_filepath, s_wave_out);
    }
    else {
        // senao, deriva-se do arquivo de entrada
        copy(str_wave_out_filepath, s_wave_in);
        strcat(str_wave_out_filepath, ".out");
    }

    sinais_saidas = simula(circuto1, sinais_entradas);

    if (sinais_saidas) {
        if (!global_silent_mode) {
            printf("Simulacao concluida com saidas geradas.\n");
        }
    }

    // free mem (inputs)
    if (sinais_entradas) {
        for ( i=0 ; i < sinais_entradas->quantidade ; i++ )
            free(sinais_entradas->lista[i].pulsos);
        
        if (sinais_entradas->lista)
            free(sinais_entradas->lista);

        free(sinais_entradas);
    }

    f_wave_out = fopen(str_wave_out_filepath, "w");

    if (!f_wave_out) {
        if (!global_silent_mode) {
            printf("Erro ao tentar abrir arquivo de saida '%s' para gravacao.\n",
                   str_wave_out_filepath);
        }

        exit(1);
    }
    
    salvarSinais(sinais_saidas, f_wave_out);
    fclose(f_wave_out);
    
    if (!global_silent_mode) {
        printf("Arquivo de saida salvo em '%s'.\n", str_wave_out_filepath);
    }

    if (s_verilog_source)
        free(s_verilog_source);

    if (s_wave_in)
        free(s_wave_in);

    if (s_wave_out)
        free(s_wave_out);

    // free mem (outputs)
    if (sinais_saidas) {
        for ( i=0 ; i < sinais_saidas->quantidade ; i++ )
            free(sinais_saidas->lista[i].pulsos);
        
        if (sinais_saidas->lista)
            free(sinais_saidas->lista);

        free(sinais_saidas);
    }

    // free mem (circuit)
    if (circuto1) {
        if (circuto1->listaFiosEntrada->itens)
            free(circuto1->listaFiosEntrada->itens);
        free(circuto1->listaFiosEntrada);

        if (circuto1->listaFiosSaida->itens)
            free(circuto1->listaFiosSaida->itens);
        free(circuto1->listaFiosSaida);

        if (circuto1->listaPortas->itens)
            free(circuto1->listaPortas->itens);
        free(circuto1->listaPortas);

        if (circuto1->listaWires->itens)
            free(circuto1->listaWires->itens);
        free(circuto1->listaWires);

        if (circuto1->listaParam.itens)
            free(circuto1->listaParam.itens);

        if (circuto1->listaReg.itens)
            free(circuto1->listaReg.itens);
        
        free(circuto1);
    }

    return 0;
}
