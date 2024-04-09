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
