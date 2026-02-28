/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

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

int validate_input_signals(Module* module, Sinais* signals)
{
    int i;
    int j;
    int validos = 0;

    // Validacao da correspencia das signals entre os arquivos '.v' e '.in'
    for ( i=0 ; i < module->list_input_net->tamanho ; i++ )
    {
        for ( j=0 ; j < signals->quantidade ; j++ )
        {
            if ( iguais(module->list_input_net->itens[i]->nome,
                        signals->lista[j].nome) ) {
                module->list_input_net->itens[i]->input_signal = &(signals->lista[j]);
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
          module->list_input_net->tamanho,
          signals->quantidade,
          validos);

    return validos;
}

Sinais* simula(Module* circuto, Sinais* entradas, Evento** initial_task_events, FILE** f_dump)
{
    int i;
    int validos; // conta correspencias de entradas entre arquivos '.v' e '.in'
    Tempo t;

    Transicao* list_tr = NULL;
    Transicao* tr = NULL; // iterator

    Evento* fila = NULL;
    Pulso* p = NULL;

    ListComponent* list_changed_gates = NULL;
    Component* gate = NULL;

    ValorLogico result;
    ValorLogico valor_xor_in_a;
    ValorLogico valor_xor_in_b;
    ValorLogico valor_xnor_in_a;
    ValorLogico valor_xnor_in_b;
    ValorLogico valor_control;
    ValorLogico valor_data;

    Sinais* saidas = new_signal_list();

    if (!circuto || !entradas) {
        return NULL;
    }

    validos = validate_input_signals(circuto, entradas);

    if (validos < circuto->list_input_net->tamanho) {
        print("AVISO: O arquivo de entradas tem menos "
              "sinais de entrada que o circuito.\n");

        return NULL;
    }

    // task ocurrences are copied into the event queue
    Evento* evit = initial_task_events ? *initial_task_events : NULL;
    while (evit)
    {
        Transicao* tranit = evit->listaTransicao;
        while (tranit)
        {
            insert_task_event(&fila,
                              evit->quando,
                              tranit->task_type,
                              tranit->task_code);

            tranit = tranit->proximo;
        }
        
        evit = evit->proximo;
    }

    // Inicializacao da fila de eventos com os valores das entradas
    for ( i=0 ; i < circuto->list_input_net->tamanho ; i++ )
    {
        t = 0;

        p = circuto->list_input_net->itens[i]->input_signal->pulsos;
        while (p->valor != VAL_BLANK)
        {
            insert_event(&fila,
                         t,
                         EVT_NET_TRANSITION,
                         circuto->list_input_net->itens[i],
                         NULL,
                         p->valor);

            t = t + p->tempo * circuto->timescale_number /* * (circuto->timescale_unit/UN_FS) */;
            p++;
        }

        insert_event(&fila,
                     t,
                     EVT_NET_TRANSITION,
                     circuto->list_input_net->itens[i],
                     NULL,
                     VAL_X); // este sinal fica ate infitito
    }

    // ATENCAO: Sabemos que todos os componentes sao
    // inicializados com o dynamic_value em X (exceto constantes literais)

    // A partir daqui, ocorre a simulacao propriamente dita:
    t = 0;

    while (fila)
    {
        list_changed_gates = new_list_component();

        t = fila->quando;

        list_tr = pop_event(&fila);
        tr = list_tr;

        // nao sendo uma system task: atualiza valores de fios e faz
        // uma lista das portas alteradas pelas transicoes em list_tr;
        // senao, caso seja task: executa a mesma.
        while (tr)
        {
            // apenas tambem se houver mudanca de valor no fio
            if ( (tr->task_type == IS_NOT_A_TASK) &&
                 (tr->fio->dynamic_value != tr->novoValor) )
            {
                for ( i=0 ; i < tr->fio->list_output->tamanho ; i++ )
                {
                    if ( !has_component_by_pointer(list_changed_gates,
                                                   tr->fio->list_output->itens[i]) ) {
                        insert_component(list_changed_gates,
                                         tr->fio->list_output->itens[i]);
                    }
                }

                if (tr->fio->atributos.role == ROLE_OUTPUT) {
                    if ( !(tr->fio->output_signal) ) {
                        tr->fio->output_signal = new_signal( tr->fio->nome );
                    }
                    add_new_pulse(tr->fio->output_signal,
                                  tr->fio->dynamic_value,
                                  t - tr->fio->output_signal->duracaoTotal);
                }

                tr->fio->dynamic_value = tr->novoValor;
            }

            switch (tr->task_type)
            {
            case TASK_DISPLAY:
                print("%s\n", tr->task_code);
                break;
            case TASK_WRITE:
                print("%s", tr->task_code);
                break;
            case TASK_DUMPFILE:
                set_dumpfile(f_dump, tr->task_code);
                // TODO: set more flags for dumpfile
                break;
            case TASK_FINISH:
                while (fila)
                {
                    Transicao* lt = pop_event(&fila);
                    if (lt)
                        delete_list_transicao(&lt);
                }
                tr = NULL; // exit outer while loop
                continue;
                break;
            case TASK_STOP:
                inspection_console(circuto, t);
                break;
            default:
                break;
            }

            tr = tr->proximo;
        }

        // pop_event() nao liberou mem da lista de transicoes, fazemos isso aqui
        delete_list_transicao(&list_tr);

        for ( i=0 ; i < list_changed_gates->tamanho ; i++ )
        {
            gate = list_changed_gates->itens[i];

            // be prepared, in case of 3 stage logic gates
            if (gate->list_input->tamanho == 2) {
                valor_data = gate->list_input->itens[0]->dynamic_value;
                valor_control =gate->list_input->itens[1]->dynamic_value;
            }

            switch (gate->atributos.role)
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

            create_events_from_outputs(&fila, t, circuto->timescale_number, gate, result);
        }

        // free mem
        if (list_changed_gates->tamanho != 0)
            free(list_changed_gates->itens);
        free(list_changed_gates);
        list_changed_gates = NULL;
    }

    // copia as saidas da simulacao do ciruito para o retorno da funcao
    for ( i=0 ; i < circuto->list_output_net->tamanho ; i++ )
    {
        Sinal* s = circuto->list_output_net->itens[i]->output_signal;

        if (s) {
            insert_signal(saidas, s);
        }
        else {
            // cria sinal de saida constante com o valor dinamico do componente
            ValorLogico v = circuto->list_output_net->itens[i]->dynamic_value;
            Sinal* s_temp = new_signal(circuto->list_output_net->itens[i]->nome);
            add_new_pulse(s_temp, v, t); // valor dinamico por todo o tempo da simulacao
            insert_signal(saidas, s_temp);
            free(s_temp->pulsos);
            free(s_temp);
        }
    }

    return saidas;
}

// IEEE Std 1364-2005, Table 7-4
ValorLogico compute_not_gate(ValorLogico input)
{
    if (input == VAL_0)
        return VAL_1;
    
    if (input == VAL_1)
        return VAL_0;
    
    return VAL_X;
}

// IEEE Std 1364-2005, Table 7-4
ValorLogico compute_buf_gate(ValorLogico input)
{
    if (input == VAL_Z)
        return VAL_X;
    
    return input;
}

// Std 1364-2005, Table 7-3
ValorLogico compute_xor_gate(ValorLogico a, ValorLogico b)
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
ValorLogico compute_xnor_gate(ValorLogico a, ValorLogico b)
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
ValorLogico compute_or_gate(ListComponent* inputs)
{
    int i;
    ValorLogico input_at_i;
    ValorLogico out;

    out = VAL_0;
    
    // computa o valor da operacao or sobre todas as entradas
    for ( i=0 ; i < inputs->tamanho ; i++ )
    {
        input_at_i = inputs->itens[i]->dynamic_value;

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
ValorLogico compute_and_gate(ListComponent* inputs)
{
    int i;
    ValorLogico input_at_i;
    ValorLogico out;
    
    out = VAL_1;

    // computa o valor da operacao and sobre todas as entradas
    for ( i=0 ; i < inputs->tamanho ; i++ )
    {
        input_at_i = inputs->itens[i]->dynamic_value;

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

ValorLogico compute_nor_gate(ListComponent* inputs)
{
    ValorLogico out;
    out = compute_or_gate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if ( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

ValorLogico compute_nand_gate(ListComponent* inputs)
{
    ValorLogico out;
    out  = compute_and_gate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if ( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

ValorLogico compute_buf_if0_gate(ValorLogico control, ValorLogico data)
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

ValorLogico compute_buf_if1_gate(ValorLogico control, ValorLogico data)
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

ValorLogico compute_not_if0_gate(ValorLogico control, ValorLogico data)
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

ValorLogico compute_not_if1_gate(ValorLogico control, ValorLogico data)
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

void create_events_from_outputs(Evento** fila, Tempo t, Tempo timescale, Component* gate, ValorLogico result)
{
    int j;

    // cria eventos relativos as saidas da porta
    for ( j=0 ; j < gate->list_output->tamanho ; j++ )
    {
        insert_event(fila,
                     t + gate->atributos.delay * timescale /* * (circuito->timescale_unit/UN_FS) */,
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

void inspection_console(Module* module, Tempo t)
{
    int code;
    print("[$stop call] in module: '%s'\n", module->name);
    print("At simulation time: %d\n", (unsigned int)t);
    print("Type 'c' to continue...\n");
    do {
        print("> ");
        code = getchar();
    }
    while ( code != (int)'c' );
}
