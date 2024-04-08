#ifndef SORT_H
#define SORT_H

#include "APIG24.h"

typedef int (*CmpFn)(u32 lhs, u32 rhs, void* user_data);

void MergeSort(u32* arr, u32 length, CmpFn cmp, void* user_data);

#endif
