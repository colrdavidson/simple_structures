#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    uint64_t *buffer;
    uint64_t size;
    uint64_t capacity;
} SimpleDynArray;

SimpleDynArray simple_dyn_init(uint64_t starting_capacity) {
    SimpleDynArray arr;
    arr.capacity = starting_capacity;
    arr.buffer = calloc(arr.capacity, sizeof(uint64_t));
    arr.size = 0;
    return arr;
}

void simple_dyn_append(SimpleDynArray *arr, uint64_t elem) {
    if (arr->size + 1 > arr->capacity) {
        arr->capacity *= 2;
        arr->buffer = realloc(arr->buffer, arr->capacity);
    }

    arr->buffer[arr->size] = elem;
    arr->size += 1;
}

uint64_t simple_dyn_get(SimpleDynArray *arr, uint64_t idx) {
    assert(idx <= arr->size);
    return arr->buffer[idx];
}

void simple_dyn_free(SimpleDynArray *arr) {
    free(arr->buffer);
    arr->size = 0;
    arr->capacity = 0;
}
