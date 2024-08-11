#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "dynarray.h"

typedef struct {
	DynArray(void *) entries;
	uint64_t len;

	Arena *alloc;
} Stack;

Stack stack_init(Arena *a, uint64_t starting_capacity) {
    Stack stack;

	stack.alloc = a;
	dyn_init(a, &stack.entries, starting_capacity);
	stack.len = 0;

    return stack;
}

void stack_push(Stack *stack, void *elem) {
    if (stack->len >= stack->entries.capacity) {
		dyn_resize(&stack->entries, stack->entries.capacity * 2);
    }

	stack->entries.arr[stack->len] = elem;
	stack->len += 1;
}

void *stack_pop(Stack *stack) {
	if (stack->len == 0) {
		return NULL;
	}

	stack->len -= 1;
	return stack->entries.arr[stack->len];
}
