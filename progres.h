/**
 * @file progres.h
 * @brief Prot�tipos do m�dulo principal do programa.
 */

#ifndef PROGRES_H

#define PROGRES_H

#include <stdio.h>

#include "estruturas.h"

#define MAX_FILE_PATH_SIZE 4096 /// Tamanho m�ximo permitido para o argumento de linha comando relat. ao arquivo de entrada

#define MSG_ARQUIVO_ENTRADA_CORROMPIDO "Arquivo de entrada corrompido.\n" /// Msg a ser impressa em caso de erros no arquivo de ondas

/** @brief Fun��o que faz a simula��o do circuito com as entradas especificadas.
           Em caso de sucesso, retorna as sa�das dessa simula��o.
 */
Sinais* simula(t_circuito* circuto, Sinais* entradas);

/** @brief Cria uma estrutura de dados representando todos os sinas de entrada
            lidos partir do arquivo de entrada correspondente (extens�o *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE *arquivo);

/** @brief Salva todos os sinais contidos no conjunto para o arquivo de saida
            com a formata��o padr�o.
 */
void salvarSinais(Sinais *sinaisSaida, FILE *arqSaida);

/** @brief Wrapper para malloc com verifica��o de erro
 */
void* xmalloc(size_t t);

/** @brief Wrapper para realloc com verifica��o de erro
 */
void* xrealloc(void* p, size_t t);

/** @brief Wrapper para calloc com verifica��o de erro
 */
void* xcalloc(size_t n, size_t t);

#endif
