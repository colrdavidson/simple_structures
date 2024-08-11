#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "macro_dynarr.h"

typedef struct {
    uint64_t key;
    uint64_t val;
} MapEntry;

typedef struct {
    DynArr(MapEntry) entries;
    DynArr(int64_t)  hashes;
} Map;

Map map_init(int elem_count) {
    Map m;

    int start_count = elem_count;
    if (start_count == 0) {
        start_count = 8;
    }

    mdyn_init(&m.entries, start_count);
    mdyn_init(&m.hashes, start_count);

    // Set the whole hashmap to empty. -1 means the slot hasn't been filled yet
    for (int i = 0; i < m.hashes.capacity; i++) {
        m.hashes.arr[i] = -1;
    }
    return m;
}

// FNV-1a
uint64_t map_hash(uint64_t key) {
    uint64_t hash = 0xcbf29ce484222325;
    uint64_t fnv_prime = 0x100000001b3;

    char *key_buf = (char *)&key;
    for (int i = 0; i < sizeof(key); i++) {
        hash = hash ^ key_buf[i];
        hash = hash * fnv_prime;
    }

    return hash;
}

void map_reinsert(Map *m, MapEntry entry, uint64_t entry_idx) {
    uint64_t hash_val = map_hash(entry.key) % m->hashes.capacity;
    for (uint64_t i = 0; i < m->hashes.capacity; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % m->hashes.capacity;
        int64_t cur_hash = m->hashes.arr[cur_hash_idx];

        if (cur_hash == -1) {
            m->hashes.arr[cur_hash_idx] = entry_idx;
            return;
        }
    }

    // This can't happen, if we're reinserting, we should already have grown
    return;
}

void map_grow(Map *m) {
    mdyn_resize(&m->hashes, m->hashes.capacity * 2);
    for (uint64_t i = 0; i < m->hashes.capacity; i++) {
        m->hashes.arr[i] = -1;
    }

    for (uint64_t i = 0; i < m->entries.size; i++) {
        MapEntry entry = m->entries.arr[i];
        map_reinsert(m, entry, i);
    }
}

bool map_insert(Map *m, uint64_t key, uint64_t val) {
    MapEntry new_entry = {key, val};

    // Grow before we insert, so we always have space for our hash
    int64_t next_resize_window = (3 * m->hashes.capacity) / 4;
    if (m->entries.size >= next_resize_window) {
        map_grow(m);
    }

    uint64_t hash_val = map_hash(key) % m->hashes.capacity;
    for (uint64_t i = 0; i < m->hashes.capacity; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % m->hashes.capacity;

        int64_t cur_hash = m->hashes.arr[cur_hash_idx];
        // Did we find an empty slot?
        if (cur_hash == -1) {
            m->hashes.arr[cur_hash_idx] = m->entries.size;
            mdyn_append(&m->entries, new_entry);
            return true;

        // Is this key already in the map? If so, replace the entry with our new entry
        } else if (m->entries.arr[cur_hash].key == key) {
            m->entries.arr[cur_hash] = new_entry;
            return true;
        }
    }

    // Our hashmap should be growing, this should never happen!
    return false;
}

bool map_get(Map *m, uint64_t key, uint64_t *result) {
    uint64_t hash_val = map_hash(key) % m->hashes.capacity;
    for (uint64_t i = 0; i < m->hashes.capacity; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % m->hashes.capacity;

        int64_t cur_hash = m->hashes.arr[cur_hash_idx];
        // There's nothing in our hashmap with our key?
        if (cur_hash == -1) {
            *result = 0;
            return false;

        // Did we find it? Check to make sure the entry we found has a matching key
        } else if (m->entries.arr[cur_hash].key == key) {
            *result = m->entries.arr[cur_hash].val;
            return true;
        }
    }

    // The hashmap is completely full?
    *result = 0;
    return false;
}

void map_clear(Map *m) {
    for (int i = 0; i < m->hashes.capacity; i++) {
        m->hashes.arr[i] = -1;
    }
    m->entries.size = 0;
}

void map_free(Map *m) {
    mdyn_free(&m->entries);
    mdyn_free(&m->hashes);
}
