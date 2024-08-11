#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "allocators/arena.h"
#include "lists/simple_linked_list.h"
#include "maps/fixed_set.h"
#include "maps/fixed_map.h"
#include "maps/growing_map.h"
#include "lists/simple_dynarray.h"
#include "lists/dynarray.h"

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

    printf("Cleaning up after ourselves\n");
    arena_free(&a);

    printf("\n");
}

void test_simple_linked_list(void) {
    printf("=== SIMPLE LINKED LIST TEST ===\n");
	printf("creating 0\n");
    Node *list_head = simple_linked_list_init(0);

	for (int i = 1; i < 4; i++) {
		printf("appending %d\n", i);
		simple_linked_list_append(&list_head, i);
	}
	simple_linked_list_print_list(list_head);

	for (int i = 0; i < 5; i++) {
		uint64_t ret = simple_linked_list_pop(&list_head);
		if (list_head != NULL) {
			printf("popped %llu\n", ret);
		} else {
			printf("list is empty!\n");
		}
	}
	simple_linked_list_print_list(list_head);

	printf("appending 8\n");
	simple_linked_list_append(&list_head, 8);

	for (int i = 0; i < 3; i++) {
		printf("inserting %d after head\n", i);
		simple_linked_list_insert_after(&list_head, 0, i);
	}

	simple_linked_list_print_list(list_head);

    printf("Cleaning up after ourselves\n");
	simple_linked_list_free(&list_head);

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

void test_simple_dynarray(void) {
    printf("=== SIMPLE DYNAMIC ARRAY TEST ===\n");

    SimpleDynArray arr = simple_dyn_init(32);

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
    simple_dyn_free(&arr);

    printf("\n");
}

void test_dynarray(void) {
    printf("=== DYNAMIC ARRAY TEST ===\n");
    DynArray(int) arr;

    dyn_init(&arr, 32);

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
    dyn_free(&arr);

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
	test_simple_linked_list();
    test_fixed_set();
    test_fixed_map();
    test_simple_dynarray();
    test_dynarray();
    test_growing_map();
}
