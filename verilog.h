/**
 * @file verilog.h
 * @brief Rotinas para análise do arquivo Verilog
 */

#ifndef VERILOG_H

#define VERILOG_H

#include <stdio.h>

#include "estruturas.h"

/** @brief Cria uma estrutura de dados representando o circuito,
            a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A estrutura de dados do circuito.
 */
t_circuito* carregaCircuito(FILE* arquivo);

/** @brief Retorna verdadeiro se uma string representa uma porta lógica em Verilog.
 *  @param s Uma string qualquer.
 *  @return Verdadeiro se s for igual a "and", "or", "nand", e etc.
 */
int isPortaLogica(char* s);

#endif // VERILOG_H
