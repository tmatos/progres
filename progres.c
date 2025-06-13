/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "inout.h"
#include "strutil.h"
#include "verilog.h"
#include "simula.h"

int global_silent_mode;

int main(int argc, char* argv[])
{
    int i;
    int arg_offset = 0;

    FILE* f_verilog_source = NULL;
    FILE* f_wave_in = NULL;
    FILE* f_wave_out = NULL;

    Sinais* sinais_entradas = NULL;
    Sinais* sinais_saidas = NULL;
    Module* circuto1 = NULL;

    char str_wave_out_filepath[MAX_FILE_PATH_SIZE] = "";

    global_silent_mode = 0;

    if ( argc < 2 || iguais(argv[1], "-h") ) {
        printf("%s", _HELP_STRING_BRIEF);
        exit(0);
    }

    if ( iguais(argv[1], "-v") ) {
        printf("Progres Verilog Simulator - version %s\n"
               "(C) 2014-2025 Tiago Matos (tmatos.net)\n",
               _PROGRES_VERSION);
        exit(0);
    }
    
    if ( iguais(argv[1], "-s") ) {
        global_silent_mode = 1;
        arg_offset++;
    }

    if ( (argc - arg_offset) < 2 ) {
        exit(0);
    }

    f_verilog_source = fopen(argv[1+arg_offset], "r");

    if (!f_verilog_source) {
        if (!global_silent_mode)
            printf("Impossibilitado de abrir o arquivo: %s\n", argv[1+arg_offset]);

        exit(1);
    }
    
    if (!global_silent_mode)
        printf("Abrindo o arquivo de circuito: %s\n", argv[1+arg_offset]);

    circuto1 = carregaCircuito(f_verilog_source);

    fclose(f_verilog_source);

    if (circuto1) {
        if (!global_silent_mode)
            printf("Circuito carregado com sucesso.\n");
    }
    else {
        if (!global_silent_mode)
            printf("Erro com o carregamento do codigo fonte do cicuito.\n");

        exit(1);
    }

    // caso onde apenas o circuito, sem sinais de entradas, foi fornecido
    if ( (argc - arg_offset) == 2 ) {
        if (!global_silent_mode)
            printf("Para haver simulacao, um arquivo de entrada deve ser fornecido.\n");

        exit(0);
    }

    // foi fornecido um path para arquivo de entradas da simulacao (argc > 2)
    f_wave_in = fopen(argv[2+arg_offset], "r");

    if (!f_wave_in) {
        if (!global_silent_mode)
            printf("Impossibilitado de abrir o arquivo de entrada: %s\n", argv[2+arg_offset]);

        exit(1);
    }

    if (!global_silent_mode)
        printf("Abrindo o arquivo de entrada: %s\n", argv[2+arg_offset]);

    sinais_entradas = carregaEntradas(f_wave_in);

    fclose(f_wave_in);

    if (!sinais_entradas) {
        if (!global_silent_mode)
            printf("Nao ha entradas para a simulacao do circuito.\n");

        exit(1);
    }

    // se foi fornecido o argumento com path para arquivo de saida
    if ( (argc - arg_offset) > 3 ) {
        copy(str_wave_out_filepath, argv[3+arg_offset]);
    }
    else {
        // senao, deriva-se do arquivo de entrada
        copy(str_wave_out_filepath, argv[2+arg_offset]);
        strncat(str_wave_out_filepath, ".out", 4);
    }

    sinais_saidas = simula(circuto1, sinais_entradas);

    if (sinais_saidas) {
        if (!global_silent_mode)
            printf("Simulacao concluida com saidas geradas.\n");
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
        if (!global_silent_mode)
            printf("Erro ao tentar abrir arquivo de saida '%s' para gravacao.\n",
                   str_wave_out_filepath);

        exit(1);
    }
    
    salvarSinais(sinais_saidas, f_wave_out);
    fclose(f_wave_out);
    
    if (!global_silent_mode)
        printf("Arquivo de saida salvo em '%s'.\n", str_wave_out_filepath);

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
