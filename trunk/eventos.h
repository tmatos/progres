/**
 * @file eventos.h
 * @brief Fila de eventos usada na simulação.
        Não é fila FIFO, a ordem é dada pelo valor de tempo dos eventos.
 */

#ifndef EVENTOS_H

#define EVENTOS_H

#include "estruturas.h"
#include "sinais.h"

typedef struct st_transicao Transicao;

/**
 * @brief .
 */
struct st_transicao {
    Componente fio; // Indica o componente sobre o qual o evento se origina, apenas fios
    ValorLogico novoValor; // Novo valor lógico a ser setado
    Transicao* proximo;
};

typedef struct st_evento Evento;

/**
 * @brief .
 */
struct st_evento {
    Tempo quando; // Indica o instante de ocorrência do evento
    Transicao* listaTransicao;
    Transicao* ultimaTransicao;
    Evento* proximo;
};

/**
 * @brief .
 */
struct st_fila_eventos {
};

/**
 * @brief .
 */
void insereEvento(Evento **fila, Tempo t, Componente comp, ValorLogico novoValor);

/**
 * @brief .
 */
Transicao* getTransicoesEm(Evento* fila, Tempo t);

/**
 * @brief .
 */
Transicao* popEvento(Evento **fila);

#endif
