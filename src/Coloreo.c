#include "APIG24.h"

void AsignarColor(color x, u32 i, Grafo G)
{
    if (i >= NumeroDeVertices(G))
        return;
    G->vertices[i].color = x;
}

// SE ASUME que Color es un array que apunta a un lugar de memoria con n lugares.
void ExtraerColores(Grafo G, color* Color)
{
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i<n; ++i)
        Color[i] = G->vertices[i].color;
}

// SE ASUME que Color es un array que apunta a un lugar de memoria con n lugares.
void ImportarColores(color* Color, Grafo G)
{
    u32 n = NumeroDeVertices(G);
    for (u32 i = 0; i<n; ++i)
        G->vertices[i].color = Color[i];
}