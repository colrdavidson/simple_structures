#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "allocators/arena.h"
#include "maps/fixed_set.h"
#include "maps/fixed_map.h"
#include "maps/growing_map.h"
#include "arrays/dynarr.h"
#include "arrays/macro_dynarr.h"

void test_arena(void) {
    printf("=== ARENA TEST ===\n");

    Arena a = arena_init(64 * 1024);
    int *array = arena_alloc(&a, sizeof(int) * 10);
    printf("allocating a little space, new head is %llu\n", a.current);

    printf("filling our new array full of data!\n");
    for (int i = 0; i < 10; i++) {
        array[0] = i;
    }

    printf("Resetting our arena\n");
    arena_clear(&a);

    printf("Freeing our arena\n");
    arena_free(&a);

    printf("\n");
}

void test_fixed_set(void) {
    printf("=== FIXED SET TEST ===\n");
    FixedSet s = fixed_set_init(64);

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
    fixed_set_free(&s);

    printf("\n");
}

void test_fixed_map(void) {
    printf("=== FIXED MAP TEST ===\n");
    FixedMap m = fixed_map_init(64);

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
    fixed_map_free(&m);

    printf("\n");
}

void test_dynarray(void) {
    printf("=== SIMPLE DYNAMIC ARRAY TEST ===\n");

    DynArray arr = dyn_init(32);

    printf("Trying to append data\n");
    dyn_append(&arr, 99);
    dyn_append(&arr, 200);
    dyn_append(&arr, 1);
    dyn_append(&arr, 0);

    printf("Checking that data is in there\n");
    for (int i = 0; i < arr.size; i++) {
        printf("arr[%d] = %llu\n", i, dyn_get(&arr, i));
    }

    printf("Cleaning up after ourselves\n");
    dyn_free(&arr);

    printf("\n");
}

void test_macro_dynarr(void) {
    printf("=== FLEXIBLE DYNAMIC ARRAY TEST ===\n");
    DynArr(int) arr;

    mdyn_init(&arr, 32);

    printf("Trying to append data\n");
    mdyn_append(&arr, 99);
    mdyn_append(&arr, 200);
    mdyn_append(&arr, 1);
    mdyn_append(&arr, 0);

    printf("Checking that data is in there\n");
    for (int i = 0; i < arr.size; i++) {
        printf("arr[%d] = %d\n", i, mdyn_get(&arr, i));
    }

    printf("Cleaning up after ourselves\n");
    mdyn_free(&arr);

    printf("\n");
}

void test_growing_map(void) {
    printf("=== Growing MAP TEST ===\n");
    Map m = map_init(0);

    for (int i = 0; i < 30; i += 1) {
        map_insert(&m, 'a'+i, i);
    }

    uint64_t val = 0;
    for (int i = 0; i < 30; i += 1) {
        uint64_t key = 'a'+i;
        map_get(&m, key, &val);
        printf("Looked for '%c' and got %llu\n", (char)key, val);
    }

    bool ok = map_get(&m, 9, &val);
    if (!ok) {
        printf("Looked for 9 and couldn't find it!\n");
    } else {
        printf("Looked for 9 and got %llu\n", val);
    }

    printf("Clearing out all our map entries\n");
    map_clear(&m);

    ok = map_get(&m, 'a', &val);
    if (!ok) {
        printf("Looked for 'a' and couldn't find it!\n");
    } else {
        printf("Looked for 'a' and got %llu\n", val);
    }

    printf("Cleaning up after ourselves\n");
    map_free(&m);

    printf("\n");
}

int main(int argc, char **argv) {
    test_arena();
    test_fixed_set();
    test_fixed_map();
    test_dynarray();
    test_macro_dynarr();
    test_growing_map();
}
