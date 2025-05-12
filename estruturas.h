/**
 * @file estruturas.h
 * @brief Protótipos das estruturas de dados chave do simulador.
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

/** @brief Estrutura que define a porta. Qual sua função logica e seu delay.
 */
typedef struct st_tipo {
    t_operador operador;
    Tempo atraso;
} t_tipo;

#define MAX_PARAM_NAME_SIZE 64

/** @brief Estrutura para parametros.
 */
typedef struct st_param {
    char name[MAX_PARAM_NAME_SIZE];
    int value;
    int is_local;
} Param;

/** @brief Estrutura para a lista de parametros de um module.
 */
typedef struct st_list_param {
    int total;
    Param** itens;
} ListaParam;

typedef struct st_componente_list ListaComponente;

typedef struct st_componente * Componente;

#define MAX_COMPONENTE_NAME_SIZE 32

/** @brief Estrutura que representa um componente do circuito (uma porta lógica)
 */
struct st_componente {
    char nome[MAX_COMPONENTE_NAME_SIZE];
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
    Componente* itens;
};

#define MAX_REGISTER_NAME_SIZE 32

/** @brief Representação de um registrador.
 */
typedef struct st_reg {
    char name[MAX_REGISTER_NAME_SIZE];
    unsigned int size; // size in bits
    unsigned int value; // stored binary value // TODO: change this type
} Register;

/** @brief Estrutura para a lista de registradores de um module.
 */
typedef struct st_list_reg {
    int total;
    Register** itens;
} ListaReg;

/** @brief Estrutura que representa um circuito, mais especificamente um 'module'.
 */
typedef struct st_module {
    ListaComponente* listaFiosEntrada;
    Sinais* sinaisEntrada;

    ListaComponente* listaFiosSaida;
    Sinais* sinaisSaida;

    ListaComponente* listaWires;

    ListaComponente* listaPortas;

    ListaReg listaReg;

    ListaParam listaParam;
} Module;

/** @brief Inicialização de uma estrutura de circuito.
 */
Module* novoCircuito();

/** @brief Inserir um registrador novo no circuito.
 *         `size`é o tamanho em bits.
 */
void addRegister(Module* circ, const char* name, unsigned int size);

/** @brief Obter um Register de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListaReg'.
 *  @param name String com o nome dado ao registrador. 
 *  @return Um ponteiro para a struct 'Register' correspondente, caso encontrado.
 *          NULL, caso nao seja encontrada uma correspondencia.
 */
Register* get_reg_by_name(ListaReg list, const char* name);

/** @brief Inserir um parametro novo no circuito.
 */
void addParam(Module* circ, Param* param);

/** @brief Obter um Param de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListaParam'.
 *  @param name String com o nome dado ao param. 
 *  @return Um ponteiro para a struct 'Param' correspondente, caso encontrado.
 *          NULL, caso nao seja encontrada uma correspondencia.
 */
Param* get_param_by_name(ListaParam list, const char* name);

/** @brief Adiciona a entrada representada por comp à lista de fios de entrada do circuito
 */
void adicionaEntrada(Module* circ, Componente comp);

/** @brief Adiciona a saída representada por comp à lista de fios de saída do circuito
 */
void adicionaSaida(Module* circ, Componente comp);

/** @brief Adiciona o fio representada por comp à lista de fios (wires) do circuito
 */
void adicionaWire(Module* circ, Componente comp);

/** @brief Adiciona a porta lógica representada por comp à lista de portas do circuito
 */
void adicionaPorta(Module* circ, Componente comp);

/** @brief Retorna a porta que tem o nome indicado, se houver na lista de portas do circuito
 */
Componente getPortaPorNome(Module* circ, const char* nome);

/** @brief Retorna o wire que tem o nome indicado, se houver.
 */
Componente getWirePorNome(Module* circ, const char* nome);

/** @brief Retorna a entrada que tem o nome indicado, se houver na lista de fios de entrada do circuito
 */
Componente getInputPorNome(Module* circ, const char* nome);

/** @brief Retorna a saída que tem o nome indicado, se houver na lista de fios de saída do circuito
 */
Componente getOutputPorNome(Module* circ, const char* nome);

/** @brief Inicialização de uma estrutura de componente.
 *  @return Um tipo Componente que é um ponteiro para a struc.
 */
Componente novoComponente(const char* nome, t_operador porta);

/** @brief Inicializa a estrutura de lista de componentes vazia.
 */
ListaComponente* novaListaComponente();

/** @brief Inicializa a estrutura de lista de componentes com o tamanho indicado.
 */
ListaComponente* novaListaComponenteTamanho(int tamanho);

/** @brief Insere o componente na lista de componentes.
 */
void insereComponente(ListaComponente* ls, Componente cp);

/** @brief Retorna verdadeiro se o componente indicado está contido na lista.
 */
int contemComponente(ListaComponente* ls, Componente cp);

/** @brief Retorna o componente da lista indica que possui o referido nome, se houver.
 */
Componente getComponenteItemPorNome(ListaComponente* ls, const char* nome);

#endif // ESTRUTURAS_H
