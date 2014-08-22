/**
 * @file sinais.h
 * @brief Estruturas e funções para manipulação de sinais de entrada e saída.
 */

#ifndef SINAIS_H

#define SINAIS_H

#define MAX_NOME_SINAL 50 /// Tamanho máximo permitido para o nome de um sinal.

/** @brief Valor lógico de um pulso. Aqui, nulo serve para indicar o fim de uma "string" de pulsos.
 */
typedef enum en_valor {
    zero = 0,
    um = 1,
    x,
    nulo
} ValorLogico;

/** @brief Unidades de tempo disponíveis para a duração de um pulso.
        Na ordem, segundo, milisegundo, microsegundo, nanosegundo e picosegundo.
        O valor numérico é equivalente ao valor absoluto do módulo do expoente.
*/
typedef enum en_un_tempo {
    UN_S = 0,
    UN_MS = 3,
    UN_US = 6,
    UN_NS = 9,
    UN_PS = 12
} UnidTempo;

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

/** @brief Um sinal contém um array de pulsos com o último pulso nulo. Semelhantemente a uma string.
 */
typedef struct st_sinal {
    char nome[MAX_NOME_SINAL];
    Pulso* pulsos;
    Tempo duracaoTotal;
} Sinal;

/** @brief Um conjunto de um ou mais sinais. Podem ser todos de entrada ou todos de saída.
 */
typedef struct st_sinais {
    int quantidade;
    Sinal* lista;
} Sinais;

/** @brief Inicializa um sinal vazio com um respectivo nome.
 */
Sinal* novoSinal(char *nome);

/** @brief Muda a string contendo o nome do sinal para a indicada.
 */
int setSinalNome(Sinal* s, char* nome);

/** @brief Define o pulso indicado com sendo nulo. Isto é, seu valor conterá nulo.
 */
int setPulsoNulo(Pulso* p);

/** @brief Adiciona ao sinal, mais especificamente ao vetor de pulsos do obj. Sinal,
            mais um pulso de valor e duração indicados.
            É como se fosse um append, aqui fazemos uso de realloc.
 */
int addPulso(Sinal* s, ValorLogico valor, Tempo duracao);

/** @brief Inicializa um nova estrutura Sinas vazia e devolve sua pos. de memória.
            Vazia significa: primeiro e ultimo apontam a NULL e num. de elem. é zero.
 */
Sinais* novaSinais();

/** @brief Insere um sinal em branco na estrutura Sinais.
 */
int addSinal(Sinais* s, char* nome);

/** @brief Copia um sinal para a estrutura Sinais.
 */
int addSinalPronto(Sinais *ls, Sinal *sinal);

#endif // SINAIS_H
