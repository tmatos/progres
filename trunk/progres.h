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
            lidos partir do arquivo de entrada correspondente (extens�o *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE *arquivo);

#endif
