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
    @param circuto Ponteiro para um Module, já inicializado.
    @param entradas Ponteiro para a struct Sinais, contendo entradas do circuito.
    @param initial_task_events Ponteiro para a fila de eventos iniciais com tasks.
    @return Um ponteiro para uma estrutura Sinais contendo as saídas do circuito
            após a simulação. Retorna NULL se houver algum erro ou se as entradas
            não corresponderem às esperadas no module.
 */
Sinais* simula(Module* circuto, Sinais* entradas, Evento** initial_task_events);

/** @brief Simulação da avaliação da porta 'buf'.
 *  @param input Valor lógico de entrada.
 *  @return O valor lógico resultante da operação BUF.
 */
ValorLogico computeBufGate(ValorLogico input);

/** @brief Simulação da avaliação da porta 'not'.
 *  @param input Valor lógico de entrada.
 *  @return O valor lógico resultante da operação NOT.
 */
ValorLogico computeNotGate(ValorLogico input);

/** @brief Simulação da avaliação da porta 'xor'.
 *  @param a Primeiro valor lógico de entrada.
 *  @param b Segundo valor lógico de entrada.
 *  @return O valor lógico resultante da operação XOR.
 */
ValorLogico computeXorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta 'xnor'.
 *  @param a Primeiro valor lógico de entrada.
 *  @param b Segundo valor lógico de entrada.
 *  @return O valor lógico resultante da operação XNOR.
 */
ValorLogico computeXnorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta 'or' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta OR.
 *  @return O valor lógico resultante da operação OR.
 */
ValorLogico computeOrGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'and' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta AND.
 *  @return O valor lógico resultante da operação AND.
 */
ValorLogico computeAndGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'nor' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta NOR.
 *  @return O valor lógico resultante da operação NOR.
 */
ValorLogico computeNorGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'nand' sobre todas as n entradas.
 *  @param inputs Lista de componentes que representam as entradas da porta NAND.
 *  @return O valor lógico resultante da operação NAND.
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

/** @brief Simulação da porta de 3 estados sobre suas duas entradas.
 *  @param control Sinal de controle. Se for VAL_1, a saída é VAL_Z.
 *  @param data Sinal lógico do dado de entrada.
 *  @return O valor lógico resultante da operação.
 */
ValorLogico compute_not_if0_gate(ValorLogico control, ValorLogico data);

/** @brief Simulação da porta de 3 estados sobre suas duas entradas.
 *  @param control Sinal de controle. Se for VAL_0, a saída é VAL_Z.
 *  @param data Sinal lógico do dado de entrada.
 *  @return O valor lógico resultante da operação.
 */
ValorLogico compute_not_if1_gate(ValorLogico control, ValorLogico data);

/** @brief Cria novos eventos na fila de acordo com a saidas (result) computadas
 *         para a porta lógica (gate), no tempo t indicado.
 *  @param fila Ponteiro para a fila de eventos onde os novos eventos serão inseridos.
 *  @param t Tempo base para o cálculo de quando os novos eventos devem ocorrer.
 *  @param timescale Escala de tempo do circuito, para calcular o tempo real de cada evento.
 *  @param gate Ponteiro para o componente que representa a porta lógica que dá origem,
 *              por meio de cada uma de suas saídas, aos novos eventos respectivos.
 *  @param result Valor lógico resultante da operação ocorrida em gate que servirá de
 *                entrada para cada uma de suas saídas conectadas, nos eventos futuro.
 */
void createEventsFromOutputs(Evento** fila, Tempo t, Tempo timescale, Component* gate, ValorLogico result);

/** @brief Set the dump file for simulation output.
 *  @param pp_file Pointer to a file pointer where the dump file will be set.
 *  @param s_path String with a path to the dump file.
 *  @return void
 */
void set_dumpfile(FILE** pp_file, const char* s_path);

#ifdef __cplusplus
}
#endif

#endif // SIMULA_H
