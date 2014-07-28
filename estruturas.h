/**
 * @file estruturas.h
 * @brief Prototipos das estruturas de dados chave do simulador.
 */

#ifndef ESTRUTURAS_H

#define ESTRUTURAS_H

#include "sinais.h"

/** @brief Enumeração para o definir as classes de componente do circuito de acordo com suas funções.
 */
typedef enum en_operador {
    op_and,
    op_or,
    op_xor,
    op_nand,
    op_nor,
    op_xnor,
    op_not,
    op_buf,
    wire,
    output,
    input
} t_operador;

/** @brief Estrutura que define a porta. Qual sua função lógica e seu delay.
 */
typedef struct st_tipo {
    t_operador operador;
    Tempo atraso;
} t_tipo;

typedef struct st_componente_list ListaComponente;

typedef struct st_componente * Componente;

/** @brief Estrutura que representa um componente do circuito (uma porta lógica)
 */
struct st_componente {
    char nome[16];
    t_tipo tipo;

    ListaComponente* listaEntrada;
    Sinal* sinalEntrada;

    ListaComponente* listaSaida;
    Sinal* sinalSaida;

    ValorLogico valorDinamico; // fio
};

/** @brief Estrutura que representa uma lista de componentes.
            Na verdade ela guarda o total e um array de ponteiros para as portas.
 */
struct st_componente_list {
    int tamanho;
    Componente *itens;
};

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

/** @brief Adiciona a entrada representada por comp à lista de fios de entrada do circuito
 */
void adicionaEntrada(t_circuito* circ, Componente comp);

/** @brief Adiciona a saída representada por comp à lista de fios de saída do circuito
 */
void adicionaSaida(t_circuito* circ, Componente comp);

/** @brief Adiciona o fio representada por comp à lista de fios (wires) do circuito
 */
void adicionaWire(t_circuito* circ, Componente comp);

/** @brief Adiciona a porta lógica representada por comp à lista de portas do circuito
 */
void adicionaPorta(t_circuito* circ, Componente comp);

/** @brief Retorna a porta que tem o nome indicado, se houver na lista de portas do circuito
 */
Componente getPortaPorNome(t_circuito* circ, char* nome);

/** @brief Retorna o wire que tem o nome indicado, se houver.
 */
Componente getWirePorNome(t_circuito* circ, char* nome);

/** @brief Retorna a entrada que tem o nome indicado, se houver na lista de fios de entrada do circuito
 */
Componente getInputPorNome(t_circuito* circ, char* nome);

/** @brief Retorna a saída que tem o nome indicado, se houver na lista de fios de saída do circuito
 */
Componente getOutputPorNome(t_circuito* circ, char* nome);

/** @brief Inicialização de uma estrutura de componente.
 *  @return Um tipo Componente que é um ponteiro para a struc.
 */
Componente novoComponente(char* nome, t_operador porta);

/** @brief Inicializa a estrutura de lista de componentes vazia
 */
ListaComponente* novaListaComponente();

/** @brief Inicializa a estrutura de lista de componentes com o tamanho indicado
 */
ListaComponente* novaListaComponenteTamanho(int tamanho);

/** @brief Insere o componente na lista de componentes
 */
void insereComponente(ListaComponente* ls, Componente cp);

/** @brief Retorna verdadeiro se o componente indicado está contido na lista
 */
int contemComponente(ListaComponente* ls, Componente cp);

/** @brief Retorna o componente da lista indica que possui o referido nome, se houver.
 */
Componente getComponenteItemPorNome(ListaComponente* ls, char* nome);

#endif
