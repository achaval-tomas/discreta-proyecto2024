#include "API2024Parte2.h"
#include "Math.h"
#include "Sort.h"
#include <assert.h>

/*
G debe ser un grafo previamente coloreado por Greedy.
Complejidad: O(n)
*/
u32 CalcularMaxColor(Grafo G)
{
    u32 max_color = 0;
    for (u32 v = 0; v < NumeroDeVertices(G); v++) {
        assert(Color(v, G) > 0); // TODO
        max_color = MAX(max_color, Color(v, G));
    }
    return max_color;
}

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
        Orden[v] = v;
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

    u32* tabla_cardinalidades = CalcularCardinalidades(G, r);
    // Hack para que los colores 1 y 2 tengan cardinalidades muy altas,
    // lo que luego se traduce a que los vertices con estos colores queden
    // al final.
    tabla_cardinalidades[0] = UINT32_MAX;
    tabla_cardinalidades[1] = UINT32_MAX - 1;

    struct ElimGarakCmpData data = { G, tabla_cardinalidades };

    InicializarOrden(Orden, n);
    MergeSort(Orden, n, CmpVerticesPorColor, (void*)(&data));
    MergeSort(Orden, n, CmpVerticesPorCardinalidad, (void*)(&data));

    free(tabla_cardinalidades);
    return 1;
}
