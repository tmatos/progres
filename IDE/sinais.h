/**
 * @file sinais.h
 * @brief Estruturas e fun��es para manipula��o de sinais de entrada e sa�da.
 */

#ifndef SINAIS_H

#define SINAIS_H

#define MAX_NOME_SINAL 50

/** @brief Valor l�gico de um pulso. Aqui, nulo serve para indicar o fim de uma "string" de pulsos.
 */
typedef enum en_valor {
    zero = 0,
    um = 1,
    xis,
    nulo
} ValorLogico;

/** @brief Unidades de tempo dispon�veis para a dura��o de um pulso.
        Na ordem, segundo, milisegundo, microsegundo, nanosegundo e picosegundo.
        O valor num�rico � equivalente ao valor absoluto do m�dulo do expoente.
*/
typedef enum en_un_tempo {
    UN_S = 0,
    UN_MS = 3,
    UN_US = 6,
    UN_NS = 9,
    UN_PS = 12
} UnidTempo;

/** @brief O tipo das vari�veis usadas na representa��o do tempo.
 */
typedef unsigned int Tempo;

/** @brief Um pulso de valor fixo e dura��o definida.
 */
typedef struct st_pulso {
    ValorLogico valor;
    Tempo tempo;
    UnidTempo unidade;
} Pulso;

/** @brief Um sinal cont�m um array de pulsos com o �ltimo pulso nulo. Semelhantemente a uma string.
 */
typedef struct st_sinal {
    char nome[MAX_NOME_SINAL];
    Pulso* pulsos;
    Tempo duracaoTotal;
} Sinal;

/** @brief Um conjunto de um ou mais sinais. Podem ser todos de entrada ou todos de sa�da.
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

/** @brief Define o pulso indicado com sendo nulo. Isto �, seu valor conter� nulo.
 */
int setPulsoNulo(Pulso* p);

/** @brief Adiciona ao sinal, mais especificamente ao vetor de pulsos do obj. Sinal,
            mais um pulso de valor e dura��o indicados.
            � como se fosse um append, aqui fazemos uso de realloc.
 */
int addPulso(Sinal* s, ValorLogico valor, Tempo duracao);

/** @brief Inicializa um nova estrutura Sinas vazia e devolve sua pos. de mem�ria.
            Vazia significa: primeiro e ultimo apontam a NULL e num. de elem. � zero.
 */
Sinais* novaSinais();

/** @brief Insere um sinal em branco na estrutura Sinais.
 */
int addSinal(Sinais* s, char* nome);

/** @brief Copia um sinal para a estrutura Sinais.
 */
int addSinalPronto(Sinais *ls, Sinal *sinal);

#endif // SINAIS_H
