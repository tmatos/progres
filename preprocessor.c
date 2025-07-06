/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "mem.h"
#include "erros.h"
#include "strutil.h"
#include "lex.h"
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
        lst->itens = (Macro**) xrealloc(lst->itens, sizeof(Macro*) * lst->total);
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

int pre_processor(ListaToken* lst)
{
    Token* it;
    Token* temp;
    Macro* macro;

    char macro_name[MAX_MACRO_NAME_SIZE];
    char macro_value[MAX_MACRO_VALUE_SIZE];

    ListMacro list_macro;
    list_macro.total = 0;
    list_macro.itens = NULL;

    it = lst->primeiro;

    while (it)
    {
        if (it->classe != SYM_GRAVE_ACCENT) {
            avanca(&it);
            continue;
        }

        if (!avanca(&it))
            goto pre_processor_error_bad_eof;

        if (iguais("define", it->valor)) {
            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            if (!isIdentificador(it)) {
                show_error_msg("Token inesperado", it->linha, it->coluna,
                               "identificador", it->valor);
                return 0;
            }

            if (len(it->valor) > MAX_MACRO_NAME_SIZE) {
                show_error_size_exceeded("Nome de macro muito grande",
                                         it->linha, it->coluna, it->valor,
                                         MAX_MACRO_NAME_SIZE);
                return 0;
            }

            copy(macro_name, it->valor);
            
            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            if (len(it->valor) > MAX_MACRO_VALUE_SIZE) {
                show_error_size_exceeded("Comprimento da macro muito grande",
                                         it->linha, it->coluna, it->valor,
                                         MAX_MACRO_VALUE_SIZE);
                return 0;
            }

            copy(macro_value, it->valor);

            insert_macro(&list_macro, macro_name, macro_value);

            remove_token(lst, it->anterior->anterior->anterior); // grave accent
            remove_token(lst, it->anterior->anterior); // define directive
            remove_token(lst, it->anterior); // macro identifier

            temp = it; // macro value

            avanca(&it);

            remove_token(lst, temp);

            continue;
        }
        else if (iguais("undef", it->valor)) {
            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            if (!isIdentificador(it)) {
                show_error_msg("Token inesperado", it->linha, it->coluna,
                               "identificador", it->valor);
                return 0;
            }

            macro = get_macro_by_name(list_macro, it->valor);

            if (!macro)
                goto pre_processor_error_undeclared_macro;

            remove_macro_by_name(&list_macro, it->valor);

            temp = it;

            avanca(&it);
            
            remove_token(lst, temp->anterior->anterior); // grave accent
            remove_token(lst, temp->anterior); // undefine directive
            remove_token(lst, temp); // macro identifier
            
            continue;
        }
        else if (iguais("timescale", it->valor)) {
            // time_unit / time_precision
            // ex.: 1 ns / 1 ps

            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            // [time_unit] / time_precision
            // [number] unit / number unit

            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            // [time_unit] / time_precision
            // number [unit] / number unit

            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            // time_unit [/] time_precision
            // number unit [/] number unit

            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            // time_unit / [time_precision]
            // number unit / [number] unit

            if (!avanca(&it))
                goto pre_processor_error_bad_eof;

            // time_unit / [time_precision]
            // number unit / number [unit]

        }
        else if (iguais("resetall", it->valor)) {

            // TODO

        }
        else if (isIdentificador(it)) {
            macro = get_macro_by_name(list_macro, it->valor);

            if (!macro)
                goto pre_processor_error_undeclared_macro;

            remove_token(lst, it->anterior); // remove previous '`' token
            
            // update current token
            copy(it->valor, macro->value);
            it->classe = get_token_class(it->valor);
        }
        else {
            show_error_msg("Token inesperado",
                           it->linha,
                           it->coluna,
                           "diretiva de compilador ou identificador",
                           it->valor);
            return 0;
        }

        avanca(&it);
    }

    return 1;

pre_processor_error_bad_eof:
    show_error_msg("Final inesperado de arquivo", -1, -1, NULL, NULL);
    return 0;

pre_processor_error_undeclared_macro:
    show_error_msg("Macro nao declarada", it->linha, it->coluna,
                   "macro previamente declarada", it->valor);
    return 0;
}
