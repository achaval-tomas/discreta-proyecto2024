#include "Sort.h"
#include "API2024Parte2.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int cmp_u32(u32 lhs, u32 rhs, void*)
{
    return (lhs - rhs);
}

void PrintArray(u32* arr, u32 n)
{
    printf("[");
    for (u32 i = 0; i < n; ++i) {
        if (i != 0) {
            printf(", ");
        }
        printf("%u", arr[i]);
    }
    printf("]\n");
}

u32 Count(u32* arr, u32 length, u32 v)
{
    u32 count = 0;
    for (u32 i = 0; i < length; i++) {
        count += (arr[i] == v);
    }
    return count;
}

int CheckSort(u32* original, u32* sorted, u32 length)
{
    for (u32 i = 0; i < length; i++) {
        u32 v = sorted[i];
        if (i > 0 && sorted[i - 1] > v) {
            printf("Array not correctly sorted\n");
            return 0;
        }

        if (Count(sorted, length, v) != Count(original, length, v)) {
            printf("Array has lost or added values\n");
            return 0;
        }
    }

    return 1;
}

int TestSort(u32* arr, u32 length)
{
    u32* sorted = malloc(length * sizeof(u32));
    memcpy(sorted, arr, length * sizeof(u32));
    printf("Before: ");
    PrintArray(sorted, length);
    MergeSort(sorted, length, cmp_u32, NULL);
    printf("After:  ");
    PrintArray(sorted, length);
    int result = CheckSort(arr, sorted, length);
    free(sorted);
    return result;
}

#define TEST_SORT(...)                             \
    {                                              \
        u32 arr[] = { __VA_ARGS__ };               \
        u32 length = sizeof(arr) / sizeof(arr[0]); \
        int result = TestSort(arr, length);        \
        assert(result == 1);                       \
    }


int main()
{
    TEST_SORT();
    TEST_SORT(0);
    TEST_SORT(1);
    TEST_SORT(0, 1);
    TEST_SORT(1, 0);
    TEST_SORT(0, 1, 2);
    TEST_SORT(2, 1, 0);
    TEST_SORT(0, 1, 2, 3);
    TEST_SORT(3, 2, 1, 0);
    TEST_SORT(0, 1, 2, 3, 4);
    TEST_SORT(4, 3, 2, 1, 0);
    TEST_SORT(3, 2, 5, 2, 1, 5);
    TEST_SORT(4, 4, 4, 4, 4);


    return 0;
}
