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

ListaToken* novaListaToken() {
    ListaToken* l = (ListaToken*) xmalloc(sizeof(ListaToken));

    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;

    return l;
}

int insereToken(ListaToken* lista, char tok, int p_linha, int p_coluna) {
    char s[2] = {tok, '\0'};

    return insereTokenString(lista, s, p_linha, p_coluna);
}

int insereTokenString(ListaToken* lista, char* tok, int p_linha, int p_coluna) {
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

int removeTokensPorValor(ListaToken* lst, char* tok) {
    if(!lst || !tok)
        return 0;

    if(!lst->primeiro)
        return 1;

    Token* tmp = NULL;

    Token* anterior = NULL;
    Token* it = lst->primeiro;
    while(it) {
        if(iguais(it->valor, tok)) {
            if(anterior) {
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
            else {
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

int anexa(char* str, char c) {
    char tmp[2] = {c, '\0'};

    strcat(str, tmp);

    // TODO: Checagens...

    return 1;
}

int isSimbolo(char c) {
    return (c == '(' || c == ')' || c == ',' || c == ';' || c == '{' || c == '}' || c == '#');
}

void exibeListaDeToken(ListaToken* tokens) {
    printf(" - LISTA DE TOKENS CAPTURADOS -\n\n");

    Token* it = tokens->primeiro;
    while(it) {
        printf("%s\n", it->valor);
        avanca(&it);
    }

    printf("\n");
}

int identExiste(ListaToken* lst, char* str) {
    int retorno = 0;

    if(!lst || !str)
        return retorno;

    if(!lst->primeiro)
        return retorno;

    Token* it = lst->primeiro;
    while(it) {
        if(iguais(it->valor, str)) {
            retorno = 1;
            break;
        }

        avanca(&it);
    }

    return retorno;
}

int iguais(char* a, char* b) {
    return !strcmp(a, b);
}

void avanca(Token** t) {
    if(*t)
        *t = (*t)->seguinte;
}

int isPalavra(Token* tk) {
    if(!tk)
        return 0;

    // em primeiro lugar, as portas
    if(iguais(tk->valor, "nand")) return 1;
    if(iguais(tk->valor, "nor")) return 1;
    if(iguais(tk->valor, "and")) return 1;
    if(iguais(tk->valor, "or")) return 1;
    if(iguais(tk->valor, "xor")) return 1;
    if(iguais(tk->valor, "xnor")) return 1;
    if(iguais(tk->valor, "not")) return 1;
    if(iguais(tk->valor, "buf")) return 1;

    if(iguais(tk->valor, "module")) return 1;
    if(iguais(tk->valor, "parameter")) return 1;
    if(iguais(tk->valor, "localparam")) return 1;
    if(iguais(tk->valor, "input")) return 1;
    if(iguais(tk->valor, "output")) return 1;
    if(iguais(tk->valor, "reg")) return 1;
    if(iguais(tk->valor, "integer")) return 1;
    if(iguais(tk->valor, "wire")) return 1;
    if(iguais(tk->valor, "always")) return 1;
    if(iguais(tk->valor, "assign")) return 1;
    if(iguais(tk->valor, "while")) return 1;
    if(iguais(tk->valor, "begin")) return 1;
    if(iguais(tk->valor, "if")) return 1;
    if(iguais(tk->valor, "else")) return 1;
    if(iguais(tk->valor, "end")) return 1;
    if(iguais(tk->valor, "case")) return 1;
    if(iguais(tk->valor, "endcase")) return 1;
    if(iguais(tk->valor, "endmodule")) return 1;

    return 0;
}

int isIdentificador(Token* tk) {
    int i = 0;
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

ListaToken* tokeniza(FILE *arquivo) {
    int linha = 1, coluna = 0;

    ListaToken* tokens = novaListaToken();

    char c = '\0';
    int erro = 0, fim = 0;

    char* tok = (char*) xmalloc( sizeof(char) * MAX_TOKEN_SIZE );

    strcpy(tok, "");

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
        else {
            // B
            if(isSimbolo(c)) {
                coluna++;
                insereToken(tokens, c, linha, coluna);
                continue;
            }
            else {
                if(isalnum(c)) {
                    coluna++;
                    anexa(tok, c);

                    while(1) {
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
                            // TODO: verificar tamanho maximo de palavra
                            continue;
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

int apenasDigitos(char* str) {
    int i, ret = 1;

    if(!str)
        return 0;

    for( i=0 ; i < strlen(str) ; i++ ) {
        if( !isdigit(str[i]) ) {
            ret = 0;
            break;
        }
    }

    return ret;
}

int isNumNaturalValido(char* str) {
    if(!str)
        return 0;

    if( !apenasDigitos(str) || !(strlen(str) <= MAX_DIGITOS_NUM) ) // importante não ser um valor muito grande, esses numeros
        return 0;

    return 1;
}
