#include "API2024Parte2.h"
#include <assert.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void Merge(u32* arr, u32 start, u32 middle, u32 end, u32* buf) {
    u32 i = start, j = middle;
    for (u32 k = start; k < end; k++) {
        if (i < middle && j < end) {
            assert(i < middle && j < end);
            if (arr[i] <= arr[j]) {
                buf[k] = arr[i];
                i++;
            } else {
                buf[k] = arr[j];
                j++;
            }
        } else if (j == end) {
            assert(i < middle && j == end);
            buf[k] = arr[i];
            i++;
        } else {
            assert(i == middle && j < end);
            buf[k] = arr[j];
            j++;
        }
    }
    assert(i == middle && j == end);
    memcpy(arr + start, buf + start, (end - start) * sizeof(u32));
}

void MergeSortRec(u32* arr, u32 start, u32 end, u32* buf) {
    assert(start <= end);

    if ((end - start) <= 1) {
        return;
    }

    u32 middle = (start + end) / 2;

    MergeSortRec(arr, start, middle, buf);
    MergeSortRec(arr, middle, end, buf);
    Merge(arr, start, middle, end, buf);
}

void MergeSort(u32* arr, u32 length) {
    u32* buf = malloc(length * sizeof(u32));
    memset(buf, ~0, length * sizeof(u32));
    MergeSortRec(arr, 0, length, buf);
    free(buf);
}

// Asume que ambos ordenes tienen r lugares de memoria.
// Complejidad O(n)
void CompletarOrdenes (u32* orden_M, u32* orden_m, u32 r, Grafo G){
    u32 n = NumeroDeVertices(G);

    for (u32 i = 0; i<r; ++i){
        orden_M[i] = 0;
        orden_m[i] = n;
    }

    for (u32 i = 0; i<n; ++i){
        u32 idx = Color(i, G) - 1;
        u32 grado = Grado(i, G);
        orden_M[idx] = MAX(orden_M[idx], grado);
        orden_m[idx] = MIN(orden_m[idx], grado);
    }
}

// Se asume que Orden apunta a una region de memoria con n lugares y
// que G tiene un coloreo propio con colores {1, 2, .., r} para algún r.
char GulDukat(Grafo G, u32* Orden){
    u32 n = NumeroDeVertices(G);
    char error = 0;

    u32 r = 0;
    for (u32 i = 0; i<n; ++i)
        r = MAX(Color(i, G), r);

    u32* orden_M = calloc(r, sizeof(color));
    u32* orden_m = calloc(r, sizeof(color));

    CompletarOrdenes(orden_M, orden_m, r, G);
    /* orden_M contiene el máximo grado de vertice por color.
     * orden_m contiene el mínimo grado de vertice por color.
     */

    // Aquí estarán los colores en el orden x1,...,xr
    color* orden_colores = calloc(r, sizeof(color));
    u32 idx = 0;

    /* Los 3 for loops que siguen utilizan SELECTION SORT
     * y son O(r*r)
     */

    // Ordenar los colores divisibles por 4 según M de mayor a menor.
    for (u32 i = 4; i<=r; i+=4){
        color max = 4;
        for (u32 j = 4; j<=r; j+=4){
            if (orden_M[j-1] > orden_M[max-1])
                max = j;
        }
        orden_colores[idx++] = max;
        orden_M[max-1] = 0;
    }

    /* Ordenar los colores divisibles por 2 y no por 4
     * segun M+m de mayor a menor.
     */
    for (u32 i = 2; i<=r; i+=4){
        color max = 2;
        u32 maxPrio = 0;
        for (u32 j = 2; j<=r; j+=4){
            u32 j_prio = orden_M[j-1] + orden_m[j-1];
            if (j_prio > maxPrio){
                max = j;
                maxPrio = j_prio;
            }
        }
        orden_colores[idx++] = max;
        orden_M[max-1] = 0;
        orden_m[max-1] = 0;
    }

    // Ordenar los colores impares según m de mayor a menor.
    for (u32 i = 1; i<=r; i+=2){
        color max = 1;
        for (u32 j = 1; j<=r; j+=2){
            if (orden_m[j-1] > orden_m[max-1])
                max = j;
        }
        orden_colores[idx++] = max;
        orden_m[max-1] = 0;
    }

    free(orden_M); free(orden_m);

    if (idx !=  r)
        error = 1;

    // orden_colores tiene el orden x1, ..., xr a utilizar.

    u32* indexes = calloc(r, sizeof(u32));
    u32* vert_por_color = calloc(r, sizeof(u32));

    for (u32 i = 0; i<n; ++i)
        vert_por_color[Color(i, G)-1] += 1;

    u32 acc = 0;
    for (u32 i = 0; i<r; ++i) {
        color c = orden_colores[i];
        indexes[c-1] = acc;
        acc += vert_por_color[c-1];
    }

    for (u32 i = 0; i<n; ++i){
        u32 index = indexes[Color(i, G)-1]++;
        Orden[index] = i;
    }

    free(orden_colores); free(indexes); free(vert_por_color);
    return error;
}

char ElimGarak(Grafo G, u32* Orden){
    // para evitar warnings
    Orden[0] = 0;
    if (Orden[0] < Color(0, G))
        return 1;
    return 0;
};