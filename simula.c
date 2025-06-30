/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdlib.h>

#include "simula.h"
#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "strutil.h"
#include "lex.h"
#include "eventos.h"
#include "erros.h"

Sinais* simula(Module* circuto, Sinais* entradas, Evento** initial_task_events)
{
    int i;
    int j;
    int validos; // conta correspencias de entradas entre arquivos '.v' e '.in'
    Tempo t;

    Transicao* list_tr = NULL;
    Transicao* tr = NULL; // iterator

    Evento* fila = NULL;
    Pulso* p = NULL;

    ListaComponente* list_changed_gates = NULL;
    Componente gate = NULL;

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

    validos = 0;

    // Validacao da correspencia das entradas entre os arquivos '.v' e '.in'
    for ( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        for ( j=0 ; j < entradas->quantidade ; j++ )
        {
            if ( iguais(circuto->listaFiosEntrada->itens[i]->nome,
                        entradas->lista[j].nome) ) {
                circuto->listaFiosEntrada->itens[i]->sinalEntrada = &(entradas->lista[j]);
                validos++;
                break;
            }
        }
    }

    // print matches msg
    if (!global_silent_mode) {
        printf("----------\n"
               "Entradas: \n"
               "   .v = %d\n"
               "  .in = %d\n"
               "match = %d\n"
               "----------\n",
               circuto->listaFiosEntrada->tamanho,
               entradas->quantidade,
               validos);
    }

    if (validos < circuto->listaFiosEntrada->tamanho) {
        if (!global_silent_mode) {
            printf("AVISO: O arquivo de entradas tem menos "
                   "sinais de entrada que o circuito.\n");
        }

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
    for ( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        t = 0;

        p = circuto->listaFiosEntrada->itens[i]->sinalEntrada->pulsos;
        while (p->valor != VAL_BLANK)
        {
            insert_event(&fila,
                         t,
                         EVT_NET_TRANSITION,
                         circuto->listaFiosEntrada->itens[i],
                         NULL,
                         p->valor);

            t = t + p->tempo * circuto->timescale_number /* * (circuto->timescale_unit/UN_FS) */;
            p++;
        }

        insert_event(&fila,
                     t,
                     EVT_NET_TRANSITION,
                     circuto->listaFiosEntrada->itens[i],
                     NULL,
                     VAL_X); // este sinal fica ate infitito
    }

    // ATENCAO: Sabemos que todos os componentes sao
    // inicializados com o valorDinamico em X.

    // A partir daqui, ocorre a simulacao propriamente dita:
    t = 0;

    while (fila)
    {
        list_changed_gates = novaListaComponente();

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
                 (tr->fio->valorDinamico != tr->novoValor) )
            {
                for ( i=0 ; i < tr->fio->listaSaida->tamanho ; i++ )
                {
                    if ( !contemComponente(list_changed_gates,
                                           tr->fio->listaSaida->itens[i]) ) {
                        insereComponente(list_changed_gates,
                                         tr->fio->listaSaida->itens[i]);
                    }
                }

                if (tr->fio->tipo.operador == output) {
                    if ( !(tr->fio->sinalSaida) ) {
                        tr->fio->sinalSaida = new_signal( tr->fio->nome );
                    }
                    add_new_pulse(tr->fio->sinalSaida,
                                  tr->fio->valorDinamico,
                                  t - tr->fio->sinalSaida->duracaoTotal);
                }

                tr->fio->valorDinamico = tr->novoValor;
            }

            switch (tr->task_type)
            {
            case TASK_DISPLAY:
                if (!global_silent_mode) {
                    printf("%s\n", tr->task_code);
                }
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
            if (gate->listaEntrada->tamanho == 2) {
                valor_data = gate->listaEntrada->itens[0]->valorDinamico;
                valor_control =gate->listaEntrada->itens[1]->valorDinamico;
            }

            switch (gate->tipo.operador)
            {
            case op_not:
                result = computeNotGate(gate->listaEntrada->itens[0]->valorDinamico);
                break;
            case op_buf:
                result = computeBufGate(gate->listaEntrada->itens[0]->valorDinamico);
                break;
            case op_and:
                result = computeAndGate(gate->listaEntrada);
                break;
            case op_or:
                result = computeOrGate(gate->listaEntrada);
                break;
            case op_xor:
                valor_xor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xor_in_b = gate->listaEntrada->itens[1]->valorDinamico;
                result = computeXorGate(valor_xor_in_a, valor_xor_in_b);
                break;
            case op_nand:
                result = computeNandGate(gate->listaEntrada);
                break;
            case op_nor:
                result = computeNorGate(gate->listaEntrada);
                break;
            case op_xnor:
                valor_xnor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xnor_in_b = gate->listaEntrada->itens[1]->valorDinamico;
                result = computeXnorGate(valor_xnor_in_a, valor_xnor_in_b);
                break;
            case OP_BUF_IF0:
                result = compute_buf_if0_gate(valor_data, valor_control);
                break;
            case OP_BUF_IF1:
                result = compute_buf_if1_gate(valor_data, valor_control);
                break;
            case OP_NOT_IF0:
                result = compute_not_if0_gate(valor_data, valor_control);
                break;
            case OP_NOT_IF1:
                result = compute_not_if1_gate(valor_data, valor_control);
                break;
            case assign:
                // TODO: implement expression evaluation and specific data structures
                result = gate->listaEntrada->itens[0]->valorDinamico;
                break;
            default:
                break;
            }

            createEventsFromOutputs(&fila, t, circuto->timescale_number, gate, result);
        }

        // free mem
        if(list_changed_gates->tamanho != 0)
            free(list_changed_gates->itens);
        free(list_changed_gates);
        list_changed_gates = NULL;
    }

    // copia as saidas da simulacao do ciruito para o retorno da funcao
    for( i=0 ; i < circuto->listaFiosSaida->tamanho ; i++ )
    {
        insert_signal(saidas,
                      circuto->listaFiosSaida->itens[i]->sinalSaida);
    }

    return saidas;
}

// IEEE Std 1364-2005, Table 7-4
ValorLogico computeNotGate(ValorLogico input)
{
    if (input == VAL_0)
        return VAL_1;
    
    if (input == VAL_1)
        return VAL_0;
    
    return VAL_X;
}

// IEEE Std 1364-2005, Table 7-4
ValorLogico computeBufGate(ValorLogico input)
{
    if (input == VAL_Z)
        return VAL_X;
    
    return input;
}

// Std 1364-2005, Table 7-3
ValorLogico computeXorGate(ValorLogico a, ValorLogico b)
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
ValorLogico computeXnorGate(ValorLogico a, ValorLogico b)
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
ValorLogico computeOrGate(ListaComponente* inputs)
{
    int i;
    ValorLogico input_at_i;
    ValorLogico out;

    out = VAL_0;
    
    // computa o valor da operacao or sobre todas as entradas
    for ( i=0 ; i < inputs->tamanho ; i++ )
    {
        input_at_i = inputs->itens[i]->valorDinamico;

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
ValorLogico computeAndGate(ListaComponente* inputs)
{
    int i;
    ValorLogico input_at_i;
    ValorLogico out;
    
    out = VAL_1;

    // computa o valor da operacao and sobre todas as entradas
    for ( i=0 ; i < inputs->tamanho ; i++ )
    {
        input_at_i = inputs->itens[i]->valorDinamico;

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

ValorLogico computeNorGate(ListaComponente* inputs)
{
    ValorLogico out;
    out = computeOrGate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if ( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

ValorLogico computeNandGate(ListaComponente* inputs)
{
    ValorLogico out;
    out  = computeAndGate(inputs);

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

void createEventsFromOutputs(Evento** fila, Tempo t, Tempo timescale, Componente gate, ValorLogico result)
{
    int j;

    // cria eventos relativos as saidas da porta
    for ( j=0 ; j < gate->listaSaida->tamanho ; j++ )
    {
        insert_event(fila,
                     t + gate->tipo.atraso * timescale /* * (circuto->timescale_unit/UN_FS) */,
                     EVT_NET_TRANSITION,
                     gate->listaSaida->itens[j],
                     NULL,
                     result);
    }
}
