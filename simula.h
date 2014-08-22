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

#endif // SIMULA_H
