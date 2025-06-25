/**
 * @file verilog.h
 * @brief Rotinas para parsing de arquivos Verilog
 */

#ifndef VERILOG_H

#define VERILOG_H

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
    NO_ERROR = 10
} VerilogError;

/** @brief Processar a parte do código que possui a declaração
 *         de cabecalho do module, até o token ';'.
 *  @param it Endereço para o iterador dos tokens do código fonte.
 *  @param identifiers Lista de tokens contendo os identificadores já detectados.
 *  @param livres Lista de identificadores de entrada ou saida ainda nao definidos como tal.
 *  @return 0 em caso de falha, 1 em caso de sucesso.
 */
int load_module_header(Token** it, ListaToken* identifiers, ListaToken* livres);

/** @brief Cria uma estrutura de dados representando o circuito,
           a partir do arquivo com o codigo fonte em Verilog.
 *  @param file_path String com o caminho do arquivo a ser processado.
 *  @return A estrutura de dados do circuito ou NULL em caso de erro.
 */
Module* carregaCircuito(const char* file_path);

/** @brief Parsing de definições de range.
 *  @param it Endereço para o iterador dos tokens.
 *  @param module Pointer to verilog module struct.
 *  @param list_param Params definidos.
 *  @param range_msb Ponteiro para a variável que guarda o indice do bit mais significativo.
 *  @param range_lsb Ponteiro para a variável que guarda o indice do bit menos significativo.
 *  @return Código de erro do tipo VerilogError.
 */
VerilogError load_range(Token** it, Module* module, ListaToken* list_param, int* range_msb, int* range_lsb);

/** @brief Parsing of 'reg' declaration
 */
VerilogError load_reg(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module);

/** @brief Parsing das diretivas, algumas que não foram tratadas no pre-processamento.
 *  @return Código de erro do tipo VerilogError.
 */
VerilogError load_directive(Token** it, Module* module);

/** @brief Parsing de blocos initial
 */
VerilogError load_initial_block(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module, Evento** initial_task_events);

/** @brief Parsing de 'assign'
 */
VerilogError load_assign(Token** it, ListaToken* list_wire, ListaToken* list_in, ListaToken* list_out, Module* module);

/** @brief Parsing of system tasks
 */
VerilogError load_systask(Token** it, Evento** initial_task_events, Module* module);

/** @brief Retorna verdadeiro se um Token representa um logic gate em Verilog.
 *  @param t Um Token pointer qualquer.
 *  @return Verdadeiro se t for: "and", "or", "nand", "buf", "not", "bufif0", etc.
 */
int is_logic_gate(const Token* t);

/** @brief Retorna verdadeiro se um logic gate é de 3 estados.
 *  @param gate Um Componente que representa um logic gate.
 *  @return Verdadeiro se o gate for: "bufif0", "bufif1", "notif0" ou "notif1".
 */
int is_tristate_logic(Componente gate);

/** @brief Retorna verdadeiro se uma string representa um logic gate em Verilog.
 *  @param s Uma string qualquer.
 *  @return Verdadeiro se s for igual a "and", "or", "nand", e etc.
 */
int isPortaLogica(char* s);

#endif // VERILOG_H
