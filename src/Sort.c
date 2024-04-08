#include "Sort.h"
#include <assert.h>
#include <string.h>

static void Merge(u32* arr, u32 start, u32 middle, u32 end, u32* buf, CmpFn cmp, void* user_data)
{
    u32 i = start, j = middle;
    for (u32 k = start; k < end; k++) {
        if (i < middle && j < end) {
            assert(i < middle && j < end);
            if (cmp(arr[i], arr[j], user_data) <= 0) {
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

static void MergeSortRec(u32* arr, u32 start, u32 end, u32* buf, CmpFn cmp, void* user_data)
{
    assert(start <= end);

    if ((end - start) <= 1) {
        return;
    }

    u32 middle = (start + end) / 2;

    MergeSortRec(arr, start, middle, buf, cmp, user_data);
    MergeSortRec(arr, middle, end, buf, cmp, user_data);
    Merge(arr, start, middle, end, buf, cmp, user_data);
}

void MergeSort(u32* arr, u32 length, CmpFn cmp, void* user_data)
{
    u32* buf = malloc(length * sizeof(u32));
    MergeSortRec(arr, 0, length, buf, cmp, user_data);
    free(buf);
}
