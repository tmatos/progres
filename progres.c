
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

t_circuito* carregaCircuito(FILE *arquivo)
{
    t_circuito *circuto = novoCircuito();
    t_circuito *retorno = NULL;

    char c = '\0';

    while(1)
    {
        // A
        c = fgetc(arquivo);

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
        }

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
