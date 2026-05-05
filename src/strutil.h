/**
 * @file strutil.h
 * @brief String handling functions and wrappers.
 */

#ifndef STRUTIL_H
#define STRUTIL_H

extern int global_silent_mode;

#ifdef __cplusplus
extern "C" {
#endif

// Stringification macros
#define STR(x) #x
#define XSTR(x) STR(x)

/** @brief Faz o append de um char numa string qualquer.
 *  @param str Uma string qualquer.
 *  @param c Um caractere.
 *  @return No momento, sempre 1.
 */
int anexa(char* str, char c);

/** @brief Retorna verdadeiro se duas strings são iguais.
 *  @param a Uma string qualquer.
 *  @param b Uma string qualquer.
 *  @return O resultado é a negação de strcmp(a,b).
 */
int iguais(const char* a, const char* b);

/** @brief Verifica se uma string contém apenas dígitos (0, 1, 2, ..., 9).
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se há apenas dígitos, falso na ocorrência de
 *          qualquer outro tipo de caractere.
 */
int has_only_digits(const char* str);

/** @brief Verifica se uma string contém um número que pode ser convertido.
           Mais especeificamente, se é um natural menor que 10000.
 *  @param str Uma string qualquer.
 *  @return Verdadeiro se pode ser convertido.
 */
int is_valid_natural_number(const char* str);

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

/** @brief Copy src to dest, but removing the first and last characters in the
 *         source string only if they both are quotes ("").
 *  @param dest Destination C string.
 *  @param src Source C string.
 *  @return Same return of the copy() function, after removing the quotes.
 */
char* copy_removing_quotes(char* dest, const char* src);

/** @brief Wrapper to printf()
 *  @param fmt String with the format argument
 */
void print(const char* fmt, ...);

/** @brief Remove all underscores from a string literal, modifying it in place.
 *  @param str Pointer to the string literal to be modified.
 */
void remove_underscores_from_literal(char* str);

#ifdef __cplusplus
}
#endif

#endif // STRUTIL_H
