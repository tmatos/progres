/**
 * @file verilog.h
 * @brief Rotinas para parsing de arquivos Verilog
 */

#ifndef VERILOG_H
#define VERILOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "estruturas.h"
#include "eventos.h"
#include "lex.h"

/** @brief Enum para tipos de error nas rotinas de parsing
 */
typedef enum en_verilog_error {
    ERROR_VERILOG = 0,
    ERROR_VERILOG_BAD_TOKEN,
    ERROR_VERILOG_BAD_EOF,
    ERROR_VERILOG_BAD_EXPRESSION,
    ERROR_VERILOG_UNDECLARED_MACRO,
    NO_ERROR = 10
} VerilogError;

/** @brief Processar a parte do código que possui a declaração
 *         de cabecalho do module, até o token ';'.
 *  @param it Endereço para o iterador dos tokens do código fonte.
 *  @param identifiers Lista de tokens contendo os identificadores já detectados.
 *  @param livres Lista de identificadores de entrada ou saida ainda nao definidos como tal.
 *  @param module Ponteiro para a struct do module a ser escrito.
 *  @return 0 em caso de falha, 1 em caso de sucesso.
 */
int load_module_header(Token** it, ListaToken* identifiers, ListaToken* livres, Module* module);

/** @brief Cria uma estrutura de dados representando um module,
           a partir do primeiro no arquivo com o codigo fonte em Verilog.
 *  @param file_path String com o caminho do arquivo a ser processado.
 *  @param initial_task_events Pointer para uma fila de eventos (para systasks).
 *  @return Pointer para estrutura de dados do circuito ou NULL em caso de erro.
 */
Module* load_module(const char* file_path, Evento** initial_task_events);

/** @brief Parsing de definições de range.
 *  @param it Endereço para o iterador dos tokens.
 *  @param module Pointer to verilog module struct.
 *  @param list_param Params definidos.
 *  @param range_msb Ponteiro para a variável que guarda o indice do bit mais significativo.
 *  @param range_lsb Ponteiro para a variável que guarda o indice do bit menos significativo.
 *  @return Código de erro do tipo VerilogError.
 */
VerilogError load_range(Token** it, Module* module, ListaToken* list_param, int* range_msb, int* range_lsb);

/**
 * @brief Parsing of 'reg' declarations.
 * @param it Pointer to a token iterator (which is also a pointer).
 * @param identifiers Pointer to a list of tokens made of all identifiers related to nets.
 * @param list_param Pointer to a list of tokens made of identifiers related to parameters.
 * @param module Pointer to the Verilog module struct.
 * @return Error code of type VerilogError.
 */
VerilogError load_reg(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module);

/**
 * @brief Parsing das diretivas, algumas que não foram tratadas no pre-processamento.
 * @param it Pointer to a token iterator (which is also a pointer).
 * @param module Pointer to the Verilog module struct.
 * @return Error code of type VerilogError.
 */
VerilogError load_directive(Token** it, Module* module);

/**
 * @brief Parsing of initial blocks.
 * @param it Pointer to a token iterator (which is also a pointer).
 * @param identifiers Pointer to a list of tokens made of all identifiers related to nets.
 * @param list_param Pointer to a list of tokens made of identifiers related to parameters.
 * @param module Pointer to the Verilog module struct.
 * @param initial_task_events Pointer to a queue of events for system tasks.
 * @return Error code of type VerilogError.
 */
VerilogError load_initial_block(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module, Evento** initial_task_events);

/**
 * @brief Parsing of register attributions inside of initial blocks.
 * @param it Pointer to a token iterator (which is also a pointer).
 * @param list_param Pointer to a list of tokens made of identifiers related to parameters.
 * @param module Pointer to the Verilog module struct.
 * @return Error code of type VerilogError.
 */
VerilogError load_reg_attribution(Token** it, ListaToken* list_param, Module* module);

/**
 * @brief Parsing of assigns.
 * @param it Pointer to a token iterator (which is also a pointer).
 * @param list_wire Pointer to a list of tokens made of identifiers related to declared wire nets.
 * @param list_in Pointer to a list of tokens made of identifiers related to declared input ports.
 * @param list_out Pointer to a list of tokens made of identifiers related to declared output ports.
 * @param module Pointer to the Verilog module struct.
 * @return Error code of type VerilogError.
 */
VerilogError load_assign(Token** it, ListaToken* list_wire, ListaToken* list_in, ListaToken* list_out, Module* module);

/**
 * @brief Parsing of system tasks.
 * @param it Pointer to a token iterator (wich is also a pointer).
 * @param initial_task_events Pointer to a queue of events for systasks.
 * @return Error code of type VerilogError.
 */
VerilogError load_systask(Token** it, Evento** initial_task_events);

/** @brief Retorna verdadeiro se um Token representa um logic gate em Verilog.
 *  @param t Um Token pointer qualquer.
 *  @return Verdadeiro se t for: "and", "or", "nand", "buf", "not", "bufif0", etc.
 */
int is_logic_gate(const Token* t);

/** @brief Retorna verdadeiro se um logic gate é de 3 estados.
 *  @param gate Um Componente que representa um logic gate.
 *  @return Verdadeiro se o gate for: "bufif0", "bufif1", "notif0" ou "notif1".
 */
int is_tristate_logic(Component* gate);

/** @brief Retorna verdadeiro se uma string representa um logic gate em Verilog.
 *  @param s Uma string qualquer.
 *  @return Verdadeiro se s for igual a "and", "or", "nand", e etc.
 */
int isPortaLogica(char* s);

#ifdef __cplusplus
}
#endif

#endif // VERILOG_H
