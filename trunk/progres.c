/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estruturas.h"
#include "erros.h"
#include "lex.h"

t_circuito* carregaCircuito(FILE *arquivo)
{
    t_circuito *circuito = novoCircuito();

    ListaToken* tokens = tokeniza(arquivo);
    ListaToken* identificadores = novaListaToken(); // lista de todos os identificadores
    ListaToken* identificLivre = novaListaToken(); // lista de ident. de entrada ou saida ainda nao definidos como tal

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
        exibeMsgErro("Final do arquivo não esperado. Era esperado um identificador", -1, -1, NULL, NULL);
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
        exibeMsgErro("Final do arquivo não esperado. Era esperado '('", -1, -1, NULL, NULL);
        return NULL;
    }
    else if(!iguais(it->valor, "(")) {
        // se it->valor não é '(', pare
        exibeMsgErro("Simbolo esperado nao foi encontrado", it->linha, it->coluna, "(", it->valor);
        return NULL;
    }

    avanca(&it);

    // devemos agora ler os parametros do modulo

    int virgula = 0; // um flag para indicar se estamos esperando por uma virgula

    while(1) {
        if(!it) {
            if(virgula)
                exibeMsgErro("Final do arquivo não esperado. Era esperada uma virgula", -1, -1, NULL, NULL);
            else
                exibeMsgErro("Final do arquivo não esperado. Era esperado um identificador valido ou ')'", -1, -1, NULL, NULL);

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
        exibeMsgErro("Final do arquivo nao esperado. Era esperado ';'", -1, -1, NULL, NULL);
        return NULL;
    }

    // (continuar aqui...)

    if(iguais(it->valor, "input")) {

    }
    else if(iguais(it->valor, "output")) {

    }
    else if(iguais(it->valor, "wire")) {

    }

    while(it) {

        avanca(&it);
    }

    return NULL;
}

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
            printf("Arquivo de entrada corrompido.\n");
            return NULL;
        }

        insereTokenString(nomesUsados, it->valor, it->linha, it->coluna);

        addSinal(entradas, it->valor);
        indice++;

        avanca(&it);

        if(!it) {
            printf("Arquivo de entrada corrompido.\n");
            return NULL;
        }

        if( iguais(it->valor, "{") ) {
            // loop para um sinal
            while(1) {
                avanca(&it);

                if(!it) {
                    printf("Arquivo de entrada corrompido.\n");
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
                    printf("Arquivo de entrada corrompido.\n");
                    return NULL;
                }

                avanca(&it);

                if(!it) {
                    printf("Arquivo de entrada corrompido.\n");
                    return NULL;
                }

                if(iguais(it->valor, "(")) {
                    avanca(&it);

                    if(!it) {
                        printf("Arquivo de entrada corrompido.\n");
                        return NULL;
                    }

                    if( apenasDigitos(it->valor) && (strlen(it->valor) < 5) ) { // importante não ser um valor muito grande
                        addPulso(entradas->lista + indice, valorLogico, atoi(it->valor));
                    }
                    else {
                        printf("Arquivo de entrada corrompido.\n");
                        return NULL;
                    }

                    avanca(&it);

                    if(!it) {
                        printf("Arquivo de entrada corrompido.\n");
                        return NULL;
                    }

                    if( iguais(it->valor, ")") ) {
                        avanca(&it);

                        if(!it) {
                            printf("Arquivo de entrada corrompido.\n");
                            return NULL;
                        }

                        if( iguais(it->valor, ",") )
                            continue;
                        else if( iguais(it->valor, "}") )
                            break;
                    }
                    else {
                        printf("Arquivo de entrada corrompido.\n");
                        return NULL;
                    }
                }
                else {
                    printf("Arquivo de entrada corrompido.\n");
                    return NULL;
                }
            }
        }
        else {
            printf("Arquivo de entrada corrompido.\n");
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

    return 0;
}
