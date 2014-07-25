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

int addPulso(Sinal* s, ValorLogico valor, Tempo tempo) {
    int tamanho;
    Pulso *it = NULL;

    if(!s)
        return 0;

    if(!s->pulsos)
        return 0;

    tamanho = 1;

    it = s->pulsos;
    while(it->valor != nulo) {
        tamanho++;
        it++;
    }

    tamanho++;

    s->pulsos = (Pulso*) xrealloc( s->pulsos, sizeof(Pulso) * tamanho ); // TODO <---

    // acessando a penúltima posição, lembre q é um vetor!
    s->pulsos[tamanho - 2].valor = valor;
    s->pulsos[tamanho - 2].tempo = tempo;

    setPulsoNulo( &(s->pulsos[tamanho - 1]) );

    return 1;
}

Sinais* novaSinais() {
    Sinais* s = (Sinais*) xmalloc(sizeof(Sinais));

    if(s) {
        s->quantidade = 0;
        s->lista = NULL;
    }

    return s;
}

int addSinal(Sinais* s, char* nome) { // perigoso
    if(!s) {
        s = novaSinais();
    }

    if(s->quantidade == 0) {
        s->quantidade++;

        s->lista = (Sinal*) xmalloc(sizeof(Sinal));

        setSinalNome( s->lista + 0, nome);

        s->lista[0].pulsos = (Pulso*) xmalloc(sizeof(Pulso));

        setPulsoNulo( s->lista[0].pulsos + 0 );
    }
    else {
        s->quantidade++;

        s->lista = (Sinal*) xrealloc( s->lista, sizeof(Sinal) * s->quantidade );

        setSinalNome( s->lista + (s->quantidade - 1), nome); // aritmetica de ponteiro aqui

        s->lista[s->quantidade - 1].pulsos = (Pulso*) xmalloc(sizeof(Pulso));

        setPulsoNulo( s->lista[s->quantidade - 1].pulsos + 0 ); // aritmetica de ponteiro aqui tb
    }

    return 1;
}
