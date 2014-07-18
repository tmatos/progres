/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "verilog.h"
#include "estruturas.h"
#include "sinais.h"
#include "lex.h"
#include "erros.h"

t_circuito* carregaCircuito(FILE *arquivo)
{
    t_circuito *circuito = novoCircuito();

    ListaToken* tokens = tokeniza(arquivo);

    ListaToken* identificadores = novaListaToken(); // lista de todos os identificadores
    ListaToken* identificLivre = novaListaToken(); // lista de ident. de entrada ou saida ainda nao definidos como tal

    ListaToken* listaInput = novaListaToken(); // lista de todos os identificadores das entradas
    ListaToken* listaOutput = novaListaToken(); // lista de todos os identificadores das saidas
    ListaToken* listaWire = novaListaToken(); // lista de todos os identificadores de wire

    if(!tokens)
        return NULL;

    Token* it = tokens->primeiro;

    if(!it)
        return NULL;

    if(!iguais(it->valor, "module"))
    {
        exibeMsgErro("Palavra-chave nao encontrada onde esperado", it->linha, it->coluna, "module", it->valor);
        return NULL;
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo n�o esperado. Era esperado um identificador", -1, -1, NULL, NULL);
        return NULL;
    }
    else if(!isIdentificador(it)) {
        exibeMsgErro("Identificador nao encontrado", it->linha, it->coluna, "identificador valido", it->valor);
        return NULL;
    }
    else {
        // senao, adicione-o a lista de identificadores
        insereTokenString(identificadores, it->valor, -1, -1);
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo n�o esperado. Era esperado '('", -1, -1, NULL, NULL);
        return NULL;
    }
    else if(!iguais(it->valor, "(")) {
        // se it->valor n�o � '(', pare
        exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, "(", it->valor);
        return NULL;
    }

    avanca(&it);

    // devemos agora ler os parametros do modulo

    int virgula = 0; // um flag para indicar se estamos esperando por uma virgula

    while(1) {
        if(!it) {
            if(virgula)
                exibeMsgErro("Final do arquivo n�o esperado. Era esperada uma virgula", -1, -1, NULL, NULL);
            else
                exibeMsgErro("Final do arquivo n�o esperado. Era esperado um identificador valido ou ')'", -1, -1, NULL, NULL);

            return NULL;
        }

        if(iguais(it->valor, ")"))
            break;

        if(virgula) {
            if(iguais(it->valor, ",")) {
                virgula = 0;
                avanca(&it);
                continue;
                // TODO: bug de virgula a mais...
            }
            else {
                exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, "uma virgula ou )", it->valor);
                return NULL;
            }
        }

        if(isIdentificador(it)) {
            if(identExiste(identificadores, it->valor)) {
                printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n", it->linha, it->coluna, it->valor);
                return NULL;
            }
            else {
                insereTokenString(identificadores, it->valor, -1, -1);
                insereTokenString(identificLivre, it->valor, -1, -1);
                virgula = 1;
            }
        }
        else {
            exibeMsgErro("Identificador nao foi encontrado", it->linha, it->coluna, "um identificador", it->valor);
            return NULL;
        }

        avanca(&it);
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo nao esperado. Era esperado ';'", -1, -1, NULL, NULL);
        return NULL;
    }

    if(!iguais(it->valor, ";")) {
        exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, ";", it->valor);
        return NULL;
    }

    avanca(&it);

    if(!it) {
        exibeMsgErro("Final do arquivo nao esperado", -1, -1, NULL, NULL);
        return NULL;
    }

    while(1) {

        if(iguais(it->valor, "input") || iguais(it->valor, "output") || iguais(it->valor, "wire")) {
            char tipo[MAX_TOKEN_SIZE]; // usado posteriormente para saber se os ident. ser�o in ou out
            strcpy(tipo, it->valor);

            avanca(&it);

            virgula = 0; // n�o esperando por uma virgula inicialmente

            while(1) {
                if(!it) {
                    if(virgula)
                        exibeMsgErro("Final do arquivo n�o esperado. Era esperada uma virgula", -1, -1, NULL, NULL);
                    else
                        exibeMsgErro("Final do arquivo n�o esperado. Era esperado um identificador valido", -1, -1, NULL, NULL);

                    return NULL;
                }

                if(iguais(it->valor, ";"))
                    break;

                if(virgula) {
                    if(iguais(it->valor, ",")) {
                        virgula = 0;
                        avanca(&it);
                        continue; // ainda permite uma virgula a mais...
                    }
                    else {
                        exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, "uma virgula ou ;", it->valor);
                        return NULL;
                    }
                }

                if(!iguais(tipo, "wire") && !identExiste(identificLivre, it->valor)) {
                    exibeMsgErro("Identificador invalido. Era esperado um identificador valido e que ainda possa ser atribuido", it->linha, it->coluna, NULL, NULL);
                    return NULL;
                }

                if(iguais(tipo, "input")) {
                    insereTokenString(listaInput, it->valor, -1, -1);

                    // TODO: atribui como entrada o ident. na estrutura
                    Componente cpIn = novoComponente(it->valor, input);
                    adicionaEntrada(circuito, cpIn);

                }
                else if (iguais(tipo, "output")) {
                    insereTokenString(listaOutput, it->valor, -1, -1);
                    circuito->numSaida++;

                    // TODO: atribui como sa�da o ident. na estrutura
                    Componente cpOut = novoComponente(it->valor, output);
                    adicionaSaida(circuito, cpOut);
                }
                else if(iguais(tipo, "wire")) {

                    if(isIdentificador(it)) {
                        if(identExiste(identificadores, it->valor)) {
                            printf("%d:%d: erro: O identificador '%s' ja estava sendo utilizado.\n", it->linha, it->coluna, it->valor);
                            return NULL;
                        }
                        else {
                            insereTokenString(identificadores, it->valor, -1, -1);
                            insereTokenString(listaWire, it->valor, -1, -1);

                            // TODO: atribui como wire o ident. na estrutura
                        }
                    }
                    else {
                        exibeMsgErro("Identificador nao foi encontrado", it->linha, it->coluna, "um identificador", it->valor);
                        return NULL;
                    }
                }

                removeTokensPorValor(identificLivre, it->valor);

                virgula = 1;

                avanca(&it);
            }
        }
        else if(iguais(it->valor, "not")) {
            Componente portaNot = novoComponente("PortaNot", op_not);

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado '( ou #'", -1, -1, NULL, NULL);
                return NULL;
            }

            if(iguais(it->valor, "#")) {
                avanca(&it);

                if(!it) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado um numero inteiro nao negativo", -1, -1, NULL, NULL);
                    return NULL;
                }
                else if(!isNumNaturalValido(it->valor)) {
                    char esperado[100];
                    sprintf(esperado, "um numero inteiro nao negativo e com ate %d digitos", MAX_DIGITOS_NUM);
                    exibeMsgErro("Numero valido nao foi encontrado", it->linha, it->coluna, esperado, it->valor);
                    return NULL;
                }
                else {
                    // Guardar o atraso dessa porta
                    portaNot->tipo.atraso = atoi(it->valor);
                }

                avanca(&it);

                if(!it) {
                    exibeMsgErro("Final do arquivo nao esperado. Era esperado '('", -1, -1, NULL, NULL);
                    return NULL;
                }
            }

            if(!iguais(it->valor, "(")) {
                exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, "(", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado um fio ou saida", -1, -1, NULL, NULL);
                return NULL;
            }

            if(identExiste(listaWire, it->valor) || identExiste(listaOutput, it->valor)) {
                //TODO: inserir a porta
            }
            else {
                exibeMsgErro("Fio ou saida nao foi encontrado", it->linha, it->coluna, "ident. para um fio ou saida", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperada uma virgula", -1, -1, NULL, NULL);
                return NULL;
            }

            if(!iguais(it->valor, ",")) {
                exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, ",", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperada um identificador", -1, -1, NULL, NULL);
                return NULL;
            }

            if(identExiste(listaWire, it->valor) || identExiste(listaInput, it->valor) || identExiste(listaOutput, it->valor)) {
                //TODO: inserir a porta
            }
            else {
                exibeMsgErro("Entrada da porta invalida", it->linha, it->coluna, "uma entrada valida (tipos: input, output ou wire)", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado )", -1, -1, NULL, NULL);
                return NULL;
            }

            if(!iguais(it->valor, ")")) {
                exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, ")", it->valor);
                return NULL;
            }

            avanca(&it);

            if(!it) {
                exibeMsgErro("Final do arquivo nao esperado. Era esperado ;", -1, -1, NULL, NULL);
                return NULL;
            }

            if(!iguais(it->valor, ";")) {
                exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, ";", it->valor);
                return NULL;
            }
        }
        else if(iguais(it->valor, "and") || iguais(it->valor, "or")) {

        }
        else if(iguais(it->valor, "endmodule")) {
            avanca(&it);

            // n�o deve haver mais nada al�m do endmodule
            if(it) {
                exibeMsgErro("Token inesperado foi encontrado", it->linha, it->coluna, "nenhum codigo a mais", it->valor);
                return NULL;
            }
            else {
                return circuito;
            }
        }

        avanca(&it);

        if(!it) {
            //exibeMsgErro("Final do arquivo nao esperado", -1, -1, NULL, NULL);
            return NULL;
        }
    }

    return NULL;
}

int isPortaLogica(char* s) {
    if(iguais(s, "and") || iguais(s, "or") || iguais(s, "xor") ||
       iguais(s, "nand") || iguais(s, "nor") || iguais(s, "xnor") || iguais(s, "not") )
        return 1;
    else
        return 0;
}
