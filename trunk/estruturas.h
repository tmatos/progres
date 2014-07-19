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

/** @brief Estrutura que representa uma lista de componentes.
            Na verdade ela guarda o total e um array de ponteiros para as portas.
 */
typedef struct st_componente_list {
    int tamanho;
    Componente *itens;
} ListaComponente;

/** @brief Estrutura que representa um circuito, mais especificamente um 'module'
 */
typedef struct st_circuito {
    ListaComponente *listaFiosEntrada;
    Sinais *sinaisEntrada;

    ListaComponente *listaFiosSaida;
    Sinais *sinaisSaida;

    ListaComponente *listaWires;

    ListaComponente *listaPortas;
} t_circuito;

/** @brief Inicialização de uma estrutura de circuito
 */
t_circuito* novoCircuito();

/** @brief .
 */
int adicionaEntrada(t_circuito* circ, Componente comp);

/** @brief .
 */
int adicionaSaida(t_circuito* circ, Componente comp);

/** @brief .
 */
Componente getPortaPorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getWirePorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getInputPorNome(t_circuito* circ, char* nome);

/** @brief .
 */
Componente getOutputPorNome(t_circuito* circ, char* nome);

/** @brief Inicialização de uma estrutura de componente.
 *  @return Um tipo Componente que é um ponteiro para a struc.
 */
Componente novoComponente(char* nome, t_operador porta);

/** @brief .
 */
ListaComponente* novaListaComponente();

/** @brief .
 */
ListaComponente* novaListaComponenteTamanho(int tamanho);

/** @brief .
 */
void insereComponente(ListaComponente* ls, Componente cp);

/** @brief .
 */
Componente getComponenteItemPorNome(ListaComponente* ls, char* nome);

#endif
