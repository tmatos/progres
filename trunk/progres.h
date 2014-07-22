/**
 * @file progres.h
 * @brief Prototipos do modulo principal do programa.
 */

#ifndef PROGRES_H

#define PROGRES_H

#include <stdio.h>
#include "estruturas.h"

#define MSG_ARQUIVO_ENTRADA_CORROMPIDO "Arquivo de entrada corrompido.\n"

/** @brief Cria uma estrutura de dados representando todos os sinas de entrada
            lidos partir do arquivo de entrada correspondente (extensão *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE *arquivo);

/** @brief Wrapper para malloc com verificação de erro
 */
void* xmalloc(size_t t);

/** @brief Wrapper para realloc com verificação de erro
 */
void* xrealloc(void* p, size_t t);

/** @brief Wrapper para calloc com verificação de erro
 */
void* xcalloc(size_t n, size_t t);

#endif
