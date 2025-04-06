/**
 * @file simula.h
 * @brief Protótipo da função principal da simulação.
 */

#ifndef SIMULA_H

#define SIMULA_H

#include "estruturas.h"

/** @brief Função que faz a simulação do circuito com as entradas especificadas.
           Em caso de sucesso, retorna as saídas dessa simulação.
 */
Sinais* simula(t_circuito* circuto, Sinais* entradas);

/** @brief Simulação da avaliação da porta not
 */
ValorLogico computeNotGate(ValorLogico input);

/** @brief Simulação da avaliação da porta xor
 */
ValorLogico computeXorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta xnor
 */
ValorLogico computeXnorGate(ValorLogico a, ValorLogico b);

/** @brief Simulação da avaliação da porta 'or' sobre todas as n entradas
 */
ValorLogico computeOrGate(ListaComponente* inputs);

/** @brief Simulação da avaliação da porta 'and' sobre todas as n entradas
 */
ValorLogico computeAndGate(ListaComponente* inputs);

#endif // SIMULA_H
