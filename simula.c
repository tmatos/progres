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
#include "lex.h"
#include "eventos.h"
#include "erros.h"

Sinais* simula(Module* circuto, Sinais* entradas)
{
    int i;
    int j;
    int validos; // conta correspencias de entradas entre arquivos '.v' e '.in'
    Tempo t;

    Transicao* listaTr = NULL;
    Transicao* itTr = NULL;

    Evento* fila = NULL;
    Pulso* p = NULL;

    ListaComponente* portasAlteradas = NULL;
    Componente gate = NULL;

    ValorLogico result;
    ValorLogico valor_xor_in_a;
    ValorLogico valor_xor_in_b;
    ValorLogico valor_xnor_in_a;
    ValorLogico valor_xnor_in_b;

    Sinais* saidas = novaSinais();

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
        printf("\nENTRADAS:\n  .v = %d\n .in = %d\n batem = %d\n\n",
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

    // Inicializacao da fila de eventos com os valores das entradas
    fila = NULL;

    for ( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        t = 0;

        p = circuto->listaFiosEntrada->itens[i]->sinalEntrada->pulsos;
        while (p->valor != VAL_BLANK)
        {
            insereEvento(&fila,
                         t,
                         circuto->listaFiosEntrada->itens[i],
                         p->valor);

            t = t + p->tempo;
            p++;
        }

        insereEvento(&fila,
                     t,
                     circuto->listaFiosEntrada->itens[i],
                     VAL_X); // este sinal fica ate infitito
    }

    // ATENCAO: Sabemos que todos os componentes sao
    // inicializados com o valorDinamico em X.

    // A partir daqui, ocorre a simulacao propriamente dita:
    t = 0;

    while (fila)
    {
        portasAlteradas = novaListaComponente();

        t = fila->quando;

        listaTr = popEvento(&fila);
        itTr = listaTr;

        // atualiza valores de fios e faz uma lista das
        // portas alteradas pelas transicoes em listaTr
        while (itTr)
        {
            // apenas se houver mudanca de valor no fio
            if ( itTr->fio->valorDinamico != itTr->novoValor )
            {
                for ( i=0 ; i < itTr->fio->listaSaida->tamanho ; i++ )
                {
                    if ( !contemComponente(portasAlteradas,
                                           itTr->fio->listaSaida->itens[i]) ) {
                        insereComponente(portasAlteradas,
                                         itTr->fio->listaSaida->itens[i]);
                    }
                }

                if (itTr->fio->tipo.operador == output) {
                    if ( !(itTr->fio->sinalSaida) ) {
                        itTr->fio->sinalSaida = novoSinal( itTr->fio->nome );
                    }
                    addPulso(itTr->fio->sinalSaida,
                             itTr->fio->valorDinamico,
                             t - itTr->fio->sinalSaida->duracaoTotal);
                }

                itTr->fio->valorDinamico = itTr->novoValor;
            }

            itTr = itTr->proximo;
        }

        // popEvento() nao liberou mem da lista de transicoes, fazemos isso aqui
        delete_list_transicao(&listaTr);

        for ( i=0 ; i < portasAlteradas->tamanho ; i++ )
        {
            gate = portasAlteradas->itens[i];

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
            case assign:
                // TODO: implement expression evaluation and specific data structures
                result = gate->listaEntrada->itens[0]->valorDinamico;
                break;
            default:
                break;
            }

            createEventsFromOutputs(&fila, t, gate, result);
        }

        // free mem
        if(portasAlteradas->tamanho != 0)
            free(portasAlteradas->itens);
        free(portasAlteradas);
        portasAlteradas = NULL;
    }

    // copia as saidas da simulacao do ciruito para o retorno da funcao
    for( i=0 ; i < circuto->listaFiosSaida->tamanho ; i++ )
    {
        addSinalPronto(saidas,
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

void createEventsFromOutputs(Evento** fila, Tempo t, Componente gate, ValorLogico result)
{
    int j;

    // cria eventos relativos as saidas da porta
    for ( j=0 ; j < gate->listaSaida->tamanho ; j++ )
    {
        insereEvento(fila,
                     t + gate->tipo.atraso,
                     gate->listaSaida->itens[j],
                     result);
    }
}
