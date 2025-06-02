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
#include "lex.h"
#include "preprocessor.h"

int load_module_header(Token** it, ListaToken* identifiers, ListaToken* livres)
{
    int virgula = 0;

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
    else if (!isIdentificador(t)) {
        show_error_msg("Identificador nao encontrado",
                       t->linha, t->coluna,"identificador valido", t->valor);
        goto load_module_header_bad_return;
    }
    else {
        // senao, adicione-o a lista de identifiers
        insereTokenString(identifiers, t->valor, -1, -1);
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
        // devemos agora ler os argumentos do modulo
        avanca(&t);

        virgula = 0; // nao esperando por virgula, por enquanto

        while (1)
        {
            if (!t) {
                if (virgula) {
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

            if (virgula) {
                if (t->classe == SYM_COMMA) {
                    virgula = 0;
                    avanca(&t);
                    continue;
                    // TODO: bug de virgula a mais...
                }
                else {
                    show_error_msg("Simbolo esperado nao foi encontrado",
                                   t->linha, t->coluna, ",' ou ')", t->valor);
                    goto load_module_header_bad_return;
                }
            }

            if (isIdentificador(t)) {
                if ( identExiste(identifiers, t->valor) ) {
                    show_error_identifier_duplicate(t->valor, t->linha, t->coluna);
                    goto load_module_header_bad_return;
                }
                else {
                    insereTokenString(identifiers, t->valor, -1, -1);
                    insereTokenString(livres, t->valor, -1, -1);
                    virgula = 1;
                }
            }
            else {
                show_error_msg("Identificador nao foi encontrado",
                               t->linha, t->coluna, "um identificador", t->valor);
                goto load_module_header_bad_return;
            }

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

Module* carregaCircuito(FILE* arquivo)
{
    Componente in;
    Componente out;
    Componente gate;
    Token* it = NULL;
    Module* circuito = NULL;

    int virgula = 0; // um flag para indicar se estamos esperando por uma virgula

    // lista de todos os identificadores
    ListaToken* identificadores = novaListaToken();

    // lista de identificadores de entrada ou saida ainda nao definidos como tal
    ListaToken* identificLivre = novaListaToken();

    // lista de todos os identificadores das entradas
    ListaToken* listaInput = novaListaToken();

    // lista de todos os identificadores das saidas
    ListaToken* listaOutput = novaListaToken();

    // lista de todos os identificadores de wire
    ListaToken* listaWire = novaListaToken();

    // list for params
    ListaToken* list_param = novaListaToken();

    ListaToken* tokens = tokeniza(arquivo);


    if (!tokens)
        goto bad_return;

    // pre-processing pass to handle compiler directives, returns 1 if ok
    if (!pre_processor(tokens))
        goto bad_return;

    circuito = novoCircuito();

    it = tokens->primeiro;

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

    if ( !load_module_header(&it, identificadores, identificLivre) )
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
            char tipo[MAX_TOKEN_SIZE];
            copy(tipo, it->valor);

            avanca(&it);

            virgula = 0; // nao esperando por uma virgula inicialmente

            while (1)
            {
                if (!it) {
                    if (virgula) {
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

                if (virgula) {
                    if (it->classe == SYM_COMMA) {
                        virgula = 0;
                        avanca(&it);
                        continue; // ainda permite uma virgula a mais...
                    }
                    else {
                        show_error_msg("Simbolo esperado nao foi encontrado",
                                       it->linha, it->coluna, ",' ou ';", it->valor);
                        goto bad_return;
                    }
                }

                if ( !iguais(tipo, "wire") && !identExiste(identificLivre, it->valor) ) {
                    show_error_msg("Identificador invalido",
                                   it->linha, it->coluna,
                                   "identificador valido e que ainda possa ser atribuido",
                                   NULL);
                    goto bad_return;
                }

                if ( iguais(tipo, "input") ) {
                    insereTokenString(listaInput, it->valor, -1, -1);

                    // atribui como entrada o identificador na estrutura
                    adicionaEntrada( circuito, novoComponente(it->valor, input) );
                }
                else if ( iguais(tipo, "output") ) {
                    insereTokenString(listaOutput, it->valor, -1, -1);

                    // atribui como saida o identificador na estrutura
                    adicionaSaida( circuito, novoComponente(it->valor, output) );
                }
                else if( iguais(tipo, "wire") ) {

                    if( isIdentificador(it) ) {
                        if( identExiste(identificadores, it->valor) ) {
                            show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                            goto bad_return;
                        }
                        else {
                            insereTokenString(identificadores, it->valor, -1, -1);
                            insereTokenString(listaWire, it->valor, -1, -1);

                            // atribui como wire o identificador na estrutura
                            adicionaWire(circuito, novoComponente(it->valor, wire));
                        }
                    }
                    else {
                        show_error_msg("Identificador nao foi encontrado",
                                       it->linha, it->coluna, "um identificador", it->valor);
                        goto bad_return;
                    }
                }

                removeTokensPorValor(identificLivre, it->valor);

                virgula = 1;

                avanca(&it);
            }
        }
        else if (it->classe == KW_REG) {
            VerilogError err = load_reg(&it, identificadores, list_param, circuito);
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
        else if( isPortaLogica(it->valor) ) {
            gate = NULL;
            
            switch (it->classe)
            {
            case KW_AND:
                gate = novoComponente("PortaAND", op_and);
                break;
            case KW_OR:
                gate = novoComponente("PortaOR", op_or);
                break;
            case KW_XOR:
                gate = novoComponente("PortaXOR", op_xor);
                break;
            case KW_NAND:
                gate = novoComponente("PortaNAND", op_nand);
                break;
            case KW_NOR:
                gate = novoComponente("PortaNOR", op_nor);
                break;
            case KW_XNOR:
                gate = novoComponente("PortaXNOR", op_xnor);
                break;
            case KW_NOT:
                gate = novoComponente("PortaNOT", op_not);
                break;
            case KW_BUF:
                gate = novoComponente("Buffer", op_buf);
            default:
                break;
            }

            if(!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1,
                               "(', identificador ou '#", NULL);
                goto bad_return;
            }

            if( isIdentificador(it) ) {  
                if( identExiste(identificadores, it->valor) ) {
                    show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                    goto bad_return;
                }

                insereTokenString(identificadores, it->valor, -1, -1);
                copy(gate->nome, it->valor);

                avanca(&it);
            }

            if (!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1,
                               "(' ou '#", NULL);
                goto bad_return;
            }

            if (it->classe == SYM_HASHTAG) {
                if(!avanca(&it)) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, "um numero inteiro nao negativo", NULL);
                    goto bad_return;
                }
                else if ( !isNumNaturalValido(it->valor) ) {
                    char esperado[100];
                    sprintf(esperado,
                            "um numero inteiro nao negativo e com ate %d digitos",
                            MAX_DIGITOS_NUM);
                    show_error_msg("Numero valido nao foi encontrado",
                                   it->linha, it->coluna, esperado, it->valor);
                    goto bad_return;
                }
                else {
                    // Guardar o atraso dessa gate
                    gate->tipo.atraso = atoi(it->valor); //FIXME: tipo errado!
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

            if(!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "identificador para wire ou output", NULL);
                goto bad_return;
            }

            if( identExiste(listaWire, it->valor) ) {
                // inserir na lista de saidas da gate, esta saida
                out = getComponenteItemPorNome(circuito->listaWires, it->valor);
                insereComponente(gate->listaSaida, out);
                insereComponente(out->listaEntrada, gate);
            }
            else if( identExiste(listaOutput, it->valor) ) {
                // inserir na lista de saidas da gate, esta saida
                out = getComponenteItemPorNome(circuito->listaFiosSaida, it->valor);
                insereComponente(gate->listaSaida, out);
                insereComponente(out->listaEntrada, gate);
            }
            else {
                show_error_msg("Fio ou saida nao foi encontrado",
                               it->linha, it->coluna,
                               "identificador para wire ou output", it->valor);
                goto bad_return;
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ",", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_COMMA ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ",", it->valor);
                goto bad_return;
            }

            gate_inputs: // Label para a parte do codigo onde ha leitura de entradas da porta logica

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um identificador", NULL);
                goto bad_return;
            }

            if( identExiste(listaWire, it->valor) ) {
                // inserir na lista de entradas da gate, esta entrada
                in = getComponenteItemPorNome(circuito->listaWires, it->valor);
                insereComponente(gate->listaEntrada, in);
                insereComponente(in->listaSaida, gate);
            }
            else if( identExiste(listaInput, it->valor) ) {
                // inserir na lista de entradas da gate, esta entrada
                in = getComponenteItemPorNome(circuito->listaFiosEntrada, it->valor);
                insereComponente(gate->listaEntrada, in);
                insereComponente(in->listaSaida, gate);
            }
            else if( identExiste(listaOutput, it->valor) ) {
                // inserir na lista de entradas da gate, esta entrada
                in = getComponenteItemPorNome(circuito->listaFiosSaida, it->valor);
                insereComponente(gate->listaEntrada, in);
                insereComponente(in->listaSaida, gate);
            }
            else {
                show_error_msg("Entrada da porta logica invalida",
                               it->linha, it->coluna,
                               "uma entrada valida (tipos: input, output ou wire)",
                               it->valor);
                goto bad_return;
            }

            if (!avanca(&it)) {
                if( gate->tipo.operador == op_not || gate->tipo.operador == op_buf ) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ")", NULL);
                }
                else {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ",' ou ')", NULL);
                }

                goto bad_return;
            }

            if( it->classe != SYM_CLOSE_BRACKET ) {
                if( gate->tipo.operador == op_not || gate->tipo.operador == op_buf ) {
                    show_error_msg("Simbolo esperado nao foi encontrado",
                                   it->linha, it->coluna, ")", it->valor);
                    goto bad_return;
                }
                else {
                    if( it->classe == SYM_COMMA ) {
                        goto gate_inputs;
                    }
                    else {
                        show_error_msg("Simbolo esperado nao foi encontrado",
                                       it->linha, it->coluna, ")' ou ',", it->valor);
                        goto bad_return;
                    }
                }
            }

            if (!avanca(&it)) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_SEMICOLON ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }

            // finalmente, inserimos a gate na lista de portas logicas do circuito
            adicionaPorta(circuito, gate);
        }
        else if( it->classe == KW_ENDMODULE ) {
            avanca(&it);

            // nao deve haver mais nada alem do endmodule
            if(it) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "nenhum codigo a mais", it->valor);
                goto bad_return;
            }
            else {
                // Liberar a memoria alocada no inicio da funcao
                delete_lista_token(identificadores);
                delete_lista_token(identificLivre);
                delete_lista_token(listaInput);
                delete_lista_token(listaOutput);
                delete_lista_token(listaWire);
                delete_lista_token(list_param);
                delete_lista_token(tokens);
                return circuito;
            }
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
            VerilogError err = load_initial_block(&it, identificadores, list_param, circuito);
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

            if (!isIdentificador(it)) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "um identificador", it->valor);
                goto bad_return;
            }
    
            if (identExiste(identificadores, it->valor)) {
                show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                goto bad_return;
            }

            if (len(it->valor) > MAX_PARAM_NAME_SIZE) {
                show_error_size_exceeded("Excedido o tamanho de caracteres para o parametro",
                                         it->linha, it->coluna, it->valor, MAX_PARAM_NAME_SIZE);

                goto bad_return;
            }

            insereTokenString(identificadores, it->valor, -1, -1);
            insereTokenString(list_param, it->valor, -1, -1);

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
            if( !isNumNaturalValido(it->valor) ) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "um numero", it->valor);
                goto bad_return;
            }

            param->value = atoi(it->valor);

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
            addParam(circuito, param);
        }
        else if (it->classe == KW_ASSIGN) {
            VerilogError err = load_assign(&it, listaWire, listaInput, listaOutput, circuito);
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

        if(!avanca(&it))
            goto bad_return_unexpected_eof;
            
    }

bad_return_unexpected_eof:
    show_error_msg("Final do arquivo nao esperado", -1, -1, NULL, NULL);

bad_return:
    // free mem allocated in the head of this func
    delete_lista_token(identificadores);
    delete_lista_token(identificLivre);
    delete_lista_token(listaInput);
    delete_lista_token(listaOutput);
    delete_lista_token(listaWire);
    delete_lista_token(list_param);
    delete_lista_token(tokens);

    return NULL;
}

int isPortaLogica(char* s)
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

VerilogError load_reg(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module)
{
    int is_signed;
    int range_msb;
    int range_lsb;

    Token* t = *it;

    if (!avanca(&t))
        goto load_reg_bad_eof;

    is_signed = 0;

    if (t->classe == KW_SIGNED) {
        is_signed = 1;
        
        avanca(&t);
        if (!t)
            goto load_reg_bad_eof;
    }

    // range specification
    range_msb = 0;
    range_lsb = 0;

    // optional range specification
    // range ::= [ msb_constant_expression : lsb_constant_expression ]
    // TODO: calculate the expressions  
    if (t->classe == SYM_OPEN_SQUAREBRACKET) {
        if (!avanca(&t))
            goto load_reg_bad_eof;

        if (isNumNaturalValido(t->valor)) {
            range_msb = atoi(t->valor);
        }
        else if (identExiste(list_param, t->valor)) {
            range_msb = get_param_by_name(module->listaParam, t->valor)->value;
        }
        else {
            show_error_msg("Numero para bit mais significativo nao foi encontrado",
                           t->linha, t->coluna, "algum numero", t->valor);
            goto load_reg_bad_token;
        }
        
        if (!avanca(&t))
            goto load_reg_bad_eof;

        if (t->classe != SYM_COLON) {
            show_error_msg("Simbolo inesperado",
                           t->linha, t->coluna, ":", t->valor);
            goto load_reg_bad_token;
        }

        if (!avanca(&t))
            goto load_reg_bad_eof;

        if (isNumNaturalValido(t->valor)) {
            range_lsb = atoi(t->valor);
        }
        else if (identExiste(list_param, t->valor)) {
            range_lsb = get_param_by_name(module->listaParam, t->valor)->value;
        }
        else {
            show_error_msg("Numero para bit menos significativo nao foi encontrado",
                           t->linha, t->coluna, "algum numero", t->valor);
            goto load_reg_bad_token;
        }

        if (!avanca(&t))
            goto load_reg_bad_eof;

        if (t->classe != SYM_CLOSE_SQUAREBRACKET) {
            show_error_msg("Simbolo inesperado", t->linha, t->coluna, "]", t->valor);
            goto load_reg_bad_token;
        }

        if ( range_msb < range_lsb ) {
            show_error_msg("Range invalido",
                            t->anterior->linha, t->anterior->coluna, NULL, NULL);
            goto load_reg_bad_token;
        }

        if (!avanca(&t))
            goto load_reg_bad_eof;
    }

    if (!isIdentificador(t)) {
        show_error_msg("Identificador nao foi encontrado",
                       t->linha, t->coluna, "um identificador", t->valor);
        goto load_reg_bad_token;
    }

    // verificar se pode utilizar este identificador
    if (identExiste(identifiers, t->valor)) {
        show_error_identifier_duplicate(t->valor, t->linha, t->coluna);
        goto load_reg_bad_token;
    }
    
    // adicionar na lista de identificadores usados
    insereTokenString(identifiers, t->valor, -1, -1);

    addRegister(module, t->valor, (range_msb - range_lsb + 1), is_signed);

    if (!avanca(&t))
        goto load_reg_bad_eof;

    if (t->classe != SYM_SEMICOLON) {
        show_error_msg("Simbolo esperado nao foi encontrado",
                       t->linha, t->coluna, ";", t->valor);
        goto load_reg_bad_token;
    }

//load_reg_sucess:
    *it = t;
    return NO_ERROR;

load_reg_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_reg_bad_eof:
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
        if (!isNumNaturalValido(t->valor))
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
        if (!isNumNaturalValido(t->valor))
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
    return NO_ERROR;

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

VerilogError load_initial_block(Token** it, ListaToken* identifiers, ListaToken* list_param, Module* module)
{
    Token* t = *it;
    Register* left_reg = NULL;

    if (!avanca(&t))
        goto load_initial_block_bad_eof;

    // treat a single statement attrib, for now

    if (!identExiste(identifiers, t->valor)) {
        show_error_msg("Infelizmente, o initial ainda nao foi devidamente implementado",
                       t->linha, t->coluna, "apenas uma atribuicao", t->valor);
        goto load_initial_block_bad_token;
    }

    // waiting for a reg, for now
    left_reg = get_reg_by_name(module->listaReg, t->valor);

    if (!left_reg) {
        show_error_msg("Infelizmente, o initial ainda nao foi devidamente implementado",
                        t->linha, t->coluna, "um registrador", t->valor);
        goto load_initial_block_bad_token;
    }

    if (!avanca(&t))
        goto load_initial_block_bad_eof;
    
    if (t->classe != SYM_EQ) {
        show_error_msg("Token inesperado foi encontrado", t->linha, t->coluna, "=", t->valor);
        goto load_initial_block_bad_token;
    }

    if (!avanca(&t))
        goto load_initial_block_bad_eof;

    // agora ele espera um literal ou parametro
    if (isNumNaturalValido(t->valor)) {
        left_reg->value = strtol(t->valor, NULL, 10);
    }
    else if (identExiste(list_param, t->valor)) {
        Param* p = get_param_by_name(module->listaParam, t->valor);
        left_reg->value = p->value;
    }
    else {
        show_error_msg("Token inesperado foi encontrado",
                        t->linha, t->coluna, "um numero ou parametro", t->valor);
        goto load_initial_block_bad_token;
    }

    if (!avanca(&t))
        goto load_initial_block_bad_eof;

    if (t->classe != SYM_SEMICOLON) {
        show_error_msg("Token inesperado foi encontrado",
                        t->linha, t->coluna, ";", t->valor);
        goto load_initial_block_bad_token;
    }

//load_initial_block_sucess:
    *it = t;
    return NO_ERROR;

load_initial_block_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_initial_block_bad_eof:
    return ERROR_VERILOG_BAD_EOF;
}

VerilogError load_assign(Token** it, ListaToken* list_wire, ListaToken* list_in, ListaToken* list_out, Module* module)
{
    Componente in;
    Componente out;
    Componente gate;

    Token* t = *it;

    if (!avanca(&t))
        goto load_assign_bad_eof;

    if (!isIdentificador(t)) {
        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, "um identificador", t->valor);
        goto load_assign_bad_token;
    }

    gate = novoComponente("assign", assign);

    // TODO: check for impossible cases
    
    if (identExiste(list_wire, t->valor)) {
        // inserir, na lista de saidas da gate, esta saida
        out = getComponenteItemPorNome(module->listaWires, t->valor);
        insereComponente(gate->listaSaida, out);
        insereComponente(out->listaEntrada, gate);
    }
    else if (identExiste(list_out, t->valor)) {
        // inserir, na lista de saidas da gate, esta saida
        out = getComponenteItemPorNome(module->listaFiosSaida, t->valor);
        insereComponente(gate->listaSaida, out);
        insereComponente(out->listaEntrada, gate);
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

    // simplest expression is another net

    // negation (~) is also simple, but, for now, is not assign anymore

    if ( t->classe == SYM_TILDE ) {
        gate->tipo.operador = op_not;

        if (!avanca(&t))
            goto load_assign_bad_eof;
    }

    if ( !isIdentificador(t) ) {
        show_error_msg("Token inesperado foi encontrado",
                        t->linha, t->coluna, "algum identificador", t->valor);
        goto load_assign_bad_token;
    }

    if ( identExiste(list_wire, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = getComponenteItemPorNome(module->listaWires, t->valor);
        insereComponente(gate->listaEntrada, in);
        insereComponente(in->listaSaida, gate);
    }
    else if ( identExiste(list_in, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = getComponenteItemPorNome(module->listaFiosEntrada, t->valor);
        insereComponente(gate->listaEntrada, in);
        insereComponente(in->listaSaida, gate);
    }
    else if( identExiste(list_out, t->valor) ) {
        // inserir, na lista de entradas da gate, esta entrada
        in = getComponenteItemPorNome(module->listaFiosSaida, t->valor);
        insereComponente(gate->listaEntrada, in);
        insereComponente(in->listaSaida, gate);
    }
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

    if (t->classe != SYM_SEMICOLON) {
        show_error_msg("Token inesperado foi encontrado",
                       t->linha, t->coluna, ";", t->valor);
        goto load_assign_bad_token;
    }

//load_assign_sucess:
    *it = t;
    return NO_ERROR;

load_assign_bad_token:
    return ERROR_VERILOG_BAD_TOKEN;

load_assign_bad_eof:
    return ERROR_VERILOG_BAD_EOF;

}
