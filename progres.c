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

void* xmalloc(size_t t) {
    void* p = malloc(t);

    if(!p)
        erroFatalMemoria();

    return p;
}

void* xrealloc(void* m, size_t t) {
    void* p = realloc(m, t);

    if(!p)
        erroFatalMemoria();

    return p;
}

void* xcalloc(size_t n, size_t t) {
    void* p = calloc(n, t);

    if(!p)
        erroFatalMemoria();

    return p;
}

Sinais* carregaEntradas(FILE *arquivo) {
    int indice = -1; // indexador do vetor de sinais de entrada
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

                ValorLogico valorLogico = nulo;

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
                case nulo:
                    break;
            }

            it++;
        }

        fprintf(arqSaida, "}\n");

        si++;
    }
}

Sinais* simula(t_circuito* circuto, Sinais* entradas)
{
    if(!circuto || !entradas)
        return NULL;

    Sinais* saida = novaSinais();

    int i, j, validos = 0;

    // Validação da correspência das entradas entre os arquivos '.v' e '.in'
    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        for( j=0 ; j < entradas->quantidade ; j++ )
        {
            if( iguais( circuto->listaFiosEntrada->itens[i]->nome, entradas->lista[j].nome ) )
            {
                circuto->listaFiosEntrada->itens[i]->sinalEntrada = &(entradas->lista[j]);
                validos++;
                break;
            }
        }
    }

    /// DBG
    printf("\nENTRADAS:\n  .v = %d\n .in = %d\n batem = %d\n",
           circuto->listaFiosEntrada->tamanho,
           entradas->quantidade,
           validos);



    return NULL;
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

        fclose(wavein);

        if(entradas)
        {
            char pathArquivoSaida[256] = "saida.out";

            FILE *waveout = fopen(pathArquivoSaida, "w");

            Sinais* saidas = simula(circuto1, entradas);

            if(!waveout)
            {
                printf("Erro ao tentar abrir arquivo de saida '%s' para gravacao.\n", pathArquivoSaida);
            }
            else
            {
                // Esta função gravará um arquivo de sinais, com os sinas presentes na estrutura indicada
                // e com o mesmo formato do arquivo de entrada.
                salvarSinais(saidas, waveout);

                fclose(waveout);
            }

            free(entradas);
        }

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
