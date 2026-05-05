/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "simula.h"
#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "strutil.h"
#include "lex.h"
#include "eventos.h"
#include "erros.h"

int validate_input_signals(Module* module, SignalArray* signals)
{
    int i;
    int j;
    int validos = 0;

    // Validacao da correspencia das signals entre os arquivos '.v' e '.in'
    for ( i=0 ; i < module->list_input_net->total ; i++ )
    {
        for ( j=0 ; j < signals->count ; j++ )
        {
            if ( iguais(module->list_input_net->itens[i]->name,
                        signals->itens[j].name) ) {
                module->list_input_net->itens[i]->input_signal = &(signals->itens[j]);
                validos++;
                break;
            }
        }
    }

    // print matches msg
    print("----------\n"
          "Entradas: \n"
          "   .v = %d\n"
          "  .in = %d\n"
          "match = %d\n"
          "----------\n",
          module->list_input_net->total,
          signals->count,
          validos);

    return validos;
}

SignalArray* simula(
    Module* module,
    SignalArray* input_stimulus,
    Event** initial_events,
    FILE** f_dump)
{
    int i;
    int validos; // conta correspencias de entradas entre arquivos '.v' e '.in'
    Time t;

    Transition* list_tr = NULL;
    Transition* tr = NULL; // iterator

    Event* queue = NULL;
    Pulse* p = NULL;

    ListComponent* list_changed_gates = NULL;
    Component* gate = NULL;

    LogicValue result;
    LogicValue valor_xor_in_a;
    LogicValue valor_xor_in_b;
    LogicValue valor_xnor_in_a;
    LogicValue valor_xnor_in_b;
    LogicValue valor_control;
    LogicValue valor_data;

    SignalArray* saidas = new_signal_list();

    if (!module || !input_stimulus) {
        return NULL;
    }

    validos = validate_input_signals(module, input_stimulus);

    if (validos < module->list_input_net->total) {
        print("AVISO: O arquivo de entradas tem menos "
              "sinais de entrada que o circuito.\n");

        return NULL;
    }

    // initial event ocurrences are copied into the event queue
    Event* evt_it = initial_events ? *initial_events : NULL;
    while (evt_it)
    {
        Transition* tran_it = evt_it->list_transition;
        while (tran_it)
        {
            if ( tran_it->task_type == IS_NOT_A_TASK ) {
                insert_event(&queue,
                             evt_it->instant,
                             EVT_NET_TRANSITION,
                             tran_it->net,
                             tran_it->reg,
                             tran_it->new_value);
            }
            else {
                insert_task_event(&queue,
                                 evt_it->instant,
                                 tran_it->task_type,
                                 tran_it->task_args);
            }

            tran_it = tran_it->next;
        }
        
        evt_it = evt_it->next;
    }

    // Inicializacao da fila de eventos com os valores das entradas
    for ( i=0 ; i < module->list_input_net->total ; i++ )
    {
        t = 0;

        p = module->list_input_net->itens[i]->input_signal->pulses;
        while (p->value != VAL_BLANK)
        {
            insert_event(&queue,
                         t,
                         EVT_NET_TRANSITION,
                         module->list_input_net->itens[i],
                         NULL,
                         p->value);

            t = t + p->time * module->timescale_number /* * (circuto->timescale_unit/UN_FS) */;
            p++;
        }

        insert_event(&queue,
                     t,
                     EVT_NET_TRANSITION,
                     module->list_input_net->itens[i],
                     NULL,
                     VAL_X); // este sinal fica ate infitito
    }

    // ATENCAO: Sabemos que todos os componentes sao
    // inicializados com o dynamic_value em X (exceto constantes literais)

    // A partir daqui, ocorre a simulacao propriamente dita:
    t = 0;

    while (queue)
    {
        list_changed_gates = new_list_component();

        t = queue->instant;

        list_tr = pop_event(&queue);
        tr = list_tr;

        // nao sendo uma system task: atualiza valores de fios e faz
        // uma lista das portas alteradas pelas transicoes em list_tr;
        // senao, caso seja task: executa a mesma.
        while (tr)
        {
            // apenas tambem se houver mudanca de valor no fio
            if ( (tr->task_type == IS_NOT_A_TASK) &&
                 (tr->net->dynamic_value != tr->new_value) )
            {
                for ( i=0 ; i < tr->net->list_output->total ; i++ )
                {
                    if ( !has_component_by_pointer(list_changed_gates,
                                                   tr->net->list_output->itens[i]) ) {
                        insert_component(list_changed_gates,
                                         tr->net->list_output->itens[i]);
                    }
                }

                if (tr->net->attributes.role == ROLE_OUTPUT) {
                    if ( !(tr->net->output_signal) ) {
                        tr->net->output_signal = new_signal( tr->net->name );
                    }
                    add_new_pulse(tr->net->output_signal,
                                  tr->net->dynamic_value,
                                  t - tr->net->output_signal->total_time);
                }

                tr->net->dynamic_value = tr->new_value;
            }

            switch (tr->task_type)
            {
            case TASK_DISPLAY:
                // TODO: make it work like printf, with format specifiers...
                for ( unsigned int j=0 ; j < tr->task_args.count ; j++ )
                {
                    if ( tr->task_args.types[j] == ARG_STRING_LITERAL ) {
                        print("%s ", tr->task_args.itens[j].string_literal);
                    }
                }
                print("\n");
                break;
            case TASK_WRITE:
                for ( unsigned int j=0 ; j < tr->task_args.count ; j++ )
                {
                    if ( tr->task_args.types[j] == ARG_STRING_LITERAL ) {
                        print("%s", tr->task_args.itens[j].string_literal);
                    }
                }
                break;
            case TASK_DUMPFILE:
                set_dumpfile(f_dump, tr->task_args.itens[0].string_literal);
                // TODO: set more flags for dumpfile
                break;
            case TASK_FINISH:
                while (queue)
                {
                    Transition* lt = pop_event(&queue);
                    if (lt)
                        delete_list_transition(&lt);
                }
                tr = NULL; // exit outer while loop
                continue;
                break;
            case TASK_STOP:
                inspection_console(module, t);
                break;
            default:
                break;
            }
            
            free(tr->task_args.itens);
            free(tr->task_args.types);

            tr = tr->next;
        }

        // pop_event() nao liberou mem da lista de transicoes, fazemos isso aqui
        delete_list_transition(&list_tr);

        for ( i=0 ; i < list_changed_gates->total ; i++ )
        {
            gate = list_changed_gates->itens[i];

            // be prepared, in case of 3 stage logic gates
            if (gate->list_input->total == 2) {
                valor_data = gate->list_input->itens[0]->dynamic_value;
                valor_control =gate->list_input->itens[1]->dynamic_value;
            }

            switch (gate->attributes.role)
            {
            case ROLE_NOT:
                result = compute_not_gate(gate->list_input->itens[0]->dynamic_value);
                break;
            case ROLE_BUF:
                result = compute_buf_gate(gate->list_input->itens[0]->dynamic_value);
                break;
            case ROLE_AND:
                result = compute_and_gate(gate->list_input);
                break;
            case ROLE_OR:
                result = compute_or_gate(gate->list_input);
                break;
            case ROLE_XOR:
                valor_xor_in_a = gate->list_input->itens[0]->dynamic_value;
                valor_xor_in_b = gate->list_input->itens[1]->dynamic_value;
                result = compute_xor_gate(valor_xor_in_a, valor_xor_in_b);
                break;
            case ROLE_NAND:
                result = compute_nand_gate(gate->list_input);
                break;
            case ROLE_NOR:
                result = compute_nor_gate(gate->list_input);
                break;
            case ROLE_XNOR:
                valor_xnor_in_a = gate->list_input->itens[0]->dynamic_value;
                valor_xnor_in_b = gate->list_input->itens[1]->dynamic_value;
                result = compute_xnor_gate(valor_xnor_in_a, valor_xnor_in_b);
                break;
            case ROLE_BUF_IF0:
                result = compute_buf_if0_gate(valor_data, valor_control);
                break;
            case ROLE_BUF_IF1:
                result = compute_buf_if1_gate(valor_data, valor_control);
                break;
            case ROLE_NOT_IF0:
                result = compute_not_if0_gate(valor_data, valor_control);
                break;
            case ROLE_NOT_IF1:
                result = compute_not_if1_gate(valor_data, valor_control);
                break;
            case ROLE_ASSIGN:
                // TODO: implement expression evaluation and specific data structures
                result = gate->list_input->itens[0]->dynamic_value;
                break;
            default:
                break;
            }

            create_events_from_outputs(&queue, t, module->timescale_number, gate, result);
        }

        // free mem
        if (list_changed_gates->total != 0)
            free(list_changed_gates->itens);
        free(list_changed_gates);
        list_changed_gates = NULL;
    }

    // move as saidas da simulacao do circuito para o retorno da funcao
    for ( i=0 ; i < module->list_output_net->total ; i++ )
    {
        Signal* s = module->list_output_net->itens[i]->output_signal;

        if (s) {
            insert_signal(saidas, s);
            free_signal(&s);
            continue;
        }

        // cria sinal de saida constante com o valor dinamico do componente
        LogicValue v = module->list_output_net->itens[i]->dynamic_value;
        Signal* s_temp = new_signal(module->list_output_net->itens[i]->name);
        add_new_pulse(s_temp, v, t); // dynamic value for all the simulat. time
        insert_signal(saidas, s_temp);
        free_signal(&s_temp);
    }

    return saidas;
}

// IEEE Std 1364-2005, Table 7-4
LogicValue compute_not_gate(LogicValue input)
{
    if (input == VAL_0)
        return VAL_1;
    
    if (input == VAL_1)
        return VAL_0;
    
    return VAL_X;
}

// IEEE Std 1364-2005, Table 7-4
LogicValue compute_buf_gate(LogicValue input)
{
    if (input == VAL_Z)
        return VAL_X;
    
    return input;
}

// Std 1364-2005, Table 7-3
LogicValue compute_xor_gate(LogicValue a, LogicValue b)
{
    if (a == VAL_X || b == VAL_X)
        return VAL_X;

    if (a == VAL_Z || b == VAL_Z)
        return VAL_X;

    if ( (a == VAL_1 && b == VAL_0) || (a == VAL_0 && b == VAL_1) )
        return VAL_1;

    return VAL_0;
}

// Std 1364-2005, Table 7-3
LogicValue compute_xnor_gate(LogicValue a, LogicValue b)
{
    if (a == VAL_X || b == VAL_X)
        return VAL_X;

    if (a == VAL_Z || b == VAL_Z)
        return VAL_X;

    if ( (a == VAL_1 && b == VAL_1) || (a == VAL_0 && b == VAL_0) )
        return VAL_1;

    return VAL_0;
}

// Std 1364-2005, Table 7-3
LogicValue compute_or_gate(ListComponent* inputs)
{
    LogicValue out = VAL_0;
    
    // compute the logic value of the 'or' operation over all its inputs
    for ( int i=0 ; i < inputs->total ; i++ )
    {
        LogicValue input_at_i = inputs->itens[i]->dynamic_value;

        if (input_at_i == VAL_1) {
            out = VAL_1;
            break;
        }   

        if (input_at_i == VAL_X || input_at_i == VAL_Z) {
            out = VAL_X;
        }
    }

    return out;
}

// Std 1364-2005, Table 7-3
LogicValue compute_and_gate(ListComponent* inputs)
{
    LogicValue out = VAL_1;

    // compute the logic value of the 'and' operation over all its inputs
    for ( int i=0 ; i < inputs->total ; i++ )
    {
        LogicValue input_at_i = inputs->itens[i]->dynamic_value;

        if (input_at_i == VAL_0) {
            out = VAL_0;
            break;
        }

        if (input_at_i == VAL_X || input_at_i == VAL_Z) {
            out = VAL_X;
        }
    }

    return out;
}

LogicValue compute_nor_gate(ListComponent* inputs)
{
    LogicValue out = compute_or_gate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if ( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

LogicValue compute_nand_gate(ListComponent* inputs)
{
    LogicValue out  = compute_and_gate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if ( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

LogicValue compute_buf_if0_gate(LogicValue control, LogicValue data)
{
    if (control == VAL_1)
        return VAL_Z;

    if ( data == VAL_X || data == VAL_Z )
        return VAL_X;

    if ( control != VAL_0 && data == VAL_0 )
        return VAL_L;

    if ( control != VAL_0 && data == VAL_1 )
        return VAL_H;
    
    return data;
}

LogicValue compute_buf_if1_gate(LogicValue control, LogicValue data)
{
    if (control == VAL_0)
        return VAL_Z;

    if ( data == VAL_X || data == VAL_Z )
        return VAL_X;

    if ( control != VAL_1 && data == VAL_0 )
        return VAL_L;

    if ( control != VAL_1 && data == VAL_1 )
        return VAL_H;
    
    return data;
}

LogicValue compute_not_if0_gate(LogicValue control, LogicValue data)
{
    if (control == VAL_1)
        return VAL_Z;

    if ( data == VAL_X || data == VAL_Z )
        return VAL_X;

    if ( control != VAL_0 && data == VAL_0 )
        return VAL_H;

    if ( control != VAL_0 && data == VAL_1 )
        return VAL_L;

    switch (data)
    {
    case VAL_0:
        return VAL_1;
        break;
    case VAL_1:
        return VAL_0;
        break;
    case VAL_L:
        return VAL_1;
        break;
    case VAL_H:
        return VAL_0;
        break;
    default:
        return VAL_X;
    }
}

LogicValue compute_not_if1_gate(LogicValue control, LogicValue data)
{
    if (control == VAL_0)
        return VAL_Z;

    if ( data == VAL_X || data == VAL_Z )
        return VAL_X;

    if ( control != VAL_1 && data == VAL_0 )
        return VAL_H;

    if ( control != VAL_1 && data == VAL_1 )
        return VAL_L;

    switch (data)
    {
    case VAL_0:
        return VAL_1;
        break;
    case VAL_1:
        return VAL_0;
        break;
    case VAL_L:
        return VAL_1;
        break;
    case VAL_H:
        return VAL_0;
        break;
    default:
        return VAL_X;
    }
}

void create_events_from_outputs(
    Event** queue,
    Time t,
    Time timescale,
    Component* gate,
    LogicValue result)
{
    // create events related to the gate outputs
    for ( int j=0 ; j < gate->list_output->total ; j++ )
    {
        insert_event(queue,
                     t + gate->attributes.delay * timescale /* * (circuito->timescale_unit/UN_FS) */,
                     EVT_NET_TRANSITION,
                     gate->list_output->itens[j],
                     NULL,
                     result);
    }
}


void set_dumpfile(FILE** pp_file, const char* s_path)
{
    if (*pp_file) {
        fclose(*pp_file);
        *pp_file = NULL;
    }

    *pp_file = fopen(s_path, "w");

    if ( *pp_file == NULL ) {
        print("erro: nao foi possivel abrir o arquivo '%s' para escrita.", s_path);
    }
}

void inspection_console(Module* module, Time t)
{
    int code;
    print("[$stop call] in module: '%s'\n", module->name);
    print("At simulation time: %d\n", (unsigned int)t);
    print("Enter 'c' to continue...\n");
    do {
        print("> ");
        code = getchar();
        if ( code == (int)'\n' ) continue;
        while ( getchar() != (int)'\n' );
    }
    while ( code != (int)'c' );
}
