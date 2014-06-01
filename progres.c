
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "estruturas.h"

t_circuito* novoCircuito()
{
    return (t_circuito*) malloc(sizeof(t_circuito));
}

t_componente* novaListaCompon(int tamanho)
{
    return (t_componente*) malloc(sizeof(t_componente) * tamanho);
}

t_componente novoComponente()
{
    return (t_componente) malloc(sizeof(struct st_componente));
}

int ehEspaco(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

void copiaSimbolo(char c){
}

void copiaTok(char* tok){
}

int ehSimbolo(char c)
{
    return (c == '(' || c == ')' || c == ',' || c == ';');
}

t_circuito* carregaCircuito(FILE *arquivo)
{
    t_circuito *circuto = novoCircuito();
    t_circuito *retorno = NULL;

    char c = '\0';
    int erro = 0;
    //char* tok;

    while(1)
    {
        // A
        c = fgetc(arquivo);

        if(c == EOF) printf("Arquivo vazio.\n");

        if(feof(arquivo))
            break;

        if(isspace(c))
            continue;

        if(c == '/')
        {
            c = fgetc(arquivo);

            if(c == '/')
            {
                while(c != '\n')
                {
                    c = fgetc(arquivo);
                }

                continue; // volta pra A
            }
        }
        else
        {
            // B
            // ...
            if(ehSimbolo())
            {
                //copiaSimbolo(c);
                continue;
            }
            else
            {
                if(isalnum(c)){
                  //tok += c;
                  while(1)
                  {
                    c = fgetc(arquivo);
                    if(isspace(c))
                    {
                        //copiaTok(c);
                        continue;
                    }
                    else
                    {
                        if(ehSimbolo(c))
                        {
                            //copiaTok(c);
                            copiaSimbolo(c);
                            continue;
                        }
                        else
                        {
                            if(isalnum(c))
                            {
                                //tok += c
                            }
                            else
                            {
                                printf("Erro: Caracter nao pertimitido.");
                                //variavel local recebera indicativo de erro
                                erro = 1
                                break;
                            }
                        }
                    }
                  }
                }
                else
                {
                    printf("Erro: Caracter nao pertimitido.");
                    break;
                }
            }
        }
        if(erro == 1)
            break;
        // ...
    }

    return retorno;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Uso: progres [nome de arquivo verilog]\n");
        exit(0);
    }

	FILE *arquivo = fopen(argv[1], "r");

    if(!arquivo)
	{
		printf("Impossibilitado de abrir o arquivo: %s\n", argv[1]);
		exit(1);
    }

    t_circuito *circuto1 = carregaCircuito(arquivo);

    if(circuto1)
    {
        printf("Circuito carregado com sucesso.\n");
    }
    else
    {
        printf("Erro com o fonte.\n");
    }

    fclose(arquivo);

    return 0;
}
