/**
 * @file progres.h
 * @brief Prototipos do modulo principal do programa.
 */

#ifndef PROGRES_H

#define PROGRES_H

/** @brief Cria uma estrutura de dados representando o circuito,
            a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A estrutura de dados do circuito.
 */
t_circuito* carregaCircuito(FILE* arquivo);

/** @brief Cria uma lista de Tokens que tem significado para o processamento
            sintatico, a partir do arquivo com o codigo fonte em Verilog.
 *  @param arquivo O handler do arquivo a ser processado.
 *  @return A lista de tokens.
 */
ListaToken* tokeniza(FILE *arquivo);

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
