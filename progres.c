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
#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "erros.h"
#include "lex.h"

Sinais* carregaEntradas(FILE *arquivo) {
    int indice = -1; // indexador do vetor de sinais de entrada
    Sinais* entradas = novaSinais();

    ListaToken* nomesUsados = novaListaToken(); // nomes de entrada ja lidos

    ListaToken* tokens = tokeniza(arquivo);

    if(!tokens)
        return NULL;

    Token* it = tokens->primeiro;

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

                t_valor valorLogico = nulo;

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

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("Uso: progres [arquivo verilog] [arquivo de entradas]\n");
        exit(0);
    }

    FILE *arquivo = fopen(argv[1], "r");

    if(!arquivo) {
        printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
        exit(1);
    }

    printf("Abrindo o arquivo de circuito: %s\n", argv[1]);

    t_circuito *circuto1 = carregaCircuito(arquivo);

    fclose(arquivo);
    free(arquivo);

    if(circuto1) {
        printf("Circuito carregado com sucesso.\n");
    }
    else {
        printf("Erro com o fonte.\n");
    }

    // parte do arquivo wave_in (meieiro isso aqui...)
    if(argc > 2)
    {
        FILE *wavein = fopen(argv[2], "r");

        if(!wavein) {
            printf("Impossibilitado de abrir o arquivo de entrada: %s\n", argv[2]);
            exit(1);
        }

        printf("Abrindo o arquivo de entrada: %s\n", argv[2]);

        Sinais* entradas = carregaEntradas(wavein);

        if(entradas)
            free(entradas);

        fclose(wavein);
        free(wavein);

        // Esta função gravará um arquivo de sinais, com os sinas presentes na estrutura indicada
        // e com o mesmo formato do arquivo de entrada.
        //salvarSinais(entradas, "saida.out");

        /// DBG - O codigo abaixo mostra na tela um array de pulosos, isto e, um sinal
        /*int i;

        Pulso* it = entradas->lista[0].pulsos; // Aqui, o indice 0 indica qual dos sinas na lista
        while(it->valor != nulo) {
            for(i = 0 ; i < it->tempo ; i++) {
                switch(it->valor) {
                    case um:
                        printf("-");
                    break;
                    case zero:
                        printf("_");
                        break;
                    case x:
                        printf("x");
                        break;
                }
            }

            it++;
        }*/
        /// DBG
    }

    //system("PAUSE"); /// DBG

    return 0;
}
