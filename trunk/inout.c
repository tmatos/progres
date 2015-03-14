/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>

#include "sinais.h"
#include "lex.h"
#include "inout.h"

Sinais* carregaEntradas(FILE *arquivo) {
    int indice = -1; // indexador do vetor de sinais de entrada
    ValorLogico valorLogico;
    Sinais *entradas = novaSinais();
    Token *it = NULL;

    ListaToken* nomesUsados = novaListaToken(); // nomes de entrada já lidos

    ListaToken* tokens = tokeniza(arquivo);

    if(!tokens)
        return NULL;

    it = tokens->primeiro;

    if(!it) {
        printf("Arquivo de entrada aparentemente vazio.\n");
        return NULL;
    }

    while(1) {
        if( isSimbolo( it->valor[0] ) ) {
            printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        insereTokenString(nomesUsados, it->valor, it->linha, it->coluna);

        addSinal(entradas, it->valor);
        indice++;

        avanca(&it);

        if(!it) {
            printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        if( iguais(it->valor, "{") ) {
            // loop para um sinal
            while(1) {
                avanca(&it);

                if(!it) {
                    printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                valorLogico = nulo;

                if(iguais(it->valor, "0")) {
                    valorLogico = zero;
                }
                else if(iguais(it->valor, "1")) {
                    valorLogico = um;
                }
                else if(iguais(it->valor, "x") || iguais(it->valor, "X")) {
                    valorLogico = x;
                }
                else if(iguais(it->valor, "}")){
                    break;
                }
                else {
                    printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                avanca(&it);

                if(!it) {
                    printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                if(iguais(it->valor, "(")) {
                    avanca(&it);

                    if(!it) {
                        printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    if(isNumNaturalValido(it->valor)) {
                        addPulso(entradas->lista + indice, valorLogico, atoi(it->valor));
                    }
                    else {
                        printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    avanca(&it);

                    if(!it) {
                        printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    if( iguais(it->valor, ")") ) {
                        avanca(&it);

                        if(!it) {
                            printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                            return NULL;
                        }

                        if( iguais(it->valor, ",") )
                            continue;
                        else if( iguais(it->valor, "}") )
                            break;
                    }
                    else {
                        printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }
                }
                else {
                    printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }
            }
        }
        else {
            printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        avanca(&it);

        if(!it) {
            printf("Arquivo de entrada completamente lido.\n");
            break;
        }
    }

    return entradas;
}

void salvarSinais(Sinais *sinaisSaida, FILE *arqSaida)
{
    int si; // indexador dos sinais na lista de sinais de entrada
    Sinal *itSinais = NULL; // Iterador para os sinais num conjunto de entrada ou saida
    Pulso *it = NULL; // Iterador para os pulsos em um Sinal

    if(!sinaisSaida || !arqSaida)
        return;

    si = 0;
    itSinais = sinaisSaida->lista;

    while(si < sinaisSaida->quantidade)
    {
        fprintf(arqSaida, "%s {", itSinais[si].nome);

        it = itSinais[si].pulsos; // Aqui, o indice 0 indica qual dos sinais na lista

        while(it->valor != nulo)
        {
            if(it != itSinais[si].pulsos) // Insere virgula apenas se não é a primeira iteração
                fprintf(arqSaida, ", ");

            switch(it->valor)
            {
                case um:
                    fprintf(arqSaida, "1(%d)", it->tempo);
                    break;
                case zero:
                    fprintf(arqSaida, "0(%d)", it->tempo);
                    break;
                case x:
                    fprintf(arqSaida, "x(%d)", it->tempo);
                    break;
                case z:
                    fprintf(arqSaida, "z(%d)", it->tempo);
                    break;
                case nulo:
                    break;
            }

            it++;
        }

        fprintf(arqSaida, "}\n");

        si++;
    }
}
