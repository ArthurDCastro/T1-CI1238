#ifndef UTILS_H
#define UTILS_H

#include "structs.h"

/**
 * Imprime os compartimentos e carregamentos lidos (para testes).
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @param l Vetor de carregamentos
 */
void print_structs(int k, int n, Compartment *c, Load *l);

/**
 * Concatena o conteúdo de aux em model, realocando e liberando aux.
 *
 * @param model Ponteiro para o ponteiro da string principal
 * @param aux String a ser concatenada (será liberada)
 * @return 1 em caso de sucesso, 0 em caso de erro (libera model se necessário)
 */
int append(char **model, char *aux);

#endif
