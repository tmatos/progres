/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>

#include "sinais.h"
#include "strutil.h"
#include "lex.h"
#include "inout.h"
#include "erros.h"

Sinais* erroFatalArquivoCorrompido()
{
    print("%s", MSG_ARQUIVO_ENTRADA_CORROMPIDO);

    return NULL;
}

Sinais* carregaEntradas(FILE* arquivo)
{
    int index; // for indexing the array in the input signal list
    Token* it; // token iterator
    ValorLogico logic_value;
    Tempo time_length;

    Sinais* list_input = new_signal_list();

    // token list to keep the names of read input identifiers
    ListaToken* list_used_identifiers = novaListaToken();

    ListaToken* tokens = tokeniza(arquivo);

    if (!tokens)
        return NULL;

    it = tokens->primeiro;

    if (!it) {
        print("%s", MSG_ARQUIVO_ENTRADA_VAZIO);

        return NULL;
    }

    index = -1;

    // loop to read the set of all signal in the file
    while (1)
    {
        if ( isSimbolo(it->valor[0]) )
            return erroFatalArquivoCorrompido();

        // TODO: check signal name duplication errors

        insereTokenString(list_used_identifiers,
                          it->valor,
                          it->linha,
                          it->coluna);

        add_new_signal(list_input, it->valor);

        index++;

        if ( !avanca(&it) )
            return erroFatalArquivoCorrompido();

        if ( !iguais(it->valor, "{") )
            return erroFatalArquivoCorrompido();

        // loop to read one signal
        while (1)
        {
            if ( !avanca(&it) )
                return erroFatalArquivoCorrompido();

            logic_value = VAL_BLANK;

            if ( iguais(it->valor, "0") ) {
                logic_value = VAL_0;
            }
            else if ( iguais(it->valor, "1") ) {
                logic_value = VAL_1;
            }
            else if ( iguais(it->valor, "x") || iguais(it->valor, "X") ) {
                logic_value = VAL_X;
            }
            else if ( iguais(it->valor, "z") || iguais(it->valor, "Z") ) {
                logic_value = VAL_Z;
            }
            else if ( iguais(it->valor, "}") ) {
                break;
            }
            else {
                return erroFatalArquivoCorrompido();
            }

            if ( !avanca(&it) )
                return erroFatalArquivoCorrompido();

            if ( !iguais(it->valor, "(") )
                return erroFatalArquivoCorrompido();

            if ( !avanca(&it) )
                return erroFatalArquivoCorrompido();

            if ( !isNumNaturalValido(it->valor) )
                return erroFatalArquivoCorrompido();
            
            time_length = strtol(it->valor, NULL, 10);

            add_new_pulse( (list_input->lista + index),
                           logic_value,
                           time_length );

            if ( !avanca(&it) )
                return erroFatalArquivoCorrompido();

            if ( !iguais(it->valor, ")") )
                return erroFatalArquivoCorrompido();

            if ( !avanca(&it) )
                return erroFatalArquivoCorrompido();

            if ( iguais(it->valor, ",") ) {
                continue;
            }
            else if ( iguais(it->valor, "}") ) {
                break;
            }             
        }

        if ( !avanca(&it) ) {
            print("%s", MSG_ARQUIVO_ENTRADA_LIDO_COMPLETO);

            break;
        }
    }

    delete_lista_token(list_used_identifiers);
    delete_lista_token(tokens);

    return list_input;
}

void salvarSinais(Sinais* sinaisSaida, FILE* arqSaida)
{
    int si; // indexador dos sinais na lista de sinais de entrada
    Sinal* it_signal = NULL; // Iterador para os sinais num conjunto de entrada ou saida
    Pulso* it = NULL; // Iterador para os pulsos em um Sinal

    if ( !sinaisSaida || !arqSaida ) {
        return;
    }

    si = 0;
    it_signal = sinaisSaida->lista;

    while ( si < sinaisSaida->quantidade )
    {
        fprintf(arqSaida, "%s {", it_signal[si].nome);

        it = it_signal[si].pulsos; // Aqui, o indice 0 indica qual dos sinais na lista

        while ( it->valor != VAL_BLANK )
        {
            // Insere virgula apenas se nao eh a primeira iteracao
            if ( it != it_signal[si].pulsos )
                fprintf(arqSaida, ", ");

            switch (it->valor)
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
