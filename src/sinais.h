/**
 * @file sinais.h
 * @brief Estruturas e funções para manipulação de sinais de entrada e saída.
 */

#ifndef SINAIS_H
#define SINAIS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NOME_SINAL 50 /// Tamanho máximo permitido para o nome de um sinal.

/** @brief Enum para representação dos valores lógicos utilizados nas simulações.
 *         Aqui, VAL_BLANK serve unicamente para indicar o final de um array de pulsos.
 *         De forma semelhante ao que acontece com '0x00' para strings em C.
 */
typedef enum en_logic_value {
    VAL_0 = 0,
    VAL_1 = 1,
    VAL_X = 2,
    VAL_Z = 3,
    VAL_L = 4,
    VAL_H = 5,
    VAL_BLANK
} ValorLogico;

/** @brief Convert from a long integer to a one bit logic value of type ValorLogico.
 *         It takes the least significant bit of n to define the value.
 *  @param n A number of type long integer.
 *  @return Either VAL_0 or VAL_1, from enum ValorLogico.
 */
ValorLogico long_to_logicvalue(long n);

/** @brief Unidades de tempo disponíveis para a duração de um pulso.
           Na ordem: segundo, milisegundo, microsegundo, nanosegundo, picosegundo e femtosegundo.
           O valor numérico é equivalente ao valor absoluto do módulo do expoente.
*/
typedef enum en_un_tempo {
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
} UnidTempo;

/** @brief Retorna o valor da enum UnidTempo relativo ao que está contido na string str fornecida.
 *  @param str String contendo a unidade. Ex.: "s", "ms", "us", "ns", "ps", "fs".
 *  @return O valor da enum UnidTempo respectivo. Se a string não contém a unidade: UN_INVALID.
 */
UnidTempo get_timeunit_from_str(const char* str);

/** @brief Retorna a string que representa unidade de tempo.
 *  @param unit Unidade de tempo, da enum UnidTempo.
 *  @return Uma string constant com valor: "s", "ms", "us", "ns", "ps" ou "fs".
 */
const char* get_str_from_timeunit(UnidTempo unit);

/** @brief O tipo das variáveis usadas na representação do tempo.
 */
typedef unsigned long long int Tempo;

/** @brief Um pulso de valor fixo e duração definida.
 */
typedef struct st_pulso {
    ValorLogico valor;
    Tempo tempo;
    UnidTempo unidade;
} Pulso;

/** @brief Um sinal contém um array de pulsos com o último pulso nulo.
 *         Semelhantemente a uma string.
 */
typedef struct st_sinal {
    char nome[MAX_NOME_SINAL];
    Pulso* pulsos;
    Tempo duracaoTotal;
} Sinal;

/** @brief Um conjunto de um ou mais sinais.
 *         Podem ser todos de entrada ou todos de saáda.
 */
typedef struct st_sinais {
    int quantidade;
    Sinal* lista;
} Sinais;

/** @brief Inicializa um sinal vazio com o respectivo nome indicado.
 *  @param nome String com o nome do sinal a ser criado.
 *  @return Ponteiro para o sinal recém criado.
 */
Sinal* new_signal(const char* nome);

/** @brief Muda a string contendo o nome do sinal dentro de s para a indicada por nome.
 *  @param s Sinal que terá o nome modificado.
 *  @param nome String contendo o novo nome do sinal.
 */
int set_signal_name(Sinal* s, const char* nome);

/** @brief Define o pulso p indicado com sendo nulo.
 *         Isto é, seu valor conterá VAL_BLANK e terá tempo zero.
 *  @param p Ponteiro para o pulso a ser afetado. 
 */
int set_pulse_blank(Pulso* p);

/** @brief Adiciona ao sinal, mais especificamente ao vetor de pulsos do objeto Sinal,
           mais um pulso de valor e duração indicados.
           é como se fosse um append, aqui fazemos uso de realloc.
    @param s Ponteiro para um sinal.
    @param valor Valor lógico do pulso a ser adicionado.
    @param duracao Duracao, em unidade adimensional, de tempo do pulso a ser inserido.
    @return 1 em caso de sucesso, 0 caso falhe.
 */
int add_new_pulse(Sinal* s, ValorLogico valor, Tempo duracao);

/** @brief Inicializa um nova estrutura Sinas vazia e retorna seu endereço de memória.
           Vazia significa que os elementos primeiro e ultimo apontam para NULL
           e o número de elementos é zero.
 */
Sinais* new_signal_list();

/** @brief Free used memory and set NULL to the pointer.
 *  @param signal Pointer to a pointer to the Sinal struct.
 */
void free_signal(Sinal** signal);

/** @brief Free mem and set NULL to pointer.
 *  @param list Pointer to Sinais*
 */
void free_signal_list(Sinais** list);

/** @brief Insere um sinal em branco na struct que representa uma lista de Sinais.
 *  @param list Uma lista de sinais.
 *  @param nome String com o nome do sinal em branco.
 *  @return 1 em caso de sucesso
 */
int add_new_signal(Sinais* list, const char* nome);

/** @brief Insere, copiando, um sinal ja existente em uma lista de sinais destino.
 *  @param destin_list A lista de sinais que recebera o sinal.
 *  @param signal O sinal original a ser copiado.
 *  @return 1 em caso de sucesso, 0 caso falhe.
 */
int insert_signal(Sinais* destin_list, Sinal* signal);

#ifdef __cplusplus
}
#endif

#endif // SINAIS_H
