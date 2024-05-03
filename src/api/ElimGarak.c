#include "API2024Parte2.h"
#include "Math.h"
#include "Sort.h"
#include "Util.h"
#include <assert.h>
#include <string.h>

/*
Dado un grafo G con r colores { 1, ..., r }, devuelve un arreglo
donde el índice representa (color - 1) y el valor representa la
cantidad de vértices pintados de ese color. Es decir,
CalcularCardinalidades(G, r)[0] representa la cantidad de vértices pintados con el color 1.
CalcularCardinalidades(G, r)[1] representa la cantidad de vértices pintados con el color 2.
...
CalcularCardinalidades(G, r)[r - 1] representa la cantidad de vértices pintados con el color r.
Complejidad: O(n)
*/
static u32* CalcularCardinalidades(Grafo G, u32 r)
{
    u32* cardinalidades = calloc(r, sizeof(u32));
    if (cardinalidades == NULL)
        return NULL;

    for (u32 v = 0; v < NumeroDeVertices(G); v++) {
        u32 c = Color(v, G);
        cardinalidades[c - 1] += 1;
    }

    return cardinalidades;
}

/* Returns 0 if successful, 1 if error */
static char OrdenarColoresPorCardinalidadVerElimGarak(Grafo G, u32 r, u32* orden_colores)
{
    u32 n = NumeroDeVertices(G);
    u32* tabla_cardinalidades = CalcularCardinalidades(G, r);
    if (tabla_cardinalidades == NULL)
        return 1;

    // NOTE: n es la máxima cardinalidad posible (se podría acotar más),
    // por eso es que asignamos n + 2 y n + 1.
    tabla_cardinalidades[0] = n + 2; // 1 -> xr
    tabla_cardinalidades[1] = n + 1; // 2 -> xr-1

    for (u32 i = 0; i < r; i++)
        orden_colores[i] = i + 1;

    int res;
    LINEAR_SORT(res, orden_colores, r, n + 2, c, card, {
        // Mapeamos el color a su cardinalidad
        card = tabla_cardinalidades[c - 1];
    });

    free(tabla_cardinalidades);
    return res;
}

char ElimGarak(Grafo G, u32* Orden)
{
    char res = 1;
    u32 r = CalcularMaxColor(G);

    u32* orden_colores = malloc(r * sizeof(u32));
    if (orden_colores == NULL)
        goto error;

    if (OrdenarColoresPorCardinalidadVerElimGarak(G, r, orden_colores) != 0)
        goto error;

    if (OrdenarVerticesEnBloques(Orden, orden_colores, r, G) != 0)
        goto error;

    res = 0;

error:
    free(orden_colores);
    return res;
}
