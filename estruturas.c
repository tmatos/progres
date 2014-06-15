/*
 Progres - Simulador de circuitos combinacionais em Verilog
*/

#include <stdlib.h>
#include "estruturas.h"

t_circuito* novoCircuito() {
    t_circuito* circuito = (t_circuito*) malloc(sizeof(t_circuito));

    circuito->numEntrada = 0;
    circuito->numSaida = 0;
    circuito->sinaisEntrada = NULL;
    circuito->sinaisSaida = NULL;
    circuito->listaFiosEntrada = NULL;
    circuito->listaFiosSaida = NULL;

    return circuito;
}

t_componente* novaListaCompon(int tamanho) {
    return (t_componente*) malloc(sizeof(t_componente) * tamanho);
}

t_componente novoComponente() {
    return (t_componente) malloc(sizeof(struct st_componente));
}
