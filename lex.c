/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

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

#define NUM_RESERV_KEYWORDS 108

char arrayPalavrasReservadas[][NUM_RESERV_KEYWORDS] = {
    "always",
    "and",
    "assign",
    "attribute",
    "begin",
    "buf",
    "bufif0",
    "bufif1",
    "case",
    "casex",
    "casez",
    "cmos",
    "deassign",
    "default",
    "defparam",
    "disable",
    "edge",
    "else",
    "end",
    "endattribute",
    "endcase",
    "endfunction",
    "endmodule",
    "endprimitive",
    "endspecify",
    "endtable",
    "endtask",
    "event",
    "for",
    "force",
    "forever",
    "fork",
    "function",
    "highz0",
    "highz1",
    "if",
    "ifnone",
    "initial",
    "inout",
    "input",
    "integer",
    "join",
    "medium",
    "module",
    "large",
    "localparam",
    "macromodule",
    "nand",
    "negedge",
    "nmos",
    "nor",
    "not",
    "notif0",
    "notif1",
    "or",
    "output",
    "parameter",
    "pmos",
    "posedge",
    "primitive",
    "pull0",
    "pull1",
    "pulldown",
    "pullup",
    "rcmos",
    "real",
    "realtime",
    "reg",
    "release",
    "repeat",
    "rnmos",
    "rpmos",
    "rtran",
    "rtranif0",
    "rtranif1",
    "scalared",
    "signed",
    "small",
    "specify",
    "specparam",
    "strength",
    "strong0",
    "strong1",
    "supply0",
    "supply1",
    "table",
    "task",
    "time",
    "tran",
    "tranif0",
    "tranif1",
    "tri",
    "tri0",
    "tri1",
    "triand",
    "trior",
    "trireg",
    "unsigned",
    "vectored",
    "wait",
    "wand",
    "weak0",
    "weak1",
    "while",
    "wire",
    "wor",
    "xnor",
    "xor",
};

ListaToken* novaListaToken()
{
    ListaToken* l = (ListaToken*) xmalloc(sizeof(ListaToken));

    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;

    return l;
}

void delete_lista_token(ListaToken* list)
{
    if(!list)
        return;

    if(list->tamanho == 0) {
        free(list);
        return;
    }

    Token* an;
    Token* it = list->primeiro;

    while(it) {
        an = it;
        it = it->seguinte;
        free(an);
    }

    free(list);
}

int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna)
{
    char s[2] = {tok, '\0'};

    return insereTokenString(lista, s, p_linha, p_coluna);
}

int insereTokenString(ListaToken* lista, const char* tok, int p_linha, int p_coluna)
{
    Token* newtok = (Token*) xmalloc(sizeof(Token));

    copy(newtok->valor, tok);
    newtok->linha = p_linha;
    newtok->coluna = p_coluna;
    newtok->anterior = NULL;
    newtok->seguinte = NULL;

    TokenClass tc = _UNKNOWN;

    if( iguais(tok, "and") )
        tc = KW_AND;
    else if( iguais(tok, "or") )
        tc = KW_OR;
    else if( iguais(tok, "not") )
        tc = KW_NOT;
    else if( iguais(tok, "buf") )
        tc = KW_BUF;
    else if( iguais(tok, "nand") )
        tc = KW_NAND;
    else if( iguais(tok, "nor") )
        tc = KW_NOR;
    else if( iguais(tok, "xor") )
        tc = KW_XOR;
    else if( iguais(tok, "xnor") )
        tc = KW_XNOR;
    else if( iguais(tok, "input") )
        tc = KW_INPUT;
    else if( iguais(tok, "output") )
        tc = KW_OUTPUT;
    else if( iguais(tok, "wire") )
        tc = KW_WIRE;
    else if( iguais(tok, "reg") )
        tc = KW_REG;
    else if( iguais(tok, "signed") )
        tc = KW_SIGNED;
    else if( iguais(tok, "module") )
        tc = KW_MODULE;
    else if( iguais(tok, "endmodule") )
        tc = KW_ENDMODULE;
    else if( iguais(tok, "initial") )
        tc = KW_INITIAL;
    else if( iguais(tok, "begin") )
        tc = KW_BEGIN;
    else if( iguais(tok, "end") )
        tc = KW_END;
    else if( iguais(tok, "parameter") )
        tc = KW_PARAMETER;
    else if( iguais(tok, "defparam") )
        tc = KW_DEFPARAM;
    else if( iguais(tok, "localparam") )
        tc = KW_LOCALPARAM;
    else if( iguais(tok, "=") )
        tc = SYM_EQ;
    else if( iguais(tok, ",") )
        tc = SYM_COMMA;
    else if( iguais(tok, ":") )
        tc = SYM_COLON;
    else if( iguais(tok, ";") )
        tc = SYM_SEMICOLON;
    else if( iguais(tok, "(") )
        tc = SYM_OPEN_BRACKET;
    else if( iguais(tok, ")") )
        tc = SYM_CLOSE_BRACKET;
    else if( iguais(tok, "[") )
        tc = SYM_OPEN_SQUAREBRACKET;
    else if( iguais(tok, "]") )
        tc = SYM_CLOSE_SQUAREBRACKET;
    else if( iguais(tok, "{") )
        tc = SYM_OPEN_BRACE;
    else if( iguais(tok, "}") )
        tc = SYM_CLOSE_BRACE;
    else if( iguais(tok, "#") )
        tc = SYM_HASHTAG;
    else if( iguais(tok, "+") )
        tc = SYM_PLUS;
    else if( iguais(tok, "-") )
        tc = SYM_MINUS;
    else if( iguais(tok, "*") )
        tc = SYM_ASTERISK;
    else if( iguais(tok, "/") )
        tc = SYM_SLASH;
    else if( iguais(tok, "%") )
        tc = SYM_PERCENT;
    else if( iguais(tok, "~") )
        tc = SYM_TILDE;
    else if( iguais(tok, "&") )
        tc = SYM_AMPERSAND;
    else if( iguais(tok, "|") )
        tc = SYM_PIPE;
    else if( iguais(tok, "^") )
        tc = SYM_CIRCUMFLEX;

    // TODO: Preencher classe do token para todas elas, nao apenas estas acima

    newtok->classe = tc;

    // TODO: Checagens...

    if(lista->tamanho == 0) {
        lista->primeiro = newtok;
        lista->ultimo = newtok;
    }
    else {
        lista->ultimo->seguinte = newtok;
        newtok->anterior = lista->ultimo;
        lista->ultimo = newtok;
    }

    lista->tamanho++;

    return 1;
}

int removeTokensPorValor(ListaToken* lst, const char* tok)
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
            if(anterior) {
                if(it->seguinte) {
                    anterior->seguinte = it->seguinte;
                    it->seguinte->anterior = anterior;
                    tmp = it;
                    avanca(&it);
                    free(tmp);
                    lst->tamanho--;
                    continue;
                }
                else { // caso em que removemos item no final da lista
                    tmp = it;
                    anterior->seguinte = NULL;
                    lst->ultimo = anterior;
                    lst->tamanho--;
                    free(tmp);
                    break;
                }
            }
            else {
                if(it->seguinte) { // primeiro da lista eh removido e ha outros itens
                    tmp = it;
                    lst->primeiro = it->seguinte;
                    avanca(&it);
                    it->anterior = NULL;
                    free(tmp);
                    lst->tamanho--;
                    continue;
                }
                else { // lista com um unico item e que sera removido
                    tmp = it;
                    lst->primeiro = NULL;
                    lst->ultimo = NULL;
                    lst->tamanho = 0;
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
    Token* it = NULL;

    printf(" - LISTA DE TOKENS CAPTURADOS -\n\n");

    it = tokens->primeiro;
    while(it) {
        printf("%s\n", it->valor);
        avanca(&it);
    }

    printf("\n");
}

int identExiste(ListaToken* lst, const char* str)
{
    Token* it = NULL;

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

int iguais(const char* a, const char* b)
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
    int i;

    if(!tk)
        return 0;

    for( i = 0 ; i < NUM_RESERV_KEYWORDS ; ++i )
    {
        if( iguais(tk->valor, arrayPalavrasReservadas[i]) )
            return 1;    
    }
    
    return 0;
}

int isIdentificador(Token* tk)
{
    int i;
    int simbol = 0;

    if(!tk)
        return 0;

    // se nao comeca com letra ou underscore, nao eh identificador valido
    if( !isalpha(tk->valor[0]) && (tk->valor[0] != '_') )
        return 0;

    for(i = 1 ; i < len(tk->valor) ; i++) {
        if( !isalnum(tk->valor[i]) && (tk->valor[i] != '_') ) {
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

ListaToken* tokeniza(FILE* arquivo)
{
    int linha = 1; // contador para linha corrente do arquivo
    int coluna = 0; // contador para coluna corrente (em determinada linha do arquivo)
    int erro = 0; // flag de erro, encerra a analise
    int fim = 0; // flag para indicar o termino da analise

    char c = '\0'; // usado para leitura de um caraceter
    char* tok; // usado para a leitura de uma string que representa um token

    ListaToken* tokens = novaListaToken();

    tok = (char*) xmalloc( sizeof(char) * MAX_TOKEN_SIZE );

    while(1) {
        A: // Label para parte A do automato

        copy(tok, "");
        c = fgetc(arquivo);

        if(c == EOF)
            goto encerrar;

        if(isspace(c)) {
            if(c == '\n') {
                coluna = 0;
                linha++;
            }
            else {
                coluna++;
            }

            goto A;
        }

        if(c == '/') {
            comentarios: // Label para inicio da parte que trata os comentarios

            coluna++;
            c = fgetc(arquivo);

            if(c == '/') {
                coluna++;

                while(c != '\n') {
                    c = fgetc(arquivo);
                    coluna++;

                    if(c == EOF)
                        goto encerrar;
                }

                coluna = 0;
                linha++;

                goto A;
            }
            else if(c == '*') {
                coluna++;

                c = fgetc(arquivo);

                while(1) {
                    M: // Label para a parte de comentario de multiplas linhas

                    if(c == EOF)
                        goto encerrar;

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
                            goto M;
                    }

                    c = fgetc(arquivo);
                }

                goto A;
            }
            else {
                show_error_lexical(MSG_ERROR_LEX_UNEXPECTED_SYMBOL, linha, coluna);
                break;
            }
        }
        else if(c == '"') {
            coluna++;

            anexa(tok, c);

            c = fgetc(arquivo);

            while(1) {
                // S: captura de strings literais
                if(c == EOF) {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    goto encerrar;
                }

                if(c == '\n') {
                    coluna = 0;
                    linha++;
                }
                else
                    coluna++;

                anexa(tok, c);

                if(c == '"') {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    break;
                }

                c = fgetc(arquivo);
            }

            goto A;
        }
        else {
            // B: a parte B do automato

            if(isSimbolo(c)) {
                coluna++;
                insereToken(tokens, c, linha, coluna);
                goto A;
            }
            else {
                if(isalnum(c) || c == '_') {
                    coluna++;
                    anexa(tok, c);

                    while(1) {
                        P: // Label para parte P do automato

                        c = fgetc(arquivo);

                        if(isspace(c)) {
                            if(c == '\n') {
                                coluna = 0;
                                linha++;
                            }
                            else {
                                coluna++;
                            }

                            insereTokenString(tokens, tok, linha, coluna - len(tok));

                            break;
                        }
                        else if(c == '/') {
                            insereTokenString(tokens, tok, linha, coluna - len(tok));
                            goto comentarios;
                        }
                        else if(isSimbolo(c)) {
                            insereTokenString(tokens, tok, linha, coluna - len(tok));
                            coluna++;
                            insereToken(tokens, c, linha, coluna);

                            break;
                        }
                        else if(isalnum(c) || c == '_') {
                            coluna++;
                            anexa(tok, c);

                            // verificar tamanho maximo de palavra
                            if( len(tok) > MAX_TOKEN_SIZE ) {
                                show_error_lexical(MSG_ERROR_LEX_TOKEN_SIZE_MAXED,
                                                   linha,
                                                   coluna - len(tok) );
                                goto encerrar;
                            }

                            goto P;
                        }
                        else if(c == EOF) {
                            insereTokenString(tokens, tok, linha, coluna - len(tok));
                            goto encerrar;
                        }
                        else {
                            show_error_lexical(MSG_ERROR_LEX_INVALID_CHAR, linha, coluna);
                            goto encerrar;
                        }
                    }
                }
                else {
                    show_error_lexical(MSG_ERROR_LEX_INVALID_CHAR, linha, coluna);
                    break;
                }
            }
        }

        if(erro || fim)
        {
            encerrar: // Label para o encerramento

            break;
        }
    }

    //exibeListaDeToken(tokens);

    free(tok);

    return tokens;
}

int apenasDigitos(const char* str)
{
    int i;

    if(!str)
        return 0;

    for( i=0 ; i < len(str) ; i++ )
    {
        if( !isdigit(str[i]) )
            return 0;
    }

    return 1;
}

int isNumNaturalValido(const char* str)
{
    if(!str)
        return 0;

    // importante nao ser um valor muito grande, esses numeros
    if( !apenasDigitos(str) || !(len(str) <= MAX_DIGITOS_NUM) ) {
        return 0;
    }

    return 1;
}

size_t len(const char* str)
{
    // TODO: restrict to a maximum length
    return strlen(str);
}

char* copy(char* dest, const char* src)
{
    // TODO: restrict to a maximum length
    return strcpy(dest, src);
}
