#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	struct Node *next;
	uint64_t value;
} Node;

void simple_linked_list_print_node(Node *node) {
	printf("%p | value - %llu, next - %p\n", node, node->value, node->next);
}

void simple_linked_list_print_list(Node *list) {
	Node *tmp = list;
	while (tmp != NULL) {
		simple_linked_list_print_node(tmp);
		tmp = tmp->next;
	}
}

Node *simple_linked_list_init(ScratchAlloc *scr, uint64_t val) {
	Node *head = scratch_alloc(scr, sizeof(Node));
	head->next = NULL;
	head->value = val;
	return head;
}

void simple_linked_list_append(ScratchAlloc *scr, Node **list, uint64_t value) {
	if (*list == NULL) {
		*list = simple_linked_list_init(scr, value);
		return;
	}

	Node *tmp = *list;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}

	Node *new_node = simple_linked_list_init(scr, value);
	tmp->next = new_node;
}
