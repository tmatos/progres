/*
 Progres - Simulador de circuitos combinacionais em Verilog
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estruturas.h"
#include "lex.h"

ListaToken* tokeniza(FILE *arquivo) {
    int linha = 1, coluna = 0;

    ListaToken* tokens = novaListaToken();

    ListaToken* retorno = NULL;

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
                            printf("Erro: Caractere nao permitido, linha %d, coluna %d.\n", linha, coluna);
                            erro = 1; //variavel local recebera indicativo de erro

                            break;
                        }
                    }
                }
                else {
                    printf("Erro: Caractere nao permitido, linha %d, coluna %d.\n", linha, coluna);
                    break;
                }
            }
        }

        if(erro || fim)
            break;

    }

    exibeListaDeToken(tokens);

    retorno = tokens;

    return retorno;
}

t_circuito* carregaCircuito(FILE *arquivo)
{
    t_circuito *circuito = novoCircuito();

    ListaToken* tokens = tokeniza(arquivo);
    ListaToken* identificadores = novaListaToken();

    if(!tokens)
        return NULL;

    Token* it = tokens->primeiro;

    if(!it)
        return NULL;

    if(!iguais(it->valor, "module"))
    {
        printf("Erro na linha %d, coluna %d. Esperava a palavra-chave '%s', no lugar esta '%s'.\n",
               it->linha, it->coluna, "module", it->valor);

        return NULL;
    }

    avanca(&it);

    if(!it) {
        printf("Final do arquivo não esperado. Era esperado um identificador.\n");
        return NULL;
    }
    else if(!isIdentificador(it)) {
        printf("Erro na linha %d, coluna %d. Esperava-se um identificador valido, no lugar esta '%s'.\n",
               it->linha, it->coluna, it->valor);

        return NULL;
    }
    else {
        // senao, adicione-o a lista de identificadores
        insereTokenString(identificadores, it->valor, -1, -1);
    }

    avanca(&it);

    if(!it) {
        printf("Final do arquivo não esperado. Era esperado '('.\n");
        return NULL;
    }
    else if(!iguais(it->valor, "(")) {
        // se it->valor não é '(', pare
        printf("Erro na linha %d, coluna %d. Esperava-se '(', no lugar esta '%s'.\n",
               it->linha, it->coluna, it->valor);

        return NULL;
    }

    avanca(&it);

    // devemos agora ler os parametros do modulo

    int virgula = 0; // um flag para indicar se estamos esperando por uma virgula

    while(1) {
        if(!it) {
            if(virgula)
                printf("Final do arquivo não esperado. Era esperada uma virgula.\n");
            else
                printf("Final do arquivo não esperado. Era esperado um identificador valido ou ')'.\n");

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
                printf("Erro na linha %d, coluna %d. Esperava-se uma virgula ou ')', no lugar esta '%s'.\n",
                      it->linha, it->coluna, it->valor);
                return NULL;
            }
        }

        if(isIdentificador(it)) {
            if(identExiste(identificadores, it->valor)) {
                printf("Erro na linha %d, coluna %d. O identificador '%s' ja estava sendo utilizado.\n",
                      it->linha, it->coluna, it->valor);
                return NULL;
            }
            else {
                insereTokenString(identificadores, it->valor, -1, -1);
                virgula = 1;
            }
        }
        else {
            printf("Erro na linha %d, coluna %d. Esperava-se um identificador, no lugar esta '%s'.\n",
                  it->linha, it->coluna, it->valor);
            return NULL;
        }

        avanca(&it);
    }

    avanca(&it);

    if(!it) {
        printf("Final do arquivo não esperado. Era esperado ';'.\n");
        return NULL;
    }

    if(!iguais(it->valor, ";")) {
        printf("Erro na linha %d, coluna %d. Esperava-se ';', no lugar esta '%s'.\n",
              it->linha, it->coluna, it->valor);
        return NULL;
    }

    avanca(&it);

    if(!it) {
        printf("Final do arquivo não esperado. Era esperado ';'.\n");
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
