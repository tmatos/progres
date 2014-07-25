/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include "eventos.h"
#include "progres.h"
#include "estruturas.h"
#include "sinais.h"

void insereEvento(Evento **fila, Tempo t, Componente comp, ValorLogico novoValor)
{
    Evento *evt;

    if(!comp)
        return;

    if( !(*fila) )
    {
        evt = (Evento*) xmalloc(sizeof(Evento));
        evt->quando = t;
        evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
        evt->listaTransicao->fio = comp;
        evt->listaTransicao->novoValor = novoValor;
        evt->listaTransicao->proximo = NULL;
        evt->ultimaTransicao = evt->listaTransicao;
        evt->proximo = NULL;

        *fila = evt;
    }
    else
    {
        Evento *it = *fila;
        while(it->quando < t) {
            it++;
        }

        if(it->quando == t) {
            it->ultimaTransicao->proximo = (Transicao*) xmalloc(sizeof(Transicao));
            it->ultimaTransicao = it->ultimaTransicao->proximo;
            it->ultimaTransicao->proximo = NULL;
            it->ultimaTransicao->fio = comp;
            it->ultimaTransicao->novoValor = novoValor;
        }
        else if(t < it->quando) {
            evt = (Evento*) xmalloc(sizeof(Evento));
            evt->quando = t;
            evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
            evt->listaTransicao->fio = comp;
            evt->listaTransicao->novoValor = novoValor;
            evt->listaTransicao->proximo = NULL;
            evt->ultimaTransicao = evt->listaTransicao;
            evt->proximo = *fila;

            *fila = evt;
        }
    }
}

Transicao* getTransicoesEm(Evento* fila, Tempo t)
{
    Evento *it = fila;
    while(it->quando < t) {
        it++;
    }

    if(it->quando == t)
        return it->listaTransicao;
    else
        return NULL;
}

Transicao* popEvento(Evento **fila)
{

}
