#include "Sort.h"
#include <assert.h>
#include <string.h>

// Nuestra versión del counting sort.
int LinearSort(u32* arr, u32 length, u32 map_max, u32 (*map)(u32, void*), void* user_data)
{
    int res;
    LINEAR_SORT(res, arr, length, map_max, x, f_x, {
        f_x = map(x, user_data);
    });
    return res;
}