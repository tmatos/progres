/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdlib.h>

#include "eventos.h"
#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "mem.h"
#include "lex.h"
#include "strutil.h"

Evento* new_event_at(Tempo t, EventKind k)
{
    Evento* e = (Evento*) xmalloc(sizeof(Evento));
    e->quando = t;
    e->kind = k;
    e->listaTransicao = NULL;
    e->ultimaTransicao = NULL;
    e->proximo = NULL;

    return e;
}

void insert_event(Evento** fila, Tempo t, EventKind k, Componente comp, Register* r, ValorLogico novoValor)
{
    Evento* evt = NULL;
    Evento* ant = NULL; // evento anterior
    Evento* it = NULL; // iterador para eventos

    if (!comp)
        return;

    evt = new_event_at(t, k);

    evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));
    
    evt->listaTransicao->fio = comp;
    evt->listaTransicao->reg = r;
    evt->listaTransicao->novoValor = novoValor;
    evt->listaTransicao->proximo = NULL;

    evt->listaTransicao->task_type = TASK_UNKNOWN;
    evt->listaTransicao->task_code = NULL;

    // empty queue
    if ( !(*fila) ) {
        evt->ultimaTransicao = evt->listaTransicao;
        *fila = evt;

        return;
    }

    ant = NULL; // evento anterior
    it = *fila; // iterador de evento
    while ( it && (it->quando < t) )
    {
        ant = it;
        it = it->proximo;
    }

     // insert the event at the last position in queue
    if (!it) {
        evt->ultimaTransicao = evt->listaTransicao;

        evt->proximo = it;
        ant->proximo = evt;

        return;
    }

    // um evento no instante existe, adicionar a lista de transicoes
    if (t == it->quando)
    {
        it->ultimaTransicao->proximo = evt->listaTransicao;
        it->ultimaTransicao = evt->listaTransicao;

        free(evt);

        return;
    }

    if (t < it->quando)
    {
        // insert event at the beginning of the queue
        if (ant == NULL) {
            evt->ultimaTransicao = evt->listaTransicao;
            evt->proximo = it;
            *fila = evt; // ele vira o primeiro da fila

            return;
        }
        
        // insert between two events, the previous and next
        evt->ultimaTransicao = evt->listaTransicao;
        evt->proximo = it;
        ant->proximo = evt;
    }
}

void insert_task_event(Evento** fila, Tempo t, SystemTask sys_task, const char* code)
{
    Evento* evt = NULL;
    Evento* ant = NULL; // evento anterior
    Evento* it = NULL;

    evt = new_event_at(t, EVT_SYS_TASK);

    evt->listaTransicao = (Transicao*) xmalloc(sizeof(Transicao));

    evt->listaTransicao->task_type = sys_task;
    evt->listaTransicao->task_code = (char*) xmalloc( sizeof(char) * MAX_TOKEN_SIZE + 1 );
    copy( evt->listaTransicao->task_code, code );

    evt->listaTransicao->fio = NULL;
    evt->listaTransicao->reg = NULL;
    evt->listaTransicao->novoValor = VAL_BLANK;
    evt->listaTransicao->proximo = NULL;

    // empty queue case
    if ( !(*fila) ) {
        evt->ultimaTransicao = evt->listaTransicao;
        *fila = evt;

        return;
    }

    // search for the position
    ant = NULL;
    it = *fila;
    while ( it && (it->quando < t) )
    {
        ant = it;
        it = it->proximo;
    }

     // insert the event at the last position in queue
    if (!it) {
        evt->ultimaTransicao = evt->listaTransicao;

        evt->proximo = it;
        ant->proximo = evt;

        return;
    }

    // an event exists at same instant, so
    // append the new evt transi list to the existing evt transi list 
    if (t == it->quando)
    {
        it->ultimaTransicao->proximo = evt->listaTransicao;
        it->ultimaTransicao = evt->listaTransicao;

        free(evt);

        return;
    }

    if (t < it->quando)
    {
        // insert event at the beginning of the queue
        if (ant == NULL) {
            evt->ultimaTransicao = evt->listaTransicao;
            evt->proximo = it;
            *fila = evt; // it becomes the first in the queue

            return;
        }
        
        // insert between two events, the previous and next
        evt->ultimaTransicao = evt->listaTransicao;
        evt->proximo = it;
        ant->proximo = evt;
    }
}

Transicao* getTransicoesEm(Evento* fila, Tempo t)
{
    Evento* it = fila; // iterador de evento
    
    while( it && (it->quando < t)  )
    {
        it = it->proximo;
    }

    if( it && (t == it->quando) )
        return it->listaTransicao;
    else
        return NULL;
}

Transicao* pop_event(Evento** fila)
{
    Transicao* ret = NULL;
    Evento* dead = NULL;

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

void delete_list_transicao(Transicao** list)
{
    Transicao* pt = *list;
    Transicao* pta;

    while (pt) {
        pta = pt;
        pt = pt->proximo;
        free(pta);
    }

    *list = NULL;
}
