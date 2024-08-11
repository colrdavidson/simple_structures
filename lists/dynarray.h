#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define DynArray(type)     \
    struct {               \
        type *arr;         \
        uint64_t size;     \
        uint64_t capacity; \
		Arena *alloc;      \
    }

#define elem_size(dyn) sizeof(*((dyn)->arr))
#define dyn_init(allo, dyn, start_capacity)                                        \
    do {                                                                           \
        (dyn)->capacity = (start_capacity);                                        \
        (dyn)->size = 0;                                                           \
		(dyn)->alloc = (allo);                                                     \
        (dyn)->arr = arena_alloc((dyn)->alloc, elem_size(dyn) * (start_capacity)); \
    } while (0)

#define dyn_resize(dyn, new_size)                                                                                            \
    do {                                                                                                                     \
        (dyn)->arr = arena_realloc((dyn)->alloc, (dyn)->arr, elem_size(dyn) * (dyn)->capacity, elem_size(dyn) * (new_size)); \
        (dyn)->capacity = (new_size);                                                                                        \
    } while (0)

#define dyn_append(dyn, elem)                        \
    do {                                             \
        if ((dyn)->size + 1 > (dyn)->capacity) {     \
            dyn_resize((dyn), (dyn)->capacity * 2);  \
        }                                            \
        (dyn)->arr[(dyn)->size] = elem;              \
        (dyn)->size += 1;                            \
    } while (0)


#define dyn_get(dyn, idx) (dyn)->arr[(idx)]
