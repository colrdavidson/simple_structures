#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "dynarray.h"

typedef struct {
	DynArray(void *) entries;
    uint64_t head;
    uint64_t tail;

	Arena *alloc;
} RingBuffer;

RingBuffer ring_init(Arena *a, uint64_t starting_capacity) {
    RingBuffer ring;

	ring.alloc = a;
	dyn_init(a, &ring.entries, starting_capacity);
	ring.head = 0;
	ring.tail = 0;

    return ring;
}

void ring_push(RingBuffer *ring, void *elem) {
    if ((ring->head - ring->tail + 1) >= ring->entries.capacity) {
		dyn_resize(&ring->entries, ring->entries.capacity * 2);
    }

	uint64_t ring_idx = ring->head % ring->entries.capacity;
    ring->entries.arr[ring_idx] = elem;
    ring->head += 1;
}

void *ring_pop(RingBuffer *ring) {
	if (ring->tail == ring->head) {
		return NULL;
	}

	uint64_t ring_idx = ring->tail % ring->entries.capacity;
	ring->tail += 1;
	return ring->entries.arr[ring_idx];
}
