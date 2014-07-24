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

        }
        else if(t < it->quando) {

        }

    }
}

Transicao getTransicaoEm(Tempo t)
{

}

Transicao* popEvento(Evento **fila)
{

}
