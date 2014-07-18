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

t_circuito* novoCircuito() {
    t_circuito* circuito = (t_circuito*) malloc(sizeof(t_circuito));

    circuito->numEntrada = 0;
    circuito->listaFiosEntrada = NULL;
    circuito->sinaisEntrada = NULL;

    circuito->numSaida = 0;
    circuito->listaFiosSaida = NULL;
    circuito->sinaisSaida = NULL;

    circuito->numWires = 0;
    circuito->listaWires = NULL;

    circuito->numPortas = 0;
    circuito->listaPortas = NULL;

    return circuito;
}

int adicionaEntrada(t_circuito* circ, Componente comp) {
    if(!circ || !comp)
        return 0;

    circ->numEntrada++;

    if(!circ->listaFiosEntrada) { //TODO: checagens de memoria
        circ->listaFiosEntrada = (Componente*) malloc(sizeof(Componente));
        circ->listaFiosEntrada[0] = comp; //(Componente) malloc(sizeof(struct st_componente));
    }
    else { //TODO: checagens de memoria
        circ->listaFiosEntrada = (Componente*) realloc( circ->listaFiosEntrada, sizeof(Componente) * circ->numEntrada );
        circ->listaFiosEntrada[circ->numEntrada - 1] = comp;
    }

    return 1;
}

int adicionaSaida(t_circuito* circ, Componente comp) {
    if(!circ || !comp)
        return 0;

    circ->numSaida++;

    if(!circ->listaFiosSaida) { //TODO: checagens de memoria
        circ->listaFiosSaida = (Componente*) malloc(sizeof(Componente));
        circ->listaFiosSaida[0] = comp; //(Componente) malloc(sizeof(struct st_componente));
    }
    else { //TODO: checagens de memoria
        circ->listaFiosSaida = (Componente*) realloc( circ->listaFiosSaida, sizeof(Componente) * circ->numSaida );
        circ->listaFiosSaida[circ->numSaida - 1] = comp;
    }

    return 1;
}

Componente* novaListaCompon(int tamanho) {
    return (Componente*) malloc(sizeof(Componente) * tamanho);
}

Componente novoComponente(char* nome, t_operador porta) {
    Componente c = (Componente) malloc(sizeof(struct st_componente));

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

Componente getComponentePorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    int i;

    for( i=0 ; i < circ->numPortas ; i++ ) {
        if( iguais( circ->listaPortas[i]->nome, nome ) ) {
            return circ->listaPortas[i];
        }
    }

    return NULL;
}

Componente getWirePorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    int i;

    for( i=0 ; i < circ->numWires ; i++ ) {
        if( iguais( circ->listaWires[i]->nome, nome ) ) {
            return circ->listaWires[i];
        }
    }

    return NULL;
}

Componente getInputPorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    int i;

    for( i=0 ; i < circ->numEntrada ; i++ ) {
        if( iguais( circ->listaFiosEntrada[i]->nome, nome ) ) {
            return circ->listaFiosEntrada[i];
        }
    }

    return NULL;
}

Componente getOutputPorNome(t_circuito* circ, char* nome) {
    if(!circ || !nome)
        return NULL;

    int i;

    for( i=0 ; i < circ->numSaida ; i++ ) {
        if( iguais( circ->listaFiosSaida[i]->nome, nome ) ) {
            return circ->listaFiosSaida[i];
        }
    }

    return NULL;
}
