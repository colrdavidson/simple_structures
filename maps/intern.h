#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../lists/dynarray.h"
#include "hashes.h"

typedef struct {
    DynArray(char *) entries;

	Arena *string_store;
	Arena *alloc;
} Intern;

void intern_clear(Intern *in) {
	for (int i = 0; i < in->entries.capacity; i++) {
		in->entries.arr[i] = NULL;
	}
}

Intern intern_init(Arena *a, Arena *string_store, int elem_count) {
    Intern in;
	in.string_store = string_store;
	in.alloc = a;

    int start_count = elem_count;
    if (start_count == 0) {
        start_count = 8;
    }

    dyn_init(a, &in.entries, start_count);
	intern_clear(&in);

    return in;
}

void intern_reinsert(Intern *in, char *str) {
    uint64_t hash_val = murmur32(str, strlen(str)) % in->entries.capacity;
    for (uint64_t i = 0; i < in->entries.capacity; i++) {
        uint64_t cur_idx = (hash_val + i) % in->entries.capacity;
		char *cur_entry = in->entries.arr[cur_idx];

        // Did we find an empty slot?
        if (cur_entry == NULL) {
            in->entries.arr[cur_idx] = cur_entry;
            return;
        }
    }

    // This can't happen, if we're reinserting, we should already have grown
	assert(true);
    return;
}

void intern_grow(Intern *in) {
	DynArray(char *) new_entries;
	dyn_init(in->alloc, &new_entries, in->entries.capacity * 2);

    for (uint64_t i = 0; i < new_entries.capacity; i++) {
        new_entries.arr[i] = NULL;
    }

	DynArray(char *) old_entries;
	dyn_assign(&old_entries, &in->entries);
	dyn_assign(&in->entries, &new_entries);

    for (uint64_t i = 0; i < old_entries.capacity; i++) {
        char *entry = old_entries.arr[i];
		if (entry != NULL) {
			intern_reinsert(in, entry);
		}
    }
}

char *intern_get(Intern *in, char *str) {

    // Grow before we insert, so we always have space for our hash
    int64_t next_resize_window = (3 * in->entries.capacity) / 4;
    if (in->entries.size >= next_resize_window) {
        intern_grow(in);
    }

	uint64_t str_len = strlen(str);
    uint64_t hash_val = murmur32(str, str_len) % in->entries.capacity;
    for (uint64_t i = 0; i < in->entries.capacity; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % in->entries.capacity;
        char *cur_entry = in->entries.arr[cur_hash_idx];

        // Did we find an empty slot?
        if (cur_entry == NULL) {

			char *buffer = arena_alloc(in->string_store, str_len + 1);
			strcpy(buffer, str);

            in->entries.arr[cur_hash_idx] = buffer;
			in->entries.size += 1;
			return buffer;

        // Is this str already in the map? Give back the stable version
        } else if (strcmp(cur_entry, str) == 0) {
			return cur_entry;
        }
    }

    // Our hashmap should be growing, this should never happen!
    return false;
}
