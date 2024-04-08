#include "API2024Parte2.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

char goes_before(color a, color b, u32* m, u32* M, char type)
{
    u32 ret = 0;
    switch (type) {
    case '1':
        ret = M[a - 1] >= M[b - 1];
        break;
    case '2':
        ret = (m[a - 1] + M[a - 1])
            >= (m[b - 1] + M[b - 1]);
        break;
    case '3':
        ret = m[a - 1] >= m[b - 1];
        break;
    }

    return ret;
}

void swap(color* a, u32 i, u32 j)
{
    color tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

u32 partition(color* a, u32 lft, u32 rgt, u32* m, u32* M, char type)
{   
    int piv = lft;
    u32 i = lft;
    while (i < rgt){
        if (goes_before(a[i], a[rgt], m , M, type)){
            swap(a, i, piv);
            ++piv;
        }
        ++i;
    }

    swap(a, rgt, piv);
    return piv;
}

void qSort(u32* arr, u32 lft, u32 rgt, u32* m, u32* M, char type)
{
    if (lft < rgt) {
        u32 ppiv = partition(arr, lft, rgt, m, M, type);
        printf("ppiv, lft, rgt: %d %d %d\n", ppiv, lft, rgt);
        qSort(arr, lft, ppiv - 1, m, M, type);
        qSort(arr, ppiv + 1, rgt, m, M, type);
    }
}

void quickSort(u32* arr, u32 n, u32* m, u32* M, char type)
{
    qSort(arr, 0, n, m, M, type);
}

// Asume que ambos ordenes tienen r lugares de memoria.
// Complejidad O(n)
void CompletarOrdenes(u32* orden_M, u32* orden_m, u32 r, Grafo G)
{
    u32 n = NumeroDeVertices(G);

    for (u32 i = 0; i < r; ++i) {
        orden_M[i] = 0;
        orden_m[i] = n;
    }

    for (u32 i = 0; i < n; ++i) {
        u32 idx = Color(i, G) - 1;
        u32 grado = Grado(i, G);
        orden_M[idx] = MAX(orden_M[idx], grado);
        orden_m[idx] = MIN(orden_m[idx], grado);
    }
}

// Se asume que Orden apunta a una region de memoria con n lugares y
// que G tiene un coloreo propio con colores {1, 2, .., r} para algún r.
char GulDukat(Grafo G, u32* Orden)
{
    u32 n = NumeroDeVertices(G);
    char error = 0;

    u32 r = 0;
    for (u32 i = 0; i < n; ++i)
        r = MAX(Color(i, G), r);

    u32* orden_M = calloc(r, sizeof(color));
    u32* orden_m = calloc(r, sizeof(color));

    u32* orden_M2 = calloc(r, sizeof(color));
    u32* orden_m2 = calloc(r, sizeof(color));
    CompletarOrdenes(orden_M2, orden_m2, r, G);


    CompletarOrdenes(orden_M, orden_m, r, G);
    /* orden_M contiene el máximo grado de vertice por color.
     * orden_m contiene el mínimo grado de vertice por color.
     */

    // Aquí estarán los colores en el orden x1,...,xr
    color* orden_colores = calloc(r, sizeof(color));

    /* Los 3 for loops que siguen utilizan SELECTION SORT
     * y son O(r*r)
     */

    u32 i = 0;
    u32 mul4 = 4;
    while (mul4 <= r) {
        orden_colores[i] = mul4;
        mul4 += 4;
        ++i;
    }
    qSort(orden_colores, 0, i - 1, orden_m, orden_M, '1');


    u32 prev_i = i;
    u32 pair = 2;
    while (pair <= r) {
        orden_colores[i] = pair;
        pair += 4;
        ++i;
    }
    qSort(orden_colores, prev_i, i - 1, orden_m, orden_M, '2');

    prev_i = i;
    u32 odd = 1;
    while (odd <= r) {
        orden_colores[i] = odd;
        odd += 2;
        ++i;
    }
    qSort(orden_colores, prev_i, i - 1, orden_m, orden_M, '3');

    for (u32 i = 0; i < r; ++i) {
        printf("%d ", orden_colores[i]);
    }
    printf("\n");


    // Ordenar los colores divisibles por 4 según M de mayor a menor.
    /*for (u32 i = 4; i <= r; i += 4) {
        color max = 4;
        for (u32 j = 4; j <= r; j += 4) {
            if (orden_M[j - 1] > orden_M[max - 1])
                max = j;
        }
        orden_colores[idx++] = max;
        orden_M[max - 1] = 0;
    }*/

    /* Ordenar los colores divisibles por 2 y no por 4
     * segun M+m de mayor a menor.
     */
    /*for (u32 i = 2; i <= r; i += 4) {
        color max = 2;
        u32 maxPrio = 0;
        for (u32 j = 2; j <= r; j += 4) {
            u32 j_prio = orden_M[j - 1] + orden_m[j - 1];
            if (j_prio > maxPrio) {
                max = j;
                maxPrio = j_prio;
            }
        }
        orden_colores[idx++] = max;
        orden_M[max - 1] = 0;
        orden_m[max - 1] = 0;
    }

    // Ordenar los colores impares según m de mayor a menor.
    for (u32 i = 1; i <= r; i += 2) {
        color max = 1;
        for (u32 j = 1; j <= r; j += 2) {
            if (orden_m[j - 1] > orden_m[max - 1])
                max = j;
        }
        orden_colores[idx++] = max;
        orden_m[max - 1] = 0;
    }*/


    free(orden_M);
    free(orden_m);

    /*if (idx != r)
        error = 1;*/

    // orden_colores tiene el orden x1, ..., xr a utilizar.

    u32* indexes = calloc(r, sizeof(u32));
    u32* vert_por_color = calloc(r, sizeof(u32));

    for (u32 i = 0; i < n; ++i)
        vert_por_color[Color(i, G) - 1] += 1;

    u32 acc = 0;
    for (u32 i = 0; i < r; ++i) {
        color c = orden_colores[i];
        indexes[c - 1] = acc;
        acc += vert_por_color[c - 1];
    }

    for (u32 i = 0; i < n; ++i) {
        u32 index = indexes[Color(i, G) - 1]++;
        Orden[index] = i;
    }

    for (u32 i = 0; i < r; ++i) {
        printf("%u (%u, %u)\n", orden_colores[i], orden_M2[orden_colores[i] - 1], orden_m2[orden_colores[i] - 1]);
    }

    printf("\n");

    free(orden_colores);
    free(indexes);
    free(vert_por_color);
    free(orden_M2);
    free(orden_m2);
    return error;
}

char ElimGarak(Grafo G, u32* Orden)
{
    // para evitar warnings
    Orden[0] = 0;
    if (Orden[0] < Color(0, G))
        return 1;
    return 0;
};