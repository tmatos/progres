/**
 * @file progres.h
 * @brief Protótipos do módulo principal (que contém o ponto de entrada) do programa.
 */

#ifndef PROGRES_H
#define PROGRES_H

#include <stdio.h>

#include "estruturas.h"
#include "sinais.h"

/// Program version number, as string
#define _PROGRES_VERSION "0.0.8-beta"

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

/** @brief Create dummy input signals for the given module.
 *  @param module Pointer to the Module structure for which to create dummy inputs.
 *  @return A pointer to a Sinais structure containing the dummy input signals.
 *  @note This function generates default or placeholder input signals for testing purposes.
 */
Sinais* create_dummy_inputs(Module* module);

/** @brief Save the output signals to the specified path.
 *  @param path Path to the output file where the signals will be saved.
 *  @param outputs Pointer to the Sinais structure containing the output signals.
 *  @note This function will write the output signals to a file in out custom format.
 */
void save_outputs_to_path(const char* path, Sinais* outputs);

/** @brief Open a file with the given path and mode, or exit the program if it fails.
 *  @param path Path to the file to be opened.
 *  @param mode Mode in which to open the file (e.g., "r" for read, "w" for write).
 *  @return A pointer to the opened FILE object.
 *  @note If the file cannot be opened, this function will print an error message and terminate the program.
 */
FILE* open_or_exit(const char* path, const char* mode);

#endif // PROGRES_H
