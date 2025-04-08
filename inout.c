/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>

#include "sinais.h"
#include "lex.h"
#include "inout.h"

Sinais* erroFatalArquivoCorrompido()
{
    printf(MSG_ARQUIVO_ENTRADA_CORROMPIDO);
    return NULL;
}

Sinais* carregaEntradas(FILE* arquivo)
{
    int indice = -1; // indexador do vetor de sinais de entrada
    ValorLogico valorLogico;
    Sinais* entradas = novaSinais();
    Token* it = NULL;

    ListaToken* nomesUsados = novaListaToken(); // nomes de entrada ja lidos

    ListaToken* tokens = tokeniza(arquivo);

    if(!tokens)
        return NULL;

    it = tokens->primeiro;

    if(!it) {
        printf(MSG_ARQUIVO_ENTRADA_VAZIO);
        return NULL;
    }

    while(1)
    {
        if( isSimbolo( it->valor[0] ) )
            return erroFatalArquivoCorrompido();

        insereTokenString(nomesUsados, it->valor, it->linha, it->coluna);

        addSinal(entradas, it->valor);
        indice++;

        avanca(&it);

        if(!it)
            return erroFatalArquivoCorrompido();

        if( !iguais(it->valor, "{") )
            return erroFatalArquivoCorrompido();

        // loop para um sinal
        while(1)
        {
            avanca(&it);

            if(!it)
                return erroFatalArquivoCorrompido();

            valorLogico = VAL_BLANK;

            if( iguais(it->valor, "0") ) {
                valorLogico = VAL_0;
            }
            else if( iguais(it->valor, "1") ) {
                valorLogico = VAL_1;
            }
            else if( iguais(it->valor, "x") || iguais(it->valor, "X") ) {
                valorLogico = VAL_X;
            }
            else if( iguais(it->valor, "}") ) {
                break;
            }
            else {
                return erroFatalArquivoCorrompido();
            }

            avanca(&it);

            if(!it)
                return erroFatalArquivoCorrompido();

            if( !iguais(it->valor, "(") )
                return erroFatalArquivoCorrompido();

            avanca(&it);

            if(!it)
                return erroFatalArquivoCorrompido();

            if( !isNumNaturalValido(it->valor) )
                return erroFatalArquivoCorrompido();
            
            addPulso(entradas->lista + indice, valorLogico, atoi(it->valor));

            avanca(&it);

            if(!it)
                return erroFatalArquivoCorrompido();

            if( !iguais(it->valor, ")") )
                return erroFatalArquivoCorrompido();

            avanca(&it);

            if(!it)
                return erroFatalArquivoCorrompido();

            if( iguais(it->valor, ",") )
                continue;
            else if( iguais(it->valor, "}") )
                break;
             
        }

        avanca(&it);

        if(!it) {
            printf(MSG_ARQUIVO_ENTRADA_LIDO_COMPLETO);
            break;
        }
    }

    return entradas;
}

void salvarSinais(Sinais* sinaisSaida, FILE* arqSaida)
{
    int si; // indexador dos sinais na lista de sinais de entrada
    Sinal* itSinais = NULL; // Iterador para os sinais num conjunto de entrada ou saida
    Pulso* it = NULL; // Iterador para os pulsos em um Sinal

    if(!sinaisSaida || !arqSaida)
        return;

    si = 0;
    itSinais = sinaisSaida->lista;

    while( si < sinaisSaida->quantidade )
    {
        fprintf(arqSaida, "%s {", itSinais[si].nome);

        it = itSinais[si].pulsos; // Aqui, o indice 0 indica qual dos sinais na lista

        while( it->valor != VAL_BLANK )
        {
            // Insere virgula apenas se nao eh a primeira iteracao
            if( it != itSinais[si].pulsos )
                fprintf(arqSaida, ", ");

            switch(it->valor)
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
