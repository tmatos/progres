/**
 * @file eventos.h
 * @brief Fila de eventos usada na simula��o.
          N�o � fila FIFO, a ordem � dada pelo valor de tempo dos eventos.
 */

#ifndef EVENTOS_H

#define EVENTOS_H

#include "estruturas.h"
#include "sinais.h"

typedef struct st_transicao Transicao;

/**
 * @brief Estrutura que representa uma transi��o do valor l�gico de um fio.
          Um lista � formada pelo encadeamento dessas transi��es.
          Esta lista � referenciada pelo seu primeiro elemento, e temos que o �ltimo precede um NULL.
 */
struct st_transicao {
    Componente fio; // Indica o componente sobre o qual o evento se origina, apenas fios
    ValorLogico novoValor; // Novo valor l�gico a ser setado
    Transicao* proximo;
};

typedef struct st_evento Evento;

/**
 * @brief Estrutura para um evento. Um lista � formada pelo encadeamento desses eventos.
          Como toda inser��o nessa lista � ordenada por tempo, temos assim uma fila de eventos.
          Esta fila � referenciada pelo seu primeiro elemento, e temos que o �ltimo precede um NULL.
 */
struct st_evento {
    Tempo quando; // Indica o instante de ocorr�ncia do evento
    Transicao* listaTransicao;
    Transicao* ultimaTransicao;
    Evento* proximo;
};

/**
 * @brief Adiciona � fila um evento no tempo t que faz a transi��o do valor de comp para o novoValor.
          Mas se houver j� na fila evento marcado para t, apenas adiciona � lista de transi��es
          desse evento, a nova transi��o.
 */
void insereEvento(Evento **fila, Tempo t, Componente comp, ValorLogico novoValor);

/**
 * @brief Retorna uma lista das transi��es que ocorrem exatamente em determinado tempo t.
          Se n�o houver evento nesse tempo t, retornar� NULL.
 */
Transicao* getTransicoesEm(Evento* fila, Tempo t);

/**
 * @brief Remove da fila o evento mais pr�ximo e devolve a lista de transi��es referente.
 */
Transicao* popEvento(Evento **fila);

#endif // EVENTOS_H
