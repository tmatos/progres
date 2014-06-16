/**
 * @file estruturas.h
 * @brief Prototipos das estruturas de dados chave do simulador.
 */

#ifndef ESTRUTURAS_H

#define ESTRUTURAS_H

/** @brief .
 */
typedef enum en_operador {op_and, op_or, op_not, op_nand, op_xor, wire} t_operador;

/** @brief .
 */
typedef enum en_valor {zero = 0, um = 1, x} t_valor;

/** @brief .
 */
typedef struct st_tipo {
    t_operador operador;
    int atraso;
} t_tipo;

/** @brief .
 */
typedef struct st_sinal {
    t_valor valor;
    int tempo;
} t_sinal;

/** @brief .
 */
typedef struct st_componente {
    char nome[16];
    struct st_componente **listaEntrada;
    int numEntrada;
    t_sinal *sinalEntrada;
    struct st_componente **listaSaida;
    int numSaida;
    t_tipo tipo;
    t_sinal saida;
} * t_componente;

/** @brief .
 */
typedef struct st_circuito {
    int numEntrada;
    struct st_sinal *sinaisEntrada;
    struct st_componente **listaFiosEntrada;
    struct st_componente **listaFiosSaida;
    int numSaida;
    struct st_sinal *sinaisSaida;
} t_circuito;

/** @brief .
 */
t_circuito* novoCircuito();

/** @brief .
 */
t_componente* novaListaCompon(int tamanho);

/** @brief .
 */
t_componente novoComponente();

#endif
