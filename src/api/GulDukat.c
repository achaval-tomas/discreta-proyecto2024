#include "API2024Parte2.h"
#include "Util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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

static void ConstruirTablaCantColoresMul4(u32* tabla_cant_colores, u32* tabla_M, u32 tam, u32 r)
{
    memset(tabla_cant_colores, 0, tam * sizeof(u32));

    color mul4 = 4;
    while (mul4 <= r) {
        u32 M = tabla_M[mul4 - 1];
        tabla_cant_colores[M] += 1;
        mul4 += 4;
    }
}

static void ConstruirTablaCantColoresPar(u32* tabla_cant_colores, u32* tabla_M, u32* tabla_m, u32 tam, u32 r)
{
    memset(tabla_cant_colores, 0, tam * sizeof(u32));

    color par = 2;
    while (par <= r) {
        u32 M_mas_m = tabla_M[par - 1] + tabla_m[par - 1];
        tabla_cant_colores[M_mas_m] += 1;
        par += 4;
    }
}

static void ConstruirTablaCantColoresImpar(u32* tabla_cant_colores, u32* tabla_m, u32 tam, u32 r)
{
    memset(tabla_cant_colores, 0, tam * sizeof(u32));

    color impar = 1;
    while (impar <= r) {
        u32 m = tabla_m[impar - 1];
        tabla_cant_colores[m] += 1;
        impar += 2;
    }
}

static void ConstruirTablaPosiciones(u32* tabla_posiciones, u32* tabla_cantidad, u32 tam)
{
    tabla_posiciones[tam - 1] = 0;
    u32 i = tam - 1;
    while (i != 0) {
        --i;
        tabla_posiciones[i] = tabla_posiciones[i + 1] + tabla_cantidad[i + 1];
    }
}

char GulDukat(Grafo G, u32* Orden)
{
    char error = 0;
    color r = CalcularMaxColor(G);
    
    // tabla_M[i] = M(i+1) = máx{Grado(j, G) : Color(j, G) == i+1}
    // tabla_m[i] = m(i+1) = min{Grado(j, G) : Color(j, G) == i+1}
    u32* tabla_M = calloc(r, sizeof(u32));
    u32* tabla_m = calloc(r, sizeof(u32));
    u32 max_M_mas_m = CompletarTablas(tabla_M, tabla_m, r, G);

    // Aquí estarán los colores en el orden x1, ..., xr
    color* orden_colores = calloc(r, sizeof(color));

    // las siguientes tablas deben tener espacio para los valores
    // del conjunto {0, ..., max_M_mas_m}
    u32 tam_tablas = max_M_mas_m + 1;

    // tabla_cantidad_colores[i] tiene la cantidad de colores con M(x) == i
    u32* tabla_cantidad_colores = calloc(tam_tablas, sizeof(u32));
    ConstruirTablaCantColoresMul4(tabla_cantidad_colores, tabla_M, tam_tablas, r);

    // tabla_posiciones[i] tiene el índice del próx color con M(x) == i
    u32* tabla_posiciones = calloc(tam_tablas, sizeof(u32));
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, tam_tablas);

    for (color c = 4; c <= r; c += 4) {
        u32 M = tabla_M[c - 1];
        u32 pos = tabla_posiciones[M];
        orden_colores[pos] = c;
        tabla_posiciones[M] += 1;
    }

    u32 cantMul4 = r / 4;
    // tabla_cantidad_colores[i] tiene la cantidad de colores con M(x) + m(x) == i
    ConstruirTablaCantColoresPar(tabla_cantidad_colores, tabla_M, tabla_m, tam_tablas, r);
    // actualizo tabla_posiciones con esta nueva tabla de colores
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, tam_tablas);
    for (color c = 2; c <= r; c += 4) {
        u32 M_mas_m = tabla_M[c - 1] + tabla_m[c - 1];
        u32 pos = tabla_posiciones[M_mas_m];
        // sumo la cantidad de colores múltiplos de 4 porque estos están antes en el orden
        orden_colores[pos + cantMul4] = c;
        tabla_posiciones[M_mas_m] += 1;
    }

    u32 cantPares = r / 2;
    // tabla_cantidad_colores[i] tiene la cantidad de colores con m(x) == i
    ConstruirTablaCantColoresImpar(tabla_cantidad_colores, tabla_m, tam_tablas, r);
    // actualizo tabla_posiciones con esta nueva tabla de colores
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, tam_tablas);

    for (color c = 1; c <= r; c += 2) {
        u32 m = tabla_m[c - 1];
        u32 pos = tabla_posiciones[m];
        // sumo la cantidad de colores pares porque estos están antes en el orden
        orden_colores[pos + cantPares] = c;
        tabla_posiciones[m] += 1;
    }

    OrdenarVerticesEnBloques(Orden, orden_colores, r, G);

    free(orden_colores);
    free(tabla_cantidad_colores);
    free(tabla_posiciones);
    free(tabla_M);
    free(tabla_m);

    return error;
}
