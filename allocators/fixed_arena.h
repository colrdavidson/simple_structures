#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void *buffer;
    uint64_t size;
    uint64_t current;
} FixedArena;

FixedArena fixed_arena_init(uint64_t size) {
    FixedArena a;
    a.buffer = malloc(size);
    a.size = size;
    a.current = 0;
    return a;
}

void *fixed_arena_alloc(FixedArena *a, uint64_t size) {
    if (a->current + size > a->size) {
        return NULL;
    }

    void *new_buffer = (void *)(a->buffer + a->current);
    a->current += size;
    return new_buffer;
}

void fixed_arena_clear(FixedArena *a) {
    a->current = 0;
}

void fixed_arena_free(FixedArena *a) {
    free(a->buffer);
}
