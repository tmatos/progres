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

/** @brief Cria uma estrutura de dados representando todos os sinas de entrada
            lidos partir do arquivo de entrada correspondente (extensão *.in).
 *  @param arquivo O handler do arquivo de entrada com sinais a ser processado.
 *  @return A estrutura de dados contendo todos os sinais lidos do arquivo.
 */
Sinais* carregaEntradas(FILE *arquivo)

#endif
