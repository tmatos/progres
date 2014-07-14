/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdlib.h>
#include <string.h>
#include "estruturas.h"

t_circuito* novoCircuito() {
    t_circuito* circuito = (t_circuito*) malloc(sizeof(t_circuito));

    circuito->numEntrada = 0;
    circuito->numSaida = 0;
    circuito->sinaisEntrada = NULL;
    circuito->sinaisSaida = NULL;
    circuito->listaFiosEntrada = NULL;
    circuito->listaFiosSaida = NULL;

    return circuito;
}

Componente* novaListaCompon(int tamanho) {
    return (Componente*) malloc(sizeof(Componente) * tamanho);
}

Componente novoComponente(char* nome, t_tipo tipo) {
    Componente c = (Componente) malloc(sizeof(struct st_componente));

    strcpy(c->nome, nome);
    c->tipo = tipo;

    c->numEntrada = 0;
    c->numSaida = 0;
    c->sinalEntrada = NULL;
    c->saida = NULL;
    c->listaEntrada = NULL;
    c->listaSaida = NULL;

    return c;
}
