/**
 * @file mem.h
 * @brief Protótipos dos wrapers para funções de manipulação de memória.
 */

#ifndef MEM_H

#define MEM_H

/** @brief Wrapper para a função malloc com verificação de erro.
 */
void* xmalloc(size_t t);

/** @brief Wrapper para a função realloc com verificação de erro.
 */
void* xrealloc(void* p, size_t t);

/** @brief Wrapper para a função calloc com verificação de erro.
 */
void* xcalloc(size_t n, size_t t);

#endif // MEM_H
