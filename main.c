#include <stdio.h>
#include <stdlib.h>
#include "lib/structs.h"
#include "lib/input.h"
#include "lib/lpwriter.h"
#include "lib/utils.h"

int main()
{
    int k, n;
    if (scanf("%d %d", &k, &n) != 2)
    {
        fprintf(stderr, "Erro ao ler k e n.\n");
        return 1;
    }

    Compartment *c;
    Load *l;

    if (!read_input(k, n, &c, &l))
    {
        return 1; // erro ao alocar memória ou ler
    }

    printf("%s", generate_lp(k, n, c, l));

    free(c);
    free(l);
    return 0;
}