/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdlib.h>

#include "eventos.h"
#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "mem.h"

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
        Evento *ant = NULL; // evento anterior
        Evento *it = *fila; // iterador de evento
        while( it && (it->quando < t) ) {
            ant = it;
            it = it->proximo;
        }

        if(!it) // inserir evento na ultima posicao da fila
        {
            evt = (Evento*) xmalloc(sizeof(Evento));
            evt->quando = t;
            evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
            evt->listaTransicao->fio = comp;
            evt->listaTransicao->novoValor = novoValor;
            evt->listaTransicao->proximo = NULL;
            evt->ultimaTransicao = evt->listaTransicao;

            evt->proximo = it;
            ant->proximo = evt;
        }
        else if(t == it->quando) // um evento no instante existe, adicionar à lista de transições
        {
            it->ultimaTransicao->proximo = (Transicao*) xmalloc(sizeof(Transicao));
            it->ultimaTransicao = it->ultimaTransicao->proximo;
            it->ultimaTransicao->proximo = NULL;
            it->ultimaTransicao->fio = comp;
            it->ultimaTransicao->novoValor = novoValor;
        }
        else if(t < it->quando)
        {
            if(ant == NULL) // inserir evento no início da fila
            {
                evt = (Evento*) xmalloc(sizeof(Evento));
                evt->quando = t;
                evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
                evt->listaTransicao->fio = comp;
                evt->listaTransicao->novoValor = novoValor;
                evt->listaTransicao->proximo = NULL;
                evt->ultimaTransicao = evt->listaTransicao;

                evt->proximo = it;

                *fila = evt; // ele vira o primeiro da fila
            }
            else // inserir entre dois eventos, o anterior e o seguinte
            {
                evt = (Evento*) xmalloc(sizeof(Evento));
                evt->quando = t;
                evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
                evt->listaTransicao->fio = comp;
                evt->listaTransicao->novoValor = novoValor;
                evt->listaTransicao->proximo = NULL;
                evt->ultimaTransicao = evt->listaTransicao;

                evt->proximo = it;
                ant->proximo = evt;
            }
        }
    }
}

Transicao* getTransicoesEm(Evento* fila, Tempo t)
{
    Evento *it = fila; // iterador de evento
    while( (it->quando < t) && it ) {
        it = it->proximo;
    }

    if( it && (t == it->quando) )
        return it->listaTransicao;
    else
        return NULL;
}

Transicao* popEvento(Evento **fila)
{
    Transicao *ret = NULL;
    Evento *dead = NULL;

    if(!fila)
        return NULL;

    if( !(*fila) )
        return NULL;

    if( ! (*fila)->listaTransicao )
        return NULL;

    ret = (*fila)->listaTransicao;

    dead = (*fila);
    (*fila) = (*fila)->proximo;
    free(dead);

    return ret;
}
