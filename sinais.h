/**
 * @file sinais.h
 * @brief Estruturas e funções para manipulação de sinais de entrada e saída.
 */

#ifndef SINAIS_H

#define SINAIS_H

#define MAX_NOME_SINAL 50 /// Tamanho máximo permitido para o nome de um sinal.

/** @brief Valor lógico de um pulso. Aqui, VAL_BLANK serve para indicar o fim de uma "string" de pulsos.
 */
typedef enum en_logic_value {
    VAL_0 = 0,
    VAL_1 = 1,
    VAL_X = 2,
    VAL_Z = 3,
    VAL_BLANK
} ValorLogico;

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

/** @brief O tipo das variáveis usadas na representação do tempo.
 */
typedef unsigned int Tempo;

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
 */
Sinal* novoSinal(const char* nome);

/** @brief Muda a string contendo o nome do sinal dentro de s para a indicada por nome.
 */
int setSinalNome(Sinal* s, const char* nome);

/** @brief Define o pulso p indicado com sendo nulo.
 *         Isto é, seu valor conterá VAL_BLANK e terá tempo zero.
 */
int setPulsoNulo(Pulso* p);

/** @brief Adiciona ao sinal, mais especificamente ao vetor de pulsos do objeto Sinal,
           mais um pulso de valor e duração indicados.
           é como se fosse um append, aqui fazemos uso de realloc.
 */
int addPulso(Sinal* s, ValorLogico valor, Tempo duracao);

/** @brief Inicializa um nova estrutura Sinas vazia e retorna seu endereço de memória.
           Vazia significa que os elementos primeiro e ultimo apontam para NULL
           e o número de elementos é zero.
 */
Sinais* novaSinais();

/** @brief Insere um sinal em branco na estrutura Sinais.
 */
int addSinal(Sinais* s, const char* nome);

/** @brief Copia um sinal para a estrutura Sinais.
 */
int addSinalPronto(Sinais* ls, Sinal* sinal);

#endif // SINAIS_H
