#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../allocators/scratch.h"
#include "hashes.h"

typedef struct {
	char **entries;
	uint64_t capacity;

	ScratchAlloc *scr;
} FixedSet;

void fixed_set_clear(FixedSet *s) {
	for (int i = 0; i < s->capacity; i++) {
		s->entries[i] = NULL;
	}
}

FixedSet fixed_set_init(ScratchAlloc *scr, int elem_count) {
	FixedSet s;

	int start_count = elem_count;
	if (start_count == 0) {
		start_count = 8;
	}

	s.entries = scratch_alloc(scr, sizeof(char *) * start_count);
	s.capacity = start_count;

	fixed_set_clear(&s);
	return s;
}

bool fixed_set_insert(FixedSet *s, char *str) {

	uint64_t hash_val = murmur32(str, strlen(str)) % s->capacity;
	for (uint64_t i = 0; i < s->capacity; i++) {
		uint64_t cur_idx = (hash_val + i) % s->capacity;
		char *cur_entry = s->entries[cur_idx];

		// Did we find an empty slot?
		if (cur_entry == NULL) {
			s->entries[cur_idx] = str;
			return true;

		// If the string is already in our set, we're all good
		} else if (strcmp(cur_entry, str) == 0) {
			return true;
		}
	}

	// Our hashset is completely full?
	return false;
}

bool fixed_set_contains(FixedSet *s, char *str) {

	uint64_t hash_val = murmur32(str, strlen(str)) % s->capacity;
	for (uint64_t i = 0; i < s->capacity; i++) {
		uint64_t cur_idx = (hash_val + i) % s->capacity;
		char *cur_entry = s->entries[cur_idx];

		// Did we find an empty slot?
		if (cur_entry == NULL) {
			return false;

		// If the string is already in our set, we're all good
		} else if (strcmp(cur_entry, str) == 0) {
			return true;
		}
	}

	// The hashset is completely full?
	return false;
}
