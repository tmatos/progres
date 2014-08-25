
#ifndef INOUT_H

#define INOUT_H

#include "sinais.h"

void salvarSinais(Sinais *sinaisSaida, FILE *arqSaida);

Sinais* carregaEntradas(FILE *arquivo);

extern "C" Sinais* carregaArquivoSinais(const char* path);

#endif // INOUT_H
