#include "API2024Parte2.h"
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Chequear que el orden dado es un orden válido.
static char ChequearOrden(u32* Orden, u32 n)
{
    char* found_flags = calloc(n, sizeof(char));

    int is_valid = 1;
    for (u32 i = 0; i < n; ++i) {
        u32 v = Orden[i];

        if (v >= n || found_flags[v]) {
            is_valid = 0;
            break;
        }

        found_flags[v] = 1;
    }

    free(found_flags);
    return is_valid;
}

// Colorear todos los vertices con 0.
static void ResetearColores(Grafo G)
{
    u32* colores = calloc(NumeroDeVertices(G), sizeof(u32));
    ImportarColores(colores, G);
    free(colores);
}

// Asume que el color 0 significa que el vértice no está pintado.
static u32 ProximoColor(Grafo G, u32 v, u32 n_colores)
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

    u32 min_color = 1;
    while (min_color <= n_colores && flags[min_color - 1])
        ++min_color;

    free(flags);
    return min_color;
}

// Se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);
    if (!ChequearOrden(Orden, n))
        return -1;

    ResetearColores(G);

    u32 num_colores = 0;

    for (u32 i = 0; i < n; ++i) {
        u32 v = Orden[i];
        u32 lowest_color = ProximoColor(G, v, num_colores);
        AsignarColor(lowest_color, v, G);
        num_colores = MAX(lowest_color, num_colores);
    }

    return num_colores;
}
