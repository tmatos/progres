/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "progres.h"
#include "sinais.h"
#include "estruturas.h"
#include "lex.h"
#include "erros.h"
#include "mem.h"

t_circuito* novoCircuito()
{
    t_circuito *circuito = (t_circuito*) xmalloc(sizeof(t_circuito));

    circuito->listaFiosEntrada = novaListaComponente();
    circuito->sinaisEntrada = NULL;

    circuito->listaFiosSaida = novaListaComponente();
    circuito->sinaisSaida = NULL;

    circuito->listaWires = novaListaComponente();

    circuito->listaPortas = novaListaComponente();

    return circuito;
}

void adicionaEntrada(t_circuito* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaFiosEntrada, comp);
}

void adicionaSaida(t_circuito* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaFiosSaida, comp);
}

void adicionaWire(t_circuito* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaWires, comp);
}

void adicionaPorta(t_circuito* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaPortas, comp);
}

ListaComponente* novaListaComponente()
{
    return novaListaComponenteTamanho(0);
}

ListaComponente* novaListaComponenteTamanho(int tamanho)
{
    int i;
    ListaComponente *listaCp;

    listaCp = (ListaComponente*) xmalloc(sizeof(ListaComponente));
    listaCp->tamanho = tamanho;

    if(tamanho == 0) {
        listaCp->itens = NULL;
    }
    else {
        listaCp->itens = (Componente*) xmalloc(sizeof(Componente) * tamanho);

        for(i=0 ; i<tamanho ; i++)
            listaCp->itens[i] = NULL;
    }

    return listaCp;
}

void insereComponente(ListaComponente* ls, Componente cp)
{
    if(!ls)
        ls = novaListaComponente();

    if(ls->tamanho == 0) {
        ls->tamanho++;
        ls->itens = (Componente*) xmalloc(sizeof(Componente));
    }
    else {
        ls->tamanho++;
        ls->itens = (Componente*) xrealloc( ls->itens, sizeof(Componente) * ls->tamanho );
    }

    ls->itens[ls->tamanho - 1] = cp;
}

int contemComponente(ListaComponente* ls, Componente cp)
{
    int i;

    if(!ls || !cp)
        return 0;

    for( i=0 ; i < ls->tamanho ; i++ )
    {
        if( ls->itens[i] == cp ) {
            return 1;
        }
    }

    return 0;
}

Componente novoComponente(char* nome, t_operador porta) {
    Componente c = (Componente) xmalloc(sizeof(struct st_componente));

    strcpy(c->nome, nome);
    c->tipo.operador = porta;
    c->tipo.atraso = 0; // atraso default é zero

    c->listaEntrada = novaListaComponente();
    c->sinalEntrada = NULL;

    c->listaSaida = novaListaComponente();
    c->sinalSaida = NULL;

    c->valorDinamico = x;

    return c;
}

Componente getComponenteItemPorNome(ListaComponente* ls, char* nome)
{
    int i;

    if(!ls || !nome)
        return NULL;

    for( i=0 ; i < ls->tamanho ; i++ )
    {
        if( iguais( ls->itens[i]->nome, nome ) )
        {
           return ls->itens[i];
        }
    }

    return NULL;
}

Componente getPortaPorNome(t_circuito* circ, char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaPortas, nome);
}

Componente getWirePorNome(t_circuito* circ, char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaWires, nome);
}

Componente getInputPorNome(t_circuito* circ, char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosEntrada, nome);
}

Componente getOutputPorNome(t_circuito* circ, char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosSaida, nome);
}
