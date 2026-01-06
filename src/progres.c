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
    FILE* f_verilog_source;

    Sinais* sinais_entradas = NULL;
    Sinais* sinais_saidas = NULL;
    ListModule* circuit = NULL;

    char str_wave_out_filepath[MAX_FILE_PATH_SIZE] = "";

    global_silent_mode = 0;

    if ( argc < 2 || iguais(argv[1], "-h") ) {
        printf("%s", _HELP_STRING_BRIEF);
        exit(EXIT_SUCCESS);
    }

    if ( iguais(argv[1], "-v") ) {
        printf("Progres Verilog Simulator - version %s\n"
               "(C) 2014-2025 Tiago Matos (tmatos.net)\n",
               _PROGRES_VERSION);
        exit(EXIT_SUCCESS);
    }
    
    if ( iguais(argv[1], "-s") ) {
        global_silent_mode = 1;
        arg_offset++;
    }

    if ( (argc - arg_offset) < 2 ) {
        exit(EXIT_SUCCESS);
    }

    str_verilog_source = argv[1 + arg_offset];
    f_verilog_source = open_or_exit(str_verilog_source, "r");
    print("Abrindo o arquivo de circuito: %s\n", str_verilog_source);
    
    Evento* initial_task_events = NULL;

    circuit = load_circuit(f_verilog_source, &initial_task_events, str_verilog_source);

    if (!circuit) {
        print("Erro com o carregamento do codigo fonte do cicuito.\n");
        exit(EXIT_FAILURE);
    }

    print("Circuito carregado com sucesso.\n");

    // caso onde apenas o circuito, sem sinais de entradas, foi fornecido
    if ( (argc - arg_offset) == 2 ) {
        print("Para haver simulacao, um arquivo de entrada deve ser fornecido.\n");
        exit(EXIT_SUCCESS);
    }

    // foi fornecido um path para arquivo de entradas da simulacao (argc > 2)
    sinais_entradas = load_inputs_from_path( argv[2 + arg_offset] );

    if (!sinais_entradas) {
        print("Nao ha entradas para a simulacao do circuito.\n");
        exit(EXIT_FAILURE);
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

    sinais_saidas = simula(circuit->itens[0], sinais_entradas, &initial_task_events);

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
        save_vcd(circuit->itens[0], sinais_saidas, f_wave_out);
        fclose(f_wave_out);
    }
    
    print("Arquivo VCD tambem salvo, em '%s'.\n", str_wave_out_filepath);

    delete_event_queue(&initial_task_events);
    free_signal_list(&sinais_entradas);
    free_signal_list(&sinais_saidas);
    free_circuit(&circuit);

    return EXIT_SUCCESS;
}

Sinais* load_inputs_from_path(const char* path)
{
    Sinais* sinais_entradas = NULL;
    FILE* f_wave_in = open_or_exit(path, "r");
    print("Abrindo o arquivo de entrada: %s\n", path);
    sinais_entradas = load_input_signals(f_wave_in);
    fclose(f_wave_in);

    return sinais_entradas;
}

void save_outputs_to_path(const char* path, Sinais* outputs)
{
    FILE* f_wave_out = open_or_exit(path, "w");
    save_signals(outputs, f_wave_out);
    fclose(f_wave_out);
    
    print("Arquivo de saida salvo em '%s'.\n", path);
}

FILE* open_or_exit(const char* path, const char* mode)
{
    FILE* f = fopen(path, mode);

    if (!f) {
        print("Erro ao abrir o arquivo: %s", path);
        if ( mode[0] == 'r' )
            print(" para leitura.\n");
        else if ( mode[0] == 'w' )
            print(" para escrita.\n");
        else
            print("\n");
        exit(EXIT_FAILURE);
    }

    return f;
}
