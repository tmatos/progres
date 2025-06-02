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
#include "lex.h"

Sinal* novoSinal(const char* nome)
{
    Sinal* sinal = (Sinal*) xmalloc(sizeof(Sinal));

    if(nome)
        setSinalNome(sinal, nome);
    else
        setSinalNome(sinal, "");

    sinal->pulsos = (Pulso*) xmalloc(sizeof(Pulso));
    setPulsoNulo( sinal->pulsos + 0 );
    sinal->duracaoTotal = 0;

    return sinal;
}

int setSinalNome(Sinal* s, const char* nome)
{
    if(!s || !nome)
        return 0;

    copy(s->nome, nome);

    return 1;
}

int setPulsoNulo(Pulso* p)
{
    if(!p)
        return 0;

    p->valor = VAL_BLANK;
    p->tempo = 0;
    p->unidade = UN_S;

    return 1;
}

int addPulso(Sinal* s, ValorLogico valor, Tempo duracao)
{
    int tamanho = 1;
    Pulso* it = NULL;

    if(!s)
        return 0;

    if(!s->pulsos)
        return 0;

    it = s->pulsos;
    while(it->valor != VAL_BLANK) {
        tamanho++;
        it++;
    }

    tamanho++;
    s->pulsos = (Pulso*) xrealloc( s->pulsos, sizeof(Pulso) * tamanho );

    // acessando a penultima posicao, lembre q eh um vetor!
    s->pulsos[tamanho - 2].valor = valor;
    s->pulsos[tamanho - 2].tempo = duracao;

    setPulsoNulo( &(s->pulsos[tamanho - 1]) );

    s->duracaoTotal += duracao;

    return 1;
}

Sinais* novaSinais()
{
    Sinais* s = (Sinais*) xmalloc(sizeof(Sinais));

    if(s) {
        s->quantidade = 0;
        s->lista = NULL;
    }

    return s;
}

int addSinal(Sinais* s, const char* nome)
{
    if(!s)
        s = novaSinais();

    if(s->quantidade == 0) {
        s->quantidade++;
        s->lista = (Sinal*) xmalloc(sizeof(Sinal));
        setSinalNome( s->lista + 0, nome );

        s->lista[0].pulsos = (Pulso*) xmalloc(sizeof(Pulso));
        setPulsoNulo( s->lista[0].pulsos + 0 );
        s->lista[0].duracaoTotal = 0;

        return 1;
    }
    
    s->quantidade++;
    s->lista = (Sinal*) xrealloc( s->lista, sizeof(Sinal) * s->quantidade );
    setSinalNome( s->lista + (s->quantidade - 1), nome); // aritmetica de ponteiro aqui

    s->lista[s->quantidade - 1].pulsos = (Pulso*) xmalloc(sizeof(Pulso));
    setPulsoNulo( s->lista[s->quantidade - 1].pulsos + 0 ); // aritmetica de ponteiro aqui tb
    s->lista[s->quantidade - 1].duracaoTotal = 0;
    
    return 1;
}

int addSinalPronto(Sinais* ls, Sinal* sinal)
{
    Pulso *it = NULL;

    if(!sinal)
        return 0;

    if(!ls)
        ls = novaSinais();

    addSinal(ls, sinal->nome);

    it = sinal->pulsos;
    while(it->valor != VAL_BLANK) {
        // adiciona cada pulso do sinal original para o novo sinal da lista (ou seja, o ultimo)
        addPulso( ls->lista + (ls->quantidade - 1), it->valor, it->tempo );
        it++;
    }

    return 1;
}

UnidTempo get_timeunit_from_str(const char* str)
{
    if (!str)
        return UN_INVALID;

    if (iguais(str, "s")) {
        return UN_S;
    } else if (iguais(str, "ms")) {
        return UN_MS;
    } else if (iguais(str, "us")) {
        return UN_US;
    } else if (iguais(str, "ns")) {
        return UN_NS;
    } else if (iguais(str, "ps")) {
        return UN_PS;
    } else if (iguais(str, "fs")) {
        return UN_FS;
    }
    
    return UN_INVALID;
}
