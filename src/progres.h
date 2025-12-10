/**
 * @file progres.h
 * @brief Protótipos do módulo principal (que contém o ponto de entrada) do programa.
 */

#ifndef PROGRES_H
#define PROGRES_H

#include "sinais.h"

/// Program version number, as string
#define _PROGRES_VERSION "0.0.7-beta"

/// Brief command line help string
#define _HELP_STRING_BRIEF "Usage: progres [-s] verilog_file.v [input stimulus file] [output stimulus file]\n"

/// Tamanho máximo permitido para o argumento de linha comando relativo ao arquivo de entrada
#define MAX_FILE_PATH_SIZE 4096

/** @brief Load the input signals from the file specified in path.
 *  @param path Path to the input file containing the signals.
 *  @return A pointer to a Sinais structure containing the loaded input signals.
 *  @note The input file should be in a custom format that the program can parse.
 */
Sinais* load_inputs_from_path(const char* path);

/** @brief Save the output signals to the specified path.
 *  @param path Path to the output file where the signals will be saved.
 *  @param outputs Pointer to the Sinais structure containing the output signals.
 *  @note This function will write the output signals to a file in out custom format.
 */
void save_outputs_to_path(const char* path, Sinais* outputs);

#endif // PROGRES_H
