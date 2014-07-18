/**
 * @file estruturas.h
 * @brief Prototipos das estruturas de dados chave do simulador.
 */

#ifndef ESTRUTURAS_H

#define ESTRUTURAS_H

#include "sinais.h"

/** @brief .
 */
typedef enum en_operador {op_and, op_or, op_not, op_nand, op_xor, wire, output, input} t_operador;

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
    t_tipo tipo;

    int numEntrada;
    struct st_componente **listaEntrada;
    Sinal* sinalEntrada;

    int numSaida;
    struct st_componente **listaSaida;
    Sinal* sinalSaida;
} * Componente;

/** @brief Estrutura que representa um circuito, mais especificamente um 'module'
 */
typedef struct st_circuito {
    int numEntrada;
    Componente *listaFiosEntrada;
    Sinais *sinaisEntrada;

    int numSaida;
    Componente *listaFiosSaida;
    Sinais *sinaisSaida;

    int numWires;
    Componente *listaWires;

    int numPortas;
    Componente *listaPortas;
} t_circuito;

/** @brief Inicialização de uma estrutura de circuito
 */
t_circuito* novoCircuito();

/** @brief Inicialização de uma estrutura de
 */
Componente* novaListaCompon(int tamanho);

/** @brief Inicialização de uma estrutura de componente.
 *  @return Um tipo Componente que é um ponteiro para a struc.
 */
Componente novoComponente(char* nome, t_operador porta);

/** @brief .
 */
int adicionaEntrada(t_circuito* circ, Componente comp);

/** @brief .
 */
int adicionaSaida(t_circuito* circ, Componente comp);

/** @brief .
 */
Componente getComponentePorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getWirePorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getInputPorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getOutputPorNome(t_circuito* circ, char* nome);

#endif
