/**
 * @file preprocessor.h
 * @brief Pre-processing of compiler directives.
 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "lex.h"
#include "verilog.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 *  @param lst Pointer to the list of macros.
 *  @param name String with the name of the macro.
 *  @param value String with the value of the macro.
 *  @return void
 */
void insert_macro(ListMacro* lst, const char* name, const char* value);

/** @brief Find a macro with the given name on the list.
 *  @param list List of macros.
 *  @param name String with the name of the macro to find.
 *  @return Pointer to the macro, case found, NULL otherwise.
 */
Macro* get_macro_by_name(ListMacro list, const char* name);

/** @brief Remove the specified macro from the list.
 *  @param list Pointer to the list of macros.
 *  @param name String with the name of the macro to remove.
 *  @return void
 */
void remove_macro_by_name(ListMacro* list, const char* name);

/** @brief Do pre-processing in the list of tokens.
 *  @param lst Pointer to the list of tokens.
 *  @return 1 if sucess, 0 otherwise.
 */
int pre_processor(ListaToken* lst);

/** @brief Process the `define` directive.
 *  @param list_tok Pointer to the list of tokens of the source.
 *  @param p_tok_it Pointer to the current token iterator.
 *  @param list_macro Pointer to the list of macros.
 *  @return VerilogError indicating success or type of failure.
 */
VerilogError preproc_define(ListaToken* list_tok, Token** p_tok_it, ListMacro* list_macro);

/** @brief Process the `undef` directive.
 *  @param list_tok Pointer to the list of tokens of the source.
 *  @param p_tok_it Pointer to the current token iterator.
 *  @param list_macro Pointer to the list of macros.
 *  @return VerilogError indicating success or type of failure.
 */
VerilogError preproc_undef(ListaToken* list_tok, Token** p_tok_it, ListMacro* list_macro);

/** @brief Process the `timescale` directive.
 *  @param p_tok_it Pointer to the current token iterator.
 *  @return VerilogError indicating success or type of failure.
 */
VerilogError preproc_timescale(Token** p_tok_it);

#ifdef __cplusplus
}
#endif

#endif // PREPROCESSOR_H
