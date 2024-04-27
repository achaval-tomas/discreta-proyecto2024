#include "API2024Parte2.h"
#include "Math.h"
#include "Util.h"
#include <stdlib.h>

// Colorear todos los vertices con 0.
static void ResetearColores(Grafo G)
{
    u32* colores = calloc(NumeroDeVertices(G), sizeof(u32));
    ImportarColores(colores, G);
    free(colores);
}

// Asume que el color 0 significa que el vértice no está pintado.
static u32 ProximoColor(Grafo G, u32 v, u32 n_colores, u32* flags)
{
    u32 grado = Grado(v, G);

    for (u32 i = 0; i < grado; ++i) {
        u32 vecino = Vecino(i, v, G);
        u32 color_vecino = Color(vecino, G);

        if (color_vecino > 0) {
            flags[color_vecino - 1] = v+1;
        }
    }

    u32 min_color = 1;
    while (min_color <= n_colores && flags[min_color - 1] == v+1)
        ++min_color;

    return min_color;
}

// Se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);
    if (!ChequearOrden(Orden, n))
        return -1;

    ResetearColores(G);

    u32 delta = Delta(G);
    u32* flags = calloc(delta+1, sizeof(u32));
    u32 num_colores = 0;

    for (u32 i = 0; i < n; ++i) {
        u32 v = Orden[i];
        u32 lowest_color = ProximoColor(G, v, num_colores, flags);
        AsignarColor(lowest_color, v, G);
        num_colores = MAX(lowest_color, num_colores);
    }

    free(flags);
    return num_colores;
}
