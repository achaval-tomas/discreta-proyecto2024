#include "Sort.h"
#include <assert.h>
#include <string.h>

static void Merge(u32* arr, u32* buf, u32 middle, u32 length, CmpFn cmp, void* user_data)
{
    u32 i = 0, j = middle;
    for (u32 k = 0; k < length; k++) {
        if (j == length) {
            assert(i < middle);
            buf[k] = arr[i];
            i++;
        } else if (i == middle) {
            assert(j < length);
            buf[k] = arr[j];
            j++;
        } else {
            assert(i < middle && j < length);
            if (cmp(arr[i], arr[j], user_data) <= 0) {
                buf[k] = arr[i];
                i++;
            } else {
                buf[k] = arr[j];
                j++;
            }
        }
    }
    assert(i == middle && j == length);
    memcpy(arr, buf, length * sizeof(u32));
}

static void MergeSortRec(u32* arr, u32* buf, u32 length, CmpFn cmp, void* user_data)
{
    if (length <= 1) {
        return;
    }

    u32 middle = length / 2; // Always > 0

    MergeSortRec(arr, buf, middle, cmp, user_data);
    MergeSortRec(arr + middle, buf + middle, length - middle, cmp, user_data);
    Merge(arr, buf, middle, length, cmp, user_data);
}

void MergeSort(u32* arr, u32 length, CmpFn cmp, void* user_data)
{
    u32* buf = malloc(length * sizeof(u32));
    MergeSortRec(arr, buf, length, cmp, user_data);
    free(buf);
}

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
    for (u32 i = lft; i < rgt; ++i) {
        if (VaAntes(a[i], a[rgt], m, M, tipo)) {
            Swap(a, i, piv);
            ++piv;
        }
    }

    Swap(a, rgt, piv);
    return piv;
}

void qSort(u32* arr, u32 izq, u32 der, u32* m, u32* M, char tipo)
{
    if (izq < der) {
        u32 ppiv = Partition(arr, izq, der, m, M, tipo);
        // printf("ppiv, lft, rgt: %d %d %d\n", ppiv, izq, der);
        if (ppiv > 0)
            qSort(arr, izq, ppiv - 1, m, M, tipo);
        qSort(arr, ppiv + 1, der, m, M, tipo);
    }
}

/* Quick Sort ordenará el arreglo arr en el orden establecido por
 * la función goes_before utilizando los m y M correspondientes
 * para el tipo seleccionado.
 */
void QuickSort(u32* arr, u32 n, u32* m, u32* M, char tipo)
{
    qSort(arr, 0, n, m, M, tipo);
}

void LinearSort(u32* arr, u32 length, u32 map_max, u32 (*map)(u32, void*), void* user_data)
{
    u32* occurrences = calloc(map_max + 1, sizeof(u32));
    for (u32 i = 0; i < length; i++) {
        u32 value = arr[i];
        u32 mapped = map(value, user_data);

        assert(mapped <= map_max);

        occurrences[mapped] += 1;
    }

    u32* positions = calloc(map_max + 1, sizeof(u32));
    for (u32 i = 1; i <= map_max; i++) {
        positions[i] = positions[i - 1] + occurrences[i - 1];
    }

    u32* buf = malloc(length * sizeof(u32));

    for (u32 i = 0; i < length; i++) {
        u32 value = arr[i];
        u32 mapped = map(value, user_data);

        u32 pos = positions[mapped];
        positions[mapped] += 1;

        buf[pos] = value;
    }

    memcpy(arr, buf, length * sizeof(u32));
    free(occurrences);
    free(positions);
    free(buf);
}