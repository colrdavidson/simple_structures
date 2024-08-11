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

Node *simple_linked_list_init(uint64_t val) {
	Node *head = malloc(sizeof(Node));
	head->next = NULL;
	head->value = val;
	return head;
}

void simple_linked_list_append(Node **list, uint64_t value) {
	if (*list == NULL) {
		*list = simple_linked_list_init(value);
		return;
	}

	Node *tmp = *list;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}

	Node *new_node = simple_linked_list_init(value);
	tmp->next = new_node;
}

uint64_t simple_linked_list_pop(Node **list) {
	if (*list == NULL) {
		return 0;
	}

	Node *tmp = *list;
	Node *prev = tmp;
	while (tmp->next != NULL) {
		prev = tmp;
		tmp = tmp->next;
	}

	uint64_t val = tmp->value;
	prev->next = NULL;
	free(tmp);

	if (tmp == *list) {
		*list = NULL;
	}

	return val;
}

Node *simple_linked_list_get(Node *list, uint64_t idx) {
	if (list == NULL) {
		return NULL;
	}

	uint64_t count = 0;
	if (idx == 0) {
		return list;
	}

	Node *tmp = list;
	while (tmp->next != NULL) {
		if (count == idx) {
			return tmp;
		}

		tmp->next = tmp;
		count += 1;
	}

	return NULL;
}

bool simple_linked_list_insert_after(Node **list, uint64_t idx, uint64_t val) {
	if (*list == NULL) {
		printf("list is empty!\n");
		return false;
	}

	// Ok, try to get to our requested index
	Node *node_before = simple_linked_list_get(*list, idx);
	if (node_before == NULL) {
		printf("idx %llu is out of bounds\n", idx);
		return false;
	}

	Node *new_node = simple_linked_list_init(val);

	// If we're inserting at the tail, there's no splicing to do
	if (node_before->next == NULL) {
		node_before->next = new_node;
		return true;
	}

	// Ok, splice time
	Node *old_next = node_before->next;
	new_node->next = old_next;
	node_before->next = new_node;
	return true;
}

void simple_linked_list_free(Node **list) {
	if (*list == NULL) {
		return;
	}

	Node *tmp = *list;
	while (tmp->next != NULL) {
		Node *prev = tmp;
		tmp = tmp->next;
		free(prev);
	}
	free(tmp);
	*list = NULL;
}
