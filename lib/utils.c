#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void print_structs(int k, int n, Compartment *c, Load *l)
{
    printf("Compartimentos (k = %d):\n", k);
    for (int i = 0; i < k; i++)
    {
        printf("  Compartimento %d: peso = %d, volume = %d\n", i + 1, c[i].w, c[i].v);
    }

    printf("\nCarregamentos (n = %d):\n", n);
    for (int j = 0; j < n; j++)
    {
        printf("  Carregamento %d: peso = %d, volume = %d, ganho = %d\n", j + 1, l[j].p, l[j].t, l[j].g);
    }
}

int append(char **model, char *aux)
{
    if (!aux)
        return 0;

    size_t len = strlen(*model) + strlen(aux) + 1;
    char *temp = realloc(*model, len);
    if (!temp)
    {
        fprintf(stderr, "[append] Erro: falha ao realocar model.\n");
        free(aux);
        return 0;
    }

    *model = temp;
    strcat(*model, aux);
    free(aux);
    return 1;
}
