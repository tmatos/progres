/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdlib.h>

#include "simula.h"
#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "lex.h"
#include "eventos.h"

Sinais* simula(Module* circuto, Sinais* entradas)
{
    int i, j, validos;
    Tempo t;

    Transicao *listaTr = NULL;
    Transicao *itTr = NULL;

    Evento *fila = NULL;
    Pulso *p = NULL;

    ListaComponente *portasAlteradas = NULL;
    Componente gate = NULL;

    ValorLogico resultado;
    ValorLogico valor_not_in;
    ValorLogico valor_xor_in_a;
    ValorLogico valor_xor_in_b;
    ValorLogico valor_xnor_in_a;
    ValorLogico valor_xnor_in_b;

    Sinais *saidas = novaSinais();

    if(!circuto || !entradas) {
        return NULL;
    }

    validos = 0;

    // Validacao da correspencia das entradas entre os arquivos '.v' e '.in'
    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        for( j=0 ; j < entradas->quantidade ; j++ )
        {
            if( iguais( circuto->listaFiosEntrada->itens[i]->nome, entradas->lista[j].nome ) )
            {
                circuto->listaFiosEntrada->itens[i]->sinalEntrada = &(entradas->lista[j]);
                validos++;
                break;
            }
        }
    }

    ///
    printf("\nENTRADAS:\n  .v = %d\n .in = %d\n batem = %d\n\n",
           circuto->listaFiosEntrada->tamanho,
           entradas->quantidade,
           validos);
    ///

    if( validos < circuto->listaFiosEntrada->tamanho )
    {
        printf("AVISO: O arquivo de entradas tem menos sinais de entrada que o circuito.\n");
        return NULL;
    }

    // Inicializacao da fila de eventos com os valores das entradas
    fila = NULL;

    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        t = 0;

        p = circuto->listaFiosEntrada->itens[i]->sinalEntrada->pulsos;
        while(p->valor != VAL_BLANK)
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

    // ATENCAO: Sabemos que todos os componentes sao inicializados com o valorDinamico em X

    // A partir daqui, ocorre a simulacao propriamente dita, usado fila de eventos
    t = 0;

    while(fila)
    {
        portasAlteradas = novaListaComponente();

        t = fila->quando;

        listaTr = popEvento(&fila);
        itTr = listaTr;

        // atualiza valores de fios e faz uma lista das portas alteradas pelas transicoes em listaTr
        while(itTr)
        {
            if( itTr->fio->valorDinamico != itTr->novoValor ) // apenas se houver mudanca de valor no fio
            {
                for( i=0 ; i < itTr->fio->listaSaida->tamanho ; i++ )
                {
                    if( !contemComponente( portasAlteradas, itTr->fio->listaSaida->itens[i] ) )
                    {
                        insereComponente(portasAlteradas,
                                         itTr->fio->listaSaida->itens[i]);
                    }
                }

                if( itTr->fio->tipo.operador == output )
                {
                    if( !(itTr->fio->sinalSaida) ) {
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

        // TODO: extrair esse codigo de liberacao de mem para funcao
        // popEvento() nao liberou mem da lista de transicoes, fazemos isso aqui
        Transicao* pt = listaTr;
        Transicao* pta;
        while (pt) {
            pta = pt;
            pt = pt->proximo;
            free(pta);
        }
        listaTr = NULL;

        for( i=0 ; i < portasAlteradas->tamanho ; i++ )
        {
            gate = portasAlteradas->itens[i];

            switch( gate->tipo.operador )
            {
            case op_not:
                valor_not_in = gate->listaEntrada->itens[0]->valorDinamico;
                resultado = computeNotGate(valor_not_in);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_buf:
                resultado = gate->listaEntrada->itens[0]->valorDinamico;
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_and:
                resultado = computeAndGate(gate->listaEntrada);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_or:
                resultado = computeOrGate(gate->listaEntrada);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_xor:
                valor_xor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xor_in_b = gate->listaEntrada->itens[1]->valorDinamico;
                resultado = computeXorGate(valor_xor_in_a, valor_xor_in_b);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_nand:
                resultado = computeNandGate(gate->listaEntrada);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_nor:
                resultado = computeNorGate(gate->listaEntrada);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            case op_xnor:
                valor_xnor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xnor_in_b = gate->listaEntrada->itens[1]->valorDinamico;
                resultado = computeXnorGate(valor_xnor_in_a, valor_xnor_in_b);
                createEventsFromOutputs(&fila, t, gate, resultado);
                break;

            default:
                break;
            }
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

ValorLogico computeNotGate(ValorLogico input)
{
    if(input == VAL_X)
        return VAL_X;
    
    if(input == VAL_0)
        return VAL_1;
    
    if(input == VAL_1)
        return VAL_0;
    
    return VAL_Z;
}

ValorLogico computeXorGate(ValorLogico a, ValorLogico b)
{
    if(a == VAL_X || b == VAL_X)
        return VAL_X;

    if(a == VAL_Z || b == VAL_Z)
        return VAL_Z;

    if( (a == VAL_1 && b == VAL_0) || (a == VAL_0 && b == VAL_1) )
        return VAL_1;

    return VAL_0;
}

ValorLogico computeXnorGate(ValorLogico a, ValorLogico b)
{
    if(a == VAL_X || b == VAL_X)
        return VAL_X;

    if(a == VAL_Z || b == VAL_Z)
        return VAL_Z;

    if( (a == VAL_1 && b == VAL_1) || (a == VAL_0 && b == VAL_0) )
        return VAL_1;

    return VAL_0;
}

ValorLogico computeOrGate(ListaComponente* inputs)
{
    int j;
    ValorLogico input_at_j;
    ValorLogico out = VAL_0;
    
    // computa o valor da operacao or sobre todas as entradas
    for( j=0 ; j < inputs->tamanho ; j++ )
    {
        input_at_j = inputs->itens[j]->valorDinamico;

        if(input_at_j == VAL_X || input_at_j == VAL_Z) {
            out = input_at_j;
            break;
        }

        if(input_at_j == VAL_1) {
            out = VAL_1;
            break;
        }
    }

    return out;
}

ValorLogico computeAndGate(ListaComponente* inputs)
{
    int j;
    ValorLogico input_at_j;
    ValorLogico out = VAL_1;

    // computa o valor da operacao and sobre todas as entradas
    for( j=0 ; j < inputs->tamanho ; j++ )
    {
        input_at_j = inputs->itens[j]->valorDinamico;

        if(input_at_j == VAL_X || input_at_j == VAL_Z) {
            out = input_at_j;
            break;
        }

        if(input_at_j == VAL_0) {
            out = VAL_0;
            break;
        }
    }

    return out;
}

ValorLogico computeNorGate(ListaComponente* inputs)
{
    ValorLogico out = computeOrGate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

ValorLogico computeNandGate(ListaComponente* inputs)
{
    ValorLogico out = computeAndGate(inputs);

    // fazemos a negativa do resultado se este for diferente de X e Z
    if( (out != VAL_X) && (out != VAL_Z) ) {
        out = (out == VAL_0) ? VAL_1 : VAL_0;
    }

    return out;
}

void createEventsFromOutputs(Evento** fila, Tempo t, Componente gate, ValorLogico result)
{
    int j;

    // cria eventos relativos as saidas da porta
    for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
    {
        insereEvento(fila,
                     t + gate->tipo.atraso,
                     gate->listaSaida->itens[j],
                     result);
    }
}
