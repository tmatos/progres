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
