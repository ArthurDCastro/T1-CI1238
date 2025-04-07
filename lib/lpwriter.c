#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lpwriter.h"
#include "utils.h"

/**
 * Gera a função objetivo do modelo LP (maximize).
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param l Vetor de carregamentos
 * @return String contendo a função objetivo no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *objective(int k, int n, Load *l)
{
    size_t size = 64;
    char *line = malloc(size);
    if (!line)
    {
        fprintf(stderr, "[objective] Erro ao alocar memória inicial.\n");
        return NULL;
    }

    strcpy(line, "max: ");

    char term[32];

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            snprintf(term, sizeof(term), "%dx%d%d", l[j].g, i + 1, j + 1);
            if (i + j + 2 < k + n)
                strcat(term, " + ");

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > size)
            {
                size = new_len;
                char *temp = realloc(line, size);
                if (!temp)
                {
                    fprintf(stderr, "[objective] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
    }

    strcat(line, ";\n");
    return line;
}

/**
 * Gera as restrições de peso para cada compartimento.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @return String contendo as restrições de peso no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *weight_constraints(int k, int n, Compartment *c)
{
    size_t size = 64, l_size = 64;
    char *constraints = malloc(size);
    char *line = malloc(size);
    if (!constraints || !line)
    {
        fprintf(stderr, "[weight_constraints] Erro ao alocar memória inicial.\n");
        return NULL;
    }
    constraints[0] = line[0] = '\0';

    for (int i = 0; i < k; i++)
    {
        char term[32];
        snprintf(term, sizeof(term), "x%d%d", i + 1, 1);
        strcpy(line, term);

        for (int j = 1; j < n; j++)
        {
            snprintf(term, sizeof(term), " + x%d%d", i + 1, j + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
                if (!temp)
                {
                    fprintf(stderr, "[weight_constraints] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
        snprintf(term, sizeof(term), " <= %d;\n", c[i].w);
        strcat(line, term);

        size_t new_len = strlen(constraints) + strlen(line) + 4;
        if (new_len > size)
        {
            size *= new_len;
            char *temp = realloc(constraints, size);
            if (!temp)
            {
                fprintf(stderr, "[weight_constraints] Erro ao realocar memória.\n");
                free(constraints);
                return NULL;
            }
            constraints = temp;
        }

        strcat(constraints, line);
    }

    return constraints;
}

/**
 * Calcula o vetor de volume por tonelada (alpha) para cada carregamento.
 *
 * @param n Quantidade de carregamentos
 * @param a Ponteiro para vetor de double, que será alocado e preenchido com alpha_j = t_j / p_j
 * @param l Vetor de carregamentos (com p_j e t_j)
 * @return 1 se sucesso, 0 se erro (ex: malloc falhou ou p_j == 0)
 */
int compute_alpha(int n, double **a, Load *l)
{
    *a = malloc(n * sizeof(double));
    if (!*a)
    {
        fprintf(stderr, "[alpha] Erro ao alocar vetor alpha.\n");
        return 0;
    }

    for (int j = 0; j < n; j++)
    {
        if (l[j].p == 0)
        {
            fprintf(stderr, "[alpha] Erro: p_%d = 0 (divisão por zero).\n", j + 1);
            free(*a);
            return 0;
        }
        (*a)[j] = (double)l[j].t / l[j].p;
    }

    return 1;
}

/**
 * Gera as restrições de volume para cada compartimento.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @param l Vetor de carregamentos
 * @return String contendo as restrições de volume no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *volume_constraints(int k, int n, Compartment *c, Load *l)
{
    double *alpha;
    if (!compute_alpha(n, &alpha, l))
        return NULL;

    size_t size = 64, l_size = 64;
    char *constraints = malloc(size);
    char *line = malloc(size);
    if (!constraints || !line)
    {
        fprintf(stderr, "[volume_constraints] Erro ao alocar memória inicial.\n");
        return NULL;
    }
    constraints[0] = line[0] = '\0';

    for (int i = 0; i < k; i++)
    {
        char term[32];
        snprintf(term, sizeof(term), "%.2fx%d%d", alpha[0], i + 1, 1);
        strcpy(line, term);

        for (int j = 1; j < n; j++)
        {
            snprintf(term, sizeof(term), " + %.2fx%d%d", alpha[j], i + 1, j + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
                if (!temp)
                {
                    fprintf(stderr, "[volume_constraints] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
        snprintf(term, sizeof(term), " <= %d;\n", c[i].v);
        strcat(line, term);

        size_t new_len = strlen(constraints) + strlen(line) + 4;
        if (new_len > size)
        {
            size *= new_len;
            char *temp = realloc(constraints, size);
            if (!temp)
            {
                fprintf(stderr, "[volume_constraints] Erro ao realocar memória.\n");
                free(constraints);
                return NULL;
            }
            constraints = temp;
        }

        strcat(constraints, line);
    }

    return constraints;
}

/**
 * Gera as restrições de disponibilidade dos carregamentos.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param l Vetor de carregamentos
 * @return String contendo as restrições de disponibilidade no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *availability_constraints(int k, int n, Load *l)
{
    size_t size = 64, l_size = 64;
    char *constraints = malloc(size);
    char *line = malloc(size);
    if (!constraints || !line)
    {
        fprintf(stderr, "[availability_constraints] Erro ao alocar memória inicial.\n");
        return NULL;
    }
    constraints[0] = line[0] = '\0';

    for (int i = 0; i < n; i++)
    {
        char term[32];
        snprintf(term, sizeof(term), "x%d%d", 1, i + 1);
        strcpy(line, term);

        for (int j = 1; j < k; j++)
        {
            snprintf(term, sizeof(term), " + x%d%d", j + 1, i + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
                if (!temp)
                {
                    fprintf(stderr, "[availability_constraints] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
        snprintf(term, sizeof(term), " <= %d;\n", l[i].p);
        strcat(line, term);

        size_t new_len = strlen(constraints) + strlen(line) + 4;
        if (new_len > size)
        {
            size *= new_len;
            char *temp = realloc(constraints, size);
            if (!temp)
            {
                fprintf(stderr, "[availability_constraints] Erro ao realocar memória.\n");
                free(constraints);
                return NULL;
            }
            constraints = temp;
        }

        strcat(constraints, line);
    }

    return constraints;
}

/**
 * Gera as restrições de proporcionalidade entre compartimentos.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @return String contendo as restrições de proporcionalidade no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *proportionality_constraints(int k, int n, Compartment *c)
{
    size_t size = 64, l_size = 64;
    char *proportionality = malloc(size);
    char *line = malloc(size);
    if (!proportionality || !line)
    {
        fprintf(stderr, "[proportionality_constraints] Erro ao alocar memória inicial.\n");
        return NULL;
    }

    proportionality[0] = line[0] = '\0';

    int a = 0;
    int b = a + 1;
    while (a < k - 1)
    {
        char term[32];
        snprintf(term, sizeof(term), "%dx%d%d", c[b].w, a + 1, 1);
        strcpy(line, term);
        for (int i = 1; i < n; i++)
        {
            snprintf(term, sizeof(term), " + %dx%d%d", c[b].w, a + 1, i + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
                if (!temp)
                {
                    fprintf(stderr, "[proportionality_constraints] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
        for (int i = 0; i < n; i++)
        {
            snprintf(term, sizeof(term), " - %dx%d%d", c[a].w, b + 1, i + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
                if (!temp)
                {
                    fprintf(stderr, "[proportionality_constraints] Erro ao realocar memória.\n");
                    free(line);
                    return NULL;
                }
                line = temp;
            }

            strcat(line, term);
        }
        snprintf(term, sizeof(term), " = 0;\n");
        strcat(line, term);

        size_t new_len = strlen(proportionality) + strlen(line) + 4;
        if (new_len > size)
        {
            size *= new_len;
            char *temp = realloc(proportionality, size);
            if (!temp)
            {
                fprintf(stderr, "[proportionality_constraints] Erro ao realocar memória.\n");
                free(proportionality);
                return NULL;
            }
            proportionality = temp;
        }

        strcat(proportionality, line);

        if (b < k - 1)
            b++;
        else
        {
            a++;
            b = a + 1;
        }
    }

    return proportionality;
}

/**
 * Gera as restrições de não-negatividade para todas as variáveis.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @return String contendo as restrições xij >= 0 no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *nonnegativity_constraints(int k, int n)
{

    size_t size = 64;
    char *n_negativity = malloc(size);
    if (!n_negativity)
    {
        fprintf(stderr, "[nonnegativity_constraints] Erro ao alocar memória inicial.\n");
        return NULL;
    }
    n_negativity[0] = '\0';

    char line[32];

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            snprintf(line, sizeof(line), "x%d%d >= 0;\n", i + 1, j + 1);

            size_t new_len = strlen(n_negativity) + strlen(line) + 4;
            if (new_len > size)
            {
                size *= new_len;
                char *temp = realloc(n_negativity, size);
                if (!temp)
                {
                    fprintf(stderr, "[nonnegativity_constraints] Erro ao realocar memória.\n");
                    free(n_negativity);
                    return NULL;
                }
                n_negativity = temp;
            }

            strcat(n_negativity, line);
        }
    }

    return n_negativity;
}

/**
 * Gera o modelo completo no formato lp_solve.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @param l Vetor de carregamentos
 * @return String contendo o modelo LP completo.
 *         Retorna NULL em caso de erro interno.
 */
char *generate_lp(int k, int n, Compartment *c, Load *l)
{
    char *model = malloc(1);
    if (!model)
    {
        fprintf(stderr, "Erro ao alocar model inicial.\n");
        return NULL;
    }
    model[0] = '\0';

    char *aux = objective(k, n, l);

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    aux = proportionality_constraints(k, n, c);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    aux = weight_constraints(k, n, c);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    aux = availability_constraints(k, n, l);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    aux = volume_constraints(k, n, c, l);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    aux = nonnegativity_constraints(k, n);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    return model;
}
