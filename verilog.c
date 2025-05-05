/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

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

Module* carregaCircuito(FILE* arquivo)
{
    Componente in;
    Componente out;
    Componente porta;
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

    Token* it = NULL;

    if(!tokens)
        goto bad_return;

    it = tokens->primeiro;

    if(!it)
        goto bad_return;

    circuito = novoCircuito();

    if( it->classe != KW_MODULE ) {
        show_error_msg("Palavra-chave nao encontrada onde esperada",
                       it->linha, it->coluna, "module", it->valor);
        goto bad_return;
    }

    avanca(&it);

    if(!it) {
        show_error_msg("Final do arquivo nao esperado",
                       -1, -1, "um identificador", NULL);
        goto bad_return;
    }
    else if( !isIdentificador(it) ) {
        show_error_msg("Identificador nao encontrado",
                       it->linha, it->coluna, "identificador valido", it->valor);
        goto bad_return;
    }
    else {
        // senao, adicione-o a lista de identificadores
        insereTokenString(identificadores, it->valor, -1, -1);
    }

    avanca(&it);

    if(!it) {
        show_error_msg("Final do arquivo nao esperado", -1, -1, "(' ou ';", NULL);
        goto bad_return;
    }
    else if( it->classe != SYM_OPEN_BRACKET && it->classe != SYM_SEMICOLON) {
        // se it->valor nao eh '(' ou ';', pare
        show_error_msg("Simbolo esperado nao foi encontrado",
                     it->linha, it->coluna, "(' ou ';", it->valor);
        goto bad_return;
    }

    if( it->classe == SYM_OPEN_BRACKET ) {
        // devemos agora ler os parametros do modulo
        avanca(&it);

        virgula = 0; // nao esperando por virgula, por enquanto

        while(1)
        {
            if(!it) {
                if(virgula) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, ",", NULL);
                }
                else {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, "identificador valido ou )", NULL);
                }

                goto bad_return;
            }

            if( it->classe == SYM_CLOSE_BRACKET ) {
                // it->valor eh ')'
                break;
            }

            if(virgula) {
                if( it->classe == SYM_COMMA ) {
                    virgula = 0;
                    avanca(&it);
                    continue;
                    // TODO: bug de virgula a mais...
                }
                else {
                    show_error_msg("Simbolo esperado nao foi encontrado",
                                   it->linha, it->coluna, ",' ou ')", it->valor);
                    goto bad_return;
                }
            }

            if( isIdentificador(it) ) {
                if( identExiste(identificadores, it->valor) ) {
                    show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                    goto bad_return;
                }
                else {
                    insereTokenString(identificadores, it->valor, -1, -1);
                    insereTokenString(identificLivre, it->valor, -1, -1);
                    virgula = 1;
                }
            }
            else {
                show_error_msg("Identificador nao foi encontrado",
                               it->linha, it->coluna, "um identificador", it->valor);
                goto bad_return;
            }

            avanca(&it);
        }

        avanca(&it);

        if(!it) {
            show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
            goto bad_return;
        }

        if( it->classe != SYM_SEMICOLON ) {
            show_error_msg("Simbolo esperado nao foi encontrado",
                           it->linha, it->coluna, ";", it->valor);
            goto bad_return;
        }
    }

    avanca(&it);

    if(!it)
        goto bad_return_unexpected_eof;

    porta = NULL;

    // process body of the module until it ends
    while(1)
    {
        if( it->classe == KW_INPUT
            || it->classe == KW_OUTPUT
            || it->classe == KW_WIRE )
        {
            // usado posteriormente para saber se os identificadores serao in ou out
            char tipo[MAX_TOKEN_SIZE];
            copy(tipo, it->valor);

            avanca(&it);

            virgula = 0; // nao esperando por uma virgula inicialmente

            while(1)
            {
                if(!it) {
                    if(virgula) {
                        show_error_msg("Final do arquivo nao esperado",
                                       -1, -1, ",", NULL);
                    } else {
                        show_error_msg("Final do arquivo nao esperado",
                                       -1, -1, "identificador valido", NULL);
                    }

                    goto bad_return;
                }

                if( it->classe == SYM_SEMICOLON ) {
                    break;
                }

                if(virgula) {
                    if( it->classe == SYM_COMMA ) {
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

                if( !iguais(tipo, "wire") && !identExiste(identificLivre, it->valor) ) {
                    show_error_msg("Identificador invalido",
                                   it->linha, it->coluna,
                                   "identificador valido e que ainda possa ser atribuido",
                                   NULL);
                    goto bad_return;
                }

                if( iguais(tipo, "input") ) {
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
        else if( it->classe == KW_REG ) {
            avanca(&it);

            if(!it)
               goto bad_return_unexpected_eof;

            if( !isIdentificador(it) ) {
                show_error_msg("Identificador nao foi encontrado",
                               it->linha, it->coluna, "um identificador", it->valor);
                goto bad_return;
            }

            // verificar se pode utilizar este identificador
            if( identExiste(identificadores, it->valor) ) {
                show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                goto bad_return;
            }
            
            // adicionar na lista de identificadores usados
            insereTokenString(identificadores, it->valor, -1, -1);

            addRegister(circuito, it->valor, 1);

            avanca(&it);

            if(!it)
               goto bad_return_unexpected_eof;

            if(it->classe != SYM_SEMICOLON) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }
        }
        else if( isPortaLogica(it->valor) ) {
            porta = NULL;
            
            switch (it->classe)
            {
            case KW_AND:
                porta = novoComponente("PortaAND", op_and);
                break;
            case KW_OR:
                porta = novoComponente("PortaOR", op_or);
                break;
            case KW_XOR:
                porta = novoComponente("PortaXOR", op_xor);
                break;
            case KW_NAND:
                porta = novoComponente("PortaNAND", op_nand);
                break;
            case KW_NOR:
                porta = novoComponente("PortaNOR", op_nor);
                break;
            case KW_XNOR:
                porta = novoComponente("PortaXNOR", op_xnor);
                break;
            case KW_NOT:
                porta = novoComponente("PortaNOT", op_not);
                break;
            case KW_BUF:
                porta = novoComponente("Buffer", op_buf);
            default:
                break;
            }

            avanca(&it);

            if(!it) {
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
                copy(porta->nome, it->valor);

                avanca(&it);
            }

            if(!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1,
                               "(' ou '#", NULL);
                goto bad_return;
            }

            if( it->classe == SYM_HASHTAG ) {
                avanca(&it);

                if(!it) {
                    show_error_msg("Final do arquivo nao esperado",
                                   -1, -1, "um numero inteiro nao negativo", NULL);
                    goto bad_return;
                }
                else if( !isNumNaturalValido(it->valor) ) {
                    char esperado[100];
                    sprintf(esperado,
                            "um numero inteiro nao negativo e com ate %d digitos",
                            MAX_DIGITOS_NUM);
                    show_error_msg("Numero valido nao foi encontrado",
                                   it->linha, it->coluna, esperado, it->valor);
                    goto bad_return;
                }
                else {
                    // Guardar o atraso dessa porta
                    porta->tipo.atraso = atoi(it->valor); //FIXME: tipo errado!
                }

                avanca(&it);

                if(!it) {
                    show_error_msg("Final do arquivo nao esperado", -1, -1, "(", NULL);
                    goto bad_return;
                }
            }

            if( it->classe != SYM_OPEN_BRACKET ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, "(", it->valor);
                goto bad_return;
            }

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "identificador para wire ou output", NULL);
                goto bad_return;
            }

            if( identExiste(listaWire, it->valor) ) {
                // inserir na lista de saidas da porta, esta saida
                out = getComponenteItemPorNome(circuito->listaWires, it->valor);
                insereComponente(porta->listaSaida, out);
                insereComponente(out->listaEntrada, porta);
            }
            else if( identExiste(listaOutput, it->valor) ) {
                // inserir na lista de saidas da porta, esta saida
                out = getComponenteItemPorNome(circuito->listaFiosSaida, it->valor);
                insereComponente(porta->listaSaida, out);
                insereComponente(out->listaEntrada, porta);
            }
            else {
                show_error_msg("Fio ou saida nao foi encontrado",
                               it->linha, it->coluna,
                               "identificador para wire ou output", it->valor);
                goto bad_return;
            }

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ",", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_COMMA ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ",", it->valor);
                goto bad_return;
            }

            porta_inputs: // Label para a parte do codigo onde ha leitura de entradas da porta logica

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um identificador", NULL);
                goto bad_return;
            }

            if( identExiste(listaWire, it->valor) ) {
                // inserir na lista de entradas da porta, esta entrada
                in = getComponenteItemPorNome(circuito->listaWires, it->valor);
                insereComponente(porta->listaEntrada, in);
                insereComponente(in->listaSaida, porta);
            }
            else if( identExiste(listaInput, it->valor) ) {
                // inserir na lista de entradas da porta, esta entrada
                in = getComponenteItemPorNome(circuito->listaFiosEntrada, it->valor);
                insereComponente(porta->listaEntrada, in);
                insereComponente(in->listaSaida, porta);
            }
            else if( identExiste(listaOutput, it->valor) ) {
                // inserir na lista de entradas da porta, esta entrada
                in = getComponenteItemPorNome(circuito->listaFiosSaida, it->valor);
                insereComponente(porta->listaEntrada, in);
                insereComponente(in->listaSaida, porta);
            }
            else {
                show_error_msg("Entrada da porta invalida",
                               it->linha, it->coluna,
                               "uma entrada valida (tipos: input, output ou wire)",
                               it->valor);
                goto bad_return;
            }

            avanca(&it);

            if(!it) {
                if( porta->tipo.operador == op_not || porta->tipo.operador == op_buf ) {
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
                if( porta->tipo.operador == op_not || porta->tipo.operador == op_buf ) {
                    show_error_msg("Simbolo esperado nao foi encontrado",
                                   it->linha, it->coluna, ")", it->valor);
                    goto bad_return;
                }
                else {
                    if( it->classe == SYM_COMMA ) {
                        goto porta_inputs;
                    }
                    else {
                        show_error_msg("Simbolo esperado nao foi encontrado",
                                       it->linha, it->coluna, ")' ou ',", it->valor);
                        goto bad_return;
                    }
                }
            }

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_SEMICOLON ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }

            // finalmente, inserimos a porta na lista de portas do circuito
            adicionaPorta(circuito, porta);
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
        else if( it->classe == KW_INITIAL ) {
            show_error_msg("Lamentamos mas o initial ainda nao foi implementado",
                           it->linha, it->coluna, "algum comando", it->valor);
            goto bad_return;
        }
        else if( it->classe == KW_LOCALPARAM ) {
            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado",
                               -1, -1, "um identificador", NULL);
                goto bad_return;
            }

            if( !isIdentificador(it) ) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "um identificador", it->valor);
                goto bad_return;
            }
    
            if( identExiste(identificadores, it->valor) ) {
                show_error_identifier_duplicate(it->valor, it->linha, it->coluna);
                goto bad_return;
            }

            if( len(it->valor) > MAX_PARAM_NAME_SIZE ) {
                printf("%d:%d: erro: O identificador '%s' excede o tamanho maximo de %d para parametros.\n",
                       it->linha, it->coluna, it->valor, MAX_PARAM_NAME_SIZE);
                goto bad_return;
            }

            insereTokenString(identificadores, it->valor, -1, -1);
            insereTokenString(list_param, it->valor, -1, -1);

            Param* param = (Param*) xcalloc(1, sizeof(Param));
            param->is_local = 1;
            copy( param->name, it->valor );

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, "=", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_EQ ) {
                show_error_msg("Token inesperado foi encontrado",
                               it->linha, it->coluna, "=", it->valor);
                goto bad_return;
            }

            avanca(&it);

            if(!it) {
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

            avanca(&it);

            if(!it) {
                show_error_msg("Final do arquivo nao esperado", -1, -1, ";", NULL);
                goto bad_return;
            }

            if( it->classe != SYM_SEMICOLON ) {
                show_error_msg("Simbolo esperado nao foi encontrado",
                               it->linha, it->coluna, ";", it->valor);
                goto bad_return;
            }

            // include the param in the circuit struct
            addParam(circuito, param);
        }
        else {
            show_error_msg("Token inesperado foi encontrado",
                           it->linha, it->coluna, "algum comando", it->valor);
            goto bad_return;
        }

        avanca(&it);

        if(!it)
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
