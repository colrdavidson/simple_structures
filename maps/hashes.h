#pragma once
#include <stdint.h>

uint64_t fibhash(uint64_t key) {
    return key * 2654435769;
}

uint64_t fnv1a(uint64_t key) {
    uint64_t hash = 0xcbf29ce484222325;
    uint64_t fnv_prime = 0x100000001b3;

    char *key_buf = (char *)&key;
    for (int i = 0; i < sizeof(key); i++) {
        hash = hash ^ key_buf[i];
        hash = hash * fnv_prime;
    }

    return hash;
}

uint32_t murmur32(void *key, uint64_t len) {
	uint32_t m = 0x5bd1e995;
	int r = 24;

	uint8_t *data = (uint8_t *)key;

	uint32_t hash = len;
	while (len >= 4) {
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		hash *= m;
		hash ^= k;

		data += 4;
		len -= 4;
	}

	switch (len) {
		case 3: hash ^= data[2] << 16;
		case 2: hash ^= data[1] << 8;
		case 1: {
			hash ^= data[0];
			hash *= m;
		}
	}

	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

	return hash;
}
