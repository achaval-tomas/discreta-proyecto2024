#include "API2024Parte2.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define GRUPO_1 '1' // Múltiplos de 4
#define GRUPO_2 '2' // Pares no divisibles por 4
#define GRUPO_3 '3' // Impares

// Orden de los colores de acuerdo a la consigna
char VaAntes(color a, color b, u32* m, u32* M, char tipo)
{
    u32 ret = 0;
    switch (tipo) {
    case GRUPO_1:
        ret = M[a - 1] >= M[b - 1];
        break;
    case GRUPO_2:
        ret = (m[a - 1] + M[a - 1])
            >= (m[b - 1] + M[b - 1]);
        break;
    case GRUPO_3:
        ret = m[a - 1] >= m[b - 1];
        break;
    }

    return ret;
}

void Swap(color* a, u32 i, u32 j)
{
    color tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

u32 Partition(color* a, u32 lft, u32 rgt, u32* m, u32* M, char tipo)
{   
    int piv = lft;
    u32 i = lft;
    while (i < rgt){
        if (VaAntes(a[i], a[rgt], m , M, tipo)){
            Swap(a, i, piv);
            ++piv;
        }
        ++i;
    }

    Swap(a, rgt, piv);
    return piv;
}

/* Quick Sort ordenará el arreglo arr en el orden establecido por
 * la función goes_before utilizando los m y M correspondientes
 * para el tipo seleccionado.
 */
void QuickSort(u32* arr, u32 izq, u32 der, u32* m, u32* M, char tipo)
{
    if (izq < der) {
        u32 ppiv = Partition(arr, izq, der, m, M, tipo);
        printf("ppiv, lft, rgt: %d %d %d\n", ppiv, izq, der);
        QuickSort(arr, izq, ppiv - 1, m, M, tipo);
        QuickSort(arr, ppiv + 1, der, m, M, tipo);
    }
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

/* Se asume que Orden apunta a una region de memoria con n lugares y
 * que G tiene un coloreo propio con colores {1, 2, .., r} para algún r.
 * Complejidad O(n + r*log(r) + n) ~ O(n)
 */
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

    u32 i = 0;
    u32 mul4 = 4;
    while (mul4 <= r) {
        orden_colores[i] = mul4;
        mul4 += 4;
        ++i;
    }
    QuickSort(orden_colores, 0, i - 1, orden_m, orden_M, GRUPO_1);


    u32 i_ant = i;
    u32 par = 2;
    while (par <= r) {
        orden_colores[i] = par;
        par += 4;
        ++i;
    }
    QuickSort(orden_colores, i_ant, i - 1, orden_m, orden_M, GRUPO_2);

    i_ant = i;
    u32 impar = 1;
    while (impar <= r) {
        orden_colores[i] = impar;
        impar += 2;
        ++i;
    }
    QuickSort(orden_colores, i_ant, i - 1, orden_m, orden_M, GRUPO_3);

    for (u32 i = 0; i < r; ++i) {
        printf("%d ", orden_colores[i]);
    }
    printf("\n");

    free(orden_M);
    free(orden_m);

    // orden_colores tiene el orden x1, ..., xr a utilizar.

    u32* indices = calloc(r, sizeof(u32));
    u32* vert_por_color = calloc(r, sizeof(u32));

    for (u32 i = 0; i < n; ++i)
        vert_por_color[Color(i, G) - 1] += 1;

    u32 acum = 0;
    for (u32 i = 0; i < r; ++i) {
        color c = orden_colores[i];
        indices[c - 1] = acum;
        acum += vert_por_color[c - 1];
    }

    for (u32 i = 0; i < n; ++i) {
        u32 ind = indices[Color(i, G) - 1]++;
        Orden[ind] = i;
    }

    for (u32 i = 0; i < r; ++i) {
        printf("%u (%u, %u)\n", orden_colores[i], orden_M2[orden_colores[i] - 1], orden_m2[orden_colores[i] - 1]);
    }

    printf("\n");

    free(orden_colores);
    free(indices);
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