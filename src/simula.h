/**
 * @file simula.h
 * @brief Protótipos da função principal da simulação.
 */

#ifndef SIMULA_H
#define SIMULA_H

#include <stdio.h>

#include "estruturas.h"
#include "eventos.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Função que faz a simulação do circuito com as entradas especificadas.
           Em caso de sucesso, retorna as saídas dessa simulação.
    @param module Ponteiro para um Module, já inicializado.
    @param input_stimulus Ponteiro para a struct SignalArray, contendo entradas
                          para p circuito.
    @param initial_events Ponteiro para a fila de eventos iniciais.
    @param f_dump Ponteiro para o ponteiro do handler do arquivo de dump (VCD)
                  resultante da simulação.
    @return Um ponteiro para uma estrutura SignalArray contendo as saídas do circuito
            após a simulação. Retorna NULL se houver algum erro ou se as entradas
            não corresponderem às esperadas no module.
 */
SignalArray* simula(
        Module* module,
        SignalArray* input_stimulus,
        Event** initial_events,
        FILE** f_dump);

/** @brief Valida se os sinais de entrada fornecidos correspondem
 *         aos esperados pelo módulo.
 *  @param module Ponteiro para o módulo a ser simulado.
 *  @param signals Ponteiro para a estrutura SignalArray contendo os sinais de entrada.
 *  @return O número de sinais de entrada válidos encontrados.
 */
int validate_input_signals(Module* module, SignalArray* signals);

/** @brief Simulação da avaliação da porta 'buf'.
 *  @param input Valor lógico de entrada.
 *  @return O valor lógico resultante da operação BUF.
 */
LogicValue compute_buf_gate(LogicValue input);

/** @brief Simulação da avaliação da porta 'not'.
 *  @param input Valor lógico de entrada.
 *  @return O valor lógico resultante da operação NOT.
 */
LogicValue compute_not_gate(LogicValue input);

/** @brief Simulação da avaliação da porta 'xor'.
 *  @param a Primeiro valor lógico de entrada.
 *  @param b Segundo valor lógico de entrada.
 *  @return O valor lógico resultante da operação XOR.
 */
LogicValue compute_xor_gate(LogicValue a, LogicValue b);

/** @brief Simulação da avaliação da porta 'xnor'.
 *  @param a Primeiro valor lógico de entrada.
 *  @param b Segundo valor lógico de entrada.
 *  @return O valor lógico resultante da operação XNOR.
 */
LogicValue compute_xnor_gate(LogicValue a, LogicValue b);

/** @brief Simulação da avaliação da porta 'or' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta OR.
 *  @return O valor lógico resultante da operação OR.
 */
LogicValue compute_or_gate(ListComponent* inputs);

/** @brief Simulação da avaliação da porta 'and' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta AND.
 *  @return O valor lógico resultante da operação AND.
 */
LogicValue compute_and_gate(ListComponent* inputs);

/** @brief Simulação da avaliação da porta 'nor' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta NOR.
 *  @return O valor lógico resultante da operação NOR.
 */
LogicValue compute_nor_gate(ListComponent* inputs);

/** @brief Simulação da avaliação da porta 'nand' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta NAND.
 *  @return O valor lógico resultante da operação NAND.
 */
LogicValue compute_nand_gate(ListComponent* inputs);

/** @brief Computes the output of a tri-state buffer (bufif0 type).
 *  This function models a buffer that is enabled when the control signal is low (0).
 *  @param control The control signal. If VAL_1, the buffer is in high-impedance (VAL_Z).
 *  @param data The input data signal.
 *  @return The resulting logic value.
 */
LogicValue compute_buf_if0_gate(LogicValue control, LogicValue data);

/** @brief Computes the output of a tri-state buffer (bufif1 type).
 *  This function models a buffer that is enabled when the control signal is high (1).
 *  @param control The control signal. If VAL_0, the buffer is in high-impedance (VAL_Z).
 *  @param data The input data signal.
 *  @return The resulting logic value.
 */
LogicValue compute_buf_if1_gate(LogicValue control, LogicValue data);

/** @brief Simulação da porta de 3 estados sobre suas duas entradas.
 *  @param control Sinal de controle. Se for VAL_1, a saída é VAL_Z.
 *  @param data Sinal lógico do dado de entrada.
 *  @return O valor lógico resultante da operação.
 */
LogicValue compute_not_if0_gate(LogicValue control, LogicValue data);

/** @brief Simulação da porta de 3 estados sobre suas duas entradas.
 *  @param control Sinal de controle. Se for VAL_0, a saída é VAL_Z.
 *  @param data Sinal lógico do dado de entrada.
 *  @return O valor lógico resultante da operação.
 */
LogicValue compute_not_if1_gate(LogicValue control, LogicValue data);

/** @brief Cria novos eventos na fila de acordo com a saidas (result) computadas
 *         para a porta lógica (gate), no tempo t indicado.
 *  @param queue Ponteiro para a fila de eventos onde os novos eventos serão inseridos.
 *  @param t Tempo base para o cálculo de quando os novos eventos devem ocorrer.
 *  @param timescale Escala de tempo do circuito, para calcular o tempo real de cada evento.
 *  @param gate Ponteiro para o componente que representa a porta lógica que dá origem,
 *              por meio de cada uma de suas saídas, aos novos eventos respectivos.
 *  @param result Valor lógico resultante da operação ocorrida em gate que servirá de
 *                entrada para cada uma de suas saídas conectadas, nos eventos futuro.
 */
void create_events_from_outputs(
        Event** queue,
        Time t,
        Time timescale,
        Component* gate,
        LogicValue result);

/** @brief Set the dump file for simulation output.
 *  @param pp_file Pointer to a file pointer where the dump file will be set.
 *  @param s_path String with a path to the dump file.
 */
void set_dumpfile(FILE** pp_file, const char* s_path);

/** @brief Inspection console launched at $stop system task calls.
 *  @param module Pointer to the module being simulated.
 *  @param t Time of the simulation when $stop was reached.
 */
void inspection_console(Module* module, Time t);

#ifdef __cplusplus
}
#endif

#endif // SIMULA_H
