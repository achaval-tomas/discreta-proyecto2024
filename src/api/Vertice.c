#include "APIG24.h"

u32 Grado(u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g))
        return 0;

    return g->vertices[i].grado;
}

color Color(u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g))
        return -1; // 2³² - 1

    return g->vertices[i].color;
}

u32 Vecino(u32 j, u32 i, Grafo g)
{
    if (i >= NumeroDeVertices(g) || j >= Grado(i, g))
        return -1; // 2³² - 1

    return g->vertices[i].vecinos[j];
}
