/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <cstdio>

#include "util.h"
#include "sinais.h"

extern "C" Sinais* carregaEntradas(FILE* arquivo);

Sinais* load_signals_from_path(const char* path)
{
    if (!path)
        return NULL;

    FILE* waveFile = fopen(path, "r");

    if (!waveFile) {
        printf("Impossibilitado de abrir o arquivo de sinais: %s\n", path);
        return NULL;
    }

    printf("Abrindo o arquivo de sinais: %s\n", path);

    Sinais* waves = carregaEntradas(waveFile);

    fclose(waveFile);

    return waves;
}
