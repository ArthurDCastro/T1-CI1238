#include <stdio.h>
#include <stdlib.h>
#include "input.h"

int read_input(int k, int n, Compartment **c, Load **l)
{
    *c = malloc(k * sizeof(Compartment));
    if (!c)
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 0;
    }

    *l = malloc(n * sizeof(Load));
    if (!l)
    {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 0;
    }

    // Leitura dos compartimentos
    for (int i = 0; i < k; i++)
    {
        if (scanf("%d %d", &(*c)[i].w, &(*c)[i].v) != 2)
        {
            fprintf(stderr, "[input] Erro ao ler peso e volume do compartimento %d.\n", i + 1);
            return 0;
        }
    }

    // Leitura dos carregamentos
    for (int j = 0; j < n; j++)
    {
        if (scanf("%d %d %d", &(*l)[j].p, &(*l)[j].t, &(*l)[j].g) != 3)
        {
            fprintf(stderr, "[input] Erro ao ler dados do carregamento %d.\n", j + 1);
            return 0;
        }
    }

    return 1;
}
