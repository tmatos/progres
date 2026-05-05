/**
 * @file eventos.h
 * @brief Fila de eventos usada na simulação.
          Não é fila FIFO, a ordem é dada pelo valor de tempo dos eventos.
 */

#ifndef EVENTOS_H
#define EVENTOS_H

#include "lex.h"
#include "estruturas.h"
#include "sinais.h"

#ifdef __cplusplus
extern "C" {
#endif

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

    // Dumping and Logging Tasks
    TASK_DUMPFILE,   // $dumpfile
    TASK_DUMPVARS,   // $dumpvars

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
    TASK_UNSUPPORTED,
    IS_NOT_A_TASK
} SystemTask;

/**
 * @brief Contains a single argument of a system task.
 */
typedef union un_systemtask_arg {
    char string_literal[MAX_TOKEN_SIZE];
    int number_literal;
    LogicValue logic_value;
    Component* net;
    Register* reg;
} SystemTaskArg;

/** @brief Enum to identify the type of a system task argument, which can be
 *         a string literal, a number literal, a logic value, a pointer
 *         to a net, or a pointer to a register, for now.
 */
typedef enum en_systemtask_arg_type {
    ARG_STRING_LITERAL,
    ARG_NUMBER_LITERAL,
    ARG_LOGIC_VALUE,
    ARG_NET_POINTER,
    ARG_REG_POINTER
} SystemTaskArgType;

/**
 * @brief Contains a list of arguments for a system task.
 *        The count field indicates how many arguments are in the list,
 *        and the itens field is an array of pointers to SystemTaskArg.
 */
typedef struct st_list_systemtaskarg {
    unsigned int count;
    SystemTaskArg* itens;
    SystemTaskArgType* types; // parallel array to 'itens' indicating type of each arg
} ListSystemTaskArg;

typedef struct st_transition Transition;

/**
 * @brief Estrutura que representa uma transição do valor lógico de um wire.
          Uma lista é formada pelo encadeamento dessas transições.
          Esta lista é referenciada pelo seu primeiro elemento,
          e temos que o último precede um NULL.
 */
struct st_transition {
    SystemTask task_type;
    ListSystemTaskArg task_args;

    Component* net; // indicates the net over wich the event originates
    Register* reg; // in case of a transition in register value
    LogicValue new_value; // new value of the net/register after transition

    Transition* next;
};

typedef Transition* ListTransicao;

/**
 * @brief Enum para identificação dos tipos de evento que podem ocorrer
 *        durante a simulação.
 */
typedef enum en_event_kind
{
    EVT_NET_TRANSITION,
    EVT_REG_ATTRIBUTION,
    EVT_SYS_TASK,
} EventKind;

typedef struct st_event Event;

/**
 * @brief Estrutura para um evento. Um lista é formada pelo encadeamento
 *        desses eventos. Como toda inserção nessa lista é ordenada por tempo,
 *        temos assim uma fila de eventos. Esta fila é referenciada pelo seu
 *        primeiro elemento, e temos que o último precede um NULL.
 */
struct st_event {
    Time instant; // indicates the instant at which the event occurs

    EventKind kind;

    Transition* list_transition;
    Transition* last_transition;

    Event* next;
};

typedef Event* EventQueue;

/** @brief Desalocar da memória a lista passada.
 *  @param list Ponteiro para a lista de transições a ser desalocada.
 *  @note A lista é desalocada, mas não os componentes ou registradores
 *        que ela possa estar referenciando.
 */
void delete_list_transition(Transition** list);

/** @brief Create a new event (or queue) at time 0 without a transi list.
 *         This is used to initialize the event queue, if needed.
 *         The only event will have no transitions defined.
 * @return Pointer to the allocated Event struct.
 */
Event* new_empty_event();

/** @brief Insere um novo evento na fila de eventos, ordenando-o pelo tempo.
 *         Se já houver um evento no tempo t, adiciona à lista de transições
 *         desse evento.
 * @param queue Ponteiro para a fila de eventos.
 * @param t Tempo em que ocorre o evento.
 * @param sys_task Tipo desta system task, conforme a enum SystemTask.
 * @param sys_task_args Lista de argumentos fornecidos à task em questão.
 */
void insert_task_event(
    Event** queue,
    Time t,
    SystemTask sys_task,
    ListSystemTaskArg sys_task_args);

/** @brief Adiciona à fila um evento no tempo t que faz a transição do valor de
 *         comp para o new_value. Mas, havendo já na fila evento marcado para t,
 *         apenas adiciona à lista de transições desse evento, a nova transição.
 * @param queue Pointer to the event queue.
 * @param t Time at which the event occurs.
 * @param kind Type of the event, as per EventKind.
 * @param comp Component that undergoes the transition.
 * @param reg Register that undergoes the transition (if applicable, else NULL).
 * @param new_value New value of the component or register after the transition.
 * @note If there is already an event at time t, the new transition is added
 *       to that event's transition list.
 */
void insert_event(
    Event** queue,
    Time t,
    EventKind kind,
    Component* comp,
    Register* reg,
    LogicValue new_value);

/** @brief Libera completamente a fila de eventos da memória.
 *  @param queue Ponteiro para a fila de eventos a ser liberada.
 */
void delete_event_queue(Event **queue);

/** @brief Cria um novo evento, sem transições definidas, no tempo t.
 *  @param t Tempo em que ocorre o evento.
 *  @param k Tipo do evento, conforme EventKind.
 *  @return Ponteiro para Event alocado.
 */
Event* new_event_at(Time t, EventKind k);

/** @brief Retorna uma lista das transições que ocorrem exatamente em determinado tempo t.
 *         Se não houver evento nesse tempo t, retornará NULL.
 *  @param queue Ponteiro para a fila de eventos.
 *  @param t Tempo em que se deseja obter as transições.
 *  @return Ponteiro para struct Transition, ou NULL caso não haja eventos no tempo t.
 */
Transition* get_transitions_at_time(Event* queue, Time t);

/** @brief Remove da fila o evento mais próximo e devolve a lista de transições referente.
 *  @param queue Ponteiro para a fila de eventos.
 *  @return Ponteiro para struct Transition, ou NULL caso a fila esteja vazia.
 *  @note A fila é atualizada, removendo o evento mais próximo.
 */
Transition* pop_event(Event** queue);

#ifdef __cplusplus
}
#endif

#endif // EVENTOS_H
