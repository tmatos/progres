/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "sinais.h"

#define MSG_ARQUIVO_ENTRADA_CORROMPIDO " " // bogus

Sinais* carregaEntradas(FILE* arquivo)
{
    int indice = -1; // indexador do vetor de sinais de entrada
    ValorLogico valorLogico;
    Tempo duracao;
    Sinais* entradas = novaSinais();
    Token* it = NULL;

    ListaToken* nomesUsados = novaListaToken(); // nomes de entrada ja lidos

    ListaToken* tokens = tokeniza(arquivo);

    if(!tokens)
        return NULL;

    it = tokens->primeiro;

    if (!it) {
        printf("Arquivo de entrada aparentemente vazio.\n");
        return NULL;
    }

    while (1)
    {
        if ( isSimbolo( it->valor[0] ) ) {
            printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        insereTokenString(nomesUsados, it->valor, it->linha, it->coluna);

        addSinal(entradas, it->valor);
        indice++;

        if (!avanca(&it)) {
            printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        if ( iguais(it->valor, "{") ) {
            // loop para um sinal
            while (1)
            {
                if (!avanca(&it)) {
                    printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                valorLogico = VAL_BLANK;

                if (iguais(it->valor, "0")) {
                    valorLogico = VAL_0;
                }
                else if (iguais(it->valor, "1")) {
                    valorLogico = VAL_1;
                }
                else if (iguais(it->valor, "x") || iguais(it->valor, "X")) {
                    valorLogico = VAL_X;
                }
                else if (iguais(it->valor, "z") || iguais(it->valor, "Z")) {
                    valorLogico = VAL_Z;
                }
                else if (iguais(it->valor, "}")){
                    break;
                }
                else {
                    printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                if (!avanca(&it)) {
                    printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }

                if (iguais(it->valor, "(")) {
                    if (!avanca(&it)) {
                        printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    if (isNumNaturalValido(it->valor)) {
                        duracao = strtol(it->valor, NULL, 10);
                        addPulso(entradas->lista + indice, valorLogico, duracao);
                    }
                    else {
                        printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    if (!avanca(&it)) {
                        printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }

                    if ( iguais(it->valor, ")") ) {
                        if (!avanca(&it)) {
                            printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                            return NULL;
                        }

                        if ( iguais(it->valor, ",") ) {
                            continue;
                        } else if ( iguais(it->valor, "}") ) {
                            break;
                        }
                    }
                    else {
                        printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                        return NULL;
                    }
                }
                else {
                    printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
                    return NULL;
                }
            }
        }
        else {
            printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);
            return NULL;
        }

        if (!avanca(&it)) {
            printf("Arquivo de entrada completamente lido.\n");
            break;
        }
    }

    return entradas;
}

void salvarSinais(Sinais *sinaisSaida, FILE *arqSaida)
{
    int si; // indexador dos sinais na lista de sinais de entrada
    Sinal* itSinais = NULL; // Iterador para os sinais num conjunto de entrada ou saida
    Pulso* it = NULL; // Iterador para os pulsos em um Sinal

    if ( !sinaisSaida || !arqSaida ) {
        return;
    }

    si = 0;
    itSinais = sinaisSaida->lista;

    while (si < sinaisSaida->quantidade)
    {
        fprintf(arqSaida, "%s {", itSinais[si].nome);

        it = itSinais[si].pulsos; // Aqui, o indice 0 indica qual dos sinais na lista

        while (it->valor != VAL_BLANK)
        {
            // Insere virgula apenas se nao e a primeira iteracao
            if ( it != itSinais[si].pulsos ) {
                fprintf(arqSaida, ", ");
            }

            switch (it->valor)
            {
                case VAL_1:
                    fprintf(arqSaida, "1(%d)", it->tempo);
                    break;
                case VAL_0:
                    fprintf(arqSaida, "0(%d)", it->tempo);
                    break;
                case VAL_X:
                    fprintf(arqSaida, "x(%d)", it->tempo);
                    break;
                case VAL_Z:
                    fprintf(arqSaida, "z(%d)", it->tempo);
                    break;
                case VAL_BLANK:
                    break;
            }

            it++;
        }

        fprintf(arqSaida, "}\n");

        si++;
    }
}

Sinais* carregaArquivoSinais(const char* path)
{
    if (!path)
        return NULL;

    FILE* waveFile = fopen(path, "r");

    if (!waveFile) {
        printf("Impossibilitado de abrir o arquivo de sinais: %s\n", path);
        return NULL;
    }

    printf("Abrindo o arquivo de sinais: %s\n", path);

    Sinais* waves = carregaEntradas(waveFile);

    fclose(waveFile);

    return waves;
}
