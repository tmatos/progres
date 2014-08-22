/**
 * @file inout.h
 * @brief Protótipos das funções de leitura e gravação dos arquivos de sinas de entrada e saída.
 */

#ifndef INOUT_H

#define INOUT_H

#define MSG_ARQUIVO_ENTRADA_CORROMPIDO "Arquivo de entrada corrompido.\n" /// Msg a ser impressa em caso de erros no arquivo de ondas

/** @brief Cria uma estrutura de dados representando todos os sinas de entrada
            lidos partir do arquivo de entrada correspondente (extensão *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE *arquivo);

/** @brief Salva todos os sinais contidos no conjunto para o arquivo de saida
            com a formatação padrão.
 */
void salvarSinais(Sinais *sinaisSaida, FILE *arqSaida);

#endif // INOUT_H
