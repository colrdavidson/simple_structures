#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define DynArr(type)       \
    struct {               \
        type *arr;         \
        uint64_t size;     \
        uint64_t capacity; \
    }

#define elem_size(dyn) sizeof(*((dyn)->arr))
#define mdyn_init(dyn, start_capacity)                          \
    do {                                                        \
        (dyn)->capacity = (start_capacity);                     \
        (dyn)->size = 0;                                        \
        (dyn)->arr = malloc(elem_size(dyn) * (start_capacity)); \
    } while (0)

#define mdyn_resize(dyn, new_size)                                     \
    do {                                                               \
        (dyn)->capacity = (new_size);                                  \
        (dyn)->arr = realloc((dyn)->arr, elem_size(dyn) * (new_size)); \
    } while (0)

#define mdyn_append(dyn, elem)                       \
    do {                                             \
        if ((dyn)->size + 1 > (dyn)->capacity) {     \
            mdyn_resize((dyn), (dyn)->capacity * 2); \
        }                                            \
        (dyn)->arr[(dyn)->size] = elem;              \
        (dyn)->size += 1;                            \
    } while (0)


#define mdyn_get(dyn, idx) (dyn)->arr[(idx)]

#define mdyn_free(dyn)       \
    do {                     \
        free((dyn)->arr);    \
        (dyn)->size = 0;     \
        (dyn)->capacity = 0; \
    } while (0)
