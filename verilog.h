/**
 * @file verilog.h
 * @brief Rotinas para parsing de arquivos Verilog
 */

#ifndef VERILOG_H

#define VERILOG_H

#include <stdio.h>

#include "estruturas.h"
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
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A estrutura de dados do circuito.
 */
Module* carregaCircuito(FILE* arquivo);

/** @brief Parsing of 'reg' declaration
 */
VerilogError load_reg(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module);

/** @brief Parsing das diretivas, algumas que não foram tratadas no pre-processamento.
 *  @return Código de erro do tipo VerilogError.
 */
VerilogError load_directive(Token** it, Module* module);

/** @brief Parsing de blocos initial
 */
VerilogError load_initial_block(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module);

/** @brief Parsing de 'assign'
 */
VerilogError load_assign(Token** it, ListaToken* list_wire, ListaToken* list_in, ListaToken* list_out, Module* module);

/** @brief Retorna verdadeiro se uma string representa uma logic gate em Verilog.
 *  @param s Uma string qualquer.
 *  @return Verdadeiro se s for igual a "and", "or", "nand", e etc.
 */
int isPortaLogica(char* s);

#endif // VERILOG_H
