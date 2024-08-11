#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint64_t key;
    uint64_t val;
} FixedMapEntry;

typedef struct {
    FixedMapEntry *entries;
    uint64_t entries_size;
    uint64_t entries_capacity;

    int64_t *hashes;
    uint64_t hashes_size;
} FixedMap;

FixedMap fixed_map_init(int elem_count) {
    FixedMap m;

    int start_count = elem_count;
    if (start_count == 0) {
        start_count = 8;
    }

    m.hashes = (int64_t *)malloc(sizeof(int64_t) * start_count);
    m.hashes_size = start_count;

    m.entries = (FixedMapEntry *)malloc(sizeof(FixedMapEntry) * start_count);
    m.entries_size = 0;
    m.entries_capacity = start_count;

    // Set the whole hashmap to empty. -1 means the slot hasn't been filled yet
    for (int i = 0; i < m.hashes_size; i++) {
        m.hashes[i] = -1;
    }
    return m;
}

// Fibhash. It's often bad, but it's simple
uint64_t fixed_map_hash(uint64_t key) {
    return key * 2654435769;
}

bool fixed_map_insert(FixedMap *m, uint64_t key, uint64_t val) {
    FixedMapEntry new_entry = {key, val};

    uint64_t hash_val = fixed_map_hash(key) % m->hashes_size;
    for (uint64_t i = 0; i < m->hashes_size; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % m->hashes_size;

        int64_t cur_hash = m->hashes[cur_hash_idx];
        // Did we find an empty slot?
        if (cur_hash == -1) {
            m->hashes[cur_hash_idx] = m->entries_size;
            m->entries[m->entries_size] = new_entry;
            m->entries_size++;
            return true;

        // Is this key already in the map? If so, replace the entry with our new entry
        } else if (m->entries[cur_hash].key == key) {
            m->entries[cur_hash] = new_entry;
            return true;
        }
    }

    // Our hashmap is completely full?
    return false;
}

bool fixed_map_get(FixedMap *m, uint64_t key, uint64_t *result) {
    uint64_t hash_val = fixed_map_hash(key) % m->hashes_size;
    for (uint64_t i = 0; i < m->hashes_size; i++) {
        uint64_t cur_hash_idx = (hash_val + i) % m->hashes_size;

        int64_t cur_hash = m->hashes[cur_hash_idx];
        // There's nothing in our hashmap with our key?
        if (cur_hash == -1) {
            *result = 0;
            return false;

        // Did we find it? Check to make sure the entry we found has a matching key
        } else if (m->entries[cur_hash].key == key) {
            *result = m->entries[cur_hash].val;
            return true;
        }
    }

    // The hashmap is completely full?
    *result = 0;
    return false;
}

void fixed_map_clear(FixedMap *m) {
    for (int i = 0; i < m->hashes_size; i++) {
        m->hashes[i] = -1;
    }
    m->entries_size = 0;
}

void fixed_map_free(FixedMap *m) {
    free(m->entries);
    free(m->hashes);
}
