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

/** @brief Enumeração para possibilitar a distinção ou definição das classes
 *  de componentes do circuito de acordo com seus papéis (roles) ou funções.
 */
typedef enum en_role {
    ROLE_AND,
    ROLE_OR,
    ROLE_XOR,
    ROLE_NAND,
    ROLE_NOR,
    ROLE_XNOR,
    ROLE_NOT,
    ROLE_BUF,
    ROLE_BUF_IF0,
    ROLE_BUF_IF1,
    ROLE_NOT_IF0,
    ROLE_NOT_IF1,
    ROLE_LITERAL_NUMBER,
    ROLE_WIRE,
    ROLE_OUTPUT,
    ROLE_INPUT,
    ROLE_ASSIGN
} Role;

/** @brief Struct que permite definir alguns atributos de um componente
 *         do circuito. Como: qual seu papel/função e qual seu delay
 *         (caso seja pertinente e exista).
 */
typedef struct st_attributes {
    Role role;
    Time delay;
} Attributes;

#define MAX_PARAM_NAME_SIZE 64 // tamanho máximo do nome de um parametro

/** @brief Estrutura para parametros.
 *  @note Cada parâmetro tem um nome, um valor e uma flag que indica
 *        se e local ou nao.
 */
typedef struct st_param {
    char name[MAX_PARAM_NAME_SIZE];
    int value;
    int is_local;
} Param;

/** @brief Estrutura para a lista de parametros de um module.
 *  @note A lista de parametros é constituída de um array de ponteiros
 *        para Param, e não uma array de Param.
 */
typedef struct st_list_param {
    int total;
    Param** itens;
} ListParam;

typedef struct st_componente_list ListComponent;

#define MAX_COMPONENT_NAME_SIZE 32 // tamanho máximo do nome de um componente

/** @brief Estrutura que representa algum componente do circuito.
 *  @note Esta estrutura é genérica, podendo representar uma porta lógica,
 *        um wire, uma entrada ou uma saída.
 */
typedef struct st_component {
    char name[MAX_COMPONENT_NAME_SIZE];
    Attributes attributes;

    ListComponent* list_input;
    Signal* input_signal;

    ListComponent* list_output;
    Signal* output_signal;

    LogicValue dynamic_value; // in case of a net type (or literal number)
    unsigned int size; // size in bits
} Component;

/** @brief Estrutura que representa uma lista de componentes.
    @note Na realidade, ela guarda o total e um array de ponteiros para
          as logic gates.
 */
struct st_componente_list {
    int total;
    Component** itens;
};

#define MAX_REGISTER_NAME_SIZE 32 // tamanho maximo do nome de um registrador

/** @brief Representação de um registrador.
 *  @note Esta estrutura guarda o nome, tamanho, se é com sinal
 *        e o valor armazenado.
 */
typedef struct st_reg {
    char name[MAX_REGISTER_NAME_SIZE];
    unsigned int size; // size in bits
    int is_signed; // true if reg signed
    unsigned int value; // stored binary value // TODO: change this type to array with 4 value logic
} Register;

/** @brief Estrutura para a lista de registradores de um module.
 *  @note A lista de registradores é constituída de um array de ponteiros
 *        para Register, e não uma array de Register.
 */
typedef struct st_list_register {
    int total;
    Register** itens;
} ListRegister;

#define MAX_MODULE_NAME 1024

/** @brief Estrutura que representa um circuito, mais especificamente um 'module'.
 */
typedef struct st_module {
    char name[MAX_MODULE_NAME];

    ListComponent* list_input_net;
    SignalArray* sinais_input;

    ListComponent* list_output_net;
    SignalArray* sinais_output;

    ListComponent* list_wire_net;
    ListComponent* list_reg_net; // complementar
    ListComponent* list_all_components;

    ListRegister list_register;
    ListParam list_param;

    Time timescale_number;
    TimeUnit timescale_unit;
    Time timescale_precision_number;
    TimeUnit timescale_precision_unit;
} Module;

/** @brief Estrutura para a lista de modules.
*/
typedef struct st_list_module {
    int total;
    Module** itens;
} ListModule;

/** @brief Dealocate all the memory of a ListModule structure.
 *  @param circuit Pointer to a pointer of ListModule type.
 */
void free_circuit(ListModule** circuit);

/** @brief Alocação e inicialização de uma struct Module.
 *  @return Um ponteiro para a struct Module alocada e pre-inicializada.
 */
Module* new_module();

/** @brief Dealocate all the memory of a Module structure.
 *  @param mod Pointer to a pointer of Module type.
 *  @return void
 */
void free_module(Module** mod);

/** @brief Inserir um registrador novo no module.
 *  @param mod Pointer to a Module type.
 *  @param name Nome dado ao registrador.
 *  @param size Comprimento, em quantidade de bits.
 *  @param is_signed Verdadeiro caso seja com sinal.
 *  @note Doesn't check for duplicities.
 */
void add_register(
    Module* mod,
    const char* name,
    unsigned int size,
    int is_signed);

/** @brief Obter um Register de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListRegister'.
 *  @param name String com o nome dado ao registrador. 
 *  @return Um ponteiro para a struct 'Register' correspondente, caso
 *          encontrado ou NULL, caso nao seja encontrada uma correspondencia.
 */
Register* get_reg_by_name(ListRegister list, const char* name);

/** @brief Inserir um parametro novo no circuito.
 *  @param circ Ponteiro para o circuito.
 *  @param param Ponteiro para a struct 'Param' a ser inserida.
 */
void add_param(Module* circ, Param* param);

/** @brief Obter um Param de uma lista usando o nome como chave.
 *  @param list Uma struct 'ListParam'.
 *  @param name String com o nome dado ao param. 
 *  @return Um ponteiro para a struct 'Param' correspondente, caso encontrado.
 *          NULL, caso nao seja encontrada uma correspondencia.
 */
Param* get_param_by_name(ListParam list, const char* name);

/** @brief Adiciona a entrada representada por comp à lista de nets de
 *         entrada do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa
 *              tal entrada.
 */
void add_input(Module* circ, Component* comp);

/** @brief Adiciona a saída representada por comp à lista de nets de
 *         saída do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa
 *              tal saída.
 */
void add_output(Module* circ, Component* comp);

/** @brief Adiciona o wire representado por comp à lista de wires do module.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa
 *              tal wire.
 */
void add_wire(Module* circ, Component* comp);

/** @brief Adiciona a porta lógica representada por comp à lista de portas
 *         lógicas do module.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param comp Ponteiro para um Component, já inicializado, que representa 
 *              a porta lógica.
 */
void add_gate(Module* circ, Component* comp);

/** @brief Retorna a porta lógica que tem o nome indicado, se houver na lista
 *         de portas lógicas do module.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param name String com o nome da porta lógica a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente, caso
 *          encontrado, ou NULL se não houver.
 */
Component* get_gate_by_name(Module* circ, const char* name);

/** @brief Retorna o wire que tem o nome indicado, se houver.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param name String com o nome do wire a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente,
 *          caso encontrado, ou NULL se não houver.
 */
Component* get_wire_by_name(Module* circ, const char* name);

/** @brief Retorna a entrada que tem o nome indicado, se houver na lista de
 *         nets (wires) de entrada do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param name String com o nome da entrada a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente,
 *          caso encontrado, ou NULL se não houver.
 */
Component* get_input_by_name(Module* circ, const char* name);

/** @brief Retorna a saída que tem o nome indicado, se houver na lista de
 *         nets de saída do circuito.
 *  @param circ Ponteiro para um Module, já inicializado.
 *  @param name String com o nome da saída a ser buscado.
 *  @return Um ponteiro para a struct Component correspondente,
 *          caso encontrado, ou NULL se não houver.
 */
Component* get_output_by_name(Module* circ, const char* name);

/** @brief Inicialização da struct de um componente do circuito.
 *  @param name String com nome do Component a ser criado.
 *  @param role Papel do Component a ser criado, segundo a enum Role.
 *  @return Um ponteiro para a struct Component alocada e pre-inicializada.
 */
Component* new_component(const char* name, Role role);

/** @brief Libera uma struct Componente da memória e muda o valor de seu
 *         ponteiro para NULL.
 *  @param c Ponteiro para um ponteiro da strcut Componente.
 *  @note As duas listas para inputs e outputs em Componente também serão
 *        liberadas da memória. contudo, em cada lista, existe um array de
 *        ponteiros para Componente. Cada componente em si que possa estar
 *        referenciado nesse array deverá ser liberado posteriormente e forma
 *        apropriada, quando conviniente.
 *        Para tal, uma lista de todos os componentes deve ser mantida.
 */
void delete_componente(Component** c);

/** @brief Inicializa a estrutura de lista de componentes vazia.
 *  @return Um ponteiro para a lista de componentes alocada.
 */
ListComponent* new_list_component();

/** @brief Inicializa a estrutura de lista de componentes com o tamanho
 *         indicado para itens.
 *  @param size Tamanho a ser pré alocado para itens.
 *  @return Um ponteiro para a lista de componentes alocada.
 */
ListComponent* new_list_component_of_size(unsigned int size);

/** @brief Libera completamente uma lista de componentes da memória.
 *  @param ppl Ponteiro para um ponteiro da struct ListComponent.
 *  @return void
 *  @note A lista de componentes e todos os componentes contidos nela
 *        são liberados.
 */
void delete_list_component(ListComponent** ppl);

/** @brief Libera completamente uma lista de parametros da memória.
 *  @param list_param Ponteiro para a struct ListParam a ser liberada.
*/
void delete_list_param(ListParam* list_param);

/** @brief Libera completamente uma lista de registradores da memória.
 *  @param list_reg Ponteiro para a struct ListRegister a ser liberada.
*/
void delete_list_register(ListRegister* list_reg);

/** @brief Insere o componente na lista de componentes.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param cp Ponteiro para o componente a ser inserido.
 *  @note A cada nova inserção a lista é realocada para aumentar o tamanho
 *        do array de itens. Doesn't check for duplicities.
 */
void insert_component(ListComponent* ls, Component* cp);

/** @brief Retorna verdadeiro se o componente indicado está contido na lista.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param cp Ponteiro para o componente a ser verificado.
 *  @return 1 se o componente estiver na lista, 0 caso contrário.
 */
int has_component_by_pointer(ListComponent* ls, Component* cp);

/** @brief Retorna o componente da lista indicada que possui o referido
 *         nome, se houver.
 *  @param ls Ponteiro para a lista de componentes.
 *  @param name Nome do componente a ser buscado.
 *  @return Um ponteiro para o componente encontrado, ou NULL se não houver.
 */
Component* get_component_by_name(ListComponent* ls, const char*  name);

#ifdef __cplusplus
}
#endif

#endif // ESTRUTURAS_H
