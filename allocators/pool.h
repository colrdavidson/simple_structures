#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

// We're going to grab and align to at least a whole page
// No sense being super stingy, we are the allocator after all
#define PAGE_SIZE 4096
#define DIV_ROUND_UP(x, y) (((x) + (y) - 1) / (y))
#define PAGE_ROUND_UP(x) (PAGE_SIZE * DIV_ROUND_UP((x), PAGE_SIZE))

typedef struct Pool {
	uint64_t unit_size;
	uint64_t capacity;

	uint64_t bitmap_size;
} Pool;

Pool *pool_init(Arena *a, uint64_t capacity, uint64_t unit_size) {

	uint64_t initial_capacity = PAGE_ROUND_UP(capacity + sizeof(Pool));
	Pool *p = (Pool *)arena_alloc(a, initial_capacity);

	uint64_t bitmap_size = DIV_ROUND_UP(initial_capacity, unit_size);
	uint64_t real_capacity = initial_capacity - sizeof(Pool) - bitmap_size;

	p->unit_size = unit_size;
	p->capacity = real_capacity;
	p->bitmap_size = bitmap_size;
	return p;
}

void *pool_get(Pool *p) {
	uint64_t *bitmap     = (uint64_t *)(p + sizeof(Pool));
	uint8_t  *data_start = (uint8_t *)(p + sizeof(Pool) + p->bitmap_size);

	// Scanning our bitmap for free chunks
	uint64_t full = ~0;
	uint64_t empty = 0;

	for (uint64_t i = 0; i < p->bitmap_size; i++) {
		uint64_t chunk = bitmap[i];
		if (chunk == full) {
			continue;
		}

		// Handle the empty case special, because count leading zeros gets weird with a 0 val
		if (chunk == empty) {
			uint64_t mask = 1ull << 63;
			bitmap[i] = chunk ^ mask;

			uint64_t offset = ((i * 64) * p->unit_size);
			return (void *)(data_start + offset);
		}

		// This is where the magic happens
		// if chunk = 0b0100
		//   ~chunk = 0b1011
		// count-leading-zeros returns 0
		// which means there's a space in slot 0
		//
		// if chunk = 0b1100
		//   ~chunk = 0b0011
		// count-leading-zeros returns 2
		// which means there's a space in slot 2
		uint64_t bit_slot = __builtin_clzll(~chunk);
		uint64_t mask = 1ull << (63 - bit_slot);
		bitmap[i] = chunk ^ mask;

		// Each chunk in the bitmap represents 64 slots of unit_size
		// Each bit is a unit_size slot
		uint64_t offset = ((i * 64) * p->unit_size) + (bit_slot * p->unit_size);
		return (void *)(data_start + offset);
	}

	// We're out of space in our pool!
	// Oh noes
	return NULL;
}

void pool_free(Pool *p, void *slot) {
	uint64_t *bitmap     = (uint64_t *)(p + sizeof(Pool));
	uint8_t  *data_start = (uint8_t *)(p + sizeof(Pool) + p->bitmap_size);
	uint8_t  *pool_max   = data_start + p->capacity;

	// Double check that the "slot" we've got actually lives in our pool
	uint8_t *slot_bytes = (uint8_t *)slot;
	assert(slot_bytes >= data_start && slot_bytes < pool_max);

	// Figure out where in our bitmap things should live
	uint64_t offset = (uint64_t)(slot_bytes - data_start);
	uint64_t chunk_idx = (offset / p->unit_size) / 64;
	uint64_t bit_idx   = (offset / p->unit_size) % 64;

	uint64_t chunk = bitmap[chunk_idx];

	// Confirm that this isn't a double-free
	uint64_t bit = (chunk << bit_idx) >> 63;
	assert(bit != 0);

	// Free up that slot
	uint64_t mask = ~(1ull << (63 - bit_idx));
	bitmap[chunk_idx] = chunk & mask;

	return;
}
