/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mem.h"
#include "erros.h"
#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "eventos.h"
#include "strutil.h"
#include "lex.h"
#include "preprocessor.h"

int load_module_header(Token** it, ListToken* identifiers, ListToken* livres, Module* module)
{
    int expect_comma = 0; //flag para indicar se estamos esperando por uma virgula

    Token* t = *it;

    if (t->classe != KW_MODULE) {
        show_error_msg("Palavra-chave nao encontrada onde esperada",
                       t->linha, t->coluna, "module", t->valor);
        goto load_module_header_bad_return;
    }

    if (!avanca(&t)) {
        show_error_msg("Final do arquivo nao esperado", -1, -1, "um identificador", NULL);
        goto load_module_header_bad_return;
    }
    else if (!is_allowed_identifier(t)) {
        show_error_msg("Identificador nao encontrado",
                       t->linha, t->coluna,"identificador valido", t->valor);
        goto load_module_header_bad_return;
    }
    else {
        // senao, adicione-o a lista de identifiers
        insert_token_of_string(identifiers, t->valor, -1, -1, IDENTIFIER);
        copy(module->name, t->valor);
    }

    if (!avanca(&t)) {
        show_error_msg("Final do arquivo nao esperado", -1, -1, "(' ou ';", NULL);
        goto load_module_header_bad_return;
    }
    else if (t->classe != SYM_OPEN_BRACKET && t->classe != SYM_SEMICOLON) {
        // se t->valor nao eh '(' ou ';', pare
        show_error_msg("Simbolo esperado nao foi encontrado",
                     t->linha, t->coluna, "(' ou ';", t->valor);
        goto load_module_header_bad_return;
    }

    if (t->classe == SYM_OPEN_BRACKET) {
        // devemos agora ler os argumentos do module
        avanca(&t);

        expect_comma = 0; // nao esperando por virgula, por enquanto

        while (1)
        {
            if (!t) {
                if (expect_comma) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ",", NULL);
                }
                else {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, "identificador valido ou )", NULL);
                }

                goto load_module_header_bad_return;
            }

            if (t->classe == SYM_CLOSE_BRACKET) {
                // t->valor eh ')'
                break;
            }

            if (expect_comma) {
                if (t->classe == SYM_COMMA) {
                    expect_comma = 0;
                    avanca(&t);
                    continue;
                    // TODO: bug de virgula a mais...
                }

                show_error_msg("Simbolo esperado nao foi encontrado",
                               t->linha, t->coluna, ",' ou ')", t->valor);
                goto load_module_header_bad_return;
            }

            if ( !is_allowed_identifier(t) ) {
                show_error_msg("Identificador nao foi encontrado",
                               t->linha, t->coluna, "um identificador", t->valor);
                goto load_module_header_bad_return;
            }

            if ( has_item_of_string_value(identifiers, t->valor) ) {
                show_error_identifier_duplicate(t->valor, t->linha, t->coluna);
                goto load_module_header_bad_return;
            }

            insert_token_of_string(identifiers, t->valor, -1, -1, IDENTIFIER);
            insert_token_of_string(livres, t->valor, -1, -1, IDENTIFIER);
            expect_comma = 1;

            avanca(&t);
        }

        if (!avanca(&t)) {
            show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
            goto load_module_header_bad_return;
        }

        if (t->classe != SYM_SEMICOLON) {
            show_error_msg("Simbolo esperado nao foi encontrado",
                           t->linha, t->coluna, ";", t->valor);
            goto load_module_header_bad_return;
        }
    }

//load_module_header_sucess:
    *it = t;
    return 1;

load_module_header_bad_return:
    return 0;
}

ListModule* load_circuit(FILE* f_verilog_source, Evento** initial_task_events, const char* file_path)
{
    ListModule* circuit;
    Module* mod;
    VerilogError err;
    Token* it;
    ListToken* tokens;
    
    tokens = tokeniza(f_verilog_source);

    if (!tokens)
        goto circuit_bad_return;

    if (!file_path)
        copy(tokens->file, "");
    else
        copy(tokens->file, file_path);

    // pre-processing pass to handle compiler directives, returns 1 if ok
    if (!pre_processor(tokens))
        goto circuit_bad_return;

    circuit = (ListModule*) xmalloc( sizeof(ListModule) );
    circuit->total = 0;
    circuit->itens = NULL;

    mod = NULL;
    it = tokens->primeiro;
    while (1)
    {
        err = load_module(&it, initial_task_events, &mod);

        if (err == END_OF_TOKENS)
            break;

        if (err != NO_ERROR_VERILOG)
            goto circuit_bad_return;

        circuit->total++;
            
        circuit->itens = (Module**) xrealloc( circuit->itens,
                                              sizeof(Module*) * (circuit->total) );

        circuit->itens[circuit->total - 1] = mod;
    }

    if (circuit->total == 0) {
        free(circuit);
        circuit = NULL;
    }

//success:
    return circuit;

circuit_bad_return:
    delete_lista_token(tokens);

    for ( int i=0 ; i < circuit->total ; i++ ) 
    {
        free_module( &(circuit->itens[i]) );
    }
    if (circuit->itens)
        free(circuit->itens);
    free(circuit);

    fclose(f_verilog_source);

    return NULL;
}

VerilogError load_module(Token** t, Evento** initial_task_events, Module** module_pointer)
{
    Component* in;
    Component* out;
    Component* gate;
    Component* net;
    Module* circuito = NULL;

    int range_msb;
    int range_lsb;
    int input_count;
    int output_count;

    Evento* initial_tran_events = NULL;

    VerilogError err;

    if ( !(*t) )
        return END_OF_TOKENS;

    int expect_comma = 0; // flag para indicar se estamos esperando por uma virgula

    // lista de todos os identificadores
    ListToken* identifiers = new_list_token();

    // lista de identificadores de entrada ou saida ainda nao definidos como tal
    ListToken* identifiers_to_be = new_list_token();

    // lista de todos os identificadores das entradas
    ListToken* list_input = new_list_token();

    // lista de todos os identificadores das saidas
    ListToken* list_output = new_list_token();

    // lista de todos os identificadores de wire
    ListToken* list_wire = new_list_token();

    // list for params
    ListToken* list_param = new_list_token();
    
    Token* it = *t;

    circuito = new_module();

    before_module:

    if (!it)
        goto bad_return;

    if (it->classe == SYM_GRAVE_ACCENT) {
        VerilogError err = load_directive(&it, circuito);

        switch (err)
        {
        case ERROR_VERILOG_BAD_TOKEN:
            goto bad_return;
            break;
        default:
            // no error
            break;
        }

        avanca(&it);

        goto before_module;
    }

    if ( !load_module_header(&it, identifiers, identifiers_to_be, circuito) )
        goto bad_return;

    if (!avanca(&it))
        goto bad_return_unexpected_eof;

    gate = NULL;

    // process body of the module until it ends
    while (1)
    {
        if (   it->classe == KW_INPUT
            || it->classe == KW_OUTPUT
            || it->classe == KW_WIRE )
        {
            // usado posteriormente para saber se os identificadores serao in ou out
            TokenClass token_subcase = it->classe;

            avanca(&it);
            if (!it)
                goto bad_return_unexpected_eof;

            if ( (token_subcase != KW_WIRE) && (it->classe == KW_WIRE) ) {
                avanca(&it);
                if (!it)
                    goto bad_return_unexpected_eof;
            }

            expect_comma = 0; // nao esperando por uma virgula inicialmente

            while (1)
            {
                if (!it) {
                    if (expect_comma) {
                        show_error_msg("Final do arquivo nao esperado",
                                       -1, -1, ",", NULL);
                    }
                    else {
                        show_error_msg("Final do arquivo nao esperado",
                                       -1, -1, "identificador valido", NULL);
                    }

                    goto bad_return;
                }

                if (it->classe == SYM_SEMICOLON)
                    break;

                if (expect_comma) {
                    if (it->classe == SYM_COMMA) {
                        expect_comma = 0;
                        avanca(&it);
                        continue; // ainda permite uma virgula a mais...
                    }
                    else {
                        show_error_msg("Simbolo esperado nao foi encontrado",
                                       it->linha, it->coluna, ",' ou ';", it->valor);
                        goto bad_return;
                    }
                }

                if ( (token_subcase != KW_WIRE) && !has_item_of_string_value(identifiers_to_be, it->valor) ) {
                    show_error_msg("Identificador invalido",
                                   it->linha, it->coluna,
                                   "identificador valido e que ainda possa ser atribuido",
                                   NULL);
                    goto bad_return;
                }

                if ( token_subcase == KW_INPUT ) {
                    insert_token_of_string(list_input, it->valor, -1, -1, IDENTIFIER);

                    // atribui como entrada o identificador na estrutura
                    add_input( circuito, new_component(it->valor, ROLE_INPUT) );
                }
                else if ( token_subcase == KW_OUTPUT ) {
                    insert_token_of_string(list_output, it->valor, -1, -1, IDENTIFIER);

                    // atribui como saida o identificador na estrutura
                    add_output( circuito, new_component(it->valor, ROLE_OUTPUT) );
                }
                else if ( token_subcase == KW_WIRE ) {
                    range_msb = 0;
                    range_lsb = 0;

                    err = load_range(&it, circuito, list_param, &range_msb, &range_lsb);
                    switch (err)
                    {
                    case ERROR_VERILOG_BAD_EOF:
                        goto bad_return_unexpected_eof;
                        break;
                    case ERROR_VERILOG_BAD_TOKEN:
                        goto bad_return;
                        break;
                    default:
                        // no error
                        break;
                    }

                    if ( !is_allowed_identifier(it) ) {
                        show_error_msg("Identificador nao foi encontrado",
                                       it->linha, it->coluna, "um identificador", it->valor);
                        goto bad_return;
                    }

                    if ( has_item_of_string_value(identifiers, it->valor) ) {
                        show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                        goto bad_return;
                    }

                    insert_token_of_string(identifiers, it->valor, -1, -1, IDENTIFIER);
                    insert_token_of_string(list_wire, it->valor, -1, -1, IDENTIFIER);

                    // atribui como wire o identificador na estrutura
                    net = new_component(it->valor, ROLE_WIRE);
                    net->size = (range_msb - range_lsb + 1);
                    add_wire(circuito, net);
                }

                remove_tokens_by_value(identifiers_to_be, it->valor);

                expect_comma = 1;

                avanca(&it);
            }
        }
        else if (it->classe == KW_REG) {
            VerilogError err = load_reg(&it, identifiers, list_param, circuito);
            switch (err)
            {
            case ERROR_VERILOG_BAD_EOF:
                goto bad_return_unexpected_eof;
                break;
            case ERROR_VERILOG_BAD_TOKEN:
                goto bad_return;
                break;
            default:
                // no error
                break;
            }
        }
        else if (is_logic_gate(it)) {
            gate = NULL;
            
            switch (it->classe)
            {
            case KW_AND:
                gate = new_component("PortaAND", ROLE_AND);
                break;
            case KW_OR:
                gate = new_component("PortaOR", ROLE_OR);
                break;
            case KW_XOR:
                gate = new_component("PortaXOR", ROLE_XOR);
                break;
            case KW_NAND:
                gate = new_component("PortaNAND", ROLE_NAND);
                break;
            case KW_NOR:
                gate = new_component("PortaNOR", ROLE_NOR);
                break;
            case KW_XNOR:
                gate = new_component("PortaXNOR", ROLE_XNOR);
                break;
            case KW_NOT:
                gate = new_component("PortaNOT", ROLE_NOT);
                break;
            case KW_BUF:
                gate = new_component("Buffer", ROLE_BUF);
                break;
            case KW_BUFIF0:
                gate = new_component("BufIf0", ROLE_BUF_IF0);
                break;
            case KW_BUFIF1:
                gate = new_component("BufIf0", ROLE_BUF_IF1);
                break;
            case KW_NOTIF0:
                gate = new_component("NotIf0", ROLE_NOT_IF0);
                break;
            case KW_NOTIF1:
                gate = new_component("NotIf0", ROLE_NOT_IF1);
                break;
            default:
                break;
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1,
                               "(', identificador ou '#", NULL);
                goto bad_return;
            }

            if (is_allowed_identifier(it)) {  
                if (has_item_of_string_value(identifiers, it->valor)) {
                    show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                    goto bad_return;
                }

                insert_token_of_string(identifiers, it->valor, -1, -1, IDENTIFIER);
                copy(gate->nome, it->valor);

                avanca(&it);
            }

            if (!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1,
                               "(' ou '#", NULL);
                goto bad_return;
            }

            if (it->classe == SYM_HASHTAG) {
                if (!avanca(&it)) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, "um numero inteiro nao negativo", NULL);
                    goto bad_return;
                }
                else if ( !is_valid_natural_number(it->valor) ) {
                    char esperado[67];
                    snprintf(esperado, 67,
                             "um numero inteiro nao negativo e com ate %d digitos",
                             MAX_DIGITOS_NUM);
                    show_error_msg("Numero valido nao foi encontrado",
                                   it->linha, it->coluna, esperado, it->valor);
                    goto bad_return;
                }
                else {
                    // Guardar o delay dessa gate
                    gate->atributos.delay = strtol(it->valor, NULL, 10); //FIXME: tipo errado!
                }

                if (!avanca(&it)) {
                    show_error_msg("Final do arquivo nao esperado", -1, -1, "(", NULL);
                    goto bad_return;
                }
            }

            if (it->classe != SYM_OPEN_BRACKET) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, "(", it->valor);
                goto bad_return;
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "identificador para wire ou output", NULL);
                goto bad_return;
            }

            output_count = 0;

        //gate_outputs: // Label para a parte onde ha leitura de saidas da porta logica

            if (has_item_of_string_value(list_wire, it->valor)) {
                // inserir na lista de saidas da gate, esta saida
                out = get_component_by_name(circuito->list_wire_net, it->valor);
                insert_component(gate->list_output, out);
                insert_component(out->list_input, gate);
            }
            else if (has_item_of_string_value(list_output, it->valor)) {
                // inserir na lista de saidas da gate, esta saida
                out = get_component_by_name(circuito->list_output_net, it->valor);
                insert_component(gate->list_output, out);
                insert_component(out->list_input, gate);
            }
            else {
                show_error_msg("Fio ou saida nao foi encontrado",
                               it->linha, it->coluna,
                               "identificador para wire ou output", it->valor);
                goto bad_return;
            }

            output_count++;

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ",", NULL);
                goto bad_return;
            }

            if (it->classe != SYM_COMMA) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ",", it->valor);
                goto bad_return;
            }

            // TODO: read possible multiple outputs for 'buf' as well 'not' gates

            input_count = 0;

        gate_inputs: // Label para a parte onde ha leitura de entradas da porta logica

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um identificador ou numero", NULL);
                goto bad_return;
            }

            if ( it->classe == NUM_BASE_DECIMAL ) {
                Component* num = new_component("literal_number_decimal", ROLE_LITERAL_NUMBER);
                num->dynamic_value = long_to_logicvalue(strtol(it->valor, NULL, 10));
                insert_component(gate->list_input, num); // TODO: free mem later
            }
            else if (has_item_of_string_value(list_wire, it->valor)) {
                // inserir na lista de entradas da gate, esta entrada
                in = get_component_by_name(circuito->list_wire_net, it->valor);
                insert_component(gate->list_input, in);
                insert_component(in->list_output, gate);
            }
            else if (has_item_of_string_value(list_input, it->valor)) {
                // inserir na lista de entradas da gate, esta entrada
                in = get_component_by_name(circuito->list_input_net, it->valor);
                insert_component(gate->list_input, in);
                insert_component(in->list_output, gate);
            }
            else if (has_item_of_string_value(list_output, it->valor)) {
                // inserir na lista de entradas da gate, esta entrada
                in = get_component_by_name(circuito->list_output_net, it->valor);
                insert_component(gate->list_input, in);
                insert_component(in->list_output, gate);
            }
            else {
                show_error_msg("Entrada da porta logica invalida",
                               it->linha, it->coluna,
                               "uma entrada valida (tipo net ou numero literal)",
                               it->valor);
                goto bad_return;
            }

            input_count++;

            if (!avanca(&it)) {
                if ( gate->atributos.role == ROLE_NOT || gate->atributos.role == ROLE_BUF ) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ")", NULL);
                }
                else {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ",' ou ')", NULL);
                }

                goto bad_return;
            }

            if (it->classe != SYM_CLOSE_BRACKET) {
                if ( (gate->atributos.role == ROLE_NOT) ||
                     (gate->atributos.role == ROLE_BUF) || 
                     (is_tristate_logic(gate) && input_count == 2) ) {
                    show_error_msg("Simbolo esperado nao foi encontrado",
                                   it->linha, it->coluna, ")", it->valor);
                    goto bad_return;
                }
                
                if (it->classe == SYM_COMMA) {
                    // keep reading all the gate inputs
                    goto gate_inputs;
                }
                
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ")' ou ',", it->valor);
                goto bad_return;
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
                goto bad_return;
            }

            if (it->classe != SYM_SEMICOLON) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }

            // finalmente, inserimos a gate na lista de portas logicas do circuito
            add_gate(circuito, gate);
        }
        else if (it->classe == KW_ENDMODULE) {
            avanca(&it);

//load_module_sucess:
            // Liberar a memoria alocada no inicio da funcao
            delete_lista_token(identifiers);
            delete_lista_token(identifiers_to_be);
            delete_lista_token(list_input);
            delete_lista_token(list_output);
            delete_lista_token(list_wire);
            delete_lista_token(list_param);

            *module_pointer = circuito;
            *t = it;

            return NO_ERROR_VERILOG;
        }
        else if (it->classe == SYM_GRAVE_ACCENT) {
            VerilogError err = load_directive(&it, circuito);
            switch (err)
            {
            case ERROR_VERILOG_BAD_TOKEN:
                goto bad_return;
                break;
            default:
                // no error
                break;
            }
        }
        else if( it->classe == KW_INITIAL ) {
            VerilogError err = load_initial_block(&it, identifiers, list_param, circuito, initial_task_events);
            switch (err)
            {
            case ERROR_VERILOG_BAD_EOF:
                goto bad_return_unexpected_eof;
                break;
            case ERROR_VERILOG_BAD_TOKEN:
                goto bad_return;
                break;
            default:
                // no error
                break;
            }
        }
        else if (it->classe == KW_LOCALPARAM) {
            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um identificador", NULL);
                goto bad_return;
            }

            if (!is_allowed_identifier(it)) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "um identificador", it->valor);
                goto bad_return;
            }
    
            if (has_item_of_string_value(identifiers, it->valor)) {
                show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                goto bad_return;
            }

            if (len(it->valor) > MAX_PARAM_NAME_SIZE) {
                show_error_size_exceeded("Excedido o tamanho de caracteres para o parametro",
                                         it->linha, it->coluna, it->valor, MAX_PARAM_NAME_SIZE);

                goto bad_return;
            }

            insert_token_of_string(identifiers, it->valor, -1, -1, IDENTIFIER);
            insert_token_of_string(list_param, it->valor, -1, -1, IDENTIFIER);

            Param* param = (Param*) xcalloc(1, sizeof(Param));
            param->is_local = 1;
            copy( param->name, it->valor );

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, "=", NULL);
                goto bad_return;
            }

            if (it->classe != SYM_EQ) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "=", it->valor);
                goto bad_return;
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um numero literal", NULL);
                goto bad_return;
            }

            // TODO: implement for all number types and notations
            if( !is_valid_natural_number(it->valor) ) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "um numero", it->valor);
                goto bad_return;
            }

            param->value = strtol(it->valor, NULL, 10);

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
                goto bad_return;
            }

            if (it->classe != SYM_SEMICOLON) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }

            // include the param in the circuit struct
            add_param(circuito, param);
        }
        else if (it->classe == KW_ASSIGN) {
            VerilogError err = load_assign(&it, list_wire, list_input, list_output, circuito);
            switch (err)
            {
            case ERROR_VERILOG_BAD_EOF:
                goto bad_return_unexpected_eof;
                break;
            case ERROR_VERILOG_BAD_TOKEN:
                goto bad_return;
                break;
            default:
                // no error
                break;
            }
        }
        else {
            show_error_msg("Token inesperado foi encontrado",
                           it->linha, it->coluna, "algum comando", it->valor);
            goto bad_return;
        }

        if (!avanca(&it))
            goto bad_return_unexpected_eof;
            
    }

bad_return_unexpected_eof:
    show_error_msg("Final do arquivo nao esperado", -1, -1, NULL, NULL);

bad_return:
    // free mem allocated in the head of this func
    delete_lista_token(identifiers);
    delete_lista_token(identifiers_to_be);
    delete_lista_token(list_input);
    delete_lista_token(list_output);
    delete_lista_token(list_wire);
    delete_lista_token(list_param);

    if (initial_tran_events)
        delete_event_queue(&initial_tran_events);

    free_module(&circuito);

    *t = it;
    *module_pointer = NULL;

    return ERROR_VERILOG;
}

int is_logic_gate(const Token* t)
{
    int i;

    TokenClass g[13] = {
        KW_AND,    // 0
        KW_OR,     // 1
        KW_NOT,    // 2
        KW_BUF,    // 3
        KW_NAND,   // 4
        KW_NOR,    // 5
        KW_XOR,    // 7
        KW_XNOR,   // 8
        KW_BUFIF0, // 9
        KW_BUFIF1, // 10
        KW_NOTIF0, // 11
        KW_NOTIF1, // 12
    };

    for ( i = 0; i < 13; i++ )
    {
        if (t->classe == g[i])
            return 1;
    }
    
    return 0;
}

int is_tristate_logic(const Component* gate)
{
    int i;

    Role op[4] = {
        ROLE_BUF_IF0, // 0
        ROLE_BUF_IF1, // 1
        ROLE_NOT_IF0, // 2
        ROLE_NOT_IF1, // 3
    };

    for ( i = 0; i < 4; i++ )
    {
        if (gate->atributos.role == op[i])
            return 1;
    }
    
    return 0;
}

int is_string_logic_gate(const char* s)
{
    return ( iguais(s, "and")
            || iguais(s, "or")
            || iguais(s, "xor")
            || iguais(s, "nand")
            || iguais(s, "nor")
            || iguais(s, "xnor")
            || iguais(s, "not")
            || iguais(s, "buf")
    );
}

VerilogError load_reg(Token** it, ListToken* identifiers, ListToken* list_param, Module* module)
{
    int is_signed;
    int range_msb;
    int range_lsb;
    VerilogError err;

    Token* t = *it;

    if (!avanca(&t))
        goto load_reg_bad_eof;

    is_signed = 0;

    if (t->classe == KW_SIGNED) {
        is_signed = 1;
        
        if (!avanca(&t)) {
            goto load_reg_bad_eof;
        }
    }

    // range specification
    range_msb = 0;
    range_lsb = 0;

    err = load_range(&t, module, list_param, &range_msb, &range_lsb);

    switch (err)
    {
    case ERROR_VERILOG_BAD_EOF:
        goto load_reg_bad_eof;
        break;
    case ERROR_VERILOG_BAD_TOKEN:
        goto load_reg_bad_token;
        break;
    default:
        // no error
        break;
    }

load_reg_identifier_list:

    if (!is_allowed_identifier(t)) {
        show_error_msg("Identificador nao foi encontrado",
                       t->linha, t->coluna, "um identificador", t->valor);
        goto load_reg_bad_token;
    }

    // verificar se pode utilizar este identificador
    if (has_item_of_string_value(identifiers, t->valor)) {
        show_error_identifier_duplicate(t->valor, t->linha, t->coluna);
        goto load_reg_bad_token;
    }
    
    // adicionar na lista de identificadores usados
    insert_token_of_string(identifiers, t->valor, -1, -1, IDENTIFIER);

    add_register(module, t->valor, (range_msb - range_lsb + 1), is_signed);

    if (!avanca(&t))
        goto load_reg_bad_eof;

    if (t->classe == SYM_COMMA) {
        if (!avanca(&t)) {
            goto load_reg_bad_eof;
        }
        goto load_reg_identifier_list;
    }

    if (t->classe != SYM_SEMICOLON) {
        show_error_msg("Simbolo esperado nao foi encontrado",
                       t->linha, t->coluna, ",' ou ';", t->valor);
        goto load_reg_bad_token;
    }

//load_reg_sucess:
    *it = t;
    return NO_ERROR_VERILOG;

load_reg_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_reg_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_range(Token** it, Module* module, ListToken* list_param, int* range_msb, int* range_lsb)
{
    Token* t = *it;
    
    // optional range specification
    // range ::= [ msb_constant_expression : lsb_constant_expression ]
    // TODO: calculate the expressions
    if (t->classe == SYM_OPEN_SQUAREBRACKET) {
        if (!avanca(&t))
            goto load_range_bad_eof;

        if (is_valid_natural_number(t->valor)) {
            *range_msb = strtol(t->valor, NULL, 10);
        }
        else if (has_item_of_string_value(list_param, t->valor)) {
            *range_msb = get_param_by_name(module->list_param, t->valor)->value;
        }
        else {
            show_error_msg("Numero para bit mais significativo nao foi encontrado",
                           t->linha, t->coluna, "algum numero", t->valor);
            goto load_range_bad_token;
        }
        
        if (!avanca(&t))
            goto load_range_bad_eof;

        if (t->classe != SYM_COLON) {
            show_error_msg("Simbolo inesperado",
                           t->linha, t->coluna, ":", t->valor);
            goto load_range_bad_token;
        }

        if (!avanca(&t))
            goto load_range_bad_eof;

        if (is_valid_natural_number(t->valor)) {
            *range_lsb = strtol(t->valor, NULL, 10);
        }
        else if (has_item_of_string_value(list_param, t->valor)) {
            *range_lsb = get_param_by_name(module->list_param, t->valor)->value;
        }
        else {
            show_error_msg("Numero para bit menos significativo nao foi encontrado",
                           t->linha, t->coluna, "algum numero", t->valor);
            goto load_range_bad_token;
        }

        if (!avanca(&t))
            goto load_range_bad_eof;

        if (t->classe != SYM_CLOSE_SQUAREBRACKET) {
            show_error_msg("Simbolo inesperado", t->linha, t->coluna, "]", t->valor);
            goto load_range_bad_token;
        }

        if ( *range_msb < *range_lsb ) {
            show_error_msg("Range invalido",
                            t->anterior->linha, t->anterior->coluna, NULL, NULL);
            goto load_range_bad_token;
        }

        if (!avanca(&t))
            goto load_range_bad_eof;
    }

//load_range_sucess:
    *it = t;
    return NO_ERROR_VERILOG;

load_range_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_range_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_directive(Token** it, Module* module)
{
    Token* t = *it;

    avanca(&t);

    // note: the pre-processor did some validation

    if (iguais("resetall", t->valor)) {
        module->timescale_number = (Tempo) 1;
        module->timescale_unit = UN_NS;
        module->timescale_precision_number = (Tempo) 1;
        module->timescale_precision_unit = UN_NS;

        // FIXME: should not be module wide
    }
    else if (iguais("timescale", t->valor)) {
        // time_unit / time_precision
        // ex.: 1 ns / 1 ps

        avanca(&t);

        // [time_unit] / time_precision
        // [number] unit / number unit
        if (!is_valid_natural_number(t->valor))
            goto load_directive_bad_number;
        
        module->timescale_number = (Tempo) strtol(t->valor, NULL, 10);

        avanca(&t);

        // [time_unit] / time_precision
        // number [unit] / number unit
        module->timescale_unit = get_timeunit_from_str(t->valor);
        if (module->timescale_unit == UN_INVALID)
            goto load_directive_bad_time_unit;

        avanca(&t);

        // time_unit [/] time_precision
        // number unit [/] number unit
        if (!iguais("/", t->valor)) {
            show_error_msg("Token inesperado", t->linha, t->coluna, "/", t->valor);
            goto load_directive_bad_token;
        }

        avanca(&t);

        // time_unit / [time_precision]
        // number unit / [number] unit
        if (!is_valid_natural_number(t->valor))
            goto load_directive_bad_number;
        
        module->timescale_precision_number = (Tempo) strtol(t->valor, NULL, 10);

        avanca(&t);

        // time_unit / [time_precision]
        // number unit / number [unit]
        module->timescale_precision_unit = get_timeunit_from_str(t->valor);
        if (module->timescale_precision_unit == UN_INVALID)
            goto load_directive_bad_time_unit;
    }

//load_directive_sucess:
    *it = t;
    return NO_ERROR_VERILOG;

load_directive_bad_number:
    show_error_msg("Numero invalido", t->linha, t->coluna,
                   "numero inteiro nao negativo", t->valor);
    return ERROR_VERILOG_BAD_TOKEN;

load_directive_bad_time_unit:
    show_error_msg("Unidade invalida", t->linha, t->coluna,
                   "unidade de tempo (us, ns, ps, ...)", t->valor);
    return ERROR_VERILOG_BAD_TOKEN;

load_directive_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;
}

VerilogError load_initial_block(Token** pit, ListToken* identifiers, ListToken* list_param, Module* module, Evento** initial_task_events)
{
    VerilogError err;
    int is_single_statement = 1;
    Tempo t = 0;
    Token* it = *pit;

    if ( !avanca(&it) ) {
        goto load_initial_block_bad_eof;
    }

    if ( it->classe == KW_BEGIN ) {
        is_single_statement = 0;

        if ( !avanca(&it) ) {
            goto load_initial_block_bad_eof;
        }
    }

initial_block_loop:

    if ( it->classe == KW_END ) {
        if ( is_single_statement ) {
            show_error_msg("Palavra chave 'end' sem o 'begin' correspondente",
                           it->linha,
                           it->coluna,
                           NULL,
                           it->valor);
            goto load_initial_block_bad_token;
        }

        goto load_initial_block_sucess;
    }

    // delay readings
    if ( it->classe == SYM_HASHTAG ) {
        if ( !avanca(&it) ) {
            goto load_initial_block_bad_eof;
        }

        if ( !is_valid_natural_number(it->valor) ) {
            show_error_msg("Token inesperado foi encontrado",
                           it->linha,
                           it->coluna,
                           "um numero inteiro nao negativo",
                           it->valor);
            goto load_initial_block_bad_token;
        }

        Tempo delay = strtol(it->valor, NULL, 10);

        // update next event time
        t = t + delay;

        if ( !avanca(&it) ) {
            goto load_initial_block_bad_eof;
        }

        // handle optional semicolon after delay number
        if ( it->classe == SYM_SEMICOLON ) {
            if ( !avanca(&it) ) {
                goto load_initial_block_bad_eof;
            }
        }        

        goto initial_block_loop;
    }

    if ( it->classe == SYM_DOLLAR ) {
        // systask handling
        err = load_systask(&it, initial_task_events, t);
    }
    else {
        // try to treat a single statement attribution, for now

        if ( !has_item_of_string_value(identifiers, it->valor) ) {
            show_error_msg("Nota, o initial ainda nao foi devidamente implementado",
                           it->linha,
                           it->coluna,
                           "apenas uma atribuicao",
                           it->valor);
            goto load_initial_block_bad_token;
        }

        err = load_reg_attribution(&it, list_param, module, initial_task_events, t);
    }

    switch (err)
    {
    case ERROR_VERILOG_BAD_EOF:
        goto load_initial_block_bad_eof;
        break;
    case ERROR_VERILOG_BAD_TOKEN:
        goto load_initial_block_bad_token;
        break;
    default:
        // no error
        goto initial_block_expect_semicolon;
    }

initial_block_expect_semicolon:

    if ( !avanca(&it) )
        goto load_initial_block_bad_eof;

    if ( it->classe != SYM_SEMICOLON ) {
        show_error_msg("Token inesperado foi encontrado",
                        it->linha, it->coluna,
                        ";",
                        it->valor);
        goto load_initial_block_bad_token;
    }

    if ( !is_single_statement ) {
        if ( !avanca(&it) ) {
            goto load_initial_block_bad_eof;
        }

        goto initial_block_loop;
    }

load_initial_block_sucess:
    *pit = it;
    return NO_ERROR_VERILOG;

load_initial_block_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_initial_block_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_reg_attribution(
    Token** it,
    ListToken* list_param,
    Module* module,
    Evento** initial_events,
    Tempo t_ev)
{
    Register* left_reg = NULL;
    Param* p = NULL;
    ValorLogico logic_value = VAL_X;
    
    Token* t = *it;

    // waiting for a reg, for now
    left_reg = get_reg_by_name(module->list_register, t->valor);

    if ( !left_reg ) {
        show_error_msg("Comando sem suporte dentro do bloco initial",
                       t->linha, t->coluna, "um registrador", t->valor);
        goto load_reg_attribution_bad_token;
    }

    if ( !avanca(&t) )
        goto load_reg_attribution_bad_eof;
    
    if ( t->classe != SYM_EQ ) {
        show_error_msg("Token inesperado foi encontrado", t->linha, t->coluna, "=", t->valor);
        goto load_reg_attribution_bad_token;
    }

    if ( !avanca(&t) )
        goto load_reg_attribution_bad_eof;

    // agora ele espera um literal ou parametro
    if ( is_valid_natural_number(t->valor) ) {
        left_reg->value = strtol(t->valor, NULL, 10);
    }
    else if ( has_item_of_string_value(list_param, t->valor) ) {
        p = get_param_by_name(module->list_param, t->valor);
        left_reg->value = p->value;
    }
    else {
        show_error_msg("Token inesperado foi encontrado",
                        t->linha, t->coluna, "um numero ou parametro", t->valor);
        goto load_reg_attribution_bad_token;
    }

    // TODO: make reg attribution not only for bits 
    logic_value = left_reg->value == 0 ? VAL_0 : VAL_1;
    insert_event(initial_events, t_ev, EVT_REG_ATTRIBUTION, NULL, left_reg, logic_value);

//load_reg_attribution_sucess:
    *it = t;
    return NO_ERROR_VERILOG;

load_reg_attribution_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_reg_attribution_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_assign(Token** it, ListToken* list_wire, ListToken* list_in, ListToken* list_out, Module* module)
{
    Component* in;
    Component* out;
    Component* gate = NULL;

    Token* t = *it;

    if (!avanca(&t))
        goto load_assign_bad_eof;

    if (!is_allowed_identifier(t)) {
        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, "um identificador", t->valor);
        goto load_assign_bad_token;
    }

    gate = new_component("assign", ROLE_ASSIGN);

    // TODO: check for impossible cases
    
    if (has_item_of_string_value(list_wire, t->valor)) {
        // inserir, na lista de saidas da gate, esta saida
        out = get_component_by_name(module->list_wire_net, t->valor);
        insert_component(gate->list_output, out);
        insert_component(out->list_input, gate);
    }
    else if (has_item_of_string_value(list_out, t->valor)) {
        // inserir, na lista de saidas da gate, esta saida
        out = get_component_by_name(module->list_output_net, t->valor);
        insert_component(gate->list_output, out);
        insert_component(out->list_input, gate);
    }
    else {
        show_error_msg("Identificador previamente declarado nao foi encontrado",
                       t->linha, t->coluna,
                       "identificador para wire ou output", t->valor);
        goto load_assign_bad_token;
    }

    if (!avanca(&t))
        goto load_assign_bad_eof;

    if (t->classe != SYM_EQ) {
        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, "=", t->valor);
        goto load_assign_bad_token;
    }

    // from here, we expect an expression...

    if (!avanca(&t))
        goto load_assign_bad_eof;

    // in fact, simplest expression is a constant value

    if ( t->classe == NUM_BASE_DECIMAL ) {
        // inserir, na lista de entradas da gate, esta entrada
        gate->atributos.role = ROLE_LITERAL_NUMBER;
        gate->dynamic_value = long_to_logicvalue(strtol(t->valor, NULL, 10));
        out->dynamic_value = gate->dynamic_value; // propagate value to output net

        if (!avanca(&t))
            goto load_assign_bad_eof;

        if (t->classe == SYM_SEMICOLON)
            goto load_assign_sucess;

        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, ";", t->valor);
        goto load_assign_bad_token;
    }

    // another simple expression case is some net,
    // logic synthesis, in this case, creates a buf.
    // negation (~) is also simple, it creates a not.

    if ( t->classe == SYM_TILDE ) {
        gate->atributos.role = ROLE_NOT;

    if (!avanca(&t))
        goto load_assign_bad_eof;
    }

load_assign_identifiers:

    if ( !is_allowed_identifier(t) ) {
        show_error_msg("Token inesperado foi encontrado",
                        t->linha, t->coluna, "algum identificador", t->valor);
        goto load_assign_bad_token;
    }

    if ( has_item_of_string_value(list_wire, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = get_component_by_name(module->list_wire_net, t->valor);
        insert_component(gate->list_input, in);
        insert_component(in->list_output, gate);
    }
    else if ( has_item_of_string_value(list_in, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = get_component_by_name(module->list_input_net, t->valor);
        insert_component(gate->list_input, in);
        insert_component(in->list_output, gate);
    }
    else if ( has_item_of_string_value(list_out, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = get_component_by_name(module->list_output_net, t->valor);
        insert_component(gate->list_input, in);
        insert_component(in->list_output, gate);
    }
    //else if ( get_reg_by_name(module->list_register, t->valor) ) {
    //}
    else {
        show_error_msg("Este identificador nao consta como alguma net declarada",
                        t->linha, t->coluna,
                        "identificador ja declarado (tipos: input, output ou wire)",
                        t->valor);
        goto load_assign_bad_token;
    }

    // TODO: implement expression evaluation and specific data structures

    if (!avanca(&t))
        goto load_assign_bad_eof;

    if (t->classe == SYM_SEMICOLON)
        goto load_assign_sucess;

    if ( gate->atributos.role == ROLE_NOT ||
         (t->classe != SYM_AMPERSAND && t->classe != SYM_PIPE) || 
         (gate->atributos.role == ROLE_AND && t->classe != SYM_AMPERSAND) ||
         (gate->atributos.role == ROLE_OR && t->classe != SYM_PIPE) ) {
        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, NULL, t->valor);
        goto load_assign_bad_token;
    }

    if (t->classe == SYM_AMPERSAND)
        gate->atributos.role = ROLE_AND;
    else
        gate->atributos.role = ROLE_OR;

    if (!avanca(&t))
        goto load_assign_bad_eof;

    goto load_assign_identifiers;

load_assign_sucess:
    *it = t;
    return NO_ERROR_VERILOG;

load_assign_bad_token:
    delete_componente(&gate);
    return ERROR_VERILOG_BAD_TOKEN;

load_assign_bad_eof:
    delete_componente(&gate);
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_systask(Token** pit, Evento** initial_task_events, Tempo t)
{
    int count = 0; // task arg counter
    SystemTask task = TASK_UNKNOWN;
    char str[MAX_TOKEN_SIZE] = "";

    Token* it = *pit;

    if ( !avanca(&it) )
        goto load_systask_bad_eof;

    if ( !is_allowed_identifier(it) ) {
        show_error_msg("Token inesperado foi encontrado",
                       it->linha,
                       it->coluna,
                       "o nome de uma task",
                       it->valor);
        goto load_systask_bad_token;
    }

    if ( iguais(it->valor, "display") ) {
        task = TASK_DISPLAY;
    }
    else if ( iguais(it->valor, "write") ) {
        task = TASK_WRITE;
    }
    else if ( iguais(it->valor, "dumpfile") ) {
        task = TASK_DUMPFILE;
    }
    else if ( iguais(it->valor, "finish") ) {
        task = TASK_FINISH;
        goto load_systask_sucess;
    }
    else if ( iguais(it->valor, "stop") ) {
        task = TASK_STOP;
        goto load_systask_sucess;
    }
    else {
        show_error_msg("Task invalida ou nao suportada",
                       it->linha,
                       it->coluna,
                       "o nome de uma task suportada",
                       it->valor);
        goto load_systask_bad_token;
    }

    if ( !avanca(&it) )
        goto load_systask_bad_eof;

    if (it->classe != SYM_OPEN_BRACKET) {
        show_error_msg("Token inesperado foi encontrado",
                       it->linha,
                       it->coluna,
                       "(",
                       it->valor);
        goto load_systask_bad_token;
    }

    // read arguments

    if ( !avanca(&it) )
        goto load_systask_bad_eof;

    // first arg SHOULD be a string (for now)

    if ( it->classe != STRING ) {
        show_error_msg("Token inesperado foi encontrado",
                       it->linha,
                       it->coluna,
                       "uma string",
                       it->valor);
        goto load_systask_bad_token;
    }

    copy(str, it->valor + 1); // remove the first quote
    str[len(it->valor) - 2] = '\0'; // remove the last quote

    // for Sdisplay: there may be zero to n more args
    // for Swrite: there may be zero to n more args
    // for $dumpfile: there are no more args

systask_args_load:

    if ( !avanca(&it) )
        goto load_systask_bad_eof;

    if ( it->classe == SYM_CLOSE_BRACKET ) {
        goto load_systask_sucess;
    }

    if ( task == TASK_DUMPFILE ) {
        show_error_msg("Token inesperado foi encontrado",
                       it->linha,
                       it->coluna,
                       ");",
                       it->valor);
        goto load_systask_bad_token;
    }

    if ( it->classe != SYM_COMMA ) {
        show_error_msg("Token inesperado foi encontrado",
                       it->linha,
                       it->coluna,
                       ", ou )",
                       it->valor);
        goto load_systask_bad_token;
    }

    if ( !avanca(&it) )
        goto load_systask_bad_eof;
    
    // TODO: check arg validity and save to some stack
    // if (it->class == ???)

    count++;

    goto systask_args_load;

load_systask_sucess:
    insert_task_event(initial_task_events, t, task, str);
    *pit = it;

    return NO_ERROR_VERILOG;

load_systask_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_systask_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}
