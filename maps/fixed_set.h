#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hashes.h"

typedef struct {
    char **entries;
	uint64_t capacity;
} FixedSet;

FixedSet fixed_set_init(int elem_count) {
    FixedSet m;

    int start_count = elem_count;
    if (start_count == 0) {
        start_count = 8;
    }

    m.entries = (char **)calloc(sizeof(char *), start_count);
    m.capacity = start_count;
    return m;
}

bool fixed_set_insert(FixedSet *m, char *str) {

    uint64_t hash_val = murmur32(str, strlen(str)) % m->capacity;
    for (uint64_t i = 0; i < m->capacity; i++) {
        uint64_t cur_idx = (hash_val + i) % m->capacity;
		char *cur_entry = m->entries[cur_idx];

        // Did we find an empty slot?
		if (cur_entry == NULL) {
            m->entries[cur_idx] = str;
            return true;

		// If the string is already in our set, we're all good
        } else if (strcmp(cur_entry, str) == 0) {
            return true;
        }
    }

    // Our hashset is completely full?
    return false;
}

bool fixed_set_contains(FixedSet *m, char *str) {

    uint64_t hash_val = murmur32(str, strlen(str)) % m->capacity;
    for (uint64_t i = 0; i < m->capacity; i++) {
        uint64_t cur_idx = (hash_val + i) % m->capacity;
		char *cur_entry = m->entries[cur_idx];

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

void fixed_set_clear(FixedSet *m) {
	for (int i = 0; i < m->capacity; i++) {
		m->entries[i] = NULL;
	}
}

void fixed_set_free(FixedSet *m) {
    free(m->entries);
}
