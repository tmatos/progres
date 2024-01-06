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

Sinais* simula(t_circuito* circuto, Sinais* entradas)
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
    ValorLogico valor_and_in_j;
    ValorLogico valor_or_in_j;
    ValorLogico valor_xor_in_a;
    ValorLogico valor_xor_in_b;
    ValorLogico valor_nand_in_j;
    ValorLogico valor_nor_in_j;
    ValorLogico valor_xnor_in_a;
    ValorLogico valor_xnor_in_b;

    Sinais *saidas = novaSinais();

    if(!circuto || !entradas) {
        return NULL;
    }

    validos = 0;

    // Validação da correspência das entradas entre os arquivos '.v' e '.in'
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
        printf("O arquivo de entradas tem menos sinais de entrada que o circuito.\n");
        return NULL;
    }

    // Inicialização da fila de eventos com os valores das entradas
    fila = NULL;

    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        t = 0;

        p = circuto->listaFiosEntrada->itens[i]->sinalEntrada->pulsos;
        while(p->valor != nulo)
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
                     x); // este sinal fica até infitito
    }

    // ATENÇÃO: Sabemos que todos os componentes são inicializados com o valorDinamico em xis

    // A partir daqui, ocorre a simulação propriamente dita, usado fila de eventos
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
            if( itTr->fio->valorDinamico != itTr->novoValor ) // apenas se houver mudança de valor no fio
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

        free(listaTr); // popEvento não liberou a lista de transições, fazemos isso aqui
        listaTr = NULL;

        for( i=0 ; i < portasAlteradas->tamanho ; i++ )
        {
            gate = portasAlteradas->itens[i];

            switch( gate->tipo.operador )
            {
            case op_not:
                valor_not_in = gate->listaEntrada->itens[0]->valorDinamico;
                if(valor_not_in == x) {
                    resultado = x;
                }
                else if (valor_not_in == zero) {
                    resultado = um;
                }
                else if (valor_not_in == um) {
                    resultado = zero;
                }
                else {
                    resultado = z;
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_buf:
                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 gate->listaEntrada->itens[0]->valorDinamico);
                }
                break;

            case op_and:
                resultado = um;

                // computa o valor da operacao and sobre todas as entradas
                for( j=0 ; j < gate->listaEntrada->tamanho ; j++ )
                {
                    valor_and_in_j = gate->listaEntrada->itens[j]->valorDinamico;

                    if(valor_and_in_j == x || valor_and_in_j == z) {
                        resultado = valor_and_in_j;
                        break;
                    }
                    else if(valor_and_in_j == zero) {
                        resultado = zero;
                    }
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_or:
                resultado = zero;

                // computa o valor da operacao or sobre todas as entradas
                for( j=0 ; j < gate->listaEntrada->tamanho ; j++ )
                {
                    valor_or_in_j = gate->listaEntrada->itens[j]->valorDinamico;

                    if(valor_or_in_j == x || valor_or_in_j == z) {
                        resultado = valor_or_in_j;
                        break;
                    }
                    else if(valor_or_in_j == um) {
                        resultado = um;
                    }
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_xor:
                valor_xor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xor_in_b = gate->listaEntrada->itens[1]->valorDinamico;

                if(valor_xor_in_a == x || valor_xor_in_b == x) {
                    resultado = x;
                }
                else if(valor_xor_in_a == z || valor_xor_in_b == z) {
                    resultado = z;
                }
                else {
                    if( (valor_xor_in_a == um && valor_xor_in_b == zero) ||
                        (valor_xor_in_a == zero && valor_xor_in_b == um) )
                    {
                        resultado = um;
                    }
                    else {
                        resultado = zero;
                    }
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_nand:
                resultado = um;

                // computa o valor da operacao nand sobre todas as entradas (em duas etapas)
                for( j=0 ; j < gate->listaEntrada->tamanho ; j++ )
                {
                    valor_nand_in_j = gate->listaEntrada->itens[j]->valorDinamico;

                    if(valor_nand_in_j == x || valor_nand_in_j == z) {
                        resultado = valor_nand_in_j;
                        break;
                    }
                    else if(valor_nand_in_j == zero) {
                        resultado = zero;
                    }
                }

                // fazemos a negativa do resultado se este for diferente de x e z (segunda etapa)
                if( (resultado != x) && (resultado != z) ) {
                    resultado = (resultado == zero) ? um : zero;
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_nor:
                resultado = zero;

                // computa o valor da operacao nor sobre todas as entradas (em duas etapas)
                for( j=0 ; j < gate->listaEntrada->tamanho ; j++ )
                {
                    valor_nor_in_j = gate->listaEntrada->itens[j]->valorDinamico;

                    if(valor_nor_in_j == x || valor_nor_in_j == z) {
                        resultado = valor_nor_in_j;
                        break;
                    }
                    else if(valor_nor_in_j == um) {
                        resultado = um;
                    }
                }

                // fazemos a negativa do resultado se este for diferente de x e z (segunda etapa)
                if( (resultado != x) && (resultado != z) ) {
                    resultado = (resultado == zero) ? um : zero;
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            case op_xnor:
                valor_xnor_in_a = gate->listaEntrada->itens[0]->valorDinamico;
                valor_xnor_in_b = gate->listaEntrada->itens[1]->valorDinamico;

                if(valor_xnor_in_a == x || valor_xnor_in_b == x) {
                    resultado = x;
                }
                else if(valor_xnor_in_a == z || valor_xnor_in_b == z) {
                    resultado = z;
                }
                else {
                    if( (valor_xnor_in_a == um && valor_xnor_in_b == um) ||
                        (valor_xnor_in_a == zero && valor_xnor_in_b == zero) )
                    {
                        resultado = um;
                    }
                    else {
                        resultado = zero;
                    }
                }

                // cria eventos relativos às saidas da porta
                for( j=0 ; j < gate->listaSaida->tamanho ; j++ )
                {
                    insereEvento(&fila,
                                 t + gate->tipo.atraso,
                                 gate->listaSaida->itens[j],
                                 resultado);
                }

                break;

            default:
                break;
            }
        }
    }

    // copia as saidas da simulação do ciruito para o retorno da funcao
    for( i=0 ; i < circuto->listaFiosSaida->tamanho ; i++ )
    {
        addSinalPronto(saidas,
                       circuto->listaFiosSaida->itens[i]->sinalSaida);
    }

    return saidas;
}
