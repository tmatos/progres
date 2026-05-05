/**
 * @file sinais.h
 * @brief Estruturas e funções para manipulação de sinais de entrada e saída.
 */

#ifndef SINAIS_H
#define SINAIS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SIGNAL_NAME 50 /// Tamanho máximo permitido para o nome de um sinal.

/** @brief Enum para representação dos valores lógicos usados nas simulações.
 *         Aqui, VAL_BLANK serve unicamente para indicar o final de um array
 *         de pulsos. De forma semelhante ao que acontece com '0x00' para as
 *         NULL terminated strings na linguagem C.
 */
typedef enum en_logic_value {
    VAL_0 = 0,
    VAL_1 = 1,
    VAL_X = 2,
    VAL_Z = 3,
    VAL_L = 4,
    VAL_H = 5,
    VAL_BLANK
} LogicValue;

/** @brief Convert from a long integer to a one bit logic value of
 *         type LogicValue. It takes the least significant bit of
 *         n to define the value.
 *  @param n A number of type long integer.
 *  @return Either VAL_0 or VAL_1, from enum LogicValue.
 */
LogicValue long_to_logicvalue(long n);

/** @brief Unidades de tempo disponíveis para a duração de um pulso.
           Na ordem: segundo, milisegundo, microsegundo, nanosegundo, 
           picosegundo e femtosegundo.
    @note  O valor numérico é equivalente ao valor absoluto do módulo
           do expoente.
*/
typedef enum en_time_unit {
    UN_S = 0,
    UN_100MS = 1,
    UN_10MS = 2,
    UN_MS = 3,
    UN_100US = 4,
    UN_10US = 5,
    UN_US = 6,
    UN_100NS = 7,
    UN_10NS = 8,
    UN_NS = 9,
    UN_100PS = 10,
    UN_10PS = 11,
    UN_PS = 12,
    UN_100FS = 13,
    UN_10FS = 14,
    UN_FS = 15,
    UN_INVALID = -1
} TimeUnit;

/** @brief Retorna o valor da enum TimeUnit relativo ao que está contido
 *         na string str fornecida.
 *  @param str String contendo a representação em texto da unidade.
 *             Ex.: "s", "ms", "us", "ns", "ps", "fs".
 *  @return O valor da enum TimeUnit respectivo.
 *          Se a string não contém a unidade: UN_INVALID.
 */
TimeUnit get_timeunit_from_str(const char* str);

/** @brief Retorna a string que representa unidade de tempo.
 *  @param unit Unidade de tempo, da enum TimeUnit.
 *  @return Uma string constant com valor: "s", "ms", "us", "ns", "ps" ou "fs".
 */
const char* get_str_from_timeunit(TimeUnit unit);

/** @brief The type for variables which represent time in the simulation.
 */
typedef unsigned long long int Time;

/** @brief Um pulso de valor fixo e duração definida.
 */
typedef struct st_pulse {
    LogicValue value;
    Time time;
    TimeUnit unit;
} Pulse;

/** @brief Um sinal contém um array de pulsos com o último pulso nulo.
 *         Semelhantemente a uma string.
 */
typedef struct st_signal {
    char name[MAX_SIGNAL_NAME];
    Pulse* pulses;
    Time total_time;
} Signal;

/** @brief Um conjunto de um ou mais sinais.
 *         Podem ser todos de entrada ou todos de saáda.
 */
typedef struct st_signal_array {
    int count;
    Signal* itens;
} SignalArray;

/** @brief Inicializa um sinal vazio com o respectivo nome indicado.
 *  @param name String com o nome do sinal a ser criado.
 *  @return Ponteiro para o sinal recém criado.
 */
Signal* new_signal(const char* name);

/** @brief Muda a string contendo o nome do sinal dentro de s para a
 *         indicada por name.
 *  @param s Signal que terá o nome modificado.
 *  @param name String contendo o novo nome do sinal.
 */
int set_signal_name(Signal* s, const char* name);

/** @brief Define o pulso p indicado com sendo nulo.
 *         Isto é, seu valor conterá VAL_BLANK e terá tempo zero.
 *  @param p Ponteiro para o pulso a ser afetado. 
 */
int set_pulse_blank(Pulse* p);

/** @brief Adiciona ao sinal, mais especificamente ao vetor de pulsos
 *         do objeto Signal, mais um pulso de valor e duração indicados.
 *         é como se fosse um append, aqui fazemos uso de realloc.
 *  @param s Ponteiro para um sinal.
 *  @param value Valor lógico do pulso a ser adicionado.
 *  @param time_length Duracao, em unidade adimensional, de tempo do
 *                     pulso a ser inserido.
 *  @return 1 em caso de sucesso, 0 caso falhe.
 */
int add_new_pulse(Signal* s, LogicValue value, Time time_length);

/** @brief Inicializa um nova estrutura Sinas vazia e retorna seu endereço
 *         de memória. Vazia significa que os elementos primeiro e ultimo
 *         apontam para NULL e o número de elementos é zero.
 */
SignalArray* new_signal_list();

/** @brief Free used memory and set NULL to the pointer.
 *  @param signal Pointer to a pointer to the Signal struct.
 */
void free_signal(Signal** signal);

/** @brief Free mem and set NULL to pointer.
 *  @param list Pointer to SignalArray*
 */
void free_signal_list(SignalArray** list);

/** @brief Insere um sinal em branco na struct que representa uma
 *         lista de Sinais.
 *  @param list Uma lista de sinais.
 *  @param name String com o nome do sinal em branco.
 *  @return 1 em caso de sucesso
 */
int add_new_signal(SignalArray* list, const char* name);

/** @brief Insere, copiando, um sinal ja existente em uma lista
 *         de sinais destino.
 *  @param destin_list A lista de sinais que recebera o sinal.
 *  @param signal O sinal original a ser copiado.
 *  @return 1 em caso de sucesso, 0 caso falhe.
 */
int insert_signal(SignalArray* destin_list, Signal* signal);

#ifdef __cplusplus
}
#endif

#endif // SINAIS_H
