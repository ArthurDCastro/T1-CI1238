#ifndef INPUT_H
#define INPUT_H

#include "structs.h"

/**
 * Lê os dados de entrada e aloca os vetores de compartimentos e carregamentos.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Ponteiro para vetor de compartimentos (será alocado)
 * @param l Ponteiro para vetor de carregamentos (será alocado)
 * @return 1 em caso de sucesso, 0 se falhar ao alocar memória
 */
int read_input(int k, int n, Compartment **c, Load **l);

#endif
