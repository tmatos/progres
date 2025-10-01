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
 *  @param file O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* load_input_signals(FILE* file);

/** @brief Salva todos os sinais contidos no conjunto para o arquivo de saída
           com a formatação padrão.
 *  @param signals Conjunto dos sinais a serem salvos.
 *  @param file Arquivo a ser escrito com a representação dos sinais.
 *  @return void
 */
void save_signals(Sinais* signals, FILE* file);

/** @brief Gera um arquivo VCD (Value Change Dump) a partir do módulo e dos sinais.
 *         O arquivo VCD é usado para registrar as mudanças de valor nos sinais
 *         durante a simulação do módulo.
 * @param module O módulo que contém o circuito simulado.
 * @param sinais A lista de sinais que serão registrados no VCD.
 * @param file O handler do arquivo onde o VCD será escrito.
 * @return void
 */
void save_vcd(Module* module, Sinais* sinais, FILE* file);

/** @brief Converte um valor lógico para o caractere correspondente.
 *  @param value Algum dos disponíveis na enum ValorLogico.
 *  @return Um dos seguintes: '0', '1', 'x' ou 'z'.
 *          Se o valor não for reconhecido, retorna 'x' por padrão.
 *  @note Os valores 'H' e 'L' são tratados como '1' e '0', respectivamente.
 */
char get_char_from_logic_value(ValorLogico value);

/** @brief Exibe uma mensagem de erro e retorna um NULL.
 *         Indicando que o arquivo de entrada está corrompido.
 *  @return Um ponteiro para uma struct Sinais com valor NULL.
*/
Sinais* show_fatal_error_corrupt_file();

#ifdef __cplusplus
}
#endif

#endif // INOUT_H
