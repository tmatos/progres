/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

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

Sinal* new_signal(const char* nome)
{
    Sinal* sinal = (Sinal*) xmalloc(sizeof(Sinal));

    if (nome)
        set_signal_name(sinal, nome);
    else
        set_signal_name(sinal, "");

    sinal->pulsos = (Pulso*) xmalloc(sizeof(Pulso));
    set_pulse_blank( sinal->pulsos + 0 );
    sinal->duracaoTotal = 0;

    return sinal;
}

int set_signal_name(Sinal* s, const char* nome)
{
    if (!s || !nome)
        return 0;

    copy(s->nome, nome);

    return 1;
}

int set_pulse_blank(Pulso* p)
{
    if (!p)
        return 0;

    p->valor = VAL_BLANK;
    p->tempo = 0;
    p->unidade = UN_S;

    return 1;
}

int add_new_pulse(Sinal* s, ValorLogico valor, Tempo duracao)
{
    int tamanho;
    int qtd;
    Pulso* it;

    if (!s)
        return 0;

    if (!s->pulsos)
        return 0;

    tamanho = 2; // tamanho minimo de array de pulsos com algum valor

    qtd = 0;
    it = s->pulsos;

    // add na contagem a qtd de pulsos (com valor) ja existente
    while(it->valor != VAL_BLANK)
    {
        qtd++;
        it++;
    }

    tamanho += qtd;

    if ( (qtd > 0) && (s->pulsos[qtd - 1].valor == valor) ) {
        s->pulsos[qtd - 1].tempo += duracao;
    }
    else {
        s->pulsos = (Pulso*) xrealloc( s->pulsos, sizeof(Pulso) * tamanho );

        // acessando a penultima posicao, lembre que eh um vetor
        s->pulsos[tamanho - 2].valor = valor;
        s->pulsos[tamanho - 2].tempo = duracao;

        // acessando a ultima posicao
        set_pulse_blank( &(s->pulsos[tamanho - 1]) );
    }

    s->duracaoTotal += duracao;

    return 1;
}

Sinais* new_signal_list()
{
    Sinais* s = (Sinais*) xmalloc(sizeof(Sinais));

    if (s) {
        s->quantidade = 0;
        s->lista = NULL;
    }

    return s;
}

void free_signal_list(Sinais** list)
{
    int i;

    if ( *list ) {
        for ( i=0 ; i < (*list)->quantidade ; i++ )
        {
            free( (*list)->lista[i].pulsos );
        }
        
        if ( (*list)->lista ) {
            free( (*list)->lista );
        }

        free( *list );
        *list = NULL;
    }
}

int add_new_signal(Sinais* list, const char* nome)
{
    Sinal* s;

    list->quantidade++;

    if ( list->quantidade == 1 ) {
        list->lista = (Sinal*) xmalloc(sizeof(Sinal));
    }
    else {
        list->lista = (Sinal*) xrealloc( list->lista,
                                         sizeof(Sinal) * list->quantidade );
    }

    s = list->lista + (list->quantidade - 1);
    set_signal_name(s, nome);

    s->pulsos = (Pulso*) xmalloc(sizeof(Pulso));
    set_pulse_blank( s->pulsos + 0 );
    s->duracaoTotal = 0;
    
    return 1;
}

int insert_signal(Sinais* destin_list, Sinal* signal)
{
    Pulso *it = NULL;

    if (!signal)
        return 0;

    if (!destin_list) // FIXME
        destin_list = new_signal_list();

    add_new_signal(destin_list, signal->nome);

    it = signal->pulsos;
    while(it->valor != VAL_BLANK)
    {
        // add cada pulso do sinal original ao novo sinal
        // criado na lista destino (que esta na ultima posicao)
        add_new_pulse( destin_list->lista + (destin_list->quantidade - 1),
                       it->valor,
                       it->tempo );

        it++;
    }

    return 1;
}

UnidTempo get_timeunit_from_str(const char* str)
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

const char* get_str_from_timeunit(UnidTempo unit)
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

ValorLogico long_to_logicvalue(long n)
{
    // Check the least significant bit of n
    // If the LSB is 1, (n & 1) will be 1
    // If the LSB is 0, (n & 1) will be 0
    
    if (n & 1)
        return VAL_1;
    
    return VAL_0;
}
