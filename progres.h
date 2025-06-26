/**
 * @file progres.h
 * @brief Protótipos do módulo principal (que contém o ponto de entrada) do programa.
 */

#ifndef PROGRES_H

#define PROGRES_H

#include "sinais.h"

/// Program version number, as string
#define _PROGRES_VERSION "0.0.6-beta"

/// Brief command line help string
#define _HELP_STRING_BRIEF "Usage: progres [-s] verilog_file.v [input stimulus file] [output stimulus file]\n"

/// Tamanho máximo permitido para o argumento de linha comando relativo ao arquivo de entrada
#define MAX_FILE_PATH_SIZE 4096

Sinais* load_inputs_from_path(const char* path);

void save_outputs_to_path(const char* path, Sinais* outputs);

#endif // PROGRES_H
