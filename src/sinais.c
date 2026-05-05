/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "progres.h"
#include "sinais.h"
#include "erros.h"
#include "mem.h"
#include "strutil.h"
#include "lex.h"

Signal* new_signal(const char* name)
{
    Signal* sinal = (Signal*) xmalloc(sizeof(Signal));

    if (name)
        set_signal_name(sinal, name);
    else
        set_signal_name(sinal, "");

    sinal->pulses = (Pulse*) xmalloc(sizeof(Pulse));
    set_pulse_blank( sinal->pulses + 0 );
    sinal->total_time = 0;

    return sinal;
}

int set_signal_name(Signal* s, const char* name)
{
    if (!s || !name)
        return 0;

    copy(s->name, name);

    return 1;
}

int set_pulse_blank(Pulse* p)
{
    if (!p)
        return 0;

    p->value = VAL_BLANK;
    p->time = 0;
    p->unit = UN_S;

    return 1;
}

int add_new_pulse(Signal* s, LogicValue value, Time time_length)
{
    int size;
    int count;
    Pulse* it;

    if (!s)
        return 0;

    if (!s->pulses)
        return 0;

    size = 2; // tamanho minimo de array de pulsos com algum valor

    count = 0;
    it = s->pulses;

    // add na contagem a quantidade de pulsos (com valor) ja existente
    while(it->value != VAL_BLANK)
    {
        count++;
        it++;
    }

    size += count;

    if ( (count > 0) && (s->pulses[count - 1].value == value) ) {
        s->pulses[count - 1].time += time_length;
    }
    else {
        s->pulses = (Pulse*) xrealloc( s->pulses, sizeof(Pulse) * size );

        // acessando a penultima posicao, lembre que eh um vetor
        s->pulses[size - 2].value = value;
        s->pulses[size - 2].time = time_length;

        // acessando a ultima posicao
        set_pulse_blank( &(s->pulses[size - 1]) );
    }

    s->total_time += time_length;

    return 1;
}

SignalArray* new_signal_list()
{
    SignalArray* s = (SignalArray*) xmalloc(sizeof(SignalArray));

    if (s) {
        s->count = 0;
        s->itens = NULL;
    }

    return s;
}

void free_signal(Signal** signal)
{
    if ( *signal ) {
        free( (**signal).pulses );
        free( *signal );
        *signal = NULL;
    }
}

void free_signal_list(SignalArray** list)
{
    if ( *list ) {
        for ( int i=0 ; i < (*list)->count ; i++ )
        {
            // NOTE: not using free_signal() here because of reallocs in itens
            free( (*list)->itens[i].pulses );
        }
        
        if ( (*list)->itens ) {
            // NOTE: the array (*list)->itens grows with reallocs
            free( (*list)->itens );
        }

        free( *list );
        *list = NULL;
    }
}

int add_new_signal(SignalArray* list, const char* name)
{
    Signal* s;

    list->count++;

    if ( list->count == 1 ) {
        list->itens = (Signal*) xmalloc(sizeof(Signal));
    }
    else {
        list->itens = (Signal*) xrealloc( list->itens,
                                         sizeof(Signal) * list->count );
    }

    s = list->itens + (list->count - 1);
    set_signal_name(s, name);

    s->pulses = (Pulse*) xmalloc(sizeof(Pulse));
    set_pulse_blank( s->pulses + 0 );
    s->total_time = 0;
    
    return 1;
}

int insert_signal(SignalArray* destin_list, Signal* signal)
{
    Pulse *it = NULL;

    if (!signal)
        return 0;

    if (!destin_list) // FIXME
        destin_list = new_signal_list();

    add_new_signal(destin_list, signal->name);

    it = signal->pulses;
    while(it->value != VAL_BLANK)
    {
        // add cada pulso do sinal original ao novo sinal
        // criado na lista destino (que esta na ultima posicao)
        add_new_pulse( destin_list->itens + (destin_list->count - 1),
                       it->value,
                       it->time );

        it++;
    }

    return 1;
}

TimeUnit get_timeunit_from_str(const char* str)
{
    if ( !str || str[0] == 0x00 )
        goto invalid_timeunit;

    if ( str[1] == 0x00 ) { // str is one char long
        if ( str[0] == 's' )
            return UN_S;
        else
            goto invalid_timeunit;
    }

    // str larger than 2 chars OR second char is not 's'
    if ( str[2] != 0x00 || str[1] != 's' ) 
        goto invalid_timeunit;

    // str has 2 chars AND second char is 's'
    switch (str[0])
    {
    case 'm':
        return UN_MS;
        break;
    case 'u':
        return UN_US;
        break;
    case 'n':
        return UN_NS;
        break;
    case 'p':
        return UN_PS;
        break;
    case 'f':
        return UN_FS;
        break;
    default:
        break;
    }
    
invalid_timeunit:
    return UN_INVALID;
}

const char* get_str_from_timeunit(TimeUnit unit)
{
    switch (unit)
    {
    case UN_S:
        return "s";
    case UN_MS:
        return "ms";
    case UN_US:
        return "us";
    case UN_NS:
        return "ns";
    case UN_PS:
        return "ps";
    case UN_FS:
        return "fs";
    default:
        return "ns";
    }
}

LogicValue long_to_logicvalue(long n)
{
    // Check the least significant bit of n
    // If the LSB is 1, (n & 1) will be 1
    // If the LSB is 0, (n & 1) will be 0
    
    if (n & 1)
        return VAL_1;
    
    return VAL_0;
}
