#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    void *buffer;
    uint64_t size;
    uint64_t current;
} ScratchAlloc;

// We're going to grab at least a whole page
// No sense being super stingy, we are the allocator after all
#define PAGE_SIZE 4096
#define PAGE_ROUND_UP(x) (((x)) & (~(PAGE_SIZE-1)))

ScratchAlloc scratch_init(uint64_t size) {
    ScratchAlloc a;
    a.size = PAGE_ROUND_UP(size);
    a.buffer = malloc(a.size);
    a.current = 0;
    return a;
}

void *scratch_alloc(ScratchAlloc *a, uint64_t size) {
    if (a->current + size > a->size) {
        return NULL;
    }

    void *new_buffer = (void *)(a->buffer + a->current);
    a->current += size;
    return new_buffer;
}

void scratch_clear(ScratchAlloc *a) {
    a->current = 0;
}

void scratch_free(ScratchAlloc *a) {
    free(a->buffer);
}
