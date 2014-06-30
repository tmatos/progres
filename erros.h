/**
 * @file erros.h
 * @brief Gerenciamento de mensagens de erros, ou casos de erro.
 */

#ifndef ERROS_H

#define ERROS_H

/** @brief Exibe na saida padrao, uma mensagem de erro relativa a analise
            lexica ou sintatica do arquivo fonte em questao.
 *  @param msg O texto da mensagem de erro a ser exibida.
 *  @param linha A linha onde ocorre o erro no fonte. -1 para omitir.
 *  @param coluna A coluna onde ocorre o erro no fonte. -1 para omitir.
 *  @param esperado Uma string com o que era esperado. NULL para omitir.
 *  @param encontrado Uma string com o que foi encontrado no lugar. NULL para omitir.
 *  @return Void.
 */
void exibeMsgErro(char* msg, int linha, int coluna, char* esperado, char *encontrado);

#endif
