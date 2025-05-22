/**
 * @file preprocessor.h
 * @brief Pre-processing of compiler directives.
 */

#ifndef PREPROCESSOR_H

#define PREPROCESSOR_H

#include "lex.h"

#define MAX_MACRO_NAME_SIZE 64

#define MAX_MACRO_VALUE_SIZE 96

/** @brief Struct for macros.
 */
typedef struct st_macro {
    char name[MAX_MACRO_NAME_SIZE];
    char value[MAX_MACRO_VALUE_SIZE];
} Macro;

/** @brief Struct for a list of macros.
 */
typedef struct st_list_macro {
    int total;
    Macro** itens;
} ListMacro;

/** @brief Insert a new macro on the list.
 */
void insert_macro(ListMacro* lst, const char* name, const char* value);

/** @brief Find a macro with the given name on the list.
 *  @return Pointer to the macro, case found, NULL otherwise.
 */
Macro* get_macro_by_name(ListMacro list, const char* name);

/** @brief Remove the specified macro from the list.
 */
void remove_macro_by_name(ListMacro* list, const char* name);

/** @brief Process the list of tokens.
 *  @return 1 if sucess, 0 otherwise.
 */
int pre_processor(ListaToken* lst);

#endif // PREPROCESSOR_H
