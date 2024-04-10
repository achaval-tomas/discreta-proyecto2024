#include "API2024Parte2.h"
#include "Util.h"
#include <assert.h>
#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define GRUPO_1 '1' // Múltiplos de 4
#define GRUPO_2 '2' // Pares no divisibles por 4
#define GRUPO_3 '3' // Impares

void RevisarResultado(color* orden, u32 r, u32* m, u32* M) {
    char mul4 = 1;
    char even = 0;
    char odd = 0;

    u32 i = 0;

    while (i < r-1) {
        color c0 = orden[i];
        color c1 = orden[i+1];

        if (mul4 && (c1 % 4 == 0)) {
            assert(M[c0-1] >= M[c1-1]);
        } else if (mul4 && (c1 % 4 != 0) && (c1 % 2 == 0)) {
            mul4 = 0;
            even = 1;
        } else if (even && (c1 % 2 == 0)) {
            assert(M[c0-1] + m[c0-1] >= M[c1-1] + m[c1-1]);
        } else if (even && (c1 % 2 != 0)) {
            even = 0;
            odd = 1;
        } else if (odd && (c1 % 2 == 1)) {
            assert(m[c0-1] >= m[c1-1]);
        } else {
            assert(0);
        }

        ++i;
    }
}

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
    for (u32 i = lft; i < rgt; ++i){
        if (VaAntes(a[i], a[rgt], m , M, tipo)){
            Swap(a, i, piv);
            ++piv;
        }
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
        //printf("ppiv, lft, rgt: %d %d %d\n", ppiv, izq, der);
        if (ppiv > 0)
            QuickSort(arr, izq, ppiv - 1, m, M, tipo);
        QuickSort(arr, ppiv + 1, der, m, M, tipo);
    }
}

// Asume que ambos ordenes tienen r lugares de memoria.
// Complejidad O(n)
void CompletarTablas(u32* mul4, u32* par, u32* impar, u32 r, Grafo G)
{
    u32 n = NumeroDeVertices(G);
    u32* M = calloc(r, sizeof(color));
    u32* m = calloc(r, sizeof(color));

    for (u32 i = 0; i < r; ++i) {
        M[i] = 0;
        m[i] = n;
    }

    for (u32 i = 0; i < n; ++i) {
        u32 idx = Color(i, G) - 1;
        u32 grado = Grado(i, G);
        M[idx] = MAX(M[idx], grado);
        m[idx] = MIN(m[idx], grado);
    }

    for (color c = 1; c <= r; ++c) {
        mul4[c-1] = M[c-1];
        par[c-1] = M[c-1] + m[c-1];
        impar[c-1] = m[c-1];
    }

    free(M); free(m);
}

void ConstruirTablaCantColoresMul4(u32* tabla_cant_colores, u32* tabla_mul4, u32 n, u32 r) {
    for (u32 i = 0; i < 2*n; ++i)
        tabla_cant_colores[i] = 0;
    
    u32 mul4 = 4;
    while (mul4 <= r) {
        u32 M = tabla_mul4[mul4-1];
        tabla_cant_colores[M] += 1;
        mul4 += 4;
    }
}

void ConstruirTablaCantColoresPar(u32* tabla_cant_colores, u32* tabla_par, u32 n, u32 r) {
    for (u32 i = 0; i < 2*n; ++i)
        tabla_cant_colores[i] = 0;
    
    u32 par = 2;
    while (par <= r) {
        u32 M_mas_m = tabla_par[par-1];
        tabla_cant_colores[M_mas_m] += 1;
        par += 4;
    }
}

void ConstruirTablaCantColoresImpar(u32* tabla_cant_colores, u32* tabla_impar, u32 n, u32 r) {
    for (u32 i = 0; i < 2*n; ++i)
        tabla_cant_colores[i] = 0;
    
    u32 impar = 1;
    while (impar <= r) {
        u32 m = tabla_impar[impar-1];
        tabla_cant_colores[m] += 1;
        impar += 2;
    }
}

void ConstruirTablaPosiciones(u32* tabla_posiciones, u32* tabla_cantidad, u32 n) {
    tabla_posiciones[2*n - 1] = 0;
    u32 i = 2*n-2;
    while (i != 0) {
        tabla_posiciones[i] = tabla_posiciones[i+1] +
                              tabla_cantidad[i+1];
        --i;
    }
    tabla_posiciones[0] = tabla_posiciones[1] +
                          tabla_cantidad[1];
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

    // tabla_mul4[i] tiene el valor M(i+1)
    u32* tabla_mul4 = calloc(r, sizeof(u32));

    // tabla_par[i] tiene el valor M(i+1) + m(i+1)
    u32* tabla_par = calloc(r, sizeof(u32));

    // tabla_impar[i] tiene el valor m(i+1)
    u32* tabla_impar = calloc(r, sizeof(u32));

    CompletarTablas(tabla_mul4, tabla_par, tabla_impar, r, G);

    // Aquí estarán los colores en el orden x1,...,xr
    color* orden_colores = calloc(r, sizeof(color));

    // tabla_cantidad_colores[i] tiene la cantidad de colores con M(x) == i
    u32* tabla_cantidad_colores = calloc(2*n, sizeof(u32));
    ConstruirTablaCantColoresMul4(tabla_cantidad_colores, tabla_mul4, n, r);

    // tabla_posiciones[i] tiene el índice del próx color con M(x) == i
    u32* tabla_posiciones = calloc(2*n, sizeof(u32));
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, n);

    for (color c = 4; c <= r; c+=4) {
        u32 M = tabla_mul4[c-1];
        u32 pos = tabla_posiciones[M];
        orden_colores[pos] = c;
        tabla_posiciones[M] += 1;
    }

    u32 cantMul4 = r / 4;
    // tabla_cantidad_colores[i] tiene la cantidad de colores con M(x) + m(x) == i
    ConstruirTablaCantColoresPar(tabla_cantidad_colores, tabla_par, n, r);
    // actualizo tabla_posiciones con esta nueva tabla de colores
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, n);
    for (color c = 2; c <= r; c+=4) {
        u32 M_mas_m = tabla_par[c-1];
        u32 pos = tabla_posiciones[M_mas_m];
        // sumo la cantidad de colores múltiplos de 4 porque estos están antes en el orden
        orden_colores[pos+cantMul4] = c;
        tabla_posiciones[M_mas_m] += 1;
    }

    u32 cantPares = r / 2;
    // tabla_cantidad_colores[i] tiene la cantidad de colores con m(x) == i
    ConstruirTablaCantColoresImpar(tabla_cantidad_colores, tabla_impar, n, r);
    // actualizo tabla_posiciones con esta nueva tabla de colores
    ConstruirTablaPosiciones(tabla_posiciones, tabla_cantidad_colores, n);

    for (color c = 1; c <= r; c+=2) {
        u32 m = tabla_impar[c-1];
        u32 pos = tabla_posiciones[m];
        // sumo la cantidad de colores pares porque estos están antes en el orden
        orden_colores[pos+cantPares] = c;
        tabla_posiciones[m] += 1;
    }

    OrdenarVerticesEnBloques(Orden, orden_colores, r, G);

    free(orden_colores);
    free(tabla_cantidad_colores);
    free(tabla_posiciones);
    free(tabla_impar);
    free(tabla_par);
    free(tabla_mul4);

    return error;
}
