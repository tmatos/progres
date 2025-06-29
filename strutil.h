/**
 * @file strutil.h
 * @brief String handling functions and wrappers.
 */

#ifndef STRUTIL_H
#define STRUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Faz o append de um char numa string qualquer.
 *  @param str Uma string qualquer.
 *  @param c Um caractere.
 *  @return No momento, sempre 1.
 */
int anexa(char* str, char c);

/** @brief Retorna verdadeiro se duas strings são iguais.
 *  @param a Uma string qualquer.
 *  @param b Uma string qualquer.
 *  return O resultado é a negação de strcmp(a,b).
 */
int iguais(const char* a, const char* b);

/** @brief Verifica se uma string contém apenas dígitos (0, 1, 2, ..., 9).
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se há apenas dígitos, falso na ocorrência de
 *          qualquer outro tipo de caractere.
 */
int apenasDigitos(const char* str);

/** @brief Verifica se uma string contém um número que pode ser convertido.
           Mais especeificamente, se é um natural menor que 10000.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se pode ser convertido.
 */
int isNumNaturalValido(const char* str);

/** @brief Wrapper to strlen()
 *  @param str A null terminated C string
 *  @return Lenght of str
 */
size_t len(const char* str);

/** @brief Wrapper to strcpy()
 *  @param dest Destination C string
 *  @param src Source C string
 *  @return Return of strcpy()
 */
char* copy(char* dest, const char* src);

#ifdef __cplusplus
}
#endif

#endif // STRUTIL_H
