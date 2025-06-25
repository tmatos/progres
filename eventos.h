/**
 * @file eventos.h
 * @brief Fila de eventos usada na simulação.
          Não é fila FIFO, a ordem é dada pelo valor de tempo dos eventos.
 */

#ifndef EVENTOS_H

#define EVENTOS_H

#include "estruturas.h"
#include "sinais.h"

/** @brief Enum to represent Verilog system tasks.
 */
typedef enum {
    // Display and Monitoring Tasks
    TASK_DISPLAY,    // $display
    TASK_WRITE,      // $write
    TASK_MONITOR,    // $monitor
    TASK_STIMULUS,   // $stb, $stw
    TASK_FDISPLAY,   // $fdisplay
    TASK_FWRITE,     // $fwrite
    TASK_FMONITOR,   // $fmonitor
    TASK_READMEMB,   // $readmemb
    TASK_READMEMH,   // $readmemh
    TASK_WRITEMEMB,  // $writememb
    TASK_WRITEMEMH,  // $writememh

    // Simulation Control Tasks
    TASK_FINISH,     // $finish
    TASK_STOP,       // $stop
    TASK_RESET,      // $reset (deprecated)

    // Time and Delay Tasks
    TASK_TIME,       // $time
    TASK_REALTIME,   // $realtime
    TASK_Q_TIME,     // $q_time (deprecated)
    TASK_SETUP,      // $setup
    TASK_HOLD,       // $hold
    TASK_SETUPHOLD,  // $setuphold
    TASK_SKEW,       // $skew
    TASK_WIDTH,      // $width
    TASK_NOCHANGE,   // $nochange

    // Logic Value Tasks
    TASK_COUNTZEROS, // $countones (not $countzeros directly, but for counting bits)
    TASK_ONEHOT,     // $onehot
    TASK_ISUNKNOWN,  // $isunknown

    // File I/O Tasks
    TASK_FOPEN,      // $fopen
    TASK_FCLOSE,     // $fclose
    TASK_FGETC,      // $fgetc
    TASK_UNGETC,     // $ungetc
    TASK_FEOF,       // $feof
    TASK_FFLUSH,     // $fflush
    TASK_FSCANF,     // $fscanf
    TASK_FGETS,      // $fgets

    // Conversion and Utility Tasks
    TASK_ITOR,       // $itor
    TASK_RTOI,       // $rtoi
    TASK_CAST,       // $cast (SystemVerilog, but common)
    TASK_RANDOM,     // $random
    TASK_SPRINTF,    // $sformatf (Verilog/SystemVerilog equivalent of sprintf)

    // Deprecated or Less Common Tasks
    TASK_DECAY,      // $decay (deprecated)
    TASK_PULLUP,     // $pullup (deprecated)
    TASK_PULLDOWN,   // $pulldown (deprecated)
    TASK_NOOP,       // Placeholder for no specific task

    TASK_UNKNOWN,
    TASK_UNSUPPORTED
} SystemTask;

typedef struct st_transicao Transicao;

/**
 * @brief Estrutura que representa uma transição do valor lógico de um fio.
          Um lista é formada pelo encadeamento dessas transições.
          Esta lista é referenciada pelo seu primeiro elemento, e temos que o último precede um NULL.
 */
struct st_transicao {
    SystemTask task_type;
    char* task_code;

    Componente fio; // Indica o componente sobre o qual o evento se origina, apenas wires
    Register* reg; // in case of a transition in register value
    ValorLogico novoValor; // Novo valor lógico a ser setado

    Transicao* proximo;
};

typedef Transicao* ListTransicao;

/**
 * @brief Enum para identificação dos tipos de evento que podem ocorrer na simulação.
 */
typedef enum en_event_kind
{
    EVT_NET_TRANSITION,
    EVT_REG_ATTRIBUTION,
    EVT_SYS_TASK,
} EventKind;

typedef struct st_evento Evento;

/**
 * @brief Estrutura para um evento. Um lista é formada pelo encadeamento desses eventos.
          Como toda inserção nessa lista é ordenada por tempo, temos assim uma fila de eventos.
          Esta fila é referenciada pelo seu primeiro elemento, e temos que o último precede um NULL.
 */
struct st_evento {
    Tempo quando; // Indica o instante de ocorrência do evento

    EventKind kind;

    Transicao* listaTransicao;
    Transicao* ultimaTransicao;

    Evento* proximo;
};

typedef Evento* FilaEventos;

/**
 * @brief Desalocar da memória a lista passada.
 */
void delete_list_transicao(Transicao** list);

/** @brief .
 */
void insert_task_event(Evento** fila, Tempo t, SystemTask sys_task, const char* code);

/**
 * @brief Adiciona à fila um evento no tempo t que faz a transição do valor de comp para o novoValor.
          Mas se houver já na fila evento marcado para t, apenas adiciona à lista de transições
          desse evento, a nova transição.
 */
void insert_event(Evento **fila, Tempo t, EventKind k, Componente comp, Register* r, ValorLogico novoValor);

/**
 * @brief Cria um novo evento, sem transições definidas, no tempo t.
 * @param t Tempo em que ocorre o evento.
 * @param k Tipo do evento, conforme EventKind.
 * @return Ponteiro para Evento alocado.
 */
Evento* new_event_at(Tempo t, EventKind k);

/**
 * @brief Retorna uma lista das transições que ocorrem exatamente em determinado tempo t.
          Se não houver evento nesse tempo t, retornará NULL.
 * @return Ponteiro para struct Transicao, ou NULL caso não haja eventos no tempo t.
 */
Transicao* getTransicoesEm(Evento* fila, Tempo t);

/**
 * @brief Remove da fila o evento mais próximo e devolve a lista de transições referente.
 * @return Ponteiro para struct Transicao, ou NULL caso a fila esteja vazia.
 */
Transicao* pop_event(Evento **fila);

#endif // EVENTOS_H
