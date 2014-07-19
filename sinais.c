/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sinais.h"
#include "erros.h"

int setSinalNome(Sinal* s, char* nome) {
    if(!s || !nome)
        return 0;

    strcpy(s->nome, nome);

    return 1;
}

int setPulsoNulo(Pulso* p) {
    if(!p)
        return 0;

    p->valor = nulo;
    p->tempo = 0;
    p->unidade = UN_S;

    return 1;
}

int addPulso(Sinal* s, t_valor valor, int tempo) {
    if(!s)
        return 0;

    if(!s->pulsos)
        return 0;

    int tamanho = 1;

    Pulso* it = s->pulsos;
    while(it->valor != nulo) {
        tamanho++;
        it++;
    }

    tamanho++;

    s->pulsos = (Pulso*) realloc( s->pulsos, sizeof(Pulso) * tamanho ); // TODO <---

    if(!s->pulsos)
        erroFatalMemoria();

    // acessando a pen�ltima posi��o, lembre q � um vetor!
    s->pulsos[tamanho - 2].valor = valor;
    s->pulsos[tamanho - 2].tempo = tempo;

    setPulsoNulo( &(s->pulsos[tamanho - 1]) );

    return 1;
}

Sinais* novaSinais() {
    Sinais* s = (Sinais*) malloc(sizeof(Sinais));

    if(s) {
        s->quantidade = 0;
        s->lista = NULL;
    }
    else {
        erroFatalMemoria();
    }

    return s;
}

int addSinal(Sinais* s, char* nome) { // perigoso
    if(!s) {
        s = novaSinais();
    }

    if(s->quantidade == 0) {
        s->quantidade++;

        s->lista = (Sinal*) malloc(sizeof(Sinal));

        if(!s->lista)
            erroFatalMemoria();

        setSinalNome( s->lista + 0, nome);

        s->lista[0].pulsos = (Pulso*) malloc(sizeof(Pulso));

        if( !(s->lista[0].pulsos) )
            erroFatalMemoria();

        setPulsoNulo( s->lista[0].pulsos + 0 );
    }
    else {
        s->quantidade++;

        s->lista = (Sinal*) realloc( s->lista, sizeof(Sinal) * s->quantidade );

        if(!s->lista)
            erroFatalMemoria();

        setSinalNome( s->lista + (s->quantidade - 1), nome); // aritmetica de ponteiro aqui

        s->lista[s->quantidade - 1].pulsos = (Pulso*) malloc(sizeof(Pulso));

        if( !( s->lista[s->quantidade - 1].pulsos ) )
            erroFatalMemoria();

        setPulsoNulo( s->lista[s->quantidade - 1].pulsos + 0 ); // aritmetica de ponteiro aqui tb
    }

    return 1;
}
