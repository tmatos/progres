/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "sinais.h"
#include "lex.h"
#include "inout.h"
#include "erros.h"

Sinais* erroFatalArquivoCorrompido()
{
    if (!global_silent_mode)
        printf("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);

    return NULL;
}

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

    if(!it) {
        if (!global_silent_mode)
            printf("%s", MSG_ARQUIVO_ENTRADA_VAZIO);

        return NULL;
    }

    // loop para o conjunto de sinais
    while (1)
    {
        if ( isSimbolo(it->valor[0]) )
            return erroFatalArquivoCorrompido();

        // TODO: check signal name duplication errors

        insereTokenString(nomesUsados, it->valor, it->linha, it->coluna);

        addSinal(entradas, it->valor);
        indice++;

        if (!avanca(&it))
            return erroFatalArquivoCorrompido();

        if ( !iguais(it->valor, "{") )
            return erroFatalArquivoCorrompido();

        // loop para um sinal
        while (1)
        {
            if (!avanca(&it))
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

            if(!avanca(&it))
                return erroFatalArquivoCorrompido();

            if( !iguais(it->valor, "(") )
                return erroFatalArquivoCorrompido();

            if(!avanca(&it))
                return erroFatalArquivoCorrompido();

            if( !isNumNaturalValido(it->valor) )
                return erroFatalArquivoCorrompido();
            
            duracao = strtol(it->valor, NULL, 10);
            addPulso((entradas->lista + indice), valorLogico, duracao);

            if(!avanca(&it))
                return erroFatalArquivoCorrompido();

            if( !iguais(it->valor, ")") )
                return erroFatalArquivoCorrompido();

            if(!avanca(&it))
                return erroFatalArquivoCorrompido();

            if( iguais(it->valor, ",") )
                continue;
            else if( iguais(it->valor, "}") )
                break;
             
        }

        if(!avanca(&it)) {
            if (!global_silent_mode)
                printf("%s", MSG_ARQUIVO_ENTRADA_LIDO_COMPLETO);

            break;
        }
    }

    delete_lista_token(nomesUsados);
    delete_lista_token(tokens);

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
                    fprintf(arqSaida, "1(%llu)", it->tempo);
                    break;
                case VAL_0:
                    fprintf(arqSaida, "0(%llu)", it->tempo);
                    break;
                case VAL_X:
                    fprintf(arqSaida, "x(%llu)", it->tempo);
                    break;
                case VAL_Z:
                    fprintf(arqSaida, "z(%llu)", it->tempo);
                    break;
                case VAL_H:
                    fprintf(arqSaida, "1(%llu)", it->tempo);
                    break;
                case VAL_L:
                    fprintf(arqSaida, "0(%llu)", it->tempo);
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
