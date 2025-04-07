#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct
{
    int w; // capacidade de peso
    int v; // capacidade de volume
} Compartment;

typedef struct
{
    int p; // peso total
    int t; // volume total
    int g; // ganho por tonelada
} Load;

#endif
