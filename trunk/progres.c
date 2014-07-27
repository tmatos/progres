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
#include "eventos.h"

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
    int i, j, validos;
    Tempo t;

    Transicao *listaTr = NULL;
    Transicao *itTr = NULL;

    Evento *fila = NULL;
    Pulso *p = NULL;

    ListaComponente *portasAlteradas = NULL;
    Componente gate = NULL;
    ValorLogico resultado;

    Sinais *saidas = novaSinais();

    if(!circuto || !entradas)
        return NULL;

    validos = 0;

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
    /// ---

    if(validos < circuto->listaFiosEntrada->tamanho)
    {
        printf("O arquivo de entradas tem menos sinais de entrada que o circuito.\n");
        return NULL;
    }

    // Inicialização da fila de eventos com os valores das entradas
    fila = NULL;

    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        t = 0;

        p = circuto->listaFiosEntrada->itens[i]->sinalEntrada->pulsos;
        while(p->valor != nulo)
        {
            insereEvento(&fila, t, circuto->listaFiosEntrada->itens[i], p->valor);
            t = t + p->tempo;

            p++;
        }

        insereEvento(&fila, t, circuto->listaFiosEntrada->itens[i], x); // este sinal fica até infitito
    }

/*
    // inicialização dos valores iniciais default das entradas (valor que estava antes de qq sinal)
    for( i=0 ; i < circuto->listaFiosEntrada->tamanho ; i++ )
    {
        circuto->listaFiosEntrada->itens[i]->valorDinamico = x;
    }

    // inicialização dos valores iniciais default das saidas (valor que estava antes de qq sinal)
    for( i=0 ; i < circuto->listaFiosSaida->tamanho ; i++ )
    {
        circuto->listaFiosSaida->itens[i]->valorDinamico = x;
    }
*/

    // aqui ocorre a simulação propriamente dita, usado fila de eventos
    t = 0;

    while(fila)
    {
        portasAlteradas = novaListaComponente();

        t = fila->quando;

        listaTr = popEvento(&fila);
        itTr = listaTr;

        // atualiza valores de fios e faz uma lista das portas alteradas pelas transicoes em listaTr
        while(itTr)
        {
            if(itTr->fio->valorDinamico != itTr->novoValor) // apenas se houver mudança de valor no fio
            {
                for( i=0 ; i < itTr->fio->listaSaida->tamanho ; i++ )
                {
                    if( !contemComponente( portasAlteradas, itTr->fio->listaSaida->itens[i] ) ) {
                        insereComponente( portasAlteradas, itTr->fio->listaSaida->itens[i] );
                    }
                }

                if(itTr->fio->tipo.operador == output)
                {
                    if( !(itTr->fio->sinalSaida) )
                        itTr->fio->sinalSaida = novoSinal( itTr->fio->nome );

                    addPulso( itTr->fio->sinalSaida, itTr->fio->valorDinamico, t - itTr->fio->sinalSaida->duracaoTotal );
                }

                itTr->fio->valorDinamico = itTr->novoValor;
            }

            itTr = itTr->proximo;
        }

        free(listaTr); // popEvento não liberou a lista de transições, fazemos isso aqui
        listaTr = NULL;

        for( i=0 ; i < portasAlteradas->tamanho ; i++ )
        {
            gate = portasAlteradas->itens[i];

            switch( gate->tipo.operador )
            {
            case op_not:
                if(gate->listaEntrada->itens[0]->valorDinamico == x)
                    resultado = x;
                else if (gate->listaEntrada->itens[0]->valorDinamico == zero)
                    resultado = um;
                else if (gate->listaEntrada->itens[0]->valorDinamico == um)
                    resultado = zero;

                insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[0], resultado);

                break;
            case op_buf:
                insereEvento(&fila, t + gate->tipo.atraso, gate->listaSaida->itens[0], gate->listaEntrada->itens[0]->valorDinamico);

                break;
            case op_and:

                break;
            case op_or:

                break;
            case op_xor:

                break;
            case op_nand:

                break;
            case op_nor:

                break;
            case op_xnor:

                break;
            default:
                break;
            }

            //portasAlteradas->itens[i]->sinaisEntrada
        }

    }

    // copia as saidas da simulação do ciruito para o retorno
    for(i=0 ; i < circuto->listaFiosSaida->tamanho ; i++)
        addSinalPronto(saidas, circuto->listaFiosSaida->itens[i]->sinalSaida);

    return saidas;
}

int main(int argc, char* argv[])
{
    FILE *arquivoVerilog = NULL;
    FILE *wavein = NULL;
    FILE *waveout = NULL;

    Sinais* entradas = NULL;
    Sinais* saidas = NULL;

    t_circuito *circuto1 = NULL;

    if(argc < 2) {
        printf("Uso: progres [arquivo verilog] [arquivo de entradas]\n");
        exit(0);
    }

    arquivoVerilog = fopen(argv[1], "r");

    if(!arquivoVerilog) {
        printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
        exit(1);
    }

    printf("Abrindo o arquivo de circuito: %s\n", argv[1]);

    circuto1 = carregaCircuito(arquivoVerilog);

    fclose(arquivoVerilog);

    if(circuto1) {
        printf("Circuito carregado com sucesso.\n");
    }
    else {
        printf("Erro com o fonte.\n");
    }

    // parte do arquivo wave_in (meieiro isso aqui...)
    if(argc > 2)
    {
        wavein = fopen(argv[2], "r");

        if(!wavein) {
            printf("Impossibilitado de abrir o arquivo de entrada: %s\n", argv[2]);
            exit(1);
        }

        printf("Abrindo o arquivo de entrada: %s\n", argv[2]);

        entradas = carregaEntradas(wavein);

        fclose(wavein);

        if(entradas)
        {
            char pathArquivoSaida[256] = "saida.out";

            waveout = fopen(pathArquivoSaida, "w");

            saidas = simula(circuto1, entradas);

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

        /// DBG - O codigo abaixo 'plota' na tela um array de pulosos, isto e, um sinal
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
