#include <stdio.h>
#include <stdlib.h>
#include "lib/structs.h"
#include "lib/input.h"
#include "lib/lpwriter.h"
#include "lib/utils.h"

int main()
{
    int k, n;
    scanf("%d %d", &k, &n);

    Compartment *c;
    Load *l;

    if (!read_input(k, n, &c, &l))
    {
        return 1; // erro ao alocar memória ou ler
    }

    print_structs(k, n, c, l);

    printf("\n%s\n", generate_lp(k, n, c, l));

    free(c);
    free(l);
    return 0;
}