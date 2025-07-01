/**
 * @file inout.h
 * @brief Protótipos das funções de leitura e gravação dos arquivos de sinas de entrada e saída.
 */

#ifndef INOUT_H
#define INOUT_H

#ifdef __cplusplus
extern "C" {
#endif

/// Mensagem a ser impressa em caso de erros no arquivo de ondas
#define MSG_ARQUIVO_ENTRADA_CORROMPIDO "Arquivo de entrada corrompido.\n"

/// Mensagem a ser impressa no caso de o arquivo estar vazio
#define MSG_ARQUIVO_ENTRADA_VAZIO "Arquivo de entrada aparentemente vazio.\n"

/// Mensagem a ser impressa quando o arquivo for lido completamente
#define MSG_ARQUIVO_ENTRADA_LIDO_COMPLETO "Arquivo de entrada completamente lido.\n"

/** @brief Cria uma estrutura de dados representando todos os sinas de entrada
           lidos partir do arquivo de entrada correspondente (extensão *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE* arquivo);

/** @brief Salva todos os sinais contidos no conjunto para o arquivo de saída
           com a formatação padrão.
 *  @param sinaisSaida Conjunto dos sinais a serem salvos.
 *  @param arqSaida Arquivo a ser escrito com a representação dos sinais.
 *  @return void
 */
void salvarSinais(Sinais* sinaisSaida, FILE* arqSaida);

/** @brief Exibe uma mensagem de erro e retorna um NULL.
 *         Indicando que o arquivo de entrada está corrompido.
 *  @return Um ponteiro para uma struct Sinais com valor NULL.
*/
Sinais* erroFatalArquivoCorrompido();

#ifdef __cplusplus
}
#endif

#endif // INOUT_H
