#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    uint64_t *buffer;
    uint64_t size;
    uint64_t capacity;

	Arena *alloc;
} SimpleDynArray;

SimpleDynArray simple_dyn_init(Arena *a, uint64_t starting_capacity) {
    SimpleDynArray arr;
	arr.alloc = a;
    arr.capacity = starting_capacity;
    arr.buffer = arena_alloc(arr.alloc, arr.capacity * sizeof(uint64_t));
    arr.size = 0;
    return arr;
}

void simple_dyn_append(SimpleDynArray *arr, uint64_t elem) {
    if (arr->size + 1 > arr->capacity) {
		uint64_t old_capacity = arr->capacity;
        arr->capacity *= 2;
        arr->buffer = arena_realloc(arr->alloc, arr->buffer, old_capacity, arr->capacity);
    }

    arr->buffer[arr->size] = elem;
    arr->size += 1;
}

uint64_t simple_dyn_get(SimpleDynArray *arr, uint64_t idx) {
    assert(idx <= arr->size);
    return arr->buffer[idx];
}
