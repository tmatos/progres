/**
 * @file estruturas.h
 * @brief Prototipos das estruturas de dados chave do simulador.
 */

#ifndef ESTRUTURAS_H

#define ESTRUTURAS_H

#include "sinais.h"

/** @brief .
 */
typedef enum en_operador {op_and, op_or, op_not, op_nand, op_xor, wire} t_operador;

/** @brief .
 */
typedef struct st_tipo {
    t_operador operador;
    int atraso;
} t_tipo;

/** @brief Estrutura que representa um componente do circuito (uma porta lógica)
 */
typedef struct st_componente {
    char nome[16];
    struct st_componente **listaEntrada;
    int numEntrada;
    Sinal* sinalEntrada;
    struct st_componente **listaSaida;
    int numSaida;
    t_tipo tipo;
    Sinal* saida;
} * Componente;

/** @brief Estrutura que representa um circuito, mais especificamente um 'module'
 */
typedef struct st_circuito {
    int numEntrada;
    Sinais *sinaisEntrada;
    struct st_componente **listaFiosEntrada;
    struct st_componente **listaFiosSaida;
    int numSaida;
    Sinais *sinaisSaida;
} t_circuito;

/** @brief Inicialização de uma estrutura de circuito
 */
t_circuito* novoCircuito();

/** @brief Inicialização de uma estrutura de
 */
Componente* novaListaCompon(int tamanho);

/** @brief Inicialização de uma estrutura de componente.
 *  @return Uma struct do componente.
 */
Componente novoComponente();

#endif
