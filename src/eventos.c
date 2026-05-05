/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

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

Event* new_event_at(Time t, EventKind k)
{
    Event* e = (Event*) xmalloc(sizeof(Event));
    e->instant = t;
    e->kind = k;
    e->list_transition = NULL;
    e->last_transition = NULL;
    e->next = NULL;

    return e;
}

Event* new_empty_event()
{
    return new_event_at( (Time)0, EVT_SYS_TASK );
}

void insert_event(
    Event** queue,
    Time t,
    EventKind kind,
    Component* comp,
    Register* reg,
    LogicValue new_value)
{
    Event* evt = NULL;
    Event* ant = NULL; // evento anterior
    Event* it = NULL; // iterador para eventos

    if (!comp)
        return;

    evt = new_event_at(t, kind);

    evt->list_transition = (Transition*) xmalloc(sizeof(Transition));
    
    evt->list_transition->net = comp;
    evt->list_transition->reg = reg;
    evt->list_transition->new_value = new_value;
    evt->list_transition->next = NULL;

    evt->list_transition->task_type = IS_NOT_A_TASK;
    evt->list_transition->task_args.count = 0;
    evt->list_transition->task_args.itens = NULL;
    evt->list_transition->task_args.types = NULL;

    // empty queue
    if ( !(*queue) ) {
        evt->last_transition = evt->list_transition;
        *queue = evt;

        return;
    }

    ant = NULL; // evento anterior
    it = *queue; // event iterator
    while ( it && (it->instant < t) )
    {
        ant = it;
        it = it->next;
    }

     // insert the event at the last position in queue
    if (!it) {
        evt->last_transition = evt->list_transition;

        evt->next = it;
        ant->next = evt;

        return;
    }

    // um evento no instante existe, adicionar a lista de transicoes
    if (t == it->instant)
    {
        if (it->last_transition)
            it->last_transition->next = evt->list_transition;
        else
            it->list_transition = evt->list_transition;

        it->last_transition = evt->list_transition;

        free(evt);

        return;
    }

    if (t < it->instant)
    {
        // insert event at the beginning of the queue
        if (ant == NULL) {
            evt->last_transition = evt->list_transition;
            evt->next = it;
            *queue = evt; // it becomes the first in the queue

            return;
        }
        
        // insert between two events, the previous and next
        evt->last_transition = evt->list_transition;
        evt->next = it;
        ant->next = evt;
    }
}

void delete_event_queue(Event** queue)
{
    if ( (queue == NULL) || (*queue == NULL) ) {
        return;
    }

    Event* evt_it = *queue;

    while (evt_it)
    {
        if ( evt_it->list_transition ) {
            delete_list_transition( &(evt_it->list_transition) );
        }

        Event* tmp = evt_it;
        evt_it = evt_it->next;
        free(tmp);
    }

    *queue = NULL;
}

void insert_task_event(
    Event** queue,
    Time t,
    SystemTask sys_task,
    ListSystemTaskArg sys_task_args)
{
    Event* evt = NULL;
    Event* ant = NULL; // evento anterior
    Event* it = NULL;

    evt = new_event_at(t, EVT_SYS_TASK);

    evt->list_transition = (Transition*) xmalloc(sizeof(Transition));

    evt->list_transition->task_type = sys_task;
    evt->list_transition->task_args = sys_task_args;

    evt->list_transition->net = NULL;
    evt->list_transition->reg = NULL;
    evt->list_transition->new_value = VAL_BLANK;
    evt->list_transition->next = NULL;

    // empty queue case
    if ( !(*queue) ) {
        evt->last_transition = evt->list_transition;
        *queue = evt;

        return;
    }

    // search for the position
    ant = NULL;
    it = *queue;
    while ( it && (it->instant < t) )
    {
        ant = it;
        it = it->next;
    }

     // insert the event at the last position in queue
    if (!it) {
        evt->last_transition = evt->list_transition;

        evt->next = it;
        ant->next = evt;

        return;
    }

    // an event exists at same instant, so
    // append the new evt transi list to the existing evt transi list
    if (t == it->instant)
    {
        if (it->last_transition)
            it->last_transition->next = evt->list_transition;
        else
            it->list_transition = evt->list_transition;

        it->last_transition = evt->list_transition;

        free(evt);

        return;
    }

    if (t < it->instant)
    {
        // insert event at the beginning of the queue
        if (ant == NULL) {
            evt->last_transition = evt->list_transition;
            evt->next = it;
            *queue = evt; // it becomes the first in the queue

            return;
        }
        
        // insert between two events, the previous and next
        evt->last_transition = evt->list_transition;
        evt->next = it;
        ant->next = evt;
    }
}

Transition* get_transitions_at_time(Event* queue, Time t)
{
    Event* it = queue; // event iterator
    
    while( it && (it->instant < t)  )
    {
        it = it->next;
    }

    if( it && (t == it->instant) )
        return it->list_transition;
    else
        return NULL;
}

Transition* pop_event(Event** queue)
{
    Transition* ret = NULL;
    Event* dead = NULL;

    if(!queue)
        return NULL;

    if( !(*queue) )
        return NULL;

    if( ! (*queue)->list_transition )
        return NULL;

    ret = (*queue)->list_transition;

    dead = (*queue);
    (*queue) = (*queue)->next;
    free(dead);

    return ret;
}

void delete_list_transition(Transition** list)
{
    if ( (list == NULL) || (*list == NULL) ) {
        return;
    }

    Transition* pt = *list;

    while (pt)
    {
        Transition* pta = pt;
        pt = pt->next;
        free(pta);
    }

    *list = NULL;
}
