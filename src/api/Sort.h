#ifndef SORT_H
#define SORT_H

#include "API2024Parte2.h"
#include <assert.h>
#include <string.h>

int LinearSort(u32* arr, u32 length, u32 map_max, u32 (*map)(u32, void*), void* user_data);

/*
Macro for inline linear sort.
res: int is the result; 0 if successful, 1 if error (i.e., out of memory).
arr: u32* is the pointer to the array to be sorted.
length: u32 is the length of arr.
f_x_max: u32 is the maximum value that can be returned by map.
x is the name of the input variable used inside map.
f_x is the name of the output variable used inside map.
map is a scope which has available u32 x and u32 f_x.
Complexity: O(n)
*/
#define LINEAR_SORT(res, arr, length, f_x_max, x, f_x, map)      \
    {                                                            \
        res = 0;                                                 \
        u32* _arr_ = arr;                                        \
        u32 _length_ = length;                                   \
        u32 _f_x_max_ = f_x_max;                                 \
                                                                 \
        u32* _occurrences_ = calloc(_f_x_max_ + 1, sizeof(u32)); \
        if (_occurrences_ == NULL) {                             \
            res = 1;                                             \
        } else {                                                 \
            for (u32 _i_ = 0; _i_ < _length_; _i_++) {           \
                u32 x = _arr_[_i_];                              \
                u32 f_x;                                         \
                map;                                             \
                                                                 \
                assert(f_x <= _f_x_max_);                        \
                                                                 \
                _occurrences_[f_x] += 1;                         \
            }                                                    \
                                                                 \
            /* convert occurrences into their prefix sums */     \
            u32 _accumulator_ = 0;                               \
            for (u32 _i_ = 0; _i_ <= _f_x_max_; _i_++) {         \
                u32 _tmp_ = _occurrences_[_i_];                  \
                _occurrences_[_i_] = _accumulator_;              \
                _accumulator_ += _tmp_;                          \
            }                                                    \
                                                                 \
            /* rename for clarity */                             \
            u32* _positions_ = _occurrences_;                    \
                                                                 \
            u32* _buf_ = malloc(_length_ * sizeof(u32));         \
            if (_buf_ == NULL) {                                 \
                res = 1;                                         \
            } else {                                             \
                for (u32 _i_ = 0; _i_ < _length_; _i_++) {       \
                    u32 x = _arr_[_i_];                          \
                    u32 f_x;                                     \
                    map;                                         \
                                                                 \
                    u32 _pos_ = _positions_[f_x];                \
                    _positions_[f_x] += 1;                       \
                                                                 \
                    _buf_[_pos_] = x;                            \
                }                                                \
                                                                 \
                memcpy(_arr_, _buf_, _length_ * sizeof(u32));    \
                                                                 \
                free(_buf_);                                     \
            }                                                    \
                                                                 \
            free(_occurrences_);                                 \
        }                                                        \
    }

#endif
