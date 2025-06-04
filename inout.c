/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "progres.h"
#include "estruturas.h"
#include "sinais.h"
#include "eventos.h"
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
                case VAL_BLANK:
                    break;
            }

            it++;
        }

        fprintf(arqSaida, "}\n");

        si++;
    }
}

void save_vcd(Module* module, Sinais* sinais, FILE* file)
{
    int i;
    char s = '%';

    time_t epoch = time(NULL);
    struct tm* dt = localtime(&epoch);
    fprintf(file, "$date\n");
    fprintf(file, "  %04d/%02d/%02d", (dt->tm_year + 1900), (dt->tm_mon + 1), dt->tm_mday);
    fprintf(file, " - %02d:%02d\n", dt->tm_hour, dt->tm_min);
    fprintf(file, "$end\n");

    fprintf(file, "$version\n");
    fprintf(file, "  progres %s\n", _PROGRES_VERSION);
    fprintf(file, "$end\n");
    fprintf(file, "$comment\n");
    fprintf(file, "  \n"); // ?
    fprintf(file, "$end\n");

    fprintf(file, "$timescale ");
    fprintf(file, "%llu", module->timescale_number);
    fprintf(file, "%s ", get_str_from_timeunit(module->timescale_unit));
    fprintf(file, "$end\n");

    fprintf(file, "$scope module %s ", module->name);
    fprintf(file, "$end\n");

    for ( i=0; i < sinais->quantidade; i++ )
    {
        fprintf(file, "$var %s 1 ", "wire"); // TODO: number of bits
        fprintf(file, "%c ", (s + i));
        fprintf(file, "%s $end\n", sinais->lista[i].nome);
    }

    fprintf(file, "$upscope $end\n");
    fprintf(file, "$enddefinitions $end\n");

    fprintf(file, "$dumpvars\n");
    for ( i=0; i < sinais->quantidade; i++ )
    {
        fprintf(file, "%c", get_char_from_logic_value(sinais->lista[i].pulsos->valor));
        fprintf(file, "%c\n", (s + i));
    }
    fprintf(file, "$end\n");

    Evento* fila = NULL;
    Tempo t = 0;
    
    Transicao* list_tran = NULL;
    Transicao* it = NULL;
    Pulso* p;
    ValorLogico v;

    for ( i=0 ; i < sinais->quantidade ; i++ )
    {
        t = 0;
        p = sinais->lista[i].pulsos;

        while (p->valor != VAL_BLANK)
        {
            insereEvento(&fila,
                         t,
                         module->listaFiosSaida->itens[i], // !! caution!
                         p->valor);

            t += p->tempo * module->timescale_number /* * (circuto->timescale_unit/UN_FS) */;

            p++;
        }

        insereEvento(&fila,
                     t,
                     module->listaFiosSaida->itens[i], // !! caution!
                     VAL_X);
    }

    t = 0;

    while (fila)
    {
        t = fila->quando;

        list_tran = popEvento(&fila);
        it = list_tran;

        while (it)
        {
            it->fio->valorDinamico = it->novoValor;
            it = it->proximo;
        }

        // #time
        fprintf(file, "#%llu\n", t);

        for ( i=0 ; i < module->listaFiosSaida->tamanho ; i++ )
        {
            v = module->listaFiosSaida->itens[i]->valorDinamico;
            fprintf(file, "%c", get_char_from_logic_value(v));
            fprintf(file, "%c\n", (char) (s + i));
        }
    }

    // final line
    fprintf(file, "#%llu\n", (Tempo) (t+1));
}

char get_char_from_logic_value(ValorLogico value)
{
    switch (value)
    {
        case VAL_1:
            return '1';
        case VAL_0:
            return '0';
        case VAL_X:
            return 'x';
        case VAL_Z:
            return 'z';
        default:
            return 'x';
    }
}
