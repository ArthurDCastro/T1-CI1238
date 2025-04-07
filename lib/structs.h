#ifndef STRUCTS_H
#define STRUCTS_H

/// Representa um compartimento de carga.
typedef struct
{
    /// Capacidade máxima de peso do compartimento.
    int w;

    /// Capacidade máxima de volume do compartimento.
    int v;
} Compartment;

/// Representa um tipo de carregamento.
typedef struct
{
    /// Peso total disponível do carregamento.
    int p;

    /// Volume total do carregamento.
    int t;

    /// Ganho (lucro) por tonelada do carregamento.
    int g;
} Load;

#endif // STRUCTS_H
