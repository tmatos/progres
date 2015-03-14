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

    Sinais *saidas = novaSinais();

    if(!circuto || !entradas)
        return NULL;

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

    if(validos < circuto->listaFiosEntrada->tamanho)
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
            insereEvento(&fila, t, circuto->listaFiosEntrada->itens[i], p->valor);
            t = t + p->tempo;

            p++;
        }

        insereEvento(&fila, t, circuto->listaFiosEntrada->itens[i], x); // este sinal fica até infitito
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
            if(itTr->fio->valorDinamico != itTr->novoValor) // apenas se houver mudança de valor no fio
            {
                for( i=0 ; i < itTr->fio->listaSaida->tamanho ; i++ )
                {
                    if( !contemComponente( portasAlteradas, itTr->fio->listaSaida->itens[i] ) ) {
                        insereComponente( portasAlteradas, itTr->fio->listaSaida->itens[i] );
                    }
                }

                if(itTr->fio->tipo.operador == output)
                {
                    if( !(itTr->fio->sinalSaida) )
                        itTr->fio->sinalSaida = novoSinal( itTr->fio->nome );

                    addPulso( itTr->fio->sinalSaida, itTr->fio->valorDinamico, t - itTr->fio->sinalSaida->duracaoTotal );
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
                if(gate->listaEntrada->itens[0]->valorDinamico == x)
                    resultado = x;
                else if (gate->listaEntrada->itens[0]->valorDinamico == zero)
                    resultado = um;
                else if (gate->listaEntrada->itens[0]->valorDinamico == um)
                    resultado = zero;

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_buf:
                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], gate->listaEntrada->itens[0]->valorDinamico);
                }

                break;

            case op_and:
                resultado = um;

                // computa o valor
                for(j=0 ; j < gate->listaEntrada->tamanho ; j++)
                {
                    if(gate->listaEntrada->itens[j]->valorDinamico == x)
                    {
                        resultado = x;
                        break;
                    }
                    else if(gate->listaEntrada->itens[j]->valorDinamico == zero)
                    {
                        resultado = zero;
                    }
                }

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_or:
                resultado = zero;

                // computa o valor
                for(j=0 ; j < gate->listaEntrada->tamanho ; j++)
                {
                    if(gate->listaEntrada->itens[j]->valorDinamico == x)
                    {
                        resultado = x;
                        break;
                    }
                    else if(gate->listaEntrada->itens[j]->valorDinamico == um)
                    {
                        resultado = um;
                    }
                }

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_xor:
                if(gate->listaEntrada->itens[0]->valorDinamico == x || gate->listaEntrada->itens[1]->valorDinamico == x)
                {
                    resultado = x;
                }
                else
                {
                    if ( (gate->listaEntrada->itens[0]->valorDinamico == um && gate->listaEntrada->itens[1]->valorDinamico == zero) ||
                         (gate->listaEntrada->itens[0]->valorDinamico == zero && gate->listaEntrada->itens[1]->valorDinamico == um) )
                    {
                        resultado = um;
                    }
                    else
                    {
                        resultado = zero;
                    }
                }

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_nand:
                resultado = um;

                // computa o valor
                for(j=0 ; j < gate->listaEntrada->tamanho ; j++)
                {
                    if(gate->listaEntrada->itens[j]->valorDinamico == x)
                    {
                        resultado = x;
                        break;
                    }
                    else if(gate->listaEntrada->itens[j]->valorDinamico == zero)
                    {
                        resultado = zero;
                    }
                }

                // fazemos a negação do resultado se este for diferente de x
                if(resultado != x)
                    resultado = (resultado == zero ? um : zero);

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_nor:
                resultado = zero;

                // computa o valor
                for(j=0 ; j < gate->listaEntrada->tamanho ; j++)
                {
                    if(gate->listaEntrada->itens[j]->valorDinamico == x)
                    {
                        resultado = x;
                        break;
                    }
                    else if(gate->listaEntrada->itens[j]->valorDinamico == um)
                    {
                        resultado = um;
                    }
                }

                // fazemos a negação do resultado se este for diferente de x
                if(resultado != x)
                    resultado = (resultado == zero ? um : zero);

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            case op_xnor:
                if(gate->listaEntrada->itens[0]->valorDinamico == x || gate->listaEntrada->itens[1]->valorDinamico == x)
                {
                    resultado = x;
                }
                else
                {
                    if ( (gate->listaEntrada->itens[0]->valorDinamico == um && gate->listaEntrada->itens[1]->valorDinamico == um) ||
                         (gate->listaEntrada->itens[0]->valorDinamico == zero && gate->listaEntrada->itens[1]->valorDinamico == zero) )
                    {
                        resultado = um;
                    }
                    else
                    {
                        resultado = zero;
                    }
                }

                // cria eventos relativ. às saidas da porta
                for(j=0 ; j < gate->listaSaida->tamanho ; j++)
                {
                    insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[j], resultado);
                }

                break;

            default:
                break;
            }
        }

    }

    // copia as saidas da simulação do ciruito para o retorno
    for(i=0 ; i < circuto->listaFiosSaida->tamanho ; i++)
        addSinalPronto(saidas, circuto->listaFiosSaida->itens[i]->sinalSaida);

    return saidas;
}
