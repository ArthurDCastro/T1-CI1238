#include <stdio.h>
#include <string.h>
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

    char term[32];
    snprintf(term, sizeof(term), "max: %dx%d%d", l[0].g, 1, 1);
    strcat(line, term);

    for (int i = 1; i < k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            snprintf(term, sizeof(term), " + %dx%d%d", l[j].g, i + 1, j + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > size)
            {
                size *= new_len;
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
        fprintf(stderr, "[objective] Erro ao alocar memória inicial.\n");
        return NULL;
    }
    constraints[0] = line[0] = '\0';

    for (int i = 0; i < k; i++)
    {
        char term[32];
        snprintf(term, sizeof(term), "x%d%d", i + 1, 1);
        strcpy(line, term);

        for (int j = 0; j < n; j++)
        {
            snprintf(term, sizeof(term), " + x%d%d", i + 1, j + 1);

            size_t new_len = strlen(line) + strlen(term) + 4;
            if (new_len > l_size)
            {
                l_size *= new_len;
                char *temp = realloc(line, l_size);
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
        snprintf(term, sizeof(term), " <= %d;\n", c[i].w);
        strcat(line, term);

        size_t new_len = strlen(constraints) + strlen(line) + 4;
        if (new_len > size)
        {
            size *= new_len;
            char *temp = realloc(constraints, size);
            if (!temp)
            {
                fprintf(stderr, "[objective] Erro ao realocar memória.\n");
                free(constraints);
                return NULL;
            }
            constraints = temp;
        }

        strcat(constraints, line);
    }

    strcat(constraints, "\n");

    return constraints;
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
static char *volume_constraints(int k, int n, Compartment *c, Load *l);

/**
 * Gera as restrições de disponibilidade dos carregamentos.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param l Vetor de carregamentos
 * @return String contendo as restrições de disponibilidade no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *availability_constraints(int k, int n, Load *l);

/**
 * Gera as restrições de proporcionalidade entre compartimentos.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @param c Vetor de compartimentos
 * @return String contendo as restrições de proporcionalidade no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *proportionality_constraints(int k, int n, Compartment *c);

/**
 * Gera as restrições de não-negatividade para todas as variáveis.
 *
 * @param k Número de compartimentos
 * @param n Número de carregamentos
 * @return String contendo as restrições xij >= 0 no formato lp_solve.
 *         Retorna NULL em caso de erro de alocação.
 */
static char *nonnegativity_constraints(int k, int n);

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

    aux = weight_constraints(k, n, c);

    strcat(model, "\n");

    if (!append(&model, aux))
    {
        free(model);
        return NULL;
    }

    return model;
}
