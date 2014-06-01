
#include <stdio.h>
#include <stdlib.h>
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

t_circuito* carregaCircuito(FILE *arquivo)
{
    char c = '\0';
    int coment = 0;
    int multEspaco = 0;

    while(!feof(arquivo))
    {
        c = fgetc(arquivo);

        if(c == '#')
        {
            multEspaco = 0;
            coment = 1;
        }
        else if(c == '\n')
        {
            coment = 0;
        }

        if(!coment)
        {
            if(c == ' ' || c == '\n' || c == '\t')
            {
                multEspaco++;
            }
            else
            {
                if(multEspaco)
                    printf("\n");

                multEspaco = 0;

                printf("%c", c);
            }
        }
    }

    return NULL;
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
