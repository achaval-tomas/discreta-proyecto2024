#include "API2024Parte2.h"
#include "Math.h"
#include "Sort.h"
#include "Util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
Asume que tabla_M y tabla_m apuntan c/u a una region de memoria con r lugares.
Devuelve el máximo valor encontrado para un par M[i] + m[i].
Complejidad: O(n)
*/
static u32 CompletarTablas(u32* tabla_M, u32* tabla_m, u32 r, Grafo G)
{
    u32 n = NumeroDeVertices(G);
    u32 max_sum = 0;

    for (u32 i = 0; i < r; ++i) {
        tabla_M[i] = 0;
        tabla_m[i] = n;
    }

    for (u32 i = 0; i < n; ++i) {
        u32 pos = Color(i, G) - 1;
        u32 grado = Grado(i, G);
        tabla_M[pos] = MAX(tabla_M[pos], grado);
        tabla_m[pos] = MIN(tabla_m[pos], grado);
        max_sum = MAX(max_sum, tabla_M[pos] + tabla_m[pos]);
    }

    return max_sum;
}

char GulDukat(Grafo G, u32* Orden)
{
    char rc = 1;
    color r = CalcularMaxColor(G);

    // tabla_M[i] = M(i+1) = máx{Grado(j, G) : Color(j, G) == i+1}
    // tabla_m[i] = m(i+1) = min{Grado(j, G) : Color(j, G) == i+1}
    u32* tabla_M = calloc(r, sizeof(u32));
    if (tabla_M == NULL)
        goto error_tabla_M;

    u32* tabla_m = calloc(r, sizeof(u32));
    if (tabla_m == NULL)
        goto error_tabla_m;

    u32 max_M_mas_m = CompletarTablas(tabla_M, tabla_m, r, G);

    // Aquí estarán los colores en el orden x1, ..., xr
    color* orden_colores = calloc(r, sizeof(color));
    if (orden_colores == NULL)
        goto error_orden_colores;

    u32 i = 0;
    for (color c = 4; c <= r; c += 4, ++i)
        orden_colores[i] = c;
    u32 cantMul4 = i;

    for (color c = 2; c <= r; c += 4, ++i)
        orden_colores[i] = c;
    u32 cantParesNoMul4 = i - cantMul4;

    for (color c = 1; c <= r; c += 2, ++i)
        orden_colores[i] = c;
    u32 cantImpares = r - cantMul4 - cantParesNoMul4;

    char res = 0;
    LINEAR_SORT(res, orden_colores, cantMul4, max_M_mas_m, c, val, {
        // Mapeamos el color c al máximo - M(c) para que el orden
        // sea de mayor a menor
        val = max_M_mas_m - tabla_M[c - 1];
    });
    if (res)
        goto error;

    // Ordena los colores desde el índice cantMul4 (los anteriores ya están ordenados)
    LINEAR_SORT(res, orden_colores + (cantMul4), cantParesNoMul4, max_M_mas_m, c, val, {
        // Mapeamos el color c al máximo - M(c) - m(c) para que el orden
        // sea de mayor a menor
        val = max_M_mas_m - (tabla_M[c - 1] + tabla_m[c - 1]);
    });
    if (res)
        goto error;

    // Ordena los colores desde el índice (cantMul4 + cantParesNoMul4)
    LINEAR_SORT(res, orden_colores + (cantMul4 + cantParesNoMul4), cantImpares, max_M_mas_m, c, val, {
        // Mapeamos el color c al máximo - m(c) para que el orden
        // sea de mayor a menor
        val = max_M_mas_m - (tabla_m[c - 1]);
    });
    if (res)
        goto error;

    if (OrdenarVerticesEnBloques(Orden, orden_colores, r, G) != 0)
        goto error;

    rc = 0;

error:
error_orden_colores:
    free(orden_colores);
error_tabla_m:
    free(tabla_m);
error_tabla_M:
    free(tabla_M);

    return rc;
}
