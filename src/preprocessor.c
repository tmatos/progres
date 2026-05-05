/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "erros.h"
#include "strutil.h"
#include "lex.h"
#include "verilog.h"
#include "estruturas.h"
#include "preprocessor.h"

void insert_macro(ListMacro* lst, const char* name, const char* value)
{
    Macro* m = NULL;

    if (lst->total == 0) {
        lst->total++;
        lst->itens = (Macro**) xmalloc(sizeof(Macro*));
    }
    else {
        lst->total++;
        lst->itens = (Macro**) xrealloc(lst->itens,
                                        sizeof(Macro*) * lst->total);
    }

    m = (Macro*) xmalloc(sizeof(Macro));
    copy(m->name, name);
    copy(m->value, value);

    lst->itens[lst->total - 1] = m;
}

Macro* get_macro_by_name(ListMacro list, const char* name)
{
    int i;

    if (!name)
        return NULL;
    
    for ( i = 0 ; i < list.total ; i++ ) {
        if( iguais(list.itens[i]->name, name) ) {
            return list.itens[i];
        }
    }
    
    return NULL;
}

void remove_macro_by_name(ListMacro* list, const char* name)
{
    int i;

    if (!name)
        return;

    // TODO: refactor this
    
    // find the index of the element to remove
    for ( i = 0 ; i < list->total ; i++ )
    {
        if ( iguais(list->itens[i]->name, name) ) {
            break;
        }
    }

    // case of removing the latest element
    if ( i == (list->total - 1) ) {
        list->total--;
        if (list->total == 0) {
            free(list->itens);
        }
        return;
    }

    // here, we always have a next element
    while ( i < (list->total - 1) )
    {
        i++;
        copy(list->itens[i-1]->name, list->itens[i]->name);
        copy(list->itens[i-1]->value, list->itens[i]->value);
    }

    list->total--;
}

PreprocesorResult pre_processor(ListToken* lst)
{
    Token* it;
    Macro* macro;
    VerilogError e;

    ListMacro list_macro;
    list_macro.total = 0;
    list_macro.itens = NULL;

    it = lst->first;

    while (it)
    {
        if ( it->classe != SYM_GRAVE_ACCENT ) {
            avanca(&it);
            continue;
        }

        if ( !avanca(&it) )
            goto pre_processor_error_bad_eof;

        if ( iguais("define", it->value) ) {
            e = preproc_define(lst, &it, &list_macro);

            if (e == ERROR_VERILOG_BAD_EOF) {
                goto pre_processor_error_bad_eof;
            }
            else if (e == ERROR_VERILOG_BAD_TOKEN) {
                return PREPROCESSOR_ERROR; // error already shown
            }

            continue;
        }
        else if (iguais("undef", it->value)) {
            e = preproc_undef(lst, &it, &list_macro);

            if (e == ERROR_VERILOG_UNDECLARED_MACRO) {
                goto pre_processor_error_undeclared_macro;
            }
            else if (e == ERROR_VERILOG_BAD_EOF) {
                goto pre_processor_error_bad_eof;
            }
            else if (e == ERROR_VERILOG_BAD_TOKEN) {
                return PREPROCESSOR_ERROR; // error already shown
            }
            
            continue;
        }
        else if ( iguais("timescale", it->value) ) {
            e = preproc_timescale(&it);

            if (e == ERROR_VERILOG_BAD_EOF) {
                goto pre_processor_error_bad_eof;
            }
        }
        else if (iguais("resetall", it->value)) {

            // TODO

        }
        else if (is_allowed_identifier(it)) {
            macro = get_macro_by_name(list_macro, it->value);

            if (!macro)
                goto pre_processor_error_undeclared_macro;

            remove_token(lst, it->previous); // remove previous '`' token
            
            // update current token
            copy(it->value, macro->value);
            it->classe = get_token_class(it->value);
        }
        else {
            show_error_msg("Token inesperado",
                           it->line,
                           it->column,
                           "diretiva de compilador ou identificador",
                           it->value);
            return PREPROCESSOR_ERROR;
        }

        avanca(&it);
    }

//pre_processor_success:
    return PREPROCESSOR_SUCCESS;

pre_processor_error_bad_eof:
    show_error_msg("Final inesperado de arquivo", -1, -1, NULL, NULL);
    return PREPROCESSOR_ERROR;

pre_processor_error_undeclared_macro:
    show_error_msg("Macro nao declarada", it->line, it->column,
                   "macro previamente declarada", it->value);
    return PREPROCESSOR_ERROR;
}

VerilogError preproc_define(
    ListToken* list_tok,
    Token** p_tok_it,
    ListMacro* list_macro)
{
    Token* temp;

    char macro_name[MAX_MACRO_NAME_SIZE];
    char macro_value[MAX_MACRO_VALUE_SIZE];

    Token* it = *p_tok_it;

    if ( !avanca(&it) )
        goto preproc_define_error_bad_eof;

    if ( !is_allowed_identifier(it) ) {
        show_error_msg("Token inesperado", it->line, it->column,
                       "identificador", it->value);
        goto preproc_define_error_bad_token;
    }

    if ( len(it->value) > MAX_MACRO_NAME_SIZE ) {
        show_error_size_exceeded("Nome de macro muito grande",
                                 it->line, it->column, it->value,
                                 MAX_MACRO_NAME_SIZE);
        goto preproc_define_error_bad_token;
    }

    copy(macro_name, it->value);
    
    if ( !avanca(&it) )
        goto preproc_define_error_bad_eof;

    if ( len(it->value) > MAX_MACRO_VALUE_SIZE ) {
        show_error_size_exceeded("Comprimento da macro muito grande",
                                 it->line, it->column, it->value,
                                 MAX_MACRO_VALUE_SIZE);
        goto preproc_define_error_bad_token;
    }

    copy(macro_value, it->value);

    insert_macro(list_macro, macro_name, macro_value);

    remove_token(list_tok, it->previous->previous->previous); // grave accent
    remove_token(list_tok, it->previous->previous); // define directive
    remove_token(list_tok, it->previous); // macro identifier

    temp = it; // macro value

    avanca(&it);

    remove_token(list_tok, temp);

    *p_tok_it = it;

//preproc_define_success:
    return NO_ERROR_VERILOG;

preproc_define_error_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

preproc_define_error_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError preproc_undef(
    ListToken* list_tok,
    Token** p_tok_it,
    ListMacro* list_macro)
{   
    Token* temp;
    Macro* macro = NULL;

    Token* it = *p_tok_it;

    if ( !avanca(&it) )
        goto preproc_undef_error_bad_eof;

    if ( !is_allowed_identifier(it) ) {
        show_error_msg("Token inesperado", it->line, it->column,
                       "identificador", it->value);
        goto preproc_undef_error_bad_token;
    }

    macro = get_macro_by_name(*list_macro, it->value);

    if (!macro)
        return ERROR_VERILOG_UNDECLARED_MACRO;

    remove_macro_by_name(list_macro, it->value);

    temp = it;

    avanca(&it);
    
    remove_token(list_tok, temp->previous->previous); // grave accent
    remove_token(list_tok, temp->previous); // undefine directive
    remove_token(list_tok, temp); // macro identifier

    *p_tok_it = it;

//preproc_undef_success:
    return NO_ERROR_VERILOG;

preproc_undef_error_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

preproc_undef_error_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError preproc_timescale(Token** p_tok_it)
{
    Token* it = *p_tok_it;

    // time_unit / time_precision
    // ex.: 1 ns / 1 ps

    if ( !avanca(&it) )
        goto pre_processor_error_bad_eof;

    // [time_unit] / time_precision
    // [number] unit / number unit

    if ( !avanca(&it) )
        goto pre_processor_error_bad_eof;

    // [time_unit] / time_precision
    // number [unit] / number unit

    if ( !avanca(&it) )
        goto pre_processor_error_bad_eof;

    // time_unit [/] time_precision
    // number unit [/] number unit

    if ( !avanca(&it) )
        goto pre_processor_error_bad_eof;

    // time_unit / [time_precision]
    // number unit / [number] unit

    if ( !avanca(&it) )
        goto pre_processor_error_bad_eof;

    // time_unit / [time_precision]
    // number unit / number [unit]

    *p_tok_it = it;

//preproc_timescale_success:
    return NO_ERROR_VERILOG;

pre_processor_error_bad_eof:
    return ERROR_VERILOG_BAD_EOF;

// preproc_timescale_error_bad_token:
//     return ERROR_VERILOG_BAD_TOKEN;
}
