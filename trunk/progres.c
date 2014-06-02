/*
 Progres - Simulador de circuitos combinacionais em Verilog
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estruturas.h"
#include "lex.h"

int ehEspaco(char c) {
    return (c == ' ' || c == '\t' || c == '\n');
}

int isSimbolo(char c) {
    return (c == '(' || c == ')' || c == ',' || c == ';');
}

t_circuito* carregaCircuito(FILE *arquivo) {
    int linha = 1, coluna = 0;

    ListaToken* tokens = novaListaToken();

    t_circuito *circuito = novoCircuito();
    t_circuito *retorno = NULL;

    char c = '\0';
    int erro = 0, fim = 0;

    char* tok = (char*) malloc(sizeof(char));
    strcpy(tok, "");

    while(1) {
        // A
        strcpy(tok, "");
        c = fgetc(arquivo);

        if(c == EOF) {
            printf("Final de arquivo alcancado.\n");
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
            coluna++;
            c = fgetc(arquivo);

            if(c == '/') {
                coluna++;

                while(c != '\n') {
                    c = fgetc(arquivo);
                    coluna++;
                }

                coluna = 0;
                linha++;

                continue; // volta pra A
            }
            /*else if(c == '*') {
                coluna++;

                c = fgetc(arquivo);

                // TODO: Comentario de multiplas linhas
            }*/
            else {
                printf("Simbolo nao esperado na linha %d, coluna %d.\n", linha, coluna);
                break;
            }
        }
        else {
            // B
            if(isSimbolo(c)) {
                insereToken(tokens, c);
                continue;
            }
            else {
                if(isalnum(c)) {
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

                            insereTokenString(tokens, tok);

                            break; // TODO: Verificar se esta indo para A
                        }
                        else if(isSimbolo(c)) {
                            coluna++;
                            insereTokenString(tokens, tok);
                            insereToken(tokens, c);

                            break; // TODO: Verificar se esta indo para A
                        }
                        else if(isalnum(c)) {
                            coluna++;
                            anexa(tok, c);
                            continue;
                        }
                        else if(c == EOF) {
                            insereTokenString(tokens, tok);
                            fim = 1;
                            break;
                        }
                        else {
                            printf("Erro: Caractere nao permitido, linha %d, coluna %d.\n", linha, coluna);
                            erro = 1; //variavel local recebera indicativo de erro

                            break;
                        }
                    }
                }
                else {
                    printf("Erro: Caracter nao pertimitido, linha %d, coluna %d.\n", linha, coluna);
                    break;
                }
            }
        }

        if(erro == 1 || fim == 1)
            break;

    }

    printf(" -=-=- LISTA DE TOKENS CAPTURADOS -=-=-\n\n");
    Token* it = tokens->primeiro;
    while(it) {
        printf("%s\n", it->valor);
        it = it->seguinte;
    }

    return retorno;
}

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("Uso: progres [nome de arquivo verilog]\n");
        exit(0);
    }

    FILE *arquivo = fopen(argv[1], "r");

    if(!arquivo) {
        printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
        exit(1);
    }

    t_circuito *circuto1 = carregaCircuito(arquivo);

    if(circuto1) {
        printf("Circuito carregado com sucesso.\n");
    }
    else {
        printf("Erro com o fonte.\n");
    }

    fclose(arquivo);

    return 0;
}
