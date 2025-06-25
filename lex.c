/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "progres.h"
#include "erros.h"
#include "strutil.h"
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
    l->file[0] = '\0';

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
    char s[2];
    s[0] = tok;
    s[1] = '\0';

    return insereTokenString(lista, s, p_linha, p_coluna);
}

TokenClass get_token_class(const char* s_tok)
{
    int i;

    // IMPORTANT: keep track of the count here!
    #define _QTD_CLASSES 48
    #define _MAX_STRLEN_IN_ARRAY 13

    char from_str[_QTD_CLASSES][_MAX_STRLEN_IN_ARRAY] = {
        "and",  // 1
        "or",   // 2
        "not",  // 3
        "buf",
        "nand",
        "nor",
        "xor",
        "xnor",
        "bufif0",
        "bufif1",
        "notif0",
        "notif1",
        "input",
        "output",
        "wire",
        "reg",
        "signed",
        "module",
        "endmodule",
        "initial",
        "begin",
        "end",
        "parameter",
        "defparam",
        "localparam",
        "assign",
        "=",
        ",",
        ":",
        ";",     // = 30
        "(",
        ")",
        "[",
        "]",
        "{",     // = 35
        "}",
        "#",
        "+",
        "-",
        "*",     // = 40
        "/",
        "\%",
        "~",
        "&",
        "|",     // = 45
        "^",
        "$",
        "\x60"   // = _QTD_CLASSES
    };

    TokenClass to_class[_QTD_CLASSES] = {
        KW_AND,  // 1
        KW_OR,   // 2
        KW_NOT,  // 3
        KW_BUF,  // 4
        KW_NAND, // 5
        KW_NOR,
        KW_XOR,
        KW_XNOR,
        KW_BUFIF0,
        KW_BUFIF1,
        KW_NOTIF0,
        KW_NOTIF1,
        KW_INPUT,
        KW_OUTPUT,
        KW_WIRE,
        KW_REG,
        KW_SIGNED,
        KW_MODULE,
        KW_ENDMODULE,
        KW_INITIAL,
        KW_BEGIN,
        KW_END,
        KW_PARAMETER,
        KW_DEFPARAM,
        KW_LOCALPARAM,
        KW_ASSIGN,
        SYM_EQ,
        SYM_COMMA,
        SYM_COLON,
        SYM_SEMICOLON,
        SYM_OPEN_BRACKET,
        SYM_CLOSE_BRACKET,
        SYM_OPEN_SQUAREBRACKET,
        SYM_CLOSE_SQUAREBRACKET,
        SYM_OPEN_BRACE,
        SYM_CLOSE_BRACE,
        SYM_HASHTAG,
        SYM_PLUS,
        SYM_MINUS,
        SYM_ASTERISK,     // = 40
        SYM_SLASH,
        SYM_PERCENT,
        SYM_TILDE,
        SYM_AMPERSAND,
        SYM_PIPE,         // = 45
        SYM_CIRCUMFLEX,
        SYM_DOLLAR,
        SYM_GRAVE_ACCENT  // = _QTD_CLASSES
    };

    for ( i = 0; i < _QTD_CLASSES; i++ )
    {
        if ( iguais(from_str[i], s_tok) ) {
            return to_class[i];
        }
    }

    if ( apenasDigitos(s_tok) ) {
        return NUM_BASE_DECIMAL;
    }
    
    // TODO: Preencher classe do token para todas elas, nao apenas estas acima

    return _UNKNOWN;
}

int insereTokenString(ListaToken* lista, const char* tok, int p_linha, int p_coluna)
{
    Token* newtok = (Token*) xmalloc(sizeof(Token));

    copy(newtok->valor, tok);
    newtok->linha = p_linha;
    newtok->coluna = p_coluna;
    newtok->anterior = NULL;
    newtok->seguinte = NULL;
    newtok->classe = get_token_class(tok);

    // TODO: Checagens...

    if (lista->tamanho == 0) {
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

void remove_token(ListaToken* list, Token* tok)
{
    Token* it = list->primeiro;
    
    while (it)
    {
        if (it == tok)
        {
            if (it->anterior) {
                if (it->seguinte) {
                    it->anterior->seguinte = it->seguinte;
                    it->seguinte->anterior = it->anterior;
                }
                else { // caso em que removemos item no final da lista
                    list->ultimo = it->anterior;
                    it->anterior->seguinte = NULL;
                }
            }
            else {
                if (it->seguinte) { // primeiro da lista eh removido e ha outros itens
                    list->primeiro = it->seguinte;
                    it->seguinte->anterior = NULL;
                }
                else { // lista com um unico item e que sera removido
                    list->primeiro = NULL;
                    list->ultimo = NULL;
                }
            }
            
            list->tamanho--;
            free(it);
            break;
        }

        avanca(&it);
    }
}

int removeTokensPorValor(ListaToken* lst, const char* tok)
{
    Token* tmp = NULL;
    Token* anterior = NULL;
    Token* it = NULL;

    if (!lst || !tok)
        return 0;

    it = lst->primeiro;
    while (it)
    {
        if (iguais(it->valor, tok))
        {
            if (anterior) {
                if (it->seguinte) {
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
                if (it->seguinte) { // primeiro da lista eh removido e ha outros itens
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
            c == '`' || // grave accent
            c == '"' ||
            c == '\'');
}

void exibeListaDeToken(ListaToken* tokens)
{
    Token* it = NULL;

    if (global_silent_mode)
        return;

    printf("-- LISTA DE TOKENS CAPTURADOS --\n\n");

    it = tokens->primeiro;
    while (it) {
        printf("%s\t\t\t\t\t%d\n", it->valor, it->classe);
        avanca(&it);
    }

    printf("\n");
}

int identExiste(ListaToken* lst, const char* str)
{
    Token* it = NULL;

    int retorno = 0;

    if (!lst || !str)
        return retorno;

    if (!lst->primeiro)
        return retorno;

    it = lst->primeiro;
    while (it)
    {
        if (iguais(it->valor, str)) {
            retorno = 1;
            break;
        }

        avanca(&it);
    }

    return retorno;
}

Token* avanca(Token** it)
{
    if (!it)
        return NULL;

    if (*it) {
        *it = (*it)->seguinte;
        return *it;
    }

    return NULL;
}

int isPalavra(Token* tk)
{
    int i;

    if (!tk)
        return 0;

    for (i = 0; i < NUM_RESERV_KEYWORDS; ++i)
    {
        if ( iguais(tk->valor, arrayPalavrasReservadas[i]) )
            return 1;    
    }
    
    return 0;
}

int isIdentificador(Token* tk)
{
    unsigned int i;
    int simbol = 0;

    if (!tk)
        return 0;

    // se nao comeca com letra ou underscore, nao eh identificador valido
    if ( !isalpha(tk->valor[0]) && (tk->valor[0] != '_') )
        return 0;

    for ( i = 1; i < len(tk->valor); ++i )
    {
        if ( !isalnum(tk->valor[i]) && (tk->valor[i] != '_') ) {
            simbol = 1;
            break;
        }
    }

    if (simbol) // se contem algo a mais que letras ou numeros, nao eh identificador valido
        return 0;

    if (isPalavra(tk)) // palavra reservada nao pode ser identificador
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

    while (1)
    {
        A: // Label para parte A do automato

        c = fgetc(arquivo);

        A_1: // parte A sem captura de novo char
        
        copy(tok, "");

        if (c == EOF)
            goto encerrar;

        if (isspace(c)) {
            if (c == '\n') {
                coluna = 0;
                linha++;
            }
            else {
                coluna++;
            }

            goto A;
        }

        if (c == '/') {
            comentarios: // Label para inicio da parte que trata os comentarios

            coluna++;
            c = fgetc(arquivo);

            // line comment
            if (c == '/') {
                coluna++;

                while (c != '\n')
                {
                    c = fgetc(arquivo);
                    coluna++;

                    if (c == EOF)
                        goto encerrar;
                }

                coluna = 0;
                linha++;

                goto A;
            }
            else if (c == '*') { // multi-line
                coluna++;

                c = fgetc(arquivo);

                while (1)
                {
                    M: // Label para a parte de comentario de multiplas linhas

                    if (c == EOF)
                        goto encerrar;

                    if (c == '\n') {
                        coluna = 0;
                        linha++;
                    }
                    else
                        coluna++;

                    if (c == '*') {
                        c = fgetc(arquivo);

                        if (c == '/') {
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
                // recognize SYM_SLASH
                insereToken(tokens, '/', linha, coluna);
                goto A_1;
            }
        }
        else if (c == '"') {
            coluna++;

            anexa(tok, c);

            c = fgetc(arquivo);

            while (1)
            {
                // S: captura de strings literais
                if (c == EOF) {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    goto encerrar;
                }

                if (c == '\n') {
                    coluna = 0;
                    linha++;
                }
                else
                    coluna++;

                anexa(tok, c);

                if (c == '"') {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    break;
                }

                c = fgetc(arquivo);
            }

            goto A;
        }

        // B: a parte B do automato

        if (isSimbolo(c)) {
            // TODO: capture symbols larger than 1 char
        symbols_capture:
            coluna++;
            insereToken(tokens, c, linha, coluna);
            goto A;
        }

        if ( !isalnum(c) && c != '_' ) {
            show_error_lexical(MSG_ERROR_LEX_INVALID_CHAR, linha, coluna);
            goto encerrar;;
        }

        if (isalnum(c) || c == '_') {
            coluna++;
            anexa(tok, c);

            while (1)
            {
                P: // Label para parte P do automato

                c = fgetc(arquivo);

                if (isspace(c)) {
                    if (c == '\n') {
                        coluna = 0;
                        linha++;
                    }
                    else {
                        coluna++;
                    }
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    break;
                }
                else if (c == '/') {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    goto comentarios;
                }
                else if (isSimbolo(c)) {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    goto symbols_capture;
                }
                else if(isalnum(c) || c == '_') {
                    coluna++;
                    anexa(tok, c);

                    // verificar tamanho maximo de palavra
                    if (len(tok) > MAX_TOKEN_SIZE) {
                        show_error_lexical(MSG_ERROR_LEX_TOKEN_SIZE_MAXED,
                                           linha,
                                           coluna - len(tok) );
                        goto encerrar;
                    }

                    goto P;
                }
                else if (c == EOF) {
                    insereTokenString(tokens, tok, linha, coluna - len(tok));
                    goto encerrar;
                }
                else {
                    show_error_lexical(MSG_ERROR_LEX_INVALID_CHAR, linha, coluna);
                    goto encerrar;
                }
            }
        }

        if (erro || fim) {
            encerrar: // Label para o encerramento
                break;
        }
    }

    //exibeListaDeToken(tokens);

    free(tok);

    return tokens;
}
