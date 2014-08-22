/**
 * @file simula.h
 * @brief Prot�tipo da fun��o principal da simula��o.
 */

#ifndef SIMULA_H

#define SIMULA_H

#include "estruturas.h"

/** @brief Fun��o que faz a simula��o do circuito com as entradas especificadas.
           Em caso de sucesso, retorna as sa�das dessa simula��o.
 */
Sinais* simula(t_circuito* circuto, Sinais* entradas);

#endif // SIMULA_H
