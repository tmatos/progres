
#ifndef INOUT_H

#define INOUT_H

#include "sinais.h"

static Sinais* carregaEntradas(FILE *arquivo);

extern "C" Sinais* carregaArquivoSinais(const char* path);

#endif
