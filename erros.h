/**
 * @file erros.h
 * @brief Gerenciamento de mensagens de erros, ou casos de erro.
 */

#ifndef ERROS_H

#define ERROS_H

extern int global_silent_mode;

#define MSG_ERROR_LEX_UNEXPECTED_SYMBOL "Simbolo nao esperado"
#define MSG_ERROR_LEX_INVALID_CHAR "Caractere nao permitido"
#define MSG_ERROR_LEX_TOKEN_SIZE_MAXED "Token excede o tamanho maximo permitido"

/** @brief Exibe uma mensagem de erro sobre declarar um identificador com nome ja utilizado.
 *  @param tok String com o identificador. 
 *  @param lin A linha onde ocorre o erro.
 *  @param col A coluna onde ocorre o erro.
 */
void show_error_identifier_duplicate(const char* tok, int lin, int col);

/** @brief Exibe uma mensagem de erro relativa a analise lexica do arquivo fonte em questao.
 *  @param msg O texto da mensagem de erro a ser exibida.
 *  @param lin A linha onde ocorre o erro no fonte. -1 para omitir.
 *  @param col A coluna onde ocorre o erro no fonte. -1 para omitir.
 */
void show_error_lexical(const char* msg, int lin, int col);

/** @brief Exibe na saida padrao, uma mensagem de erro relativa a analise
            lexica ou sintatica do arquivo fonte em questao.
 *  @param msg O texto da mensagem de erro a ser exibida.
 *  @param lin A linha onde ocorre o erro no fonte. -1 para omitir.
 *  @param col A coluna onde ocorre o erro no fonte. -1 para omitir.
 *  @param expected Uma string com o que era esperado. NULL para omitir.
 *  @param found Uma string com o que foi encontrado no lugar. NULL para omitir.
 */
void show_error_msg(const char* msg, int lin, int col, const char* expected, const char* found);

/** @brief Exibe, na saida padrao, uma mensagem de erro relativa a violacao de restricao para tamanhos.
 *  @param msg Um texto para a mensagem de erro a ser exibida.
 *  @param lin A linha onde ocorre o erro, no codigo fonte.
 *  @param col A coluna onde ocorre o erro, no codigo fonte.
 *  @param tok String com a representacao do token relativo ao erro.
 *  @param max Numero que representa o valor maximo admitido.
 */
void show_error_size_exceeded(const char *msg, int lin, int col, const char *tok, int max);

/** @brief Exibe uma mensagem de erro por falta de memoria e encerra o programa.
 *  @return void (program error code: -1)
 */
void fatal_error_no_memory();

#endif // ERROS_H
