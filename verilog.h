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

/** @brief Cria uma estrutura de dados representando o circuito,
           a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A estrutura de dados do circuito.
 */
Module* carregaCircuito(FILE* arquivo);

/** @brief Parsing de blocos initial
 */
VerilogError load_initial_block(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module);

/** @brief Retorna verdadeiro se uma string representa uma logic gate em Verilog.
 *  @param s Uma string qualquer.
 *  @return Verdadeiro se s for igual a "and", "or", "nand", e etc.
 */
int isPortaLogica(char* s);

#endif // VERILOG_H
