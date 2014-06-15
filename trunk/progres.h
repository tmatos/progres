#ifndef PROGRES_H

#define PROGRES_H

t_circuito* carregaCircuito(FILE *arquivo);

ListaToken* tokeniza(FILE *arquivo);

void exibeMsgErro(char* msg, int linha, int coluna, char* esperado, char *encontrado);

#endif
