/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sinais.h"
#include "estruturas.h"
#include "lex.h"
#include "erros.h"

t_circuito* novoCircuito() {
    t_circuito* circuito = (t_circuito*) malloc(sizeof(t_circuito));

    if(!circuito)
        erroFatalMemoria();

    circuito->listaFiosEntrada = novaListaComponente();
    circuito->sinaisEntrada = NULL;

    circuito->listaFiosSaida = novaListaComponente();
    circuito->sinaisSaida = NULL;

    circuito->listaWires = novaListaComponente();

    circuito->listaPortas = novaListaComponente();

    return circuito;
}

int adicionaEntrada(t_circuito* circ, Componente comp) {
    if(!circ || !comp)
        return 0;

    insereComponente(circ->listaFiosEntrada, comp);

    return 1;
}

int adicionaSaida(t_circuito* circ, Componente comp) {
    if(!circ || !comp)
        return 0;

    insereComponente(circ->listaFiosSaida, comp);

    return 1;
}

ListaComponente* novaListaComponente() {
    return novaListaComponenteTamanho(0);
}

ListaComponente* novaListaComponenteTamanho(int tamanho) {
    ListaComponente* listaCp = (ListaComponente*) malloc(sizeof(ListaComponente));

    if(!listaCp)
        erroFatalMemoria();

    listaCp->tamanho = tamanho;

    if(tamanho == 0) {
        listaCp->itens = NULL;
    }
    else {
        listaCp->itens = (Componente*) malloc(sizeof(Componente) * tamanho);

        if(!listaCp->itens)
            erroFatalMemoria();

        int i;

        for(i=0 ; i<tamanho ; i++)
            listaCp->itens[i] = NULL;
    }

    return listaCp;
}

void insereComponente(ListaComponente* ls, Componente cp) {
    if(!ls)
        ls = novaListaComponente();

    if(ls->tamanho == 0) {
        ls->tamanho++;
        ls->itens = (Componente*) malloc(sizeof(Componente));
    }
    else {
        ls->tamanho++;
        ls->itens = (Componente*) realloc( ls->itens, sizeof(Componente) * ls->tamanho );
    }

    if(!ls->itens)
        erroFatalMemoria();

    ls->itens[ls->tamanho - 1] = cp;
}

Componente novoComponente(char* nome, t_operador porta) {
    Componente c = (Componente) malloc(sizeof(struct st_componente));

    if(!c)
        erroFatalMemoria();

    strcpy(c->nome, nome);
    c->tipo.operador = porta;
    c->tipo.atraso = 0; // atraso default é zero

    c->numEntrada = 0;
    c->listaEntrada = NULL;
    c->sinalEntrada = NULL;

    c->numSaida = 0;
    c->listaSaida = NULL;
    c->sinalSaida = NULL;

    return c;
}

Componente getComponenteItemPorNome(ListaComponente* ls, char* nome) {
    if(!ls || !nome)
        return NULL;

    int i;

    for( i=0 ; i < ls->tamanho ; i++ ) {
        if( iguais( ls->itens[i]->nome, nome ) ) {
           return ls->itens[i];
        }
    }

    return NULL;
}

Componente getPortaPorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaPortas, nome);
}

Componente getWirePorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaWires, nome);
}

Componente getInputPorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosEntrada, nome);
}

Componente getOutputPorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosSaida, nome);
}
