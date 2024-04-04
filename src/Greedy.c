#include "API2024Parte2.h"
#include <stdlib.h>

char CheckOrder(u32* Orden, u32 length)
{
    char* flags = calloc(length, sizeof(char));

    for (u32 i = 0; i < length; ++i) {
        if (Orden[i] >= length)
            return 0;

        if (flags[Orden[i]])
            return 0;

        flags[Orden[i]] = 1;
    }

    free(flags);
    return 1;
}

// Colorear todos los vertices con 0.
void ResetearColores(Grafo G)
{
    u32* colores = calloc(NumeroDeVertices(G), sizeof(u32));
    ImportarColores(colores, G);
    free(colores);
}

// Asume que el color 0 significa que el vértice no está pintado
u32 ProximoColor(Grafo G, u32 v, u32 n_colores)
{
    char* flags = calloc(n_colores, sizeof(char));

    u32 grado = Grado(v, G);

    for (u32 i = 0; i < grado; ++i) {
        u32 vecino = Vecino(i, v, G);
        u32 color_vecino = Color(vecino, G);

        if (color_vecino > 0) {
            flags[color_vecino - 1] = 1;
        }
    }

    for (u32 i = 1; i <= n_colores; ++i) {
        if (!flags[i - 1])
            return i;
    }

    free(flags);

    return n_colores + 1;
}

// se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);
    if (!CheckOrder(Orden, n))
        return -1;

    ResetearColores(G);

    u32 num_colores = 0;

    for (u32 i = 0; i < n; ++i) {
        u32 lowest_color = ProximoColor(G, i, num_colores);
        AsignarColor(lowest_color, i, G);
        num_colores = max(lowest_color, num_colores);
    }

    return num_colores;
}
