/**
 * @file mem.h
 * @brief Prot�tipos dos wrapers para fun��es de manipula��o de mem�ria.
 */

#ifndef MEM_H

#define MEM_H

/** @brief Wrapper para malloc com verifica��o de erro
 */
void* xmalloc(size_t t);

/** @brief Wrapper para realloc com verifica��o de erro
 */
void* xrealloc(void* p, size_t t);

/** @brief Wrapper para calloc com verifica��o de erro
 */
void* xcalloc(size_t n, size_t t);

#endif // MEM_H
