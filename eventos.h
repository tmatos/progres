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
 * @brief Estrutura que representa uma transição do valor lógico de um fio.
          Um lista é formada pelo encadeamento dessas transições.
          Esta lista é referenciada pelo seu primeiro elemento, e temos que o último precede um NULL.
 */
struct st_transicao {
    Componente fio; // Indica o componente sobre o qual o evento se origina, apenas fios
    ValorLogico novoValor; // Novo valor lógico a ser setado
    Transicao* proximo;
};

typedef struct st_evento Evento;

/**
 * @brief Estrutura para um evento. Um lista é formada pelo encadeamento desses eventos.
          Como toda inserção nessa lista é ordenada por tempo, temos assim uma fila de eventos.
          Esta fila é referenciada pelo seu primeiro elemento, e temos que o último precede um NULL.
 */
struct st_evento {
    Tempo quando; // Indica o instante de ocorrência do evento
    Transicao* listaTransicao;
    Transicao* ultimaTransicao;
    Evento* proximo;
};

/**
 * @brief Adiciona à fila um evento no tempo t que faz a transição do valor de comp para o novoValor.
          Mas se houver já na fila evento marcado para t, apenas adiciona à lista de transições
          desse evento, a nova transição.
 */
void insereEvento(Evento **fila, Tempo t, Componente comp, ValorLogico novoValor);

/**
 * @brief Retorna uma lista das transições que ocorrem exatamente em determinado tempo t.
          Se não houver evento nesse tempo t, retornará NULL.
 */
Transicao* getTransicoesEm(Evento* fila, Tempo t);

/**
 * @brief Remove da fila o evento mais próximo e devolve a lista de transições referente.
 */
Transicao* popEvento(Evento **fila);

#endif // EVENTOS_H
