#include "../include/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// protected function, that only this .c can use
int comparator_func(const void *a, const void *b) {
    return *(uint8_t*)a - *(uint8_t*)b;
}

bool terrible_sort(uint16_t *data_array, const size_t value_count) {

    if (!data_array) {
        return false;
    }
    if (value_count == 0) {
        return false;
    }

    uint16_t* sorting_array = malloc(value_count * sizeof(*data_array));
    if (!sorting_array) {
        return false;
    }

    for (size_t i = 0; i < value_count; ++i) {
        sorting_array[i] = data_array[i];
    }

    size_t elm_size = sizeof(uint16_t);
    qsort(sorting_array, value_count, elm_size, comparator_func);

    bool sorted = true;
    for (size_t i = 0; i < (value_count-1); ++i) {
        if (sorting_array[i] > sorting_array[i + 1]) {
            sorted = false;
            break;
        }
    }

    if (sorted) {
        memcpy(data_array, sorting_array, elm_size*value_count);
    }

    free(sorting_array);
    return sorted;
}

