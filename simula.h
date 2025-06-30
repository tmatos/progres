/**
 * @file simula.h
 * @brief Protótipos da função principal da simulação.
 */

#ifndef SIMULA_H
#define SIMULA_H

#include "estruturas.h"
#include "eventos.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Função que faz a simulação do circuito com as entradas especificadas.
           Em caso de sucesso, retorna as saídas dessa simulação.
 */
Sinais* simula(Module* circuto, Sinais* entradas, Evento** initial_task_events);

/** @brief Simulação da avaliação da porta 'buf'
 */
ValorLogico computeBufGate(ValorLogico input);

/** @brief Simulação da avaliação da porta 'not'
 */
ValorLogico computeNotGate(ValorLogico input);

/** @brief Simulação da avaliação da porta 'xor'
 */
ValorLogico computeXorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta 'xnor'
 */
ValorLogico computeXnorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta 'or' sobre todas as n entradas
 */
ValorLogico computeOrGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'and' sobre todas as n entradas
 */
ValorLogico computeAndGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'nor' sobre todas as n entradas
 */
ValorLogico computeNorGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'nand' sobre todas as n entradas
 */
ValorLogico computeNandGate(ListaComponente* inputs);

/** @brief Computes the output of a tri-state buffer (bufif0 type).
 *  This function models a buffer that is enabled when the control signal is low (0).
 *  @param control The control signal. If VAL_1, the buffer is in high-impedance (VAL_Z).
 *  @param data The input data signal.
 *  @return The resulting logic value.
 */
ValorLogico compute_buf_if0_gate(ValorLogico control, ValorLogico data);

/** @brief Computes the output of a tri-state buffer (bufif1 type).
 *  This function models a buffer that is enabled when the control signal is high (1).
 *  @param control The control signal. If VAL_0, the buffer is in high-impedance (VAL_Z).
 *  @param data The input data signal.
 *  @return The resulting logic value.
 */
ValorLogico compute_buf_if1_gate(ValorLogico control, ValorLogico data);

/** @brief Simulação da porta de 3 estados sobre suas duas entradas
 */
ValorLogico compute_not_if0_gate(ValorLogico control, ValorLogico data);

/** @brief Simulação da porta de 3 estados sobre suas duas entradas
 */
ValorLogico compute_not_if1_gate(ValorLogico control, ValorLogico data);

/** @brief Cria novos eventos na fila de acordo com a saidas (result) computadas
 *         para a porta lógica (gate), no tempo t indicado. 
 */
void createEventsFromOutputs(Evento** fila, Tempo t, Tempo timescale, Componente gate, ValorLogico result);

#ifdef __cplusplus
}
#endif

#endif // SIMULA_H
