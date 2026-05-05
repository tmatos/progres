/**
 * @file mem.h
 * @brief Protótipos dos wrapers para funções de manipulação de memória.
 */

#ifndef MEM_H
#define MEM_H

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Wrapper para a função malloc com verificação de erro.
 *  @param t Tamanho em bytes a ser alocado.
 *  @return Um ponteiro para a memória alocada.
 *  @note Se a alocação falhar, chama fatal_error_no_memory()
 *        e encerra o programa.
 */
void* xmalloc(size_t t);

/** @brief Wrapper para a função realloc com verificação de erro.
 *  @param p Ponteiro para o endereço na memória onde será iniciada
 *           a realocação.
 *  @param t Tamanho em bytes a ser realocado.
 *  @return Um ponteiro para a memória realocada.
 *  @note Se a realocação falhar, chama fatal_error_no_memory()
 *        e encerra o programa.
 */
void* xrealloc(void* p, size_t t);

/** @brief Wrapper para a função calloc com verificação de erro.
 *  @return Um ponteiro para a memória alocada e inicializada com zero.
 *  @note Se a alocação falhar, chama fatal_error_no_memory()
 *        e encerra o programa. 
 */
void* xcalloc(size_t n, size_t t);

#ifdef __cplusplus
}
#endif

#endif // MEM_H
