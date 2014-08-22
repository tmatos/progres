/**
 * @file inout.h
 * @brief Prot�tipos das fun��es de leitura e grava��o dos arquivos de sinas de entrada e sa�da.
 */

#ifndef INOUT_H

#define INOUT_H

#define MSG_ARQUIVO_ENTRADA_CORROMPIDO "Arquivo de entrada corrompido.\n" /// Msg a ser impressa em caso de erros no arquivo de ondas

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

#endif // INOUT_H
