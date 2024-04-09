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
u32* CalcularCardinalidades(Grafo G, u32 r)
{
    u32* cardinalidades = calloc(r, sizeof(u32));

    for (u32 v = 0; v < NumeroDeVertices(G); v++) {
        u32 c = Color(v, G);
        cardinalidades[c - 1] += 1;
    }

    return cardinalidades;
}

void InicializarOrden(u32* Orden, u32 n)
{
    for (u32 v = 0; v < n; v++) {
        Orden[v] = 0;
    }
}

struct ElimGarakCmpData {
    Grafo G;
    const u32* cardinalidades;
};

int CmpVerticesPorColor(u32 lhs, u32 rhs, void* user_data)
{
    struct ElimGarakCmpData* data = (struct ElimGarakCmpData*)(user_data);
    Grafo G = data->G;

    u32 col_lhs = Color(lhs, G);
    u32 col_rhs = Color(rhs, G);

    return col_lhs - col_rhs;
}

int CmpVerticesPorCardinalidad(u32 lhs, u32 rhs, void* user_data)
{
    struct ElimGarakCmpData* data = (struct ElimGarakCmpData*)(user_data);
    Grafo G = data->G;
    const u32* cardinalidades = data->cardinalidades;

    u32 col_lhs = Color(lhs, G);
    u32 col_rhs = Color(rhs, G);

    u32 card_lhs = cardinalidades[col_lhs - 1];
    u32 card_rhs = cardinalidades[col_rhs - 1];

    if (card_lhs < card_rhs) {
        return -1;
    } else if (card_lhs > card_rhs) {
        return 1;
    } else {
        return 0;
    }
}

char ElimGarak(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);
    u32 r = CalcularMaxColor(G);

    memset(Orden, ~0, n * sizeof(u32));

    u32* tabla_cardinalidades = CalcularCardinalidades(G, r);

    u32* tabla_cantidades_colores = calloc(n + 1, sizeof(u32));
    for (u32 c = 0; c < r; c++) {
        u32 card = tabla_cardinalidades[c];
        tabla_cantidades_colores[card] += 1;
    }
    // tabla_cantidades_colores[i] tiene la cantidad de colores
    // de cardinalidad == i

    u32* tabla_posiciones_colores = calloc(n + 1, sizeof(u32));
    for (u32 i = 1; i < n + 1; i++) {
        tabla_posiciones_colores[i] = tabla_posiciones_colores[i - 1]
            + tabla_cantidades_colores[i - 1];
    }
    // tabla_posiciones_colores[i] tiene la cantidad (acumulada)
    // de colores con cardinalidad <= i

    u32* orden_colores = malloc(r * sizeof(u32));
    for (u32 c = 1; c <= r; c++) {
        u32 card = tabla_cardinalidades[c - 1];
        u32 pos = tabla_posiciones_colores[card];
        tabla_posiciones_colores[card] += 1;
        orden_colores[pos] = c;
    }
    // orden_colores: { 1, ..., r } -> { x1, ..., xr }
    // orden_colores tiene x1, ..., xr

    OrdenarVerticesEnBloques(Orden, orden_colores, r, G);

    free(tabla_cardinalidades);
    free(tabla_cantidades_colores);
    free(tabla_posiciones_colores);
    free(orden_colores);
    return 0;
}
