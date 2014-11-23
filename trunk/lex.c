/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, Tiago Matos, Joao Victor, Luciano Almeida

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "progres.h"
#include "erros.h"
#include "lex.h"
#include "mem.h"

ListaToken* novaListaToken()
{
    ListaToken* l = (ListaToken*) xmalloc(sizeof(ListaToken));

    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;

    return l;
}

int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna)
{
    char s[2] = {tok, '\0'};

    return insereTokenString(lista, s, p_linha, p_coluna);
}

int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna)
{
    Token* novo = (Token*) xmalloc(sizeof(Token));

    strcpy(novo->valor, tok);
    novo->linha = p_linha;
    novo->coluna = p_coluna;
    novo->seguinte = NULL;

    // TODO: Preencher o tipo
    // TODO: Checagens...

    if(lista->tamanho == 0) {
        lista->primeiro = novo;
        lista->ultimo = novo;
    }
    else if(lista->tamanho == 1) {
        lista->primeiro->seguinte = novo;
        lista->ultimo = novo;
    }
    else {
        lista->ultimo->seguinte = novo;
        lista->ultimo = novo;
    }

    lista->tamanho++;

    return 1;
}

int removeTokensPorValor(ListaToken* lst, char* tok)
{
    Token *tmp = NULL;
    Token *anterior = NULL;
    Token *it = NULL;

    if(!lst || !tok)
        return 0;

    if(!lst->primeiro)
        return 1;

    it = lst->primeiro;
    while(it)
    {
        if(iguais(it->valor, tok))
        {
            if(anterior)
            {
                if(it->seguinte) {
                    anterior->seguinte = it->seguinte;
                    tmp = it;
                    anterior = it;
                    avanca(&it);
                    free(tmp);
                    lst->tamanho--;
                    continue;
                }
                else { // caso em que removemos do final da lista
                    tmp = it;
                    anterior->seguinte = NULL;
                    lst->ultimo = anterior;
                    lst->tamanho--;
                    free(tmp);
                    break;
                }
            }
            else
            {
                if(it->seguinte) {
                    tmp = it;
                    lst->primeiro = it->seguinte;
                    avanca(&it);
                    free(tmp);
                    lst->tamanho--;
                    continue;
                }
                else { // lista com um unico item e que será removido
                    tmp = it;
                    lst->primeiro = NULL;
                    lst->ultimo = NULL;
                    lst->tamanho--;
                    free(tmp);
                    break;
                }
            }
        }

        anterior = it;
        avanca(&it);
    }

    return 1;
}

int anexa(char* str, char c)
{
    char tmp[2] = {c, '\0'};

    strcat(str, tmp);

    // TODO: Checagens...

    return 1;
}

int isSimbolo(char c)
{
    return (c == '(' ||
            c == ')' ||
            c == ',' ||
            c == ';' ||
            c == ':' ||
            c == '{' ||
            c == '}' ||
            c == '[' ||
            c == ']' ||
            c == '?' ||
            c == '=' ||
            c == '<' ||
            c == '>' ||
            c == '~' ||
            c == '&' ||
            c == '|' ||
            c == '!' ||
            c == '+' ||
            c == '-' ||
            c == '*' ||
            c == '/' ||
            c == '#' ||
            c == '@' ||
            c == '$' ||
            c == '"' ||
            c == '\'');
}

void exibeListaDeToken(ListaToken* tokens)
{
    Token *it = NULL;

    printf(" - LISTA DE TOKENS CAPTURADOS -\n\n");

    it = tokens->primeiro;
    while(it) {
        printf("%s\n", it->valor);
        avanca(&it);
    }

    printf("\n");
}

int identExiste(ListaToken* lst, char* str)
{
    Token *it = NULL;

    int retorno = 0;

    if(!lst || !str)
        return retorno;

    if(!lst->primeiro)
        return retorno;

    it = lst->primeiro;
    while(it)
    {
        if(iguais(it->valor, str)) {
            retorno = 1;
            break;
        }

        avanca(&it);
    }

    return retorno;
}

int iguais(char* a, char* b)
{
    return !strcmp(a, b);
}

void avanca(Token** t)
{
    if(*t)
        *t = (*t)->seguinte;
}

int isPalavra(Token* tk)
{
    if(!tk)
        return 0;

    if(iguais(tk->valor, "always")) return 1;
    if(iguais(tk->valor, "and")) return 1;
    if(iguais(tk->valor, "assign")) return 1;
    if(iguais(tk->valor, "attribute")) return 1;
    if(iguais(tk->valor, "begin")) return 1;
    if(iguais(tk->valor, "buf")) return 1;
    if(iguais(tk->valor, "bufif0")) return 1;
    if(iguais(tk->valor, "bufif1")) return 1;
    if(iguais(tk->valor, "case")) return 1;
    if(iguais(tk->valor, "casex")) return 1;
    if(iguais(tk->valor, "casez")) return 1;
    if(iguais(tk->valor, "cmos")) return 1;
    if(iguais(tk->valor, "deassign")) return 1;
    if(iguais(tk->valor, "default")) return 1;
    if(iguais(tk->valor, "defparam")) return 1;
    if(iguais(tk->valor, "disable")) return 1;
    if(iguais(tk->valor, "edge")) return 1;
    if(iguais(tk->valor, "else")) return 1;
    if(iguais(tk->valor, "end")) return 1;
    if(iguais(tk->valor, "endattribute")) return 1;
    if(iguais(tk->valor, "endcase")) return 1;
    if(iguais(tk->valor, "endfunction")) return 1;

    if(iguais(tk->valor, "endmodule")) return 1;
    if(iguais(tk->valor, "endprimitive")) return 1;
    if(iguais(tk->valor, "endspecify")) return 1;
    if(iguais(tk->valor, "endtable")) return 1;
    if(iguais(tk->valor, "endtask")) return 1;
    if(iguais(tk->valor, "event")) return 1;
    if(iguais(tk->valor, "for")) return 1;
    if(iguais(tk->valor, "force")) return 1;
    if(iguais(tk->valor, "forever")) return 1;
    if(iguais(tk->valor, "fork")) return 1;
    if(iguais(tk->valor, "function")) return 1;
    if(iguais(tk->valor, "highz0")) return 1;
    if(iguais(tk->valor, "highz1")) return 1;
    if(iguais(tk->valor, "if")) return 1;
    if(iguais(tk->valor, "ifnone")) return 1;
    if(iguais(tk->valor, "initial")) return 1;
    if(iguais(tk->valor, "inout")) return 1;
    if(iguais(tk->valor, "input")) return 1;
    if(iguais(tk->valor, "integer")) return 1;
    if(iguais(tk->valor, "join")) return 1;
    if(iguais(tk->valor, "medium")) return 1;
    if(iguais(tk->valor, "module")) return 1;

    if(iguais(tk->valor, "large")) return 1;
    if(iguais(tk->valor, "macromodule")) return 1;
    if(iguais(tk->valor, "nand")) return 1;
    if(iguais(tk->valor, "negedge")) return 1;
    if(iguais(tk->valor, "nmos")) return 1;
    if(iguais(tk->valor, "nor")) return 1;
    if(iguais(tk->valor, "not")) return 1;
    if(iguais(tk->valor, "notif0")) return 1;
    if(iguais(tk->valor, "notif1")) return 1;
    if(iguais(tk->valor, "or")) return 1;
    if(iguais(tk->valor, "output")) return 1;
    if(iguais(tk->valor, "parameter")) return 1;
    if(iguais(tk->valor, "pmos")) return 1;
    if(iguais(tk->valor, "posedge")) return 1;
    if(iguais(tk->valor, "primitive")) return 1;
    if(iguais(tk->valor, "pull0")) return 1;
    if(iguais(tk->valor, "pull1")) return 1;
    if(iguais(tk->valor, "pulldown")) return 1;
    if(iguais(tk->valor, "pullup")) return 1;
    if(iguais(tk->valor, "rcmos")) return 1;
    if(iguais(tk->valor, "real")) return 1;
    if(iguais(tk->valor, "realtime")) return 1;

    if(iguais(tk->valor, "reg")) return 1;
    if(iguais(tk->valor, "release")) return 1;
    if(iguais(tk->valor, "repeat")) return 1;
    if(iguais(tk->valor, "rnmos")) return 1;
    if(iguais(tk->valor, "rpmos")) return 1;
    if(iguais(tk->valor, "rtran")) return 1;
    if(iguais(tk->valor, "rtranif0")) return 1;
    if(iguais(tk->valor, "rtranif1")) return 1;
    if(iguais(tk->valor, "scalared")) return 1;
    if(iguais(tk->valor, "signed")) return 1;
    if(iguais(tk->valor, "small")) return 1;
    if(iguais(tk->valor, "specify")) return 1;
    if(iguais(tk->valor, "specparam")) return 1;
    if(iguais(tk->valor, "strength")) return 1;
    if(iguais(tk->valor, "strong0")) return 1;
    if(iguais(tk->valor, "strong1")) return 1;
    if(iguais(tk->valor, "supply0")) return 1;
    if(iguais(tk->valor, "supply1")) return 1;
    if(iguais(tk->valor, "table")) return 1;
    if(iguais(tk->valor, "task")) return 1;
    if(iguais(tk->valor, "time")) return 1;
    if(iguais(tk->valor, "tran")) return 1;

    if(iguais(tk->valor, "tranif0")) return 1;
    if(iguais(tk->valor, "tranif1")) return 1;
    if(iguais(tk->valor, "tri")) return 1;
    if(iguais(tk->valor, "tri0")) return 1;
    if(iguais(tk->valor, "tri1")) return 1;
    if(iguais(tk->valor, "triand")) return 1;
    if(iguais(tk->valor, "trior")) return 1;
    if(iguais(tk->valor, "trireg")) return 1;
    if(iguais(tk->valor, "unsigned")) return 1;
    if(iguais(tk->valor, "vectored")) return 1;
    if(iguais(tk->valor, "wait")) return 1;
    if(iguais(tk->valor, "wand")) return 1;
    if(iguais(tk->valor, "weak0")) return 1;
    if(iguais(tk->valor, "weak1")) return 1;
    if(iguais(tk->valor, "while")) return 1;
    if(iguais(tk->valor, "wire")) return 1;
    if(iguais(tk->valor, "wor")) return 1;
    if(iguais(tk->valor, "xnor")) return 1;
    if(iguais(tk->valor, "xor")) return 1;

    return 0;
}

int isIdentificador(Token* tk)
{
    int i;
    int simbol = 0;

    if(!tk)
        return 0;

    if(!isalpha(tk->valor[0])) // se nao comeca com letra, nao eh identificador valido
        return 0;

    for(i = 1 ; i < strlen(tk->valor) ; i++) {
        if(!isalnum(tk->valor[i])) {
            simbol = 1;
            break;
        }
    }

    if(simbol) // se contem algo a mais que letras ou numeros, nao eh identificador valido
        return 0;

    if(isPalavra(tk)) // palavra reservada nao pode ser identificador
        return 0;

    return 1;
}

ListaToken* tokeniza(FILE *arquivo)
{
    int linha = 1; // contador para linha corrente do arquivo
    int coluna = 0; // contador para coluna corrente (em determinada linha do arquivo)
    int erro = 0; // flag de erro, encerra a análise
    int fim = 0; // flag para indicar o término da análise

    char c = '\0'; // usado para leitura de um caraceter
    char *tok; // usado para a leitura de uma string que representa um token

    ListaToken *tokens = novaListaToken();

    tok = (char*) xmalloc( sizeof(char) * MAX_TOKEN_SIZE );

    while(1) {
        // A
        strcpy(tok, "");
        c = fgetc(arquivo);

        if(c == EOF) {
            //printf("Final de arquivo alcancado.\n");
            break;
        }

        if(isspace(c)) {
            if(c == '\n') {
                coluna = 0;
                linha++;
            }
            else {
                coluna++;
            }

            continue; // Volta para A
        }

        if(c == '/') {
            comentarios: // Label para inicio da parte que trata os comentários

            coluna++;
            c = fgetc(arquivo);

            if(c == '/') {
                coluna++;

                while(c != '\n') {
                    c = fgetc(arquivo);
                    coluna++;

                    if(c == EOF) {
                        fim = 1;
                        break;
                    }
                }

                coluna = 0;
                linha++;

                continue; // volta pra A
            }
            else if(c == '*') {
                coluna++;

                c = fgetc(arquivo);

                while(1) {
                    // M
                    if(c == EOF) {
                        fim = 1;
                        break;
                    }

                    if(c == '\n') {
                        coluna = 0;
                        linha++;
                    }
                    else
                        coluna++;

                    if(c == '*') {
                        c = fgetc(arquivo);

                        if(c == '/') {
                            coluna++;
                            break;
                        }
                        else
                            continue; // Volta para M
                    }

                    c = fgetc(arquivo);
                }

                if(!fim) {
                    continue; // Volta para A
                }
            }
            else {
                exibeMsgErro("Simbolo nao esperado", linha, coluna, NULL, NULL);
                break;
            }
        }
        else if(c == '"') {
            coluna++;

            anexa(tok, c);

            c = fgetc(arquivo);

            while(1) {
                // S
                if(c == EOF) {
                    insereTokenString(tokens, tok, linha, coluna - strlen(tok));
                    fim = 1;
                    break;
                }

                if(c == '\n') {
                    coluna = 0;
                    linha++;
                }
                else
                    coluna++;

                anexa(tok, c);

                if(c == '"') {
                    insereTokenString(tokens, tok, linha, coluna - strlen(tok));
                    break;
                }

                c = fgetc(arquivo);
            }

            continue; // Volta para A
        }
        else {
            // B
            if(isSimbolo(c)) {
                coluna++;
                insereToken(tokens, c, linha, coluna);
                continue; // Volta para A
            }
            else {
                if(isalnum(c)) {
                    coluna++;
                    anexa(tok, c);

                    while(1) {
                        // P
                        c = fgetc(arquivo);

                        if(isspace(c)) {
                            if(c == '\n') {
                                coluna = 0;
                                linha++;
                            }
                            else {
                                coluna++;
                            }

                            insereTokenString(tokens, tok, linha, coluna - strlen(tok));

                            break;
                        }
                        else if(c == '/') {
                            insereTokenString(tokens, tok, linha, coluna - strlen(tok));
                            goto comentarios;
                        }
                        else if(isSimbolo(c)) {
                            insereTokenString(tokens, tok, linha, coluna - strlen(tok));
                            coluna++;
                            insereToken(tokens, c, linha, coluna);

                            break;
                        }
                        else if(isalnum(c)) {
                            coluna++;
                            anexa(tok, c);

                            // verificar tamanho máximo de palavra
                            if( strlen(tok) > MAX_TOKEN_SIZE ) {
                                exibeMsgErro("Token excede o tamanho maximo permitido", linha, coluna - strlen(tok), NULL, NULL);
                                erro = 1;

                                break;
                            }

                            continue; // Vai para P
                        }
                        else if(c == EOF) {
                            insereTokenString(tokens, tok, linha, coluna - strlen(tok));
                            fim = 1;
                            break;
                        }
                        else {
                            exibeMsgErro("Caractere nao permitido", linha, coluna, NULL, NULL);
                            erro = 1; //variavel local recebera indicativo de erro

                            break;
                        }
                    }
                }
                else {
                    exibeMsgErro("Caractere nao permitido", linha, coluna, NULL, NULL);
                    break;
                }
            }
        }

        if(erro || fim)
            break;

    }

    //exibeListaDeToken(tokens);

    free(tok);

    return tokens;
}

int apenasDigitos(char* str)
{
    int i;
    int retorno = 1;

    if(!str)
        return 0;

    for( i=0 ; i < strlen(str) ; i++ )
    {
        if( !isdigit(str[i]) )
        {
            retorno = 0;
            break;
        }
    }

    return retorno;
}

int isNumNaturalValido(char* str)
{
    if(!str)
        return 0;

    // importante não ser um valor muito grande, esses numeros
    if( !apenasDigitos(str) || !(strlen(str) <= MAX_DIGITOS_NUM) )
    {
        return 0;
    }

    return 1;
}
