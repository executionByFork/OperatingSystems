#include "../include/structures.h"
#include <stdio.h>

int compare_structs(sample_t* a, sample_t* b) {
	//check both pointers for NULL
	if (a == NULL || b == NULL) {
		return 0;
	}

	//compare data structures member by member
	if (a->a != b->a) {
		return 0;
	}
	if (a->b != b->b) {
		return 0;
	}
	if (a->c != b->c) {
		return 0;
	}

	return 1;	//checks passed
}

void print_alignments() {

	printf("Alignment of int is %zu bytes\n",__alignof__(int));
	printf("Alignment of double is %zu bytes\n",__alignof__(double));
	printf("Alignment of float is %zu bytes\n",__alignof__(float));
	printf("Alignment of char is %zu bytes\n",__alignof__(char));
	printf("Alignment of long long is %zu bytes\n",__alignof__(long long));
	printf("Alignment of short is %zu bytes\n",__alignof__(short));
	printf("Alignment of structs are %zu bytes\n",__alignof__(fruit_t));
}

int sort_fruit(const fruit_t* a, int* apples, int* oranges, const size_t size) {
	//check for null pointer
	if (a == NULL) {
		return -1;
	}
	if (apples == NULL) {
		return -1;
	}
	if (oranges == NULL) {
		return -1;
	}

	//make sure size is valid
	if (size <= 0) {
		return -1;
	}

	size_t i;
	for (i = 0; i < size; ++i) {
		if (a[i].type == APPLE) {
			(*apples)++;
		}
		if (a[i].type == ORANGE) {
			(*oranges)++;
		}
	}

	return i;
}

int initialize_array(fruit_t* a, int apples, int oranges) {
	//check for null pointer
	if (a == NULL) {
		return -1;
	}

	int i, j;
	for (i = 0; i < apples; ++i) {
		if ( initialize_apple( (apple_t*) &a[i] ) < 0) return -1;
	}
	for (j = 0; j < oranges; ++j) {
		if ( initialize_orange( (orange_t*) &a[i+j] ) < 0) return -1;
	}

	return i+j;
}

int initialize_orange(orange_t* a) {
	//check for null pointer
	if (a == NULL) {
		return -1;
	}

	a->type = ORANGE;
	a->weight = 0;
	a->peeled = 0;

	return 0;
}

int initialize_apple(apple_t* a) {
	//check for null pointer
	if (a == NULL) {
		return -1;
	}

	a->type = APPLE;
	a->weight = 0;
	a->worms = 0;

	return 0;
}
