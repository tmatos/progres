/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "eventos.h"
#include "strutil.h"
#include "lex.h"
#include "inout.h"
#include "erros.h"

SignalArray* show_fatal_error_corrupt_file()
{
    print("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);

    return NULL;
}

SignalArray* load_input_signals(FILE* file)
{
    int index; // for indexing the array in the input signal list
    Token* it; // token iterator
    LogicValue logic_value;
    Time time_length;

    SignalArray* list_input = new_signal_list();

    // token list to keep the names of read input identifiers
    ListToken* list_used_identifiers = new_list_token();

    ListToken* tokens = tokenize(file);

    if (!tokens)
        return NULL;

    it = tokens->first;

    if (!it) {
        print("%s", MSG_ARQUIVO_ENTRADA_VAZIO);

        return NULL;
    }

    index = -1;

    // loop to read the set of all signal in the file
    while (1)
    {
        if ( is_single_char_symbol(it->value[0]) )
            return show_fatal_error_corrupt_file();

        // TODO: check signal name duplication errors

        insert_token_of_string(list_used_identifiers,
                               it->value,
                               it->line,
                               it->column,
                               IDENTIFIER);

        add_new_signal(list_input, it->value);

        index++;

        if ( !avanca(&it) )
            return show_fatal_error_corrupt_file();

        if ( !iguais(it->value, "{") )
            return show_fatal_error_corrupt_file();

        // loop to read one signal
        while (1)
        {
            if ( !avanca(&it) )
                return show_fatal_error_corrupt_file();

            logic_value = VAL_BLANK;

            if ( iguais(it->value, "0") ) {
                logic_value = VAL_0;
            }
            else if ( iguais(it->value, "1") ) {
                logic_value = VAL_1;
            }
            else if ( iguais(it->value, "x") || iguais(it->value, "X") ) {
                logic_value = VAL_X;
            }
            else if ( iguais(it->value, "z") || iguais(it->value, "Z") ) {
                logic_value = VAL_Z;
            }
            else if ( iguais(it->value, "}") ) {
                break;
            }
            else {
                return show_fatal_error_corrupt_file();
            }

            if ( !avanca(&it) )
                return show_fatal_error_corrupt_file();

            if ( !iguais(it->value, "(") )
                return show_fatal_error_corrupt_file();

            if ( !avanca(&it) )
                return show_fatal_error_corrupt_file();

            if ( !is_valid_natural_number(it->value) )
                return show_fatal_error_corrupt_file();
            
            time_length = strtol(it->value, NULL, 10);

            add_new_pulse( (list_input->itens + index),
                           logic_value,
                           time_length );

            if ( !avanca(&it) )
                return show_fatal_error_corrupt_file();

            if ( !iguais(it->value, ")") )
                return show_fatal_error_corrupt_file();

            if ( !avanca(&it) )
                return show_fatal_error_corrupt_file();

            if ( iguais(it->value, ",") ) {
                continue;
            }
            else if ( iguais(it->value, "}") ) {
                break;
            }             
        }

        if ( !avanca(&it) ) {
            print("%s", MSG_ARQUIVO_ENTRADA_LIDO_COMPLETO);

            break;
        }
    }

    delete_lista_token(list_used_identifiers);
    delete_lista_token(tokens);

    return list_input;
}

void save_signals(SignalArray* signals, FILE* file)
{
    int si; // indexador dos sinais na lista de sinais de entrada
    Signal* it_signal = NULL; // Iterador para os sinais num conjunto de entrada ou saida
    Pulse* it = NULL; // Iterador para os pulsos em um Signal

    if ( !signals || !file ) {
        return;
    }

    si = 0;
    it_signal = signals->itens;

    while ( si < signals->count )
    {
        fprintf(file, "%s {", it_signal[si].name);

        it = it_signal[si].pulses; // Aqui, o indice 0 indica qual dos sinais na lista

        while ( it->value != VAL_BLANK )
        {
            // Insere virgula apenas se nao eh a primeira iteracao
            if ( it != it_signal[si].pulses )
                fprintf(file, ", ");

            switch (it->value)
            {
            case VAL_1:
                fprintf(file, "1(%llu)", it->time);
                break;
            case VAL_0:
                fprintf(file, "0(%llu)", it->time);
                break;
            case VAL_X:
                fprintf(file, "x(%llu)", it->time);
                break;
            case VAL_Z:
                fprintf(file, "z(%llu)", it->time);
                break;
            case VAL_H:
                fprintf(file, "1(%llu)", it->time);
                break;
            case VAL_L:
                fprintf(file, "0(%llu)", it->time);
                break;
            case VAL_BLANK:
                break;
            }

            it++;
        }

        fprintf(file, "}\n");

        si++;
    }
}

void save_vcd(Module* module, SignalArray* sinais, FILE* file)
{
    int i;
    char s = '%';

    time_t epoch = time(NULL);
    struct tm* dt = localtime(&epoch);
    fprintf(file, "$date\n");
    fprintf(file, "  %04d/%02d/%02d",
            (dt->tm_year + 1900),
            (dt->tm_mon + 1),
            dt->tm_mday);
    fprintf(file, " - %02d:%02d\n", dt->tm_hour, dt->tm_min);
    fprintf(file, "$end\n");

    fprintf(file, "$version\n");
    fprintf(file, "  progres %s\n", _PROGRES_VERSION);
    fprintf(file, "$end\n");
    fprintf(file, "$comment\n");
    fprintf(file, "  \n"); // ?
    fprintf(file, "$end\n");

    fprintf(file, "$timescale ");
    fprintf(file, "%llu", module->timescale_number);
    fprintf(file, "%s ", get_str_from_timeunit(module->timescale_unit));
    fprintf(file, "$end\n");

    fprintf(file, "$scope module %s ", module->name);
    fprintf(file, "$end\n");

    for ( i=0; i < sinais->count; i++ )
    {
        fprintf(file, "$var %s 1 ", "wire"); // TODO: number of bits
        fprintf(file, "%c ", (s + i));
        fprintf(file, "%s $end\n", sinais->itens[i].name);
    }

    fprintf(file, "$upscope $end\n");
    fprintf(file, "$enddefinitions $end\n");

    fprintf(file, "$dumpvars\n");
    for ( i=0; i < sinais->count; i++ )
    {
        fprintf(file,
                "%c",
                get_char_from_logic_value(sinais->itens[i].pulses->value));
        fprintf(file, "%c\n", (s + i));
    }
    fprintf(file, "$end\n");

    Event* queue = NULL;
    Time t = (Time) 0;
    
    Transition* list_tran = NULL;
    Transition* it = NULL;
    LogicValue v;

    for ( i=0 ; i < sinais->count ; i++ )
    {
        t = (Time) 0;
        Pulse* p = sinais->itens[i].pulses;

        while (p->value != VAL_BLANK)
        {
            insert_event(&queue,
                         t,
                         EVT_NET_TRANSITION,
                         module->list_output_net->itens[i], // !! caution!
                         NULL,
                         p->value);

            t += p->time * module->timescale_number /* * (module->timescale_unit/UN_FS) */;

            p++;
        }

        insert_event(&queue,
                     t,
                     EVT_NET_TRANSITION,
                     module->list_output_net->itens[i], // !! caution!
                     NULL,
                     VAL_X);
    }

    t = (Time) 0;

    while (queue)
    {
        t = queue->instant;

        list_tran = pop_event(&queue);
        it = list_tran;

        while (it)
        {
            it->net->dynamic_value = it->new_value;
            it = it->next;
        }

        // #time
        fprintf(file, "#%llu\n", t);

        for ( i=0 ; i < module->list_output_net->total ; i++ )
        {
            v = module->list_output_net->itens[i]->dynamic_value;
            fprintf(file, "%c", get_char_from_logic_value(v));
            fprintf(file, "%c\n", (char) (s + i));
        }

        // pop_event() didn't free the memory, we do it here
        delete_list_transition(&list_tran);
    }

    // final line
    fprintf(file, "#%llu\n", (Time) (t+1));
}

char get_char_from_logic_value(LogicValue value)
{
    switch (value)
    {
        case VAL_1:
            return '1';
        case VAL_0:
            return '0';
        case VAL_X:
            return 'x';
        case VAL_Z:
            return 'z';
        case VAL_H:
            return '1';
        case VAL_L:
            return '0';
        default:
            return 'x';
    }
}
