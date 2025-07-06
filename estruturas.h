/**
 * @file estruturas.h
 * @brief Protótipos das estruturas de dados chave do simulador.
 */

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include "sinais.h"

#ifdef __cplusplus
extern "C" {
#endif

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
    OP_BUF_IF0,
    OP_BUF_IF1,
    OP_NOT_IF0,
    OP_NOT_IF1,
    LITERAL_NUMBER,
    wire,
    output,
    input,
    assign
} t_operador;

/** @brief Estrutura que define a porta. Qual sua função logica e seu delay.
 */
typedef struct st_tipo {
    t_operador operador;
    Tempo atraso;
} t_tipo;

#define MAX_PARAM_NAME_SIZE 64 // tamanho máximo do nome de um parâmetro

/** @brief Estrutura para parametros.
 *  @note Cada parâmetro tem um nome, um valor e uma flag que indica se é local ou não.
 */
typedef struct st_param {
    char name[MAX_PARAM_NAME_SIZE];
    int value;
    int is_local;
} Param;

/** @brief Estrutura para a lista de parametros de um module.
 *  @note A lista de parametros é constituída de um array de ponteiros para Param, e não uma array de Param.
 */
typedef struct st_list_param {
    int total;
    Param** itens;
} ListaParam;

typedef struct st_componente_list ListaComponente;

#define MAX_COMPONENT_NAME_SIZE 32 // tamanho máximo do nome de um componente

/** @brief Estrutura que representa um componente do circuito (ex.: uma porta lógica).
 *  @note Esta estrutura é genérica, podendo ser uma porta lógica, um wire, uma entrada ou uma saída.
 */
typedef struct st_component {
    char nome[MAX_COMPONENT_NAME_SIZE];
    t_tipo tipo;

    ListaComponente* listaEntrada;
    Sinal* sinalEntrada;

    ListaComponente* listaSaida;
    Sinal* sinalSaida;

    ValorLogico valorDinamico; // in case of a net type (or literal number)
    unsigned int size; // size in bits
} Component;

/** @brief Estrutura que representa uma lista de componentes.
    @note Na realidade, ela guarda o total e um array de ponteiros para as portas.
 */
struct st_componente_list {
    int tamanho;
    Component** itens;
};

#define MAX_REGISTER_NAME_SIZE 32 // tamanho máximo do nome de um registrador

/** @brief Representação de um registrador.
 *  @note Esta estrutura guarda o nome, tamanho, se é com sinal e o valor armazenado.
 */
typedef struct st_reg {
    char name[MAX_REGISTER_NAME_SIZE];
    unsigned int size; // size in bits
    int is_signed; // true if reg signed
    unsigned int value; // stored binary value // TODO: change this type
} Register;

/** @brief Estrutura para a lista de registradores de um module.
 *  @note A lista de registradores é constituída de um array de ponteiros para Register, e não uma array de Register.
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

    Tempo timescale_number;
    UnidTempo timescale_unit;
    Tempo timescale_precision_number;
    UnidTempo timescale_precision_unit;
} Module;

/** @brief Inicialização de uma estrutura de circuito.
 *  @return Um ponteiro para a struct Module alocada e pre-inicializada.
 */
Module* novoCircuito();

/** @brief Dealocate all the memory of a Module structure.
 *  @param mod Pointer to a pointer of Module type.
 *  @return void
 */
void free_module(Module** mod);

/** @brief Inserir um registrador novo no circuito.
 *  @param circ Ponteiro para o circuito.
 *  @param name Nome dado ao registrador.
 *  @param size Comprimento, em quantidade de bits.
 *  @param is_signed Verdadeiro caso seja com sinal.
 *  @return void
 */
void addRegister(Module* circ, const char* name, unsigned int size, int is_signed);

/** @brief Obter um Register de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListaReg'.
 *  @param name String com o nome dado ao registrador. 
 *  @return Um ponteiro para a struct 'Register' correspondente, caso encontrado.
 *          NULL, caso nao seja encontrada uma correspondencia.
 */
Register* get_reg_by_name(ListaReg list, const char* name);

/** @brief Inserir um parametro novo no circuito.
 *  @param circ Ponteiro para o circuito.
 *  @param param Ponteiro para a struct 'Param' a ser inserida.
 *  @return void
 */
void addParam(Module* circ, Param* param);

/** @brief Obter um Param de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListaParam'.
 *  @param name String com o nome dado ao param. 
 *  @return Um ponteiro para a struct 'Param' correspondente, caso encontrado.
 *          NULL, caso nao seja encontrada uma correspondencia.
 */
Param* get_param_by_name(ListaParam list, const char* name);

/** @brief Adiciona a entrada representada por comp à lista de fios de entrada do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa a entrada.
 *  @return void
 */
void adicionaEntrada(Module* circ, Component* comp);

/** @brief Adiciona a saída representada por comp à lista de fios de saída do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa a saída.
 *  @return void
 */
void adicionaSaida(Module* circ, Component* comp);

/** @brief Adiciona o fio representada por comp à lista de fios (wires) do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa o wire.
 *  @return void
 */
void adicionaWire(Module* circ, Component* comp);

/** @brief Adiciona a porta lógica representada por comp à lista de portas do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa a porta lógica.
 *  @return void
 */
void adicionaPorta(Module* circ, Component* comp);

/** @brief Retorna a porta que tem o nome indicado, se houver na lista de portas do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param nome String com o nome da porta a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente, caso encontrado, ou NULL se não houver.
 */
Component* getPortaPorNome(Module* circ, const char* nome);

/** @brief Retorna o wire que tem o nome indicado, se houver.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param nome String com o nome do wire a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente, caso encontrado, ou NULL se não houver.
 */
Component* getWirePorNome(Module* circ, const char* nome);

/** @brief Retorna a entrada que tem o nome indicado, se houver na lista de fios de entrada do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param nome String com o nome da entrada a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente, caso encontrado, ou NULL se não houver.
 */
Component* getInputPorNome(Module* circ, const char* nome);

/** @brief Retorna a saída que tem o nome indicado, se houver na lista de fios de saída do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param nome String com o nome da saída a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente, caso encontrado, ou NULL se não houver.
 */
Component* getOutputPorNome(Module* circ, const char* nome);

/** @brief Inicialização de uma estrutura de componente.
 *  @param nome String com nome do componente a ser criado.
 *  @param porta Tipo do Component a ser criado, segundo a enum t_operador.
 *  @return Um ponteiro para a struct Component alocada e pre-inicializada.
 */
Component* novoComponente(const char* nome, t_operador porta);

/** @brief Libera completamente um Componente da memória.
 *  @param c Ponteiro para um ponteiro da strcut Componente.
 *  @return void
 *  @note A lista de entradas e saídas do componente também é liberada.
 */
void delete_componente(Component** c);

/** @brief Inicializa a estrutura de lista de componentes vazia.
 *  @return Um ponteiro para a lista de componentes alocada.
 */
ListaComponente* novaListaComponente();

/** @brief Inicializa a estrutura de lista de componentes com o tamanho indicado para itens.
 *  @param size Tamanho a ser pré alocado para itens.
 *  @return Um ponteiro para a lista de componentes alocada.
 */
ListaComponente* novaListaComponenteTamanho(unsigned int size);

/** @brief Libera completamente uma lista de componentes da memória.
 *  @param ppl Ponteiro para um ponteiro da struct ListaComponente.
 *  @return void
 *  @note A lista de componentes e todos os componentes contidos nela são liberados.
 */
void delete_list_component(ListaComponente** ppl);

/** @brief Insere o componente na lista de componentes.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param cp Ponteiro para o componente a ser inserido.
 *  @return void
 *  @note A cada nova inserção a lista é realocada para aumentar o tamanho do array de itens.
 */
void insereComponente(ListaComponente* ls, Component* cp);

/** @brief Retorna verdadeiro se o componente indicado está contido na lista.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param cp Ponteiro para o componente a ser verificado.
 *  @return 1 se o componente estiver na lista, 0 caso contrário.
 */
int contemComponente(ListaComponente* ls, Component* cp);

/** @brief Retorna o componente da lista indicada que possui o referido nome, se houver.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param nome Nome do componente a ser buscado.
 *  @return Um ponteiro para o componente encontrado, ou NULL se não houver.
 */
Component* getComponenteItemPorNome(ListaComponente* ls, const char* nome);

#ifdef __cplusplus
}
#endif

#endif // ESTRUTURAS_H
