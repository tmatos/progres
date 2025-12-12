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
    int arg_offset = 0;
    char* str_verilog_source;

    Sinais* sinais_entradas = NULL;
    Sinais* sinais_saidas = NULL;
    Module* circuit = NULL;

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

    str_verilog_source = argv[1 + arg_offset];

    Evento* initial_task_events = NULL;

    circuit = load_module(str_verilog_source, &initial_task_events);

    if (!circuit) {
        print("Erro com o carregamento do codigo fonte do cicuito.\n");
        exit(1);
    }

    print("Circuito carregado com sucesso.\n");

    // caso onde apenas o circuito, sem sinais de entradas, foi fornecido
    if ( (argc - arg_offset) == 2 ) {
        print("Para haver simulacao, um arquivo de entrada deve ser fornecido.\n");
        exit(0);
    }

    // foi fornecido um path para arquivo de entradas da simulacao (argc > 2)
    sinais_entradas = load_inputs_from_path( argv[2 + arg_offset] );

    if (!sinais_entradas) {
        print("Nao ha entradas para a simulacao do circuito.\n");
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

    sinais_saidas = simula(circuit, sinais_entradas, &initial_task_events);

    if (sinais_saidas) {
        print("Simulacao concluida com saidas geradas.\n");
    }

    save_outputs_to_path(str_wave_out_filepath, sinais_saidas);

    // export of VCD file
    strncat(str_wave_out_filepath, ".vcd", 4);
    FILE* f_wave_out = fopen(str_wave_out_filepath, "w");

    if (!f_wave_out) {
        print("Erro ao tentar abrir arquivo VCD de saida '%s' para gravacao.\n",
                str_wave_out_filepath);
    }
    else {
        save_vcd(circuit, sinais_saidas, f_wave_out);
        fclose(f_wave_out);
    }
    
    print("Arquivo VCD tambem salvo, em '%s'.\n", str_wave_out_filepath);


    delete_event_queue(&initial_task_events);
    free_signal_list(&sinais_entradas);
    free_signal_list(&sinais_saidas);
    free_module(&circuit);

    return 0;
}

Sinais* load_inputs_from_path(const char* path)
{
    Sinais* sinais_entradas = NULL;

    FILE* f_wave_in = fopen(path, "r");

    if (!f_wave_in) {
        print("Impossibilitado de abrir o arquivo de entrada: %s\n", path);
        exit(1);
    }

    print("Abrindo o arquivo de entrada: %s\n", path);

    sinais_entradas = load_input_signals(f_wave_in);

    fclose(f_wave_in);

    return sinais_entradas;
}

void save_outputs_to_path(const char* path, Sinais* outputs)
{
    FILE* f_wave_out = fopen(path, "w");

    if (!f_wave_out) {
        print("Erro ao abrir arquivo de saida '%s' para gravacao.\n", path);
        exit(1);
    }
    
    save_signals(outputs, f_wave_out);
    fclose(f_wave_out);
    
    print("Arquivo de saida salvo em '%s'.\n", path);
}
