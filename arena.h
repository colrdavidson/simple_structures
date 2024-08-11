#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	void *buffer;
	uint64_t size;
	uint64_t current;
} Arena;

Arena arena_init(uint64_t size) {
	Arena a;
	a.buffer = malloc(size);
	a.size = size;
	a.current = 0;
	return a;
}

void *arena_alloc(Arena *a, uint64_t size) {
	if (a->current + size > a->size) {
		return NULL;
	}

	void *new_buffer = (void *)(a->buffer + a->current);
	a->current += size;
	return new_buffer;
}

void arena_clear(Arena *a) {
	a->current = 0;
}

void arena_free(Arena *a) {
	free(a->buffer);
}
