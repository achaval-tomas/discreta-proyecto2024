#include "API2024Parte2.h"
#include "Math.h"
#include "Util.h"
#include <stdlib.h>
#include <assert.h>

// Colorear todos los vertices con 0.
static char ResetearColores(Grafo G)
{
    u32* colores = calloc(NumeroDeVertices(G), sizeof(u32));
    if (colores == NULL)
        return 1;
    ImportarColores(colores, G);
    free(colores);
    return 0;
}

/*
Asume que el color 0 significa que el vértice no está pintado
y que flags tiene espacio suficiente para todos los colores (delta+2).
También se asume que flags[c] != v + 1 para todo c.
Devuelve el mínimo color disponible para el vertice v.
*/
static u32 ProximoColor(Grafo G, u32 v, u32* flags)
{
    u32 grado = Grado(v, G);

    for (u32 i = 0; i < grado; ++i) {
        u32 vecino = Vecino(i, v, G);
        u32 color_vecino = Color(vecino, G);

        // Nos ahorramos el siguiente if:
        // if (color_vecino != 0)
        //     flags[color_vecino] = v + 1;
        // Esto resulta en una buena mejora en algunos grafos como Harkon,
        // pero también reduce el rendimiento en otros grafos como 8suPGJ.
        flags[color_vecino] = v + 1;
    }

    color min_color = 1;
    // SAFE: debe ser que flags[c] != v + 1 para algún c=1, ..., Delta + 1
    while (flags[min_color] == v + 1)
        ++min_color;

    assert(min_color <= Delta(G) + 1);

    return min_color;
}

// Se asume que Orden apunta a un sector de memoria con n lugares.
u32 Greedy(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);

    // si Orden no es una biyección, retornamos error
    if (!ChequearOrden(Orden, n))
        return -1;

    if (ResetearColores(G))
        return -1;

    // flags será utilizado por ProximoColor para buscar.
    // Se pone espacio para Delta(G) + 2 colores. Explicación: Greedy usará como mucho Delta + 1 colores,
    // pues el vértice con más vecinos tiene Delta vecinos, y si cada vecino tiene un color distinto,
    // los vecinos tendrán en total Delta colores distintos. Luego, todo vértice, independientemente de su grado, se coloreará
    // con algún color en { 1, ..., Delta + 1 }, y por lo tanto, (a priori) esos son todos los colores posibles.
    // Ahora bien, nosotros hacemos una optimización para ahorrar un if la cual consiste en tratar al color 0 (asignado
    // cuando todavía no fue coloreado) como cualquier otro color, y luego al buscar el mínimo color simplemente
    // ignorarlo. Por lo tanto se requieren Delta + 2 lugares, pues los colores posibles son { 0, ..., Delta + 1 }.
    u32* flags = calloc(Delta(G) + 2, sizeof(u32));
    if (flags == NULL)
        return -1;

    u32 num_colores = 0;
    for (u32 i = 0; i < n; i++) {
        u32 v = Orden[i];
        u32 color_v = ProximoColor(G, v, flags);
        AsignarColor(color_v, v, G);
        num_colores = MAX(color_v, num_colores);
    }

    free(flags);
    return num_colores;
}
