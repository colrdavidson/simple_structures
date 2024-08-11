#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "allocators/scratch.h"
#include "allocators/arena.h"
#include "lists/simple_linked_list.h"
#include "maps/fixed_set.h"
#include "maps/fixed_map.h"
#include "maps/growing_map.h"
#include "lists/simple_dynarray.h"
#include "lists/dynarray.h"

void test_scratch(ScratchAlloc *scr) {
    printf("=== SCRATCH TEST ===\n");

    int *array = scratch_alloc(scr, sizeof(int) * 10);
    printf("allocating a little space, new head is %llu\n", scr->current);

    printf("filling our new array full of data!\n");
    for (int i = 0; i < 10; i++) {
        array[0] = i;
    }

    printf("Resetting our scratch\n");
    scratch_clear(scr);

    printf("\n");
}

void test_fixed_set(ScratchAlloc *scr) {
    printf("=== FIXED SET TEST ===\n");
    FixedSet s = fixed_set_init(scr, 64);

    fixed_set_insert(&s, "Hello");
    fixed_set_insert(&s, "Goodbye");

    if (fixed_set_contains(&s, "Hello")) {
		printf("Looked for 'Hello' and got it\n");
	}
	
    if (fixed_set_contains(&s, "Goodbye")) {
		printf("Looked for 'Goodbye' and got it\n");
	}

	bool ok = fixed_set_contains(&s, "Hamburger");
	if (!ok) {
		printf("Looked for 'Hamburger' couldn't find it\n");
	}

    printf("Clearing out all our set entries\n");
    fixed_set_clear(&s);

    if (!fixed_set_contains(&s, "Goodbye")) {
		printf("Looked for 'Goodbye' couldn't find it\n");
	}

    printf("Cleaning up after ourselves\n");
	scratch_clear(scr);

    printf("\n");
}

void test_fixed_map(ScratchAlloc *scr) {
    printf("=== FIXED MAP TEST ===\n");
    FixedMap m = fixed_map_init(scr, 64);

    fixed_map_insert(&m, 'b', 1000);
    fixed_map_insert(&m, '~', 5);

    uint64_t val = 0;
    bool ok = fixed_map_get(&m, 'b', &val);
    printf("Looked for 'b' and got %llu\n", val);

    ok = fixed_map_get(&m, '~', &val);
    printf("Looked for '~' and got %llu\n", val);

    ok = fixed_map_get(&m, 9, &val);
    if (!ok) {
        printf("Looked for 9 and couldn't find it!\n");
    } else {
        printf("Looked for 9 and got %llu\n", val);
    }

    printf("Clearing out all our map entries\n");
    fixed_map_clear(&m);

    ok = fixed_map_get(&m, '~', &val);
    if (!ok) {
        printf("Looked for '~' and couldn't find it!\n");
    } else {
        printf("Looked for '~' and got %llu\n", val);
    }

    printf("Cleaning up after ourselves\n");
	scratch_clear(scr);

    printf("\n");
}

void test_simple_linked_list(ScratchAlloc *scr) {
    printf("=== SIMPLE LINKED LIST TEST ===\n");
	printf("creating 0\n");
    Node *list_head = simple_linked_list_init(scr, 0);

	for (int i = 1; i < 4; i++) {
		printf("appending %d\n", i);
		simple_linked_list_append(scr, &list_head, i);
	}

	simple_linked_list_print_list(list_head);

    printf("Cleaning up after ourselves\n");
	scratch_clear(scr);

	printf("\n");
}

void test_arena(Arena *a) {
    printf("=== ARENA V1 TEST ===\n");

	printf("current arena size: 0x%08llx\n", a->total_size);

	printf("testing a small allocation\n");
	char *buffer = arena_alloc(a, 50);
	snprintf(buffer, 50, "Hello Arena Test!\n");
	printf("> %s\n", buffer);

	printf("current arena size: 0x%08llx\n", a->total_size);

	printf("testing a big allocation\n");
	int *array = arena_alloc(a, sizeof(int) * 10000);
	array[9999] = 36;
	array[9998] = 72;

	printf("current arena size: 0x%08llx\n", a->total_size);

    printf("Cleaning up after ourselves\n");
	arena_clear(a);

	printf("current arena size: 0x%08llx\n", a->total_size);
	printf("\n");
}

void test_simple_dynarray(Arena *a) {
    printf("=== SIMPLE DYNAMIC ARRAY TEST ===\n");

    SimpleDynArray arr = simple_dyn_init(a, 32);

    printf("Trying to append data\n");
    simple_dyn_append(&arr, 99);
    simple_dyn_append(&arr, 200);
    simple_dyn_append(&arr, 1);
    simple_dyn_append(&arr, 0);

    printf("Checking that data is in there\n");
    for (int i = 0; i < arr.size; i++) {
        printf("arr[%d] = %llu\n", i, simple_dyn_get(&arr, i));
    }

    printf("Cleaning up after ourselves\n");
	arena_clear(a);

    printf("\n");
}

void test_dynarray(Arena *a) {
    printf("=== DYNAMIC ARRAY TEST ===\n");
    DynArray(int) arr;

    dyn_init(a, &arr, 32);

    printf("Trying to append data\n");
    dyn_append(&arr, 99);
    dyn_append(&arr, 200);
    dyn_append(&arr, 1);
    dyn_append(&arr, 0);

    printf("Checking that data is in there\n");
    for (int i = 0; i < arr.size; i++) {
        printf("arr[%d] = %d\n", i, dyn_get(&arr, i));
    }

    printf("Cleaning up after ourselves\n");
	arena_clear(a);

    printf("\n");
}

void test_growing_map(Arena *a) {
    printf("=== Growing MAP TEST ===\n");
    Map m = map_init(a, 0);

    for (int i = 0; i < 20; i += 1) {
		printf("inserting map['%c'] = %d\n", 'a'+i, i);
        map_insert(&m, 'a'+i, i);
    }

    uint64_t val = 0;
    for (int i = 0; i < 20; i += 1) {
        uint64_t key = 'a'+i;
        bool ok = map_get(&m, key, &val);
		assert(ok);
        printf("Looked for '%c' and got %llu\n", (char)key, val);
    }

    bool ok = map_get(&m, 9, &val);
	assert(!ok);

    printf("Clearing out all our map entries\n");
    map_clear(&m);

    ok = map_get(&m, 'a', &val);
	assert(!ok);

    printf("Cleaning up after ourselves\n");
	arena_clear(a);

    printf("\n");
}

int main(int argc, char **argv) {
	ScratchAlloc scr = scratch_init(8 * 1024);
	printf("~~ Hello Memory, it's me the ScratchAllocator! ~~\n\n");

    test_scratch(&scr);
    test_fixed_set(&scr);
    test_fixed_map(&scr);
	test_simple_linked_list(&scr);

	printf("~~ Ugh, that's a lot of work... Time for retirement! ~~ \n\n");
	scratch_free(&scr);

	Arena *a = arena_init(8 * 1024);
	printf("~~ WELCOME TO THE THUNDERDOME! ~~\n\n");
	test_arena(a);

    test_simple_dynarray(a);
    test_dynarray(a);
    test_growing_map(a);
}
