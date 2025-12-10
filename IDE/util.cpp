/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <cstdio>

#include "estruturas.h"
#include "sinais.h"
#include "util.h"

extern "C" Sinais* load_input_signals(FILE* file);

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

    Sinais* waves = load_input_signals(waveFile);

    fclose(waveFile);

    return waves;
}
