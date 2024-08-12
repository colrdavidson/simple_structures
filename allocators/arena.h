#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// This arena is laid out in memory
// with data immediately after each Arena Header
// ----------------------------------------------
// struct Arena {
//     ....
//     tail -----------|
//     next -------|   |
// }               |   |
// ...<data>       |   |
//                 |   |
// struct Arena {<-|   |
//     ....            |
//     tail ---------| |
//     next -------| | |
// }               | | |
// ...<data>       | | |
//                 | | |
// struct Arena { <----|
// ^    ....
// |--- tail
//      next = NULL
// }
// ...<data>

typedef struct Arena {
	uint64_t current;
	uint64_t capacity;

	uint64_t total_size;
	uint64_t total_capacity;

	struct Arena *tail;
	struct Arena *next;
} Arena;

// We're going to grab and align to at least a whole page
// No sense being super stingy, we are the allocator after all
#define PAGE_SIZE 4096
#define PAGE_ROUND_UP(x) (((x)) & (~(PAGE_SIZE-1)))

Arena *arena_init(uint64_t capacity) {
	uint64_t chunk_size = PAGE_ROUND_UP(sizeof(Arena) + capacity);
	Arena *a = (Arena *)malloc(chunk_size);

	a->capacity = chunk_size - sizeof(Arena);
	a->total_capacity = a->capacity;
	a->current = 0;
	a->total_size = 0;

	// Being a little tricky here.
	// If your tail points at yourself, when you get your tail's next,
	// it's also your next, which means the append logic is simpler
	a->next = NULL;
	a->tail = a;
	return a;
}

void arena_grow(Arena *arena_head, uint64_t size) {
	uint64_t chunk_size = PAGE_ROUND_UP(sizeof(Arena) + size);
	Arena *new_arena = (Arena *)malloc(chunk_size);

	new_arena->capacity = chunk_size - sizeof(Arena);
	new_arena->current = 0;
	new_arena->next = NULL;
	arena_head->total_capacity += new_arena->capacity;

	// Time to slap this new arena on the tail, and patch up our previous tail
	Arena *tmp       = arena_head->tail;
	arena_head->tail = new_arena;
	tmp->next        = new_arena;
}

void *arena_alloc(Arena *arena_head, uint64_t size) {
	Arena *cur_arena = arena_head->tail;

	if ((cur_arena->current + size) > cur_arena->capacity) {
		arena_grow(arena_head, size);
	}

	void *new_buffer = (void *)((cur_arena + sizeof(Arena)) + cur_arena->current);
	cur_arena->current += size;
	arena_head->total_size += size;
	return new_buffer;
}

void *arena_realloc(Arena *arena_head, void *buffer, uint64_t old_size, uint64_t new_size) {
	void *new_buffer = arena_alloc(arena_head, new_size);
	memcpy(new_buffer, buffer, old_size);
	return new_buffer;
}

void arena_clear(Arena *arena_head) {
	Arena *cur = arena_head->next;
	while (cur != NULL) {
		Arena *tmp = cur;
		cur = cur->next;
		arena_head->total_capacity -= tmp->capacity;

		free(tmp);
	}

	arena_head->current = 0;
	arena_head->tail = arena_head;
	arena_head->next = NULL;
	arena_head->total_size = 0;
}

void arena_free(Arena *arena_head) {
	arena_clear(arena_head);
	free(arena_head);
}
