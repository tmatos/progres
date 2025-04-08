/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "erros.h"
#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "lex.h"

t_circuito* carregaCircuito(FILE* arquivo)
{
    Componente in;
    Componente out;
    Componente porta;
    t_circuito* circuito = NULL;

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

    ListaToken* tokens = tokeniza(arquivo);

    Token* it = NULL;

    if(!tokens) {
        return NULL;
    }

    it = tokens->primeiro;

    if(!it) {
        return NULL;
    }

    circuito = novoCircuito();

    if( it->classe != KW_MODULE ) {
        exibeMsgErro("Palavra-chave nao encontrada onde esperada",
                     it->linha, it->coluna, "module", it->valor);
        return NULL;
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo nao esperado. Era esperado um identificador",
                     -1, -1, NULL, NULL);
        return NULL;
    }
    else if( !isIdentificador(it) ) {
        exibeMsgErro("Identificador nao encontrado",
                     it->linha, it->coluna, "identificador valido", it->valor);
        return NULL;
    }
    else {
        // senao, adicione-o a lista de identificadores
        insereTokenString(identificadores, it->valor, -1, -1);
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo nao esperado. Era esperado '(' ou ';'",
                     -1, -1, NULL, NULL);
        return NULL;
    }
    else if( it->classe != SYM_OPEN_BRACKET && it->classe != SYM_SEMICOLON) {
        // se it->valor nao eh '(' ou ';', pare
        exibeMsgErro("Simbolo esperado nao foi encontrado",
                     it->linha, it->coluna, "( ou ;", it->valor);
        return NULL;
    }

    if( it->classe == SYM_OPEN_BRACKET ) {
        // devemos agora ler os parametros do modulo
        avanca(&it);

        virgula = 0; // nao esperando por virgula, por enquanto

        while(1)
        {
            if(!it) {
                if(virgula) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperada uma virgula",
                                -1, -1, NULL, NULL);
                }
                else {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado um identificador valido ou ')'",
                                -1, -1, NULL, NULL);
                }

                return NULL;
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
                    exibeMsgErro("Simbolo esperado nao foi encontrado",
                                it->linha, it->coluna, "uma virgula ou )", it->valor);
                    return NULL;
                }
            }

            if( isIdentificador(it) ) {
                if( identExiste(identificadores, it->valor) ) {
                    printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n",
                        it->linha, it->coluna, it->valor);
                    return NULL;
                }
                else {
                    insereTokenString(identificadores, it->valor, -1, -1);
                    insereTokenString(identificLivre, it->valor, -1, -1);
                    virgula = 1;
                }
            }
            else {
                exibeMsgErro("Identificador nao foi encontrado",
                            it->linha, it->coluna, "um identificador", it->valor);
                return NULL;
            }

            avanca(&it);
        }

        avanca(&it);

        if(!it) {
            exibeMsgErro("Final do arquivo nao esperado. Era esperado ';'",
                        -1, -1, NULL, NULL);
            return NULL;
        }

        if( it->classe != SYM_SEMICOLON ) {
            exibeMsgErro("Simbolo esperado nao foi encontrado",
                        it->linha, it->coluna, ";", it->valor);
            return NULL;
        }
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo nao esperado", -1, -1, NULL, NULL);
        return NULL;
    }

    porta = NULL;

    while(1)
    {
        if( it->classe == KW_INPUT
            || it->classe == KW_OUTPUT
            || it->classe == KW_WIRE )
        {
            // usado posteriormente para saber se os identificadores serao in ou out
            char tipo[MAX_TOKEN_SIZE];
            strcpy(tipo, it->valor);

            avanca(&it);

            virgula = 0; // nao esperando por uma virgula inicialmente

            while(1)
            {
                if(!it) {
                    if(virgula) {
                        exibeMsgErro("Final do arquivo nao esperado. Era esperada uma virgula",
                                     -1, -1, NULL, NULL);
                    } else {
                        exibeMsgErro("Final do arquivo nao esperado. Era esperado um identificador valido",
                                     -1, -1, NULL, NULL);
                    }

                    return NULL;
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
                        exibeMsgErro("Simbolo esperado nao foi encontrado",
                                     it->linha, it->coluna, "uma virgula ou ;", it->valor);
                        return NULL;
                    }
                }

                if( !iguais(tipo, "wire") && !identExiste(identificLivre, it->valor) ) {
                    exibeMsgErro("Identificador invalido. Era esperado um identificador valido e que ainda possa ser atribuido",
                                 it->linha, it->coluna, NULL, NULL);
                    return NULL;
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
                            printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n",
                                   it->linha, it->coluna, it->valor);
                            return NULL;
                        }
                        else {
                            insereTokenString(identificadores, it->valor, -1, -1);
                            insereTokenString(listaWire, it->valor, -1, -1);

                            // atribui como wire o identificador na estrutura
                            adicionaWire(circuito, novoComponente(it->valor, wire));
                        }
                    }
                    else {
                        exibeMsgErro("Identificador nao foi encontrado",
                                     it->linha, it->coluna, "um identificador", it->valor);
                        return NULL;
                    }
                }

                removeTokensPorValor(identificLivre, it->valor);

                virgula = 1;

                avanca(&it);
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
                exibeMsgErro("Final do arquivo nao esperado. Era esperado '( ou #'",
                             -1, -1, NULL, NULL);
                return NULL;
            }

            if( it->classe == SYM_HASHTAG ) {
                avanca(&it);

                if(!it) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado um numero inteiro nao negativo",
                                 -1, -1, NULL, NULL);
                    return NULL;
                }
                else if( !isNumNaturalValido(it->valor) ) {
                    char esperado[100];
                    sprintf(esperado,
                            "um numero inteiro nao negativo e com ate %d digitos",
                            MAX_DIGITOS_NUM);
                    exibeMsgErro("Numero valido nao foi encontrado",
                                 it->linha, it->coluna, esperado, it->valor);
                    return NULL;
                }
                else {
                    // Guardar o atraso dessa porta
                    porta->tipo.atraso = atoi(it->valor); //FIXME: tipo errado!
                }

                avanca(&it);

                if(!it) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado '('",
                                 -1, -1, NULL, NULL);
                    return NULL;
                }
            }

            if( it->classe != SYM_OPEN_BRACKET ) {
                exibeMsgErro("Simbolo esperado nao foi encontrado",
                             it->linha, it->coluna, "(", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado um fio ou saida",
                             -1, -1, NULL, NULL);
                return NULL;
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
                exibeMsgErro("Fio ou saida nao foi encontrado",
                             it->linha, it->coluna,
                             "ident. para um fio ou saida", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperada uma virgula",
                             -1, -1, NULL, NULL);
                return NULL;
            }

            if( it->classe != SYM_COMMA ) {
                exibeMsgErro("Simbolo esperado nao foi encontrado",
                             it->linha, it->coluna, ",", it->valor);
                return NULL;
            }

            porta_inputs: // Label para a parte do codigo onde ha leitura de entradas da porta logica

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperada um identificador",
                             -1, -1, NULL, NULL);
                return NULL;
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
                exibeMsgErro("Entrada da porta invalida",
                             it->linha, it->coluna,
                             "uma entrada valida (tipos: input, output ou wire)",
                             it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                if( porta->tipo.operador == op_not || porta->tipo.operador == op_buf ) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado ')'",
                                 -1, -1, NULL, NULL);
                }
                else {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado ',' ou ')'",
                                 -1, -1, NULL, NULL);
                }

                return NULL;
            }

            if( it->classe != SYM_CLOSE_BRACKET ) {
                if( porta->tipo.operador == op_not || porta->tipo.operador == op_buf ) {
                    exibeMsgErro("Simbolo esperado nao foi encontrado",
                                 it->linha, it->coluna, ")", it->valor);
                    return NULL;
                }
                else {
                    if( it->classe == SYM_COMMA ) {
                        goto porta_inputs;
                    }
                    else {
                        exibeMsgErro("Simbolo esperado nao foi encontrado",
                                     it->linha, it->coluna, "')' ou ','", it->valor);
                        return NULL;
                    }
                }
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado ';'",
                             -1, -1, NULL, NULL);
                return NULL;
            }

            if( it->classe != SYM_SEMICOLON ) {
                exibeMsgErro("Simbolo esperado nao foi encontrado",
                             it->linha, it->coluna, ";", it->valor);
                return NULL;
            }

            // finalmente, inserimos a porta na lista de portas do circuito
            adicionaPorta(circuito, porta);
        }
        else if( it->classe == KW_ENDMODULE ) {
            avanca(&it);

            // nao deve haver mais nada alem do endmodule
            if(it) {
                exibeMsgErro("Token inesperado foi encontrado",
                             it->linha, it->coluna, "nenhum codigo a mais", it->valor);
                return NULL;
            }
            else {
                // TODO: Liberar a memoria alocada
                return circuito;
            }
        }
        else if( it->classe == KW_INITIAL ) {
            exibeMsgErro("Lamentamos mas o initial ainda nao foi implementado",
                         it->linha, it->coluna, "algum comando", it->valor);
            return NULL;
        }
        else {
            exibeMsgErro("Token inesperado foi encontrado",
                         it->linha, it->coluna, "algum comando", it->valor);
            return NULL;
        }

        avanca(&it);

        if(!it) {
            exibeMsgErro("Final do arquivo nao esperado", -1, -1, NULL, NULL);
            return NULL;
        }
    }

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
